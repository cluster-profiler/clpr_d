#include "grp_proc.hpp"

namespace clpr_d {

// Constructor
grp_proc::grp_proc(const vector<string>& tokens, const string& in, const uint64_t& label):
	hash_index(in),
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

	uid 		= get_uid(tokens);
//	gid 		= get_gid(tokens);
};

void grp_proc::update_time(){

	tstamp = (uint64_t) time(NULL);

}

// Time getter
uint64_t const& grp_proc::get_time() const{

	return tstamp;
}

string grp_proc::get_header() const{

	stringstream ss;

	ss << "## " << label << "," << hash_index << "," << max_mem << "," << min_mem << "," << max_disk << "," << min_disk;
	ss << "," << max_fds << "," << min_fds << "," << max_cpu << "," << min_cpu << "," << total_process;
	return ss.str();

}

// TODO: Sanitize fields; report fields that are fucked up 
void grp_proc::update(const string& host_info, vector<string> &tokens){

	history_key key;
	char spec_chars[] = "\n\t";

	// Do not add blank space before command
	key.command = "";

	// Parse the command (can be contained in multiple tokens)
	for (int i=CMD_POS; i<tokens.size(); ++i)
	{
		// Find double quote character, replace with single character
		std::replace( tokens[i].begin(), tokens[i].end(), '\"', '\'');	

		// Remove other special characters
		for (unsigned int j = 0; j < strlen(spec_chars); ++j)
			tokens[i].erase (std::remove(tokens[i].begin(), tokens[i].end(), spec_chars[j]), tokens[i].end());

		key.command += tokens[i];

		// Do not add blank space at the end of the command, but add space between parts of the command
		if( i != tokens.size() -1 )	
			key.command			+= " ";
	}

	// Store the hostname and the PID for that particular entry
	key.hostname 	+= host_info;
	key.pid		+= tokens[PID_POS];

	pid_data value = get_pid_data(tokens);

	vector<pid_data> tmp = history[key];

	tmp.push_back(value);
	history[key] = tmp;

	total_process = history.size();

	// Update statistics
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

/*
// something approaching json format
ostream& operator<<(ostream &out, const grp_proc& in){

	string kill_me="abcdefghijklmnopqrstuvwxyz";

	// CSV format for the output
	// Data is denormalized 
	// Data out:
	// grp_proc_id, start time, max mem, min mem, max disk, min disk, max fds, min fds, max cpu, min cpu, total processes, ...
	// ... 
	BOOST_FOREACH( HASH_MAP::value_type v, in._history ) {	

		history_key tmp = v.first;
		vector<pid_blob> tmp_vals = v.second;

		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){


			// if there are alphas in pid, something screwed up, probably not with pidstat, but with the data gathering process
			bool test_for_alpha=false;

			for (auto& x: kill_me) 
				if (tmp.pid.find(x) < tmp.pid.size()){
					test_for_alpha=true;
					break;
				}


			if (test_for_alpha)
				cerr << "whoa; grp_proc with index " << in._label << " has an ill-formed pid : " << tmp.pid << endl; 

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

		vector<pid_blob> tmp_vals = v.second;

		out << "\"time\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->time << ",";
		}

		out << "]," << endl;
		
		out << "\"user\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->user << ",";
		}

		out << "]," << endl;
		
		out << "\"\% cpu\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->perc_cpu << ",";
		}

		out << "]," << endl;
		
		out << "\"majflt_s\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->majflt_s << ",";
		}

		out << "]," << endl;
		
		out << "\"vsz\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->vsz << ",";
		}

		out << "]," << endl;
		
		out << "\"kd_rd_s\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->kb_rd_s << ",";
		}

		out << "]," << endl;
		
		out << "\"kb_wr_s\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->kb_wr_s << ",";
		}

		out << "]," << endl;
		
		out << "\"cswch_s\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			out << it->cswch_s << ",";
		}

		out << "]," << endl;

		out << "\"fds\":[";
		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
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
ostream& operator<<(ostream &out, const grp_proc& in){

	out << "## " << in._label << "," << in._hash_index << "," << in._max_mem << "," << in._min_mem << "," << in._max_disk << "," << in._min_disk;
	out << "," << in._max_fds << "," << in._min_fds << "," << in._max_cpu << "," << in._min_cpu << "," << in._total_procs << endl;


	BOOST_FOREACH( HASH_MAP::value_type v, in._history ){

		history_key tmp = v.first;

		out << "## " << tmp.command << "," << tmp.hostname << "," << tmp.pid << endl;

		vector<pid_blob> tmp_vals = v.second;

		for (vector<pid_blob>::iterator it = tmp_vals.begin(); it != tmp_vals.end(); it++){
			boost::fusion::for_each(*it, out << arg1 << ",");
			out << endl;
		}


	}

	return out;

}

#endif


*/
}; // End of namespace clpr_d
