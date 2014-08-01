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


#ifndef _CLPR_DB_
#define _CLPR_DB_

#ifndef NDEBUG

#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE

#endif

//// STD
#include <mutex>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

//// Boost
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/shared_ptr.hpp>

//// Project 
#include "key_defines.hpp"
#include "process_grp.hpp"

using namespace std;
using namespace boost;
using namespace boost::multi_index;

namespace clpr_d {

//// Labels for the multi index
/// convenience label for the database
struct global_label{};
/// convenience label for the database
struct index{};
/// convenience label for the database
struct max_mem{};
/// convenience label for the database
struct min_mem{};
/// convenience label for the database
struct max_disk{};
/// convenience label for the database
struct min_disk{};
/// convenience label for the database
struct max_fds{};
/// convenience label for the database
struct min_fds{};
/// convenience label for the database
struct max_cpu{};
/// convenience label for the database
struct min_cpu{};
/// convenience label for the database
struct process_grp_label{};



// A container for process data, allowing multiple search/sort
class clpr_db {

/*
	typedef multi_index_container<
		boost::shared_ptr<process_grp>,
		indexed_by<
			hashed_unique<
				tag<process_grp_label>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,std::string,get_hash_index) 
				>,
			ordered_unique<
				tag<global_label>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,uint64_t,get_label) 
				>,
			ordered_non_unique<
				tag<max_mem>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_max_mem) 
				>,
			ordered_non_unique<
				tag<min_mem>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_min_mem) 
				>,
			ordered_non_unique<
				tag<max_disk>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_max_disk) 
				>,
			ordered_non_unique<
				tag<min_disk>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_min_disk) 
				>,
			ordered_non_unique<	
				tag<max_fds>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_max_fds) 
				>,
			ordered_non_unique<
				tag<min_fds>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_min_fds) 
				>,
			ordered_non_unique<
				tag<max_cpu>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_max_cpu)
				>,
			ordered_non_unique<
				tag<min_cpu>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,float,get_min_cpu)
				> 
		> // End of indexed_by
	> process_grp_db; // End of multi_index_container
*/

	typedef multi_index_container<
		boost::shared_ptr<process_grp>,
		indexed_by<
			hashed_unique<
				tag<process_grp_label>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,std::string,get_hash_index) 
				>,
			ordered_unique<
				tag<global_label>,
				BOOST_MULTI_INDEX_CONST_MEM_FUN(process_grp,uint64_t,get_label) 
				>
		> // End of indexed_by
	> process_grp_db; // End of multi_index_container

	// Iterators to crawl over database
	typedef process_grp_db::index<process_grp_label>::type::iterator ind_it;
	typedef process_grp_db::index<global_label>::type::iterator lab_it;
	/*
	typedef process_grp_db::index<min_mem>::type::iterator mmin_it;
	typedef process_grp_db::index<max_mem>::type::iterator mmax_it;
	typedef process_grp_db::index<min_cpu>::type::iterator cmin_it;
	typedef process_grp_db::index<max_cpu>::type::iterator cmax_it;
	typedef process_grp_db::index<min_fds>::type::iterator fmin_it;
	typedef process_grp_db::index<max_fds>::type::iterator fmax_it;
	typedef process_grp_db::index<min_disk>::type::iterator dmin_it;
	typedef process_grp_db::index<max_disk>::type::iterator dmax_it;
*/
	private:

		// For setting/getting process groups 
		boost::mutex mutex_pgrp;
		// For setting/getting time
		boost::mutex mutex_time;
		// Data ready condition
		boost::condition ready;
		// Iterators for different indices
		lab_it label_it;
		ind_it index_it;
		/*
		mmin_it min_mem_it;
		mmax_it max_mem_it;
		cmin_it min_cpu_it;
		cmax_it max_cpu_it;
		fmin_it min_fds_it;
		fmax_it max_fds_it;
		dmin_it min_dsk_it;
		dmax_it max_dsk_it;
*/
		// Actual database content
		process_grp_db db_content;

		// When this was last written to a socket
		uint64_t write_time;

		// Dependency injection -- smart pointer for the log file
		clpr_d::p_log p_log_file;

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
		clpr_db(const clpr_d::p_log& p_log_file, const clpr_d::p_conf& p_conf_file);


		/**
		 * Update the process group indexed by pgrp_idx. If it does not exist, create it.
		 * @param[in] tokens a line in pid_stat in a string format
		 * @param[in] pgrp_idx hashed index
		 * @param[in] label
		 *
		 */
		void update(const vector<std::string>& tokens, const std::string& pgrp_idx, const std::string& host_info, uint64_t& label); 


		/**
		 * Take a process_grp and add in to the db
		 * @param process_grp input 
		 * @param string& (hashed) index
		 */	
//		void insert(process_grp& in, string &i);


		bool is_present(const std::string& idx); 
		void insert(clpr_d::process_grp_ptr pgrp_ptr); 
		clpr_d::process_grp_ptr find(const std::string& pgrp_idx); 

/*
		///get the size of the map
		int size();

		/// release data
		void clean_by_time();

		/// release data
		void clean_by_size();

*/
		/// update write time
		void update_write_time();

		/// read write time
		uint64_t read_write_time();
		/**
		 * Get a blob by hashed index
		 * @param string& i index
		 * @param uint64_t& label
		 *
		 */
	//	process_grp get_process_grp(const vector<string>& tokens,const string &pgrp_idx, const uint64_t& label);



		


		// Send all out to stream
		//friend std::ostream& operator<<(std::ostream &out, clpr_db& in);
		//friend std::ifstream& operator<<(std::ifstream &out, clpr_db& in);
		friend std::ifstream& operator<<(std::ifstream &out, boost::shared_ptr<clpr_db>& in);

/*
		// Dump to file, ordered by max_mem
		void print_by_max_mem(ofstream &file);

		// Dump to file, ordered by max_cpu
		void print_by_max_cpu(ofstream &file);

		// Dump to file, ordered by max_fds
		void print_by_max_fds(ofstream &file);
		
		// Dump to file, ordered by max_dsk
		void print_by_max_dsk(ofstream &file);
*/


}; // End of class clpr_db

// Typedef for shared pointer
typedef boost::shared_ptr<clpr_db> db_ptr;

}; // End of namespace clpr_d

#endif
