#include "clpr_proc_db.hpp"

namespace clpr_d{

	//
	clpr_proc_db::clpr_proc_db(){

		update_write_time();

	}

	// grab a blob if it exists; if not, return a new one
	uid_blob clpr_proc_db::get(string& i, uint64_t &label){
		_mutex_blobs.lock();
		_index_it=_set.get<uid_blob_label>().find(i);


		if (_set.get<uid_blob_label>().end() != _index_it){
			_mutex_blobs.unlock();
			return *_index_it;

		}
		_mutex_blobs.unlock();
		label++;
		return uid_blob(i,label);

	}


	// insert; const elements so delete first if exists
	// FIXME this is crappy, probably should use smrt ptrs
	void clpr_proc_db::insert(uid_blob &in, string &i){
	
		_mutex_blobs.lock();
		_index_it=_set.get<uid_blob_label>().find(i);
		if (_set.get<uid_blob_label>().end() != _index_it){

			_set.get<uid_blob_label>().erase(_index_it);
			_set.insert(in);

			_mutex_blobs.unlock();	
			return;			

		}
		_set.insert(in);
		_mutex_blobs.unlock();	
	}

	// stamp time
	void clpr_proc_db::update_write_time(){

		_mutex_time.lock();
		write_time = (uint64_t) time(NULL);
		_mutex_time.unlock();

	}

	// read time
	uint64_t clpr_proc_db::read_write_time(){

		_mutex_time.lock();
		uint64_t ret = write_time;
		_mutex_time.unlock();
		return ret;
	}


	// write the stream op
	ostream& operator<<(ostream &out, clpr_proc_db& in) {

		//boost::mutex::scoped_lock lock(in._mutex_blobs);
		//while(in._set.size()==0){
		//	in._ready.wait(lock);
		//}
		in._mutex_blobs.lock();
		for (in._label_it=in._set.get<global_label>().begin(); \
				in._label_it != in._set.get<global_label>().end(); in._label_it++)
			out << *in._label_it << endl;
		in._mutex_blobs.unlock();
		//lock.unlock();
		//in._ready.notify_all();
		return out;
	}
	// print by max_mem
	void clpr_proc_db::print_by_max_mem(ofstream &file){

		_mutex_blobs.lock();

		time_t current_time = time(NULL);
		file << ctime(&current_time) << endl;

		int k=0;
		for( _max_mem_it=_set.get<max_mem>().end(); _max_mem_it != _set.get<max_mem>().begin(); _max_mem_it--){

			if (k> CLPR_LOGGER_ENTRIES)
				break;

			if (k>0)
				file << _max_mem_it->get_header() << endl;

				k++;
		}

		_mutex_blobs.unlock();

	}
	// print by max_cpu
	void clpr_proc_db::print_by_max_cpu(ofstream &file){

		_mutex_blobs.lock();

		time_t current_time = time(NULL);
		file << ctime(&current_time) << endl;

		int k=0;
		
		for(_max_cpu_it=_set.get<max_cpu>().end(); _max_cpu_it != _set.get<max_cpu>().begin(); _max_cpu_it--){

			if (k> CLPR_LOGGER_ENTRIES)
				break;

			if (k>0)
			file << _max_cpu_it -> get_header() << endl;

			k++;
		}

		_mutex_blobs.unlock();

	}

	// print by max_fds
	void clpr_proc_db::print_by_max_fds(ofstream &file){

		_mutex_blobs.lock();

		time_t current_time = time(NULL);
		file << ctime(&current_time) << endl;

		int k=0;
		for(_max_fds_it=_set.get<max_fds>().end(); _max_fds_it != _set.get<max_fds>().begin(); _max_fds_it--){

			if (k> CLPR_LOGGER_ENTRIES)
				break;

			if (k>0)
			file << _max_fds_it -> get_header();

			k++;
		}

		_mutex_blobs.unlock();

	}

	// print by max_dsk
	void clpr_proc_db::print_by_max_dsk(ofstream &file){

		_mutex_blobs.lock();

		time_t current_time = time(NULL);
		file << ctime(&current_time) << endl;
		int k=0;
		for (_max_dsk_it=_set.get<max_disk>().end(); _max_dsk_it != _set.get<max_disk>().begin(); _max_dsk_it--){

			if (k> CLPR_LOGGER_ENTRIES)
				break;

			if (k>0)
			file << _max_dsk_it -> get_header();

			k++;
		}

		_mutex_blobs.unlock();

	}

	// clean up entries over size, keeping newest by global label
	void clpr_proc_db::clean_by_size(){

		int k=0;
		_mutex_blobs.lock();
		for(_label_it=_set.get<global_label>().end(); _label_it != _set.get<global_label>().begin(); _label_it--){

			if (k>MAX_DB_ENTRIES)
				_set.get<global_label>().erase(_label_it);


			k++;
		}
		_mutex_blobs.unlock();


	}


	// clean up according to timestamps
	void clpr_proc_db::clean_by_time(){
		_mutex_blobs.lock();
		uint64_t t = read_write_time();
	for (	_label_it=_set.get<global_label>().begin(); _label_it != _set.get<global_label>().begin(); _label_it++){


			if ((t > _label_it->get_time()) && ((abs(t-_label_it->get_time()) / 3600) > CLPR_HOURS_BTWN_WRITES))
				_set.get<global_label>().erase(_label_it);


		}
		_mutex_blobs.unlock();


	}

}



