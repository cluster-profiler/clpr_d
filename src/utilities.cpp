#include <ctime>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <pwd.h>

#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>

#include "pid_data.hpp"
#include "utilities.hpp"

namespace clpr_d{

// Function: file_exists 
/**
  Check if a file exists
  @param[in] filename - the name of the file to check

  @return    true if the file exists, else false

*/
bool file_exists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

//std::size_t get_process_label(const std::string& cmd, const int& pid, const uint64_t& start_time) {
std::size_t get_process_label(const clpr_d::proc_stat& pstat) {
	return (boost::hash<std::string>()(pstat.comm) ^\
		boost::hash<std::string>()(std::to_string(pstat.pid)) ^\
		boost::hash<std::string>()(std::to_string(pstat.starttime)) );
}		














	string get_start_time(vector<string>& input){

		// First five fields of pidstat:
		// weekday month day hour year
		// Mon Apr 28 15:08:01 2014
		// 0 1 2 3 4
		string tmp="";
		for(int i = 1; i<=4; i++)
			tmp += input[i];


		// Convert to UNIX time since epoch
		struct tm tm;
		time_t epoch;
		if( strptime(const_cast<char*>(tmp.c_str()), "%b%d%H:%M:%S%Y", &tm) != NULL ) 
			epoch = mktime(&tm);
		else
			std::cerr << "Error converting time to epoch" << std::endl;
		
		string ret = boost::lexical_cast<std::string>(epoch);

#if 0
		string temp = input[3];
		temp[6]='0';
		temp[7]='0';


		string ret;

		ret		= input[0];
		ret 		+= " ";
		ret	 	+= input[1];
		ret 		+= " ";
		ret	 	+= input[2];
		ret 		+= " ";
		ret		+= temp;
		ret 		+= " ";
		ret		+= input[4];
	
#endif

		return ret;
		

	}

	string get_uid(const vector<string>& input){


		return input[6];

	}


/*
	string get_process_grp_label(vector<string>& input){

		string ret 	= get_start_time(input);
		ret		+= " ";
		ret 		+= get_uid(input);
		

		// when this group of pid's started
		// lose the seconds in the hope of grouping more procs together
		//string temp = input[3];
		//temp[6]='0';
		//temp[7]='0';


		//string ret;

		//ret		= input[0];
		//ret 		+= " ";
		//ret	 	+= input[1];
		//ret 		+= " ";
		//ret	 	+= input[2];
		//ret 		+= " ";
		//ret		+= temp;
		//ret 		+= " ";
		//ret		+= input[4];
		//ret 		+= " ";
		//ret		+= input[6];

		return ret;
	}
*/
	std::string get_process_grp_label(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus) {
		std::string ret;

		struct passwd *pwd;
		pwd = getpwuid(pstatus.uid);
		
		std::string uid_str(pwd->pw_name);

		ret = uid_str + " " + std::to_string(pstat.pgid);
		//ret = std::to_string(pstatus.uid) + " " + std::to_string(pstat.pgid);

		return ret;
	} // End of get_process_grp_label

	// pid_data is simple data; assignment operator will work here
	pid_data get_pid_data(const std::vector<std::string>& input){

		pid_data ret;

		// timestamp
		ret.time 	= std::stof(input[TIME_POS]);
		// percent cpu on app
		ret.p_usr 	= std::stof(input[USER_POS]);
		// percent cpu on system stuff
		ret.p_sys 	= std::stof(input[SYSTEM_POS]);
		//
		ret.p_cpu 	= std::stof(input[PCPU_POS]);
		// the core we are running
		ret.cpu_num 	= std::stof(input[CORE_CPU_POS]);
		// the page faults not requring disk
		ret.minflt_s 	= std::stof(input[MINFLT_S_POS]);
		// the page faults requiring disk
		ret.majflt_s	= std::stof(input[MAJFLT_S_POS]);
		// virtual memory size bytes
		ret.vsz		= std::stof(input[VSZ_POS]);
		// resident mem, non-swapped
		ret.rss		= std::stof(input[RSS_POS]);
		//
		ret.p_mem	= std::stof(input[MEM_POS]);
		// kb read disk
		ret.kb_rd_s	= std::stof(input[KB_RD_S_POS]);
		//
		ret.kb_wr_s   	= std::stof(input[KB_WR_S_POS]);
		// canceled writes
		ret.kb_ccwr_s	= std::stof(input[KB_CCWR_S_POS]);
		//
		ret.iodelay	= std::stof(input[IODELAY_POS]);
		// context switches
		ret.cswch_s	= std::stof(input[CSWCH_S_POS]);
		// non-voluntary
		ret.nvcswh_s 	= std::stof(input[NVCSWCH_S_POS]);
		// threads in my group
		ret.threads	= std::stof(input[NTH_POS]);
		// open file descriptors
		ret.fds		= std::stof(input[FDS_POS]);


		return ret;


	}


}

