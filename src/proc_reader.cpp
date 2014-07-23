#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/thread/thread.hpp>

#include "proc_reader.hpp"
#include "utilities.hpp"

using namespace boost::posix_time;
namespace fs = boost::filesystem;

namespace clpr_d {

proc_reader::proc_reader(const clpr_d::p_log p_log_file) : p_log_file(p_log_file) {
	p_log_file->write(CLPR_LOG_DEBUG,"Calling proc_reader constructor");
} // End of proc_reader::proc_reader

proc_reader::~proc_reader() {
	p_log_file->write(CLPR_LOG_DEBUG,"Calling proc_reader destructor");
} // End of proc_reader::~proc_reader

void proc_reader::read(clpr_d::db_ptr p_db) {

	//// Time stamp at which we are looking at the data
	long unsigned int tstamp;

	//// Hostname
	struct utsname uname_data;
	uname(&uname_data);
	std::string hostname(uname_data.sysname);
	
	//// Total memory, in kB
	long unsigned int total_host_mem = 0; 
	std::ifstream file("/proc/meminfo");
	// Get the first line, that contains the total memory
	std::string _;
	file >> _ >> total_host_mem; 
	// Clean up
	file.close();
	
	boost::regex pid_regex("/proc/([0-9]|[1-9][0-9]*)", boost::regex::extended);

	proc_stat pstat;
	proc_status pstatus;
	proc_io pio;
	std::string wchan;
	std::string cmd_line;


	std::string msg;
	uint64_t uptime_c;
	uint64_t uptime_p;

	cpu_usage cpu_usage_c;
	cpu_usage cpu_usage_p;

	while(1) {
		// Discover the /proc directory
		fs::path p("/proc");

		// If /proc does not exist or is not a directory, we have a bigger problem ! Better check anyway...
		if( !fs::exists(p) ) {
			p_log_file->write(CLPR_LOG_CRITICAL,"The /proc directory does not exist ! Killing the daemon now.");	
			break;
		} else if (! fs::is_directory(p) {
			p_log_file->write(CLPR_LOG_CRITICAL,"The /proc directory is not a directory ! Killing the daemon now.");	
			break;
		} else {
			//// Copy the list of files in /proc to a vector for easy iteration
			std::vector<fs::path> proc_file_list;
			std::copy(fs::directory_iterator(p), fs::directory_iterator(), std::back_inserter(proc_file_list));

			// Optional sorting ?
			// std::sort(proc_file_list.begin(),proc_file_list.end());
			
			//// Get the current uptime
			uptime_p = uptime_c;
			read_stat(uptime_c);

			delta_cpu = (float)(uptime_c - uptime_p);

			//// Iterate over all the files in /proc; pick only the directories named after pids
			for(auto it = proc_file_list.begin(); it != proc_file_list.end(); ++it) {
				///////////////////////////////
				// PARSE THE /PROC DIRECTORY //
				///////////////////////////////
				//// Only /proc/<pid> stuff is used
				if(boost::regex_match(it->generic_string(),pid_regex) ) {
					//// Check that the directory still exists
					if(fs::exists(*it)) {
						pid_dir_it = it;

						// FD and environment. Destroyed at end of if scope
						std::map<int,std::string> fd_list;
						std::map<std::string, std::string> env;
					
						try {
							//// /proc/<pid>/stat
							read_pid_stat(pstat);

							//// /proc/<pid>/io
							read_pid_io(pio);

							//// /proc/<pid>/status
							read_pid_status(pstatus);

							//// /proc/<pid>/wchan
							read_pid_wchan(wchan);

							//// /proc/<pid>/cmdline
							// if the process is not a zombie, then try to parse
							if( strcmp(pstat.state.c_str(),"Z") != 0) {
								std::string cmd_file_path = it->generic_string() + "/cmdline";
								file.open(cmd_file_path.c_str());
								if(!file.is_open()) {
									msg = "Can not open " + cmd_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
								}	

								std::getline(file,cmd_line);
								// Replace null character with spaces
								std::replace(cmd_line.begin(),cmd_line.end(),'\0',' ');
								file.close();
							} else {
								// Otherwise we shall be content with the simple command name w/o arguments
								cmd_line = pstat.comm;
							}	

							//// /proc/<pid>/environ
							read_pid_environ(env);

							//// /proc/<pid>/fd
							read_pid_fd(fd_list);

						} catch( std::exception& e) {
							// Logging has already been taken care of; just break from the for loop and go to next dir.
							continue;
						}	
							
						///////////////////////////////
						// GET THE PROCESS GRP LABEL //
						///////////////////////////////
						// A process grp exists for trying to identify groups of processes
						// that can be signaled together (e.g. mpi processes).
						// The PGID is therefore of use here
						std::string process_grp_label = get_process_grp_label(pstat,pstatus); 

						//// Process groups
						// Do we already have such process group in the database ?
						if( !p_db->is_present(process_grp_label) ) {
							// If not, create a new process group
							process_grp_ptr pgrp_insert(new process_grp(pstat,pstatus));
							p_db->insert(pgrp_insert);
						} // End if 

						// Now, get the considered process group
						process_grp_ptr pgrp = p_db->find(process_grp_label);

						//// Processes
						// Get the process key of the considered process
						std::size_t process_label = get_process_label(pstat);

						// Does the process exists in the considered group ?
						if( !pgrp->is_present(process_label) ) {
							uint64_t bdate;
							read_pid_bdate(bdate);
							// If not, create a new process
							process_ptr proc_insert(new process(cmd_line, env,pstat.pid,pstat.ppid,bdate));

							pgrp->insert(process_label,proc_insert);

						} // End if	

						// Now, get the considered process
						process_ptr process = pgrp->find(process_label);
						
						// Construct the previous and current cpu usages
						cpu_usage_p = cpu_usage_c;
						cpu_usage_c.utime = pstat.utime;
						cpu_usage_c.stime = pstat.stime;
						cpu_usage_c.gtime = pstat.gtime;

						// ... and finally, add the data !
						
						process->push_back(new clpr_d::snapshot_ptr(pstat,
											pstatus,
											pio,wchan,
											total_host_mem,
											cpu_usage_c, 
											cpu_usage_p,
											delta_cpu)); 

					} // if fs::exists
				} // End if(boost::regex_match)
			} // End for loop over directory list
		} // End else if

		// Wait until next sampling
		boost::this_thread::sleep(boost::posix_time::seconds(CLPR_SAMPLE_RATE));
	} // End while

} // End of proc_reader::read

void proc_reader::read_pid_stat(clpr_d::proc_stat& pstat) {

	//// Open /proc/<pid>/stat
	std::string file_path = pid_dir_it->generic_string() + "/stat";
	file.open(file_path.c_str());

	// If we can't open the file, the process is most likely done, so just skip over it
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);

		throw std::runtime_error(msg);
	}	

	// Parse stat
	file >> pstat.pid  >> pstat.comm >> pstat.state >> pstat.ppid >> pstat.pgid >> pstat.sid>> pstat.tty \
		>> pstat.tpgid >> pstat.flags >> pstat.minflt >> pstat.cminflt >> pstat.majflt >> pstat.cmajflt \
		>> pstat.utime >> pstat.stime >> pstat.cutime >> pstat.cstime >> pstat.priority >> pstat.nice \
		>> pstat.n_th >> pstat.itrealvalue >> pstat.starttime >> pstat.vsz >> pstat.rss \
		>> pstat.rsslim >> pstat.startcode >> pstat.endcode >> pstat.startstack >> pstat.kstkesp >> pstat.kstkeip \
		>> pstat.signal >> pstat.blocked >> pstat.sigignore >> pstat.sigcatch >> pstat.wchan\
		>> pstat.nswap >> pstat.cnswap >> pstat.exit_sig >> pstat.proc >> pstat.rt_prio\
		>> pstat.policy >> pstat.delay >> pstat.gtime; 

	file.close();
} // End of proc_reader::read_pid_stat

void proc_reader::read_pid_io(clpr_d::proc_io& pio) {

	std::string file_path = pid_dir_it->generic_string() + "/io";
	file.open(file_path.c_str());
	// If we can't open the file, the process is most likely done, so just skip over it
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);

		throw std::runtime_error(msg);
	}	

	// Parse io otherwise
	file >> _ >> pio.rchar;
	file >> _ >> pio.wchar;
	file >> _ >> pio.syscr;
	file >> _ >> pio.syscw;
	file >> _ >> pio.read_bytes;
	file >> _ >> pio.write_bytes;
	file >> _ >> pio.cancelled_write_bytes;

	file.close();

} // End of proc_reader::read_pid_io


void proc_reader::read_pid_status(clpr_d::proc_status& pstatus) {

	std::string file_path = pid_dir_it->generic_string() + "/status";
	file.open(file_path.c_str());
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);

		throw std::runtime_error(msg);
	}	
	// Parse
	std::string line;
	while(std::getline(file,line)) {
		std::istringstream iss(line);
		std::string lhs;

		iss >> lhs;	

		if(lhs.compare("voluntary_ctxt_switches:") == 0) {
			iss >> pstatus.cswch;
		} else if(lhs.compare("nonvoluntary_ctxt_switches:") == 0) {
			iss >> pstatus.nvcswch;
		} else if(lhs.compare("FDSize:") == 0) {
			iss >> pstatus.fdsize;
		} else if(lhs.compare("Uid:") == 0 ) {
			iss >> pstatus.uid;
		}	
	}	
	file.close();
} // End of proc_reader::read_pid_status

void proc_reader::read_pid_wchan(std::string& wchan) {
	std::string file_path = pid_dir_it->generic_string() + "/wchan";
	file.open(file_path.c_str());
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);

		throw std::runtime_error(msg);

	}	
	// Parse
	file >> wchan;

	file.close();

} // End of proc_reader::read_pid_wchan
 
//void proc_reader::read_pid_cmdline(std::string& cmd, const clpr_d::proc_stat& pstat);

void proc_reader::read_pid_environ(std::map<std::string, std::string>& env) {

	// Open the file
	std::string file_path = pid_dir_it->generic_string() + "/environ";
	file.open(file_path.c_str()); 
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);

		throw std::runtime_error(msg);
	}	

	std::string env_line;

	// Exploit getline with '\0' separator instead of \n
	while(std::getline(file,env_line , '\0')) {
		// Split the string VARIABLE=data at the equal sign
		std::vector<std::string> vstr;
		boost::split(vstr, env_line , "=");
		// TODO : add a check if vstr.size() > 2... Should not happen (see https://stackoverflow.com/questions/2821043/allowed-characters-in-linux-environment-variable-names)
		// but you never know which crappy software will set an equal variable in the value of a variable ! 

		// KEY=VALUE
		env.insert(std::make_pair(vstr[0],vstr[1]));		

	}
	file.close();


} // End of proc_reader::read_pid_environ


void proc_reader::read_pid_fd(std::map<int,std::string>& fd_list) {
	// Get path to fd
	std::string dir_path = pid_dir_it->generic_string() + "/fd";
	fs::path fd_path(dir_path);
	if( !fs::exists(fd_path) ) {
		msg = "Can not read " + dir_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);
		
		throw std::runtime_error(msg);
	}	

	std::vector<fs::path> fd_vec;
	std::copy(fs::directory_iterator(fd_path), fs::directory_iterator(), std::back_inserter(fd_vec));

	// For all file descriptors listed in /fd
	for(auto it = fd_vec.begin(); it != fd_vec.end(); ++it) {
		int fd_num = std::stoi(it->leaf().generic_string());
		fd_list.insert(std::make_pair(fd_num, fs::read_symlink(*it).generic_string())); 
	}

} // End of proc_reader::read_pid_fd


void proc_reader::read_pid_bdate(uint64_t& bdate) {
	struct stat statbuf;
	std::string dir_path = pid_dir_it->generic_string();

	if(stat(dir_path.c_str(), &statbuf == -1) {
		msg = "Can not read " + dir_path + " ; skipping entry";
		p_log_file->write(CLPR_LOG_ERROR, msg);
		
		throw std::runtime_error(msg);
	}

	// TODO: might want to check that cast
	bdate = (uint64_t)statbuf.st_ctime;
} // End of proc_reader::read_pid_bdate


// Get the current uptime
void proc_reader::read_stat(uint64_t& uptime_c) {
	std::string file_path = "/proc/stat";
	
	file.open(file_path.c_str()); 
	if(!file.is_open()) {
		msg = "Can not open " + file_path + " ! "; 
		p_log_file->write(CLPR_LOG_CRITICAL, msg);

		// TODO: Better exceptions -- this one is critical
		throw std::runtime_error(msg);
	}	

	
	//// Get first line of /proc/stat, e.g.
	// TODO: Add check so that we get the correct line
	// cpu user user-nice system idle iowait irq softirq steal guest 
	// cpu 133473989 27287 12925885 4073999423 1927197 3535 523304 0 0
	std::string line;
	std::getline(file,line);

	// Split the line at spaces
	std::vector<std::string> vstr;
	boost::split(vstr, line , " ");

	// Calculate the total uptime
	uint64_t uptime = 0;
	for(auto it = vstr.begin() + 1; it != vstr.end(); ++it) 
		uptime += std::stoi(*it);

	// Remove Guest since already contained in 
	uptime -= vstr[PROC_STAT_GUEST];	

	uptime_c = uptime;

} // End of proc_reader::read_stat


} // End of namespace clpr_d
