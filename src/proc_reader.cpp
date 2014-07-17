#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>

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
	std::string env_line;
	std::string msg;


	while(1) {

		// Discover the /proc directory
		fs::path p("/proc");
			// If /proc does not exist or is not a directory, we do have a problem ! Better check anyway...
			if( fs::exists(p) ) {
				if( fs::is_directory(p) ) {

					//// Copy the list of files in /proc to a vector for easy iteration
					std::vector<fs::path> proc_file_list;
					std::copy(fs::directory_iterator(p), fs::directory_iterator(), std::back_inserter(proc_file_list));
					// Optional sorting ?
					// std::sort(proc_file_list.begin(),proc_file_list.end());

					//// Iterate over all the files in /proc; pick only the directories named after pids
					for(auto it = proc_file_list.begin(); it != proc_file_list.end(); ++it) {


						///////////////////////////////
						// PARSE THE /PROC DIRECTORY //
						///////////////////////////////

						//// Only /proc/<pid> stuff is used
						if(boost::regex_match(it->generic_string(),pid_regex) ) {
							//// Check that the directory still exists
							if(fs::exists(*it)) {

								//// Open /proc/<pid>/stat
								std::string stat_file_path = it->generic_string() + "/stat";
								file.open(stat_file_path.c_str());

								// If we can't open the file, the process is most likely done, so just skip over it
								if(!file.is_open()) {
									msg = "Can not open " + stat_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
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
							

								//// /proc/<pid>/io
								std::string io_file_path = it->generic_string() + "/io";
								file.open(io_file_path.c_str());
								// If we can't open the file, the process is most likely done, so just skip over it
								if(!file.is_open()) {
									msg = "Can not open " + io_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
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

								//// /proc/<pid>/status
								std::string status_file_path = it->generic_string() + "/status";
								file.open(status_file_path.c_str());
								if(!file.is_open()) {
									msg = "Can not open " + status_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
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

								//// /proc/<pid>/wchan
								std::string wchan_file_path = it->generic_string() + "/wchan";
								file.open(wchan_file_path.c_str());
								if(!file.is_open()) {
									msg = "Can not open " + wchan_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
								}	
								// Parse
								file >> wchan;
								
								file.close();

								//// /proc/<pid>/cmdline
								// if the process is not a zombie, then try to parse
								if( strcmp(&pstat.state,"Z") != 0) {
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
								std::string env_file_path = it->generic_string() + "/environ";
								file.open(env_file_path.c_str()); 
								if(!file.is_open()) {
									msg = "Can not open " + env_file_path + " ; skipping entry";
									p_log_file->write(CLPR_LOG_ERROR, msg);

									// Skip over this entry
									continue;
								}	

								std::getline(file,env_line);
								// Replace null character with spaces
								std::replace(env_line.begin(),env_line.end(),'\0','\n');
								file.close();

								//// /proc/<pid>/fd
									

								///////////////////////////////
								// GET THE PROCESS GRP LABEL //
								///////////////////////////////
								// A process grp exists for trying to identify groups of processes
								// that can be signaled together (e.g. mpi processes).
								// The PGID is therefore of use here
								std::string process_grp_label = get_process_grp_label(pstat,pstatus); 

								
							} // if fs::exists
						} // End if(boost::regex_match)
					} // End for loop over directory list
				} // End if fs::is_directory(p)
			} // End of if fs::exists(p)	

		// Wait until next sampling
		boost::this_thread::sleep(boost::posix_time::seconds(CLPR_SAMPLE_RATE));
	} // End while

} // End of proc_reader::read

} // End of namespace clpr_d
