#include "uid_blob.hpp"

namespace clpr_d{
	/// 
	void uid_blob::set(string host_info,vector<string> tokens){

		history_key key;
		char specChars[] = "\n\t";

		// Do not add blank space before command
		key.command="";

		for (int i=CMD_POS; i<tokens.size(); i++){

			// Find double quote character, replace with single character
			std::replace( tokens[i].begin(), tokens[i].end(), '\"', '\'');	
			//std::replace( tokens[i].begin(), tokens[i].end(), ',', '.');	

			// Remove other special characters
			for (unsigned int j = 0; j < strlen(specChars); ++j)
				tokens[i].erase (std::remove(tokens[i].begin(), tokens[i].end(), specChars[j]), tokens[i].end());

			key.command			+= tokens[i];

			// Do not add blank space at the end of the command, but add space between parts of the command
			if( i != tokens.size() -1 )	
				key.command			+= " ";
		}

		key.hostname		+= host_info;
		key.pid			+= tokens[PID_POS];

		pid_blob value = get_pid_blob(tokens);


		vector<pid_blob> tmp = _history[key];


		tmp.push_back(value);
		_history[key] = tmp;

		_total_procs=_history.size();


		///statistics
		if (_max_mem < value.mem) 			_max_mem = value.mem;
		if (_min_mem > value.mem) 			_min_mem = value.mem;
		if (_max_disk < (value.kb_rd_s+value.kb_wr_s)) 	_max_disk= value.kb_rd_s+value.kb_wr_s;
		if (_min_disk > (value.kb_rd_s+value.kb_wr_s)) 	_min_disk= value.kb_rd_s+value.kb_wr_s;
		if (_max_fds < value.fds)			_max_fds = value.fds;
		if (_min_fds > value.fds)			_min_fds = value.fds;
		if (_max_cpu < value.perc_cpu)			_max_cpu = value.perc_cpu;
		if (_min_cpu > value.perc_cpu)			_min_cpu = value.perc_cpu;


		update_time();

	}

	// something approaching json format
	ostream& operator<<(ostream &out, const uid_blob& in){

		string kill_me="abcdefghijklmnopqrstuvwxyz";

		// CSV format for the output
		// Data is denormalized 
		// Data out:
		// uid_blob_id, start time, max mem, min mem, max disk, min disk, max fds, min fds, max cpu, min cpu, total processes, ...
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
	ostream& operator<<(ostream &out, const uid_blob& in){

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
	string uid_blob::get_header() const{

		stringstream ss;

		ss << "## " << _label << "," << _hash_index << "," << _max_mem << "," << _min_mem << "," << _max_disk << "," << _min_disk;
		ss << "," << _max_fds << "," << _min_fds << "," << _max_cpu << "," << _min_cpu << "," << _total_procs;
		return ss.str();

	}

	uid_blob::uid_blob(vector<string>& tokens, string &in, uint64_t &label):
		_hash_index(in),
		_label(label),
		_max_mem(0.0),
		_min_mem(0.0),
		_max_disk(0.0),
		_min_disk(0.0),
		_max_fds(0.0),
		_min_fds(0.0),
		_max_cpu(0.0),
		_min_cpu(0.0),
		_total_procs(0)
	{
		update_time();
		_start_time 	= get_start_time(tokens);
		_uid 		= get_uid(tokens);


	};

	void uid_blob::update_time(){

		_time = (uint64_t) time(NULL);


	}

	uint64_t uid_blob::get_time() const{

		return _time;
	}

};
