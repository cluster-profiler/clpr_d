/**
 * @file clpr_proc_db.hpp
 * @author  Bill Brouwer <whiksyejulietb@gmail.com>
 * @version 1.0
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
#include <mutex>
#include <algorithm>
#include <iostream>
#include <iterator>


#include "key_defines.hpp"
#include "grp_proc.hpp"

using namespace std;
using namespace boost;
using namespace boost::multi_index;


namespace clpr_d{


	///a container for process data, allowing multiple search/sort
	class clpr_proc_db {


		typedef multi_index_container<
			grp_proc,
			indexed_by<
				hashed_unique<
					tag<grp_proc_label>,BOOST_MULTI_INDEX_MEMBER(grp_proc,string,hash_index)>,
				ordered_unique<
					tag<global_label>,BOOST_MULTI_INDEX_MEMBER(grp_proc,uint64_t,label)>,
				ordered_non_unique<
					tag<max_mem>,BOOST_MULTI_INDEX_MEMBER(grp_proc,float,max_mem)>,
				ordered_non_unique<
					tag<min_mem>,BOOST_MULTI_INDEX_MEMBER(grp_proc,float,min_mem)>,
				ordered_non_unique<
					tag<max_disk>,BOOST_MULTI_INDEX_MEMBER(grp_proc,float,max_disk)>,
				ordered_non_unique<
					tag<min_disk>,BOOST_MULTI_INDEX_MEMBER(grp_proc,float,min_disk)>,
				ordered_non_unique<	
					tag<max_fds>, BOOST_MULTI_INDEX_MEMBER(grp_proc,float,max_fds)>,
				ordered_non_unique<
					tag<min_fds>, BOOST_MULTI_INDEX_MEMBER(grp_proc,float,min_fds)>,
				ordered_non_unique<
					tag<max_cpu>, BOOST_MULTI_INDEX_MEMBER(grp_proc,float,max_cpu)>,
				ordered_non_unique<
					tag<min_cpu>, BOOST_MULTI_INDEX_MEMBER(grp_proc,float,min_cpu)> >
				> blob_proc_db;

		/// iterators
		typedef blob_proc_db::index<grp_proc_label>::type::iterator ind_it;
		typedef blob_proc_db::index<global_label>::type::iterator lab_it;
		typedef blob_proc_db::index<min_mem>::type::iterator mmin_it;
		typedef blob_proc_db::index<max_mem>::type::iterator mmax_it;
		typedef blob_proc_db::index<min_cpu>::type::iterator cmin_it;
		typedef blob_proc_db::index<max_cpu>::type::iterator cmax_it;
		typedef blob_proc_db::index<min_fds>::type::iterator fmin_it;
		typedef blob_proc_db::index<max_fds>::type::iterator fmax_it;
		typedef blob_proc_db::index<min_disk>::type::iterator dmin_it;
		typedef blob_proc_db::index<max_disk>::type::iterator dmax_it;




		private :

		/// for setting/getting blobs
		boost::mutex _mutex_blobs;
		/// for setting/getting time
		boost::mutex _mutex_time;
		/// data ready condition
		boost::condition _ready;
		/// iterators for different indices
		lab_it _label_it;
		ind_it _index_it;
		mmin_it _min_mem_it;
		mmax_it _max_mem_it;
		cmin_it _min_cpu_it;
		cmax_it _max_cpu_it;
		fmin_it _min_fds_it;
		fmax_it _max_fds_it;
		dmin_it _min_dsk_it;
		dmax_it _max_dsk_it;
		blob_proc_db _set;

		/// when this was last written to a socket
		uint64_t write_time;

		public :

		~clpr_proc_db(){};


		/**
		 * Construct the database, a collection of grp_procs
		 * @param 
		 */
		clpr_proc_db();


		/**
		 * Take a grp_proc and add in to the db
		 * @param grp_proc input 
		 * @param string& (hashed) index
		 */	
		void insert(grp_proc& in, string &i);

		///get the size of the map
		int size();

		/// release data
		void clean_by_time();

		/// release data
		void clean_by_size();

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
		grp_proc get(vector<string>& tokens, string &i, uint64_t &label);

		///send all out to stream
		friend ostream& operator<<(ostream &out, clpr_proc_db& in);

		/// dump to file, ordered by max_mem
		void print_by_max_mem(ofstream &file);

		/// dump to file, ordered by max_cpu
		void print_by_max_cpu(ofstream &file);

		/// dump to file, ordered by max_fds
		void print_by_max_fds(ofstream &file);
		
		/// dump to file, ordered by max_dsk
		void print_by_max_dsk(ofstream &file);

	};

};

#endif
