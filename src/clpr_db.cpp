#include <ctime>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>

#include "clpr_db.hpp"


namespace clpr_d {

	// Constructor
	// Set the log file and the max number of database entries from conf_file
	clpr_db::clpr_db(const clpr_d::log_ptr& log_file, const clpr_d::conf_ptr& conf_file){

		this->log_file = log_file;
		this->log_file->write(CLPR_LOG_DEBUG,"Calling clpr_db constructor");

		this->db_max_entries = conf_file->get_db_max_entries();

		this->tstamp = time(NULL);

	} // End of clpr_db constructor


	// Destructor
	clpr_db::~clpr_db() {
		this->log_file->write(CLPR_LOG_DEBUG,"Calling clpr_db destructor");
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


	void clpr_db::set_time_stamp(uint64_t const& tstamp) {
		this->tstamp = tstamp;
	}	

	
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
		out << "Hostname UID STARTTIME PID PPID TSTAMP TTY PUSR PSYS PGST PCPU CORE# NTH MINFLT MAJFLT VSZ RSS PMEM RCHAR WCHAR SYSCR SYSCW RD_B WR_B CC_WR_B IODELAY CSWCH NVCSWCH NFD WCHAN STATE CMD ENV FDS" << std::endl; 

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



