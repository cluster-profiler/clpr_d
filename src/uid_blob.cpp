#include "uid_blob.hpp"

namespace clpr_d{
	/// 
	void uid_blob::set(string host_info,vector<string> tokens){

		history_key key;
		key.command="";

		for (int i=26; i<tokens.size(); i++){
			key.command			+= tokens[i];
			key.command			+= " ";
		}

		key.hostname		+= host_info;
		key.pid			+= tokens[7];

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

	string uid_blob::get_header() const{

		stringstream ss;

		ss << "## " << _label << "," << _hash_index << "," << _max_mem << "," << _min_mem << "," << _max_disk << "," << _min_disk;
		ss << "," << _max_fds << "," << _min_fds << "," << _max_cpu << "," << _min_cpu << "," << _total_procs;
		return ss.str();

	}

	uid_blob::uid_blob(string &in, uint64_t &label):
		_hash_index(in),
		_label(label),
		_max_mem(FLT_MIN),
		_min_mem(FLT_MAX),
		_max_disk(FLT_MIN),
		_min_disk(FLT_MAX),
		_max_fds(FLT_MIN),
		_min_fds(FLT_MAX),
		_max_cpu(FLT_MIN),
		_min_cpu(FLT_MAX),
		_total_procs(0)
	{update_time();};

	void uid_blob::update_time(){

		_time = (uint64_t) time(NULL);


	}

	uint64_t uid_blob::get_time() const{

		return _time;
	}

};
