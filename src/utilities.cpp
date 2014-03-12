#include "utilities.hpp"



namespace clpr_d{



	string get_uid_blob_label(vector<string>& input){


		// when this group of pid's started
		// lose the seconds in the hope of grouping more procs together
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
		ret 		+= " ";
		ret		+= input[6];

		return ret;
	}


	pid_blob get_pid_blob(vector<string>& input){

		pid_blob ret;

		// timestamp
		ret.time 	= atof(input[time_pos].c_str());
		// percent cpu on app
		ret.user 	= atof(input[user_pos].c_str());
		// percent cpu on system stuff
		ret.system 	= atof(input[system_pos].c_str());
		//
		ret.perc_cpu 	= atof(input[perc_cpu_pos].c_str());
		// the core we are running
		ret.core_cpu 	= atof(input[core_cpu_pos].c_str());
		// the page faults not requring disk
		ret.minflt_s 	= atof(input[minflt_s_pos].c_str());
		// the page faults requiring disk
		ret.majflt_s	= atof(input[majflt_s_pos].c_str());
		// virtual memory size bytes
		ret.vsz		= atof(input[vsz_pos].c_str());
		// resident mem, non-swapped
		ret.rss		= atof(input[rss_pos].c_str());
		//
		ret.mem		= atof(input[mem_pos].c_str());
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

