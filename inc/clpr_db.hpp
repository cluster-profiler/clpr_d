/**
 * @file clpr_db.hpp
 * @author  Bill Brouwer <whiksyejulietb@gmail.com>
 * @author Pierre-Yves Taunay <py.taunay@psu.edu>
 * @version 0.2
 *
 * @section LICENSE
 * Copyright 2014 William J. Brouwer, Pierre-Yves Taunay
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * @section LICENSE
 *
 * 
 *
 * @section DESCRIPTION
 *
 * the main database for the daemon,  a mighty thin wrapper around boost::multi_index_container
 */


#ifndef _CLPR_DB_HPP_
#define _CLPR_DB_HPP_

//// STD
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <map>

//// Boost
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/shared_ptr.hpp>

//// Project 
#include "process_grp.hpp"

using namespace std;
using namespace boost;

namespace clpr_d {

// A container for process data, allowing multiple search/sort
class clpr_db {

	private:

		// For setting/getting process groups 
		boost::mutex mutex_pgrp;
		// For setting/getting time
		boost::mutex mutex_time;
		// Data ready condition
		boost::condition ready;

		// Actual database content
		std::map<std::string, process_grp_ptr> db_content;

		// When was this last touched ? aka current time stamp
		uint64_t tstamp;

		// Dependency injection -- smart pointer for the log file
		clpr_d::log_ptr log_file;

		// Maximum number of entries in the database for cleanup
		int db_max_entries;


	public:

		/** 
		 * Destructor of the database
		 *
		 */
		~clpr_db();


		/**
		 * Construct the database, a collection of process_grps
		 *
		 */
		clpr_db(const clpr_d::log_ptr& log_file, const clpr_d::conf_ptr& conf_file);

		bool is_present(const std::string& idx); 
		void insert(const std::pair<std::string,process_grp_ptr>& in); 
		clpr_d::process_grp_ptr find(const std::string& pgrp_idx); 

		void dump(std::ostream& out); 

		uint64_t const& get_time_stamp() const;
		void set_time_stamp(uint64_t const& tstamp);

		/// update write time
		void update_write_time();

		/// read write time
		uint64_t read_write_time();

		// Send all out to stream
		friend std::ostream& operator<<(std::ostream &out, boost::shared_ptr<clpr_db>& in);

}; // End of class clpr_db

// Typedef for shared pointer
typedef boost::shared_ptr<clpr_db> db_ptr;

}; // End of namespace clpr_d

#endif
