#include <ctime>
#include <iostream>
#include <string>
#include <sys/stat.h>

#include <boost/lexical_cast.hpp>

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


	string get_grp_proc_label(vector<string>& input){

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


	pid_data get_pid_data(vector<string>& input){

		pid_data ret;

		// timestamp
		ret.time 	= atof(input[time_pos].c_str());
		// percent cpu on app
		ret.p_usr 	= atof(input[user_pos].c_str());
		// percent cpu on system stuff
		ret.p_sys 	= atof(input[system_pos].c_str());
		//
		ret.p_cpu 	= atof(input[perc_cpu_pos].c_str());
		// the core we are running
		ret.cpu_num 	= atof(input[core_cpu_pos].c_str());
		// the page faults not requring disk
		ret.minflt_s 	= atof(input[minflt_s_pos].c_str());
		// the page faults requiring disk
		ret.majflt_s	= atof(input[majflt_s_pos].c_str());
		// virtual memory size bytes
		ret.vsz		= atof(input[vsz_pos].c_str());
		// resident mem, non-swapped
		ret.rss		= atof(input[rss_pos].c_str());
		//
		ret.p_mem		= atof(input[mem_pos].c_str());
		// kb read disk
		ret.kb_rd_s	= atof(input[kb_rd_s_pos].c_str());
		//
		ret.kb_wr_s   	= atof(input[kb_wr_s_pos].c_str());
		// canceled writes
		ret.kb_ccwr_s	= atof(input[kb_ccwr_s_pos].c_str());
		//
		ret.iodelay	= atof(input[iodelay_pos].c_str());
		// context switches
		ret.cswch_s	= atof(input[cswch_s_pos].c_str());
		// non-voluntary
		ret.nvcswh_s 	= atof(input[nvcswh_s_pos].c_str());
		// threads in my group
		ret.threads	= atof(input[threads_pos].c_str());
		// open file descriptors
		ret.fds		= atof(input[fds_pos].c_str());


		return ret;


	}


}

