#include <ctime>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <pwd.h>

#include <boost/lexical_cast.hpp>
#include <boost/functional/hash.hpp>

#include "utilities.hpp"
#include "process_grp.hpp"

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


clpr_d::process_key get_process_key(const clpr_d::proc_stat& pstat) {
	clpr_d::process_key pkey;
	pkey.command = pstat.comm;
	pkey.pid = pstat.pid;
	pkey.starttime = pstat.starttime;

	return pkey;
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

}

