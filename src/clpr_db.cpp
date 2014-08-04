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
		auto index_it = db_content.find(idx);

		if( index_it == db_content.end() ) {
			return false;
		} 
		
		return true;
	} // End clpr_db::is_present	

	
//	void clpr_db::insert(const clpr_d::process_grp_ptr& pgrp_ptr) {
	void clpr_db::insert(const std::pair<std::string,process_grp_ptr>& in) {
		db_content.insert(in);
	} // End clpr_db::insert

	clpr_d::process_grp_ptr clpr_db::find(const std::string& pgrp_idx) {
		// Find the process group corresponding to pgrp_idx
		auto index_it = db_content.find(pgrp_idx);

		return (index_it->second);
	} // End clpr_db::find


	void clpr_db::dump(std::ostream& out) {
		struct utsname uname_data;
		uname(&uname_data);
		std::string line_header(uname_data.nodename);

		// Add the header
		out << "Hostname UID STARTTIME PID PPID TTY PCPU PSYS PUSR PGST CORE# NTH MINFLT MAJFLT VSZ RSS PMEM RCHAR WCHAR SYSCR SYSCW RD_B WR_B CC_WR_B IODELAY CSWCH NVCSWCH FDS WCHAN STATE CMD" << std::endl; 

		for(auto it = db_content.begin(); \
			it != db_content.end(); \
			++it) {
				it->second->dump(line_header,out);
				line_header.clear();
				line_header = uname_data.nodename;
		}	
	} // End clpr_db::dump


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
		for( auto it = in->db_content.begin(); \
			it != in->db_content.end(); \
			++it) {
				out << it->second << endl;
		}		

		//in.mutex_pgrp.unlock();
		//lock.unlock();
		//in.ready.notify_all();
		
		return out;
	}

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



/*
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



