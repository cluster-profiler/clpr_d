#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include "clpr_db.hpp"


namespace clpr_d {

	// Constructor
	// Set the log file and the max number of database entries from conf_file
	clpr_db::clpr_db(const clpr_d::p_log& p_log_file, const clpr_d::p_conf& p_conf_file){

		this->p_log_file = p_log_file;
		this->p_log_file->write(CLPR_LOG_DEBUG,"Calling clpr_db constructor");

		this->db_max_entries = p_conf_file->get_db_max_entries();

		update_write_time();

	} // End of clpr_db constructor


	// Destructor
	clpr_db::~clpr_db() {
		this->p_log_file->write(CLPR_LOG_DEBUG,"Calling clpr_db destructor");
	} // End of clpr_db destructor	


	//// clpr_db::is_present
	// Finds if an index "idx" for a process group is already in the database
	bool clpr_db::is_present(const std::string& idx) {
		index_it = db_content.get<process_grp_label>().find(idx);

		if( index_it == db_content.get<process_grp_label>().end() ) {
			return false;
		} 
		
		return true;
	} // End clpr_db::is_present	

	
	void clpr_db::insert(clpr_d::process_grp_ptr pgrp_ptr) {
		db_content.insert(pgrp_ptr);
	}

	clpr_d::process_grp_ptr clpr_db::find(const std::string& pgrp_idx) {
		// Find the process group corresponding to pgrp_idx
		index_it = db_content.get<process_grp_label>().find(pgrp_idx);

		return *index_it;
	}

	// Write the stream op
	std::ostream& operator<<(std::ostream &out, boost::shared_ptr<clpr_db>& in) {

		//boost::mutex::scoped_lock lock(in.mutex_pgrp);
		//while(in._set.size()==0){
		//	in._ready.wait(lock);
		//}
		
		
		//in.mutex_pgrp.lock();
		struct utsname uname_data;
		uname(&uname_data);
		std::string hostname(uname_data.sysname);

		// Iterate over all of the process groups
		for( in->label_it = in->db_content.get<global_label>().begin(); \
			in->label_it != in->db_content.get<global_label>().end(); \
			++(in->label_it)) {
				out << *(in->label_it) << endl;
		}		

//		for( auto it = in.db_content.get<global_label>().begin(); \
//			it != in.db_content.get<global_label>().end(); \
//			++(it)) {
				//out << *it << endl;
//		}		
		//in.mutex_pgrp.unlock();
		//lock.unlock();
		//in.ready.notify_all();
		
		return out;
	}


/*
	// Update the process group indexed by pgrp_indx. If it does not exist, create it
	// TODO: Try/catch and logging
	void clpr_db::update(const vector<std::string>& tokens, const std::string& pgrp_idx, const std::string& host_info, uint64_t& label) {

		// For logging
		std::string msg;

		mutex_pgrp.lock();
		p_log_file->write(CLPR_LOG_DEBUG,"Mutex lock database for update");	

		// Find the process group corresponding to pgrp_idx
		index_it = db_content.get<process_grp_label>().find(pgrp_idx);

		// If it does not exist, then create it
		if( index_it == db_content.get<process_grp_label>().end() ) {

			msg = "Process group indexed by " + pgrp_idx + " could not be found";
			p_log_file->write(CLPR_LOG_DEBUG, msg);

			msg = "Creating process group " + pgrp_idx + " in database";
			p_log_file->write(CLPR_LOG_DEBUG, msg);

			// Create a new ptr to process grp
			process_grp_ptr pgrp_insert(new process_grp(tokens,pgrp_idx,label,p_log_file));


			// Insert data in process grp
			pgrp_insert->push_back(host_info,tokens);
			// Insert the ptr in the database
			msg = "Inserting process group " + pgrp_idx + " in database";
			p_log_file->write(CLPR_LOG_DEBUG, msg);

			db_content.insert(pgrp_insert);
//			++label;
		} 
		// Otherwise just push data in the process grp
		else {
			msg = "Process group indexed by " + pgrp_idx + " was found in database. Adding data now.";
			p_log_file->write(CLPR_LOG_DEBUG, msg);
			// Insert data in the process grp
			(*index_it)->push_back(host_info,tokens);
		}	
		mutex_pgrp.unlock();
		p_log_file->write(CLPR_LOG_DEBUG,"Unlocking database");	

	} // End of clpr_db::update			
*/	



/*

	// Grab a blob if it exists; if not, return a new one
	process_grp clpr_db::get_process_grp(const vector<string>& tokens, const string& pgrp_idx, const uint64_t& label){

		mutex_pgrp.lock();
		// Try to find the process group corresponding to pgrp_idx
		index_it = db_content.get<process_grp_label>().find(pgrp_idx);

		// If it exists, return it
		if ( db_content.get<process_grp_label>().end() != index_it ) {
			mutex_pgrp.unlock();
			return *index_it;

		}

		// If it does not exist, create a new process_grp
		mutex_pgrp.unlock();
		++label;

		return process_grp(tokens,pgrp_idx,label);
	}


	// insert; const elements so delete first if exists
	// FIXME this is crappy, probably should use smrt ptrs
	void clpr_db::insert(process_grp &in, string &i){

		mutex_pgrp.lock();
		index_it = db_content.get<process_grp_label>().find(i);
		if (db_content.get<process_grp_label>().end() != index_it){

			db_content.get<process_grp_label>().erase(index_it);
			db_content.insert(in);

			mutex_pgrp.unlock();	
			return;			

		}
		db_content.insert(in);
		mutex_pgrp.unlock();	
	}
*/

	// Stamp time
	void clpr_db::update_write_time() {

		mutex_time.lock();
		write_time = (uint64_t) time(NULL);
		mutex_time.unlock();

	}

	// Read time
	uint64_t clpr_db::read_write_time() {

		mutex_time.lock();
		uint64_t ret = write_time;
		mutex_time.unlock();

		return ret;
	}





	// Print by max_mem
/* 
	void clpr_db::print_by_max_mem(ofstream &file){

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
	void clpr_db::print_by_max_cpu(ofstream &file){

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
	void clpr_db::print_by_max_fds(ofstream &file){

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
	void clpr_db::print_by_max_dsk(ofstream &file){

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
	void clpr_db::clean_by_size(){

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
	void clpr_db::clean_by_time(){
		_mutex_blobs.lock();
		uint64_t t = read_write_time();
		for (	_label_it=_set.get<global_label>().begin(); _label_it != _set.get<global_label>().begin(); _label_it++){


			if ((t > _label_it->get_time()) && ((abs(t-_label_it->get_time()) / 3600) > CLPR_HOURS_BTWN_WRITES))
				_set.get<global_label>().erase(_label_it);


		}
		_mutex_blobs.unlock();


	}
*/


} // End of namespace clpr_d



