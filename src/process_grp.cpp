#include <pwd.h>
#include <utility>

#include "process_grp.hpp"

namespace clpr_d {


//// Constructor
process_grp::process_grp(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus) {

	// PGID, SID
	pgid = pstat.pgid;
	sid = pstat.sid;

	// Get uid
	struct passwd *pwd;
	pwd = getpwuid(pstatus.uid);

	uid = pwd->pw_name;

	// Hash
	hash_index = uid + " " + std::to_string(pgid);
}

// TODO: Add is_present here. Might require to reorganize the HASH_MAP from process_grp
// bool process_grp::is_present(const std::size_t& idx) {
bool process_grp::is_present(const process_key& pkey) {
	if (process_list.find(pkey) == process_list.end()) {
		return false;
	}

	return true;
}

//void process_grp::insert(std::size_t& idx, clpr_d::process_ptr pproc_insert) {
void process_grp::insert(process_key& pkey, clpr_d::process_ptr pproc_insert) {
	// Some C++11
	this->process_list.insert({pkey,pproc_insert});
}

//clpr_d::process_ptr find(const std::size_t& idx) {
clpr_d::process_ptr process_grp::find(const process_key& pkey) {
	// Get the actual value from the returned iterator
	return process_list.find(pkey)->second;
}	



	
// Time setter
void process_grp::update_time(){
	tstamp = (uint64_t) time(NULL);
}

// Time getter
uint64_t const& process_grp::get_time() const{
	return tstamp;
}


//// Getters	
std::string process_grp::get_hash_index() const {return hash_index;}	
uint64_t process_grp::get_label() const{return label;}


//// Output stream
//std::ostream& operator<<(std::ostream &out, const process_grp& in) {
//std::ifstream& operator<<(std::ifstream &out, boost::shared_ptr<process_grp>& in) {
std::ifstream& operator<<(std::ifstream &out, clpr_db::lab_it& in) {
	process_grp_ptr pgrp = *in;

	BOOST_FOREACH( HASH_MAP::value_type v, pgrp->process_list) {
		out << v.first << v.second << endl;
	}
	return out;
} // End of operator<<


/*
// Constructor
process_grp::process_grp(const std::vector<std::string>& tokens, const std::string& idx, const uint64_t& label, const clpr_d::p_log& p_log_file):
	hash_index(idx),
	label(label),
	max_mem(0.0),
	min_mem(0.0),
	max_disk(0.0),
	min_disk(0.0),
	max_fds(0.0),
	min_fds(0.0),
	max_cpu(0.0),
	min_cpu(0.0),
	total_process(0)
{
	update_time();

//	start_time 	= get_start_time(tokens);
	this->p_log_file = p_log_file;
	this->p_log_file->write(CLPR_LOG_DEBUG,"Calling process_grp constructor");

//	uid 		= get_uid(tokens);
//	gid 		= get_gid(tokens);
}

*/

// Destructor
/*
process_grp::~process_grp() {
	this->p_log_file->write(CLPR_LOG_DEBUG,"Calling process_grp destructor");
}	
*/

// Get the header
/*
std::string process_grp::get_header() const{

	std::stringstream ss;

	ss << "## " << label << "," << hash_index << "," << max_mem << "," << min_mem << "," << max_disk << "," << min_disk;
	ss << "," << max_fds << "," << min_fds << "," << max_cpu << "," << min_cpu << "," << total_process;
	return ss.str();

}
*/


// Pushing back a row of pidstat in a process_grp
// TODO: check that the tokens are valid
/*
void process_grp::push_back(const std::string& host_info, const std::vector<std::string> &tokens){

	//// For logging
	std::string msg;

	// Copy data from original tokens 
	std::vector<std::string> tmp_tokens;
	std::copy(tokens.begin(), tokens.end(), tmp_tokens.begin());

	//// Log the line to sanitize
	p_log_file->write(CLPR_LOG_DEBUG,"Sanitizing parsed pidstat line");
	for(auto it = tmp_tokens.begin(); it != tmp_tokens.end(); ++it) {	
		p_log_file->write(CLPR_LOG_DEBUG,*it);
	}

	//// Sanitizing the command (can contain return, tabs, etc.)
	history_key key;
	char spec_chars[] = "\n\t";

	// Do not add blank space before command
	key.command = "";

	

	// Parse the command (can be contained in multiple tokens)
	// Loop over each part of the command, and erase double quote chars and special characters
	for(auto it = tmp_tokens.begin() + CMD_POS; it != tmp_tokens.end(); ++it) 
	{

		// Find double quote character, replace with single character
		std::replace( it->begin(), it->end(), '\"', '\'');	

		// Remove other special characters
		for (unsigned int j = 0; j < strlen(spec_chars); ++j)
			it->erase (std::remove(it->begin(), it->end(), spec_chars[j]), it->end());

		key.command += *it;
		// Do not add blank space at the end of the command, but add space between parts of the command
		if( it != tmp_tokens.end()-1 )	
			key.command			+= " ";
	}		

	
	//// TODO: Sanitize other fields


	//// Log the sanitized pidstat line 
	p_log_file->write(CLPR_LOG_DEBUG,"Sanitized line");
	for(auto it = tmp_tokens.begin(); it != tmp_tokens.end(); ++it) 
	{
		p_log_file->write(CLPR_LOG_DEBUG,*it);
	}	
	
	//// Get hostname and PID
	// Store the hostname and the PID for that particular entry
	key.hostname 	+= host_info;
	key.pid		+= tmp_tokens[PID_POS];

	//// Store data of pidstat, then add it to the history
//	pid_data value = get_pid_data(tmp_tokens);

	pid_data value;
	history[key].push_back(value);

	total_process = history.size();

	//// Update statistics
	if (max_mem < value.p_mem) 			max_mem = value.p_mem;
	if (min_mem > value.p_mem) 			min_mem = value.p_mem;
	if (max_disk < (value.kb_rd_s+value.kb_wr_s)) 	max_disk= value.kb_rd_s+value.kb_wr_s;
	if (min_disk > (value.kb_rd_s+value.kb_wr_s)) 	min_disk= value.kb_rd_s+value.kb_wr_s;
	if (max_fds < value.fds)			max_fds = value.fds;
	if (min_fds > value.fds)			min_fds = value.fds;
	if (max_cpu < value.p_cpu)			max_cpu = value.p_cpu;
	if (min_cpu > value.p_cpu)			min_cpu = value.p_cpu;


	// Update timestamp
	update_time();

}
*/

/*
float process_grp::get_max_mem() const{return max_mem;}
float process_grp::get_min_mem() const{return min_mem;}

float process_grp::get_max_disk() const{return max_disk;}
float process_grp::get_min_disk() const{return min_disk;}

float process_grp::get_max_fds() const{return max_fds;}
float process_grp::get_min_fds() const{return min_fds;}

float process_grp::get_max_cpu() const{return max_cpu;}
float process_grp::get_min_cpu() const{return min_cpu;}
*/






/*
// something approaching json format
ostream& operator<<(ostream &out, const process_grp& in){

	std::string kill_me="abcdefghijklmnopqrstuvwxyz";

	// CSV format for the output
	// Data is denormalized 
	// Data out:
	// process_grp_id, start time, max mem, min mem, max disk, min disk, max fds, min fds, max cpu, min cpu, total processes, ...
	// ... 
	BOOST_FOREACH( HASH_MAP::value_type v, in._history ) {	

		history_key tmp = v.first;
		std::vector<pid_blob> tmp_vals = v.second;

		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){


			// if there are alphas in pid, something screwed up, probably not with pidstat, but with the data gathering process
			bool test_for_alpha=false;

			for (auto& x: kill_me) 
				if (tmp.pid.find(x) < tmp.pid.size()){
					test_for_alpha=true;
					break;
				}


			if (test_for_alpha)
				cerr << "whoa; process_grp with index " << in._label << " has an ill-formed pid : " << tmp.pid << endl; 

			if (!test_for_alpha){

				// UID Blob info
				// Unique hash: label, pid, startime, user id, time stamp
				out << "\"" << in._label << tmp.pid << in._start_time << in._uid << it->time << "\",";
				out << "\"" << in._label << "\",\"";
				out << in._start_time << "\",\"";
				out << in._uid << "\",\"";
				out << in._max_mem << "\",\"";
				out << in._min_mem << "\",\"";
				out << in._max_disk << "\",\"";
				out << in._min_disk << "\",\"";
				out << in._max_fds << "\",\"";
				out << in._min_fds << "\",\"";
				out << in._max_cpu << "\",\"";
				out << in._min_cpu << "\",\"";
				out << in._total_procs << "\",\"";

				// Process blob info
				out << tmp.pid << "\",\"";
				out << tmp.command << "\",\"";
				out << tmp.hostname << "\",\"";

				// Content at time t
				boost::fusion::for_each(*it, out << arg1 << "\",\"");
				out << endl;

			}
		}	
	}	


#if 0
	out << "\"id\":\"" << in._label << "\"," << endl;
	out << "\"stime\":\"" << in._start_time << "\"," << endl;
	out << "\"uid\":\"" << in._uid << "\"," << endl;
	out << "\"max_mem\":\"" << in._max_mem << "\"," << endl;
	out << "\"min_mem\":\"" << in._min_mem << "\"," << endl;
	out << "\"max_disk\":\"" << in._max_disk << "\"," << endl;
	out << "\"min_disk\":\"" << in._min_disk << "\"," << endl;
	out << "\"max_fds\":\"" << in._max_fds << "\"," << endl;
	out << "\"min_fds\":\"" << in._min_fds << "\"," << endl;
	out << "\"max_cpu\":\"" << in._max_cpu << "\"," << endl;
	out << "\"min_cpu\":\"" << in._min_cpu << "\"," << endl;
	out << "\"total_procs\":\"" << in._total_procs << "\"," << endl;


	out << "\"pgroup\":[" << endl;
	BOOST_FOREACH( HASH_MAP::value_type v, in._history ){

		history_key tmp = v.first;

		out << "{\"pid\":\"" << tmp.pid << "\"," << endl;
		out << "\"command\":\"" << tmp.command << "\"," << endl;
		out << "\"host\":\"" << tmp.hostname << "\"," << endl;

		std::vector<pid_blob> tmp_vals = v.second;

		out << "\"time\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->time << ",";
		}

		out << "]," << endl;
		
		out << "\"user\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->user << ",";
		}

		out << "]," << endl;
		
		out << "\"\% cpu\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->perc_cpu << ",";
		}

		out << "]," << endl;
		
		out << "\"majflt_s\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->majflt_s << ",";
		}

		out << "]," << endl;
		
		out << "\"vsz\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->vsz << ",";
		}

		out << "]," << endl;
		
		out << "\"kd_rd_s\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->kb_rd_s << ",";
		}

		out << "]," << endl;
		
		out << "\"kb_wr_s\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->kb_wr_s << ",";
		}

		out << "]," << endl;
		
		out << "\"cswch_s\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->cswch_s << ",";
		}

		out << "]," << endl;

		out << "\"fds\":[";
		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->fds << ",";
		}

		out << "]" << endl;

		out << "}," << endl;


	}
	out << "]" << endl;
	out << "}" << endl;
#endif


	return out;

}




#if 0
// original format
ostream& operator<<(ostream &out, const process_grp& in){

	out << "## " << in._label << "," << in._hash_index << "," << in._max_mem << "," << in._min_mem << "," << in._max_disk << "," << in._min_disk;
	out << "," << in._max_fds << "," << in._min_fds << "," << in._max_cpu << "," << in._min_cpu << "," << in._total_procs << endl;


	BOOST_FOREACH( HASH_MAP::value_type v, in._history ){

		history_key tmp = v.first;

		out << "## " << tmp.command << "," << tmp.hostname << "," << tmp.pid << endl;

		std::vector<pid_blob> tmp_vals = v.second;

		for (std::vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			boost::fusion::for_each(*it, out << arg1 << ",");
			out << endl;
		}


	}

	return out;

}

#endif


*/

}; // End of namespace clpr_d
