/**
 * @file uid_blob.hpp
 * @author  Bill Brouwer <whiskeyjulietb@gmail.com>
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
 *
 * @section DESCRIPTION
 *
 * Main aggregate object for a process collection
 */


#ifndef _UID_BLOB
#define _UID_BLOB

#include "utilities.hpp"
#include <float.h>
#include <boost/spirit/include/phoenix.hpp>

using namespace boost;
using boost::phoenix::arg_names::arg1;
namespace clpr_d{


	/// an aggregator of pidstat data, clustered by start date/time and uid
	class uid_blob{



		private:
			

			/// all your base are belong to us
			HASH_MAP _history;
			///
			string _hash_index;
			/// a global label, sort of
			uint64_t _label;
			/// last time this was touched
			uint64_t _time;
			string _uid;
			string _start_time;

			///
			float _max_mem;
			///
			float _min_mem;
			/// 
			float _max_disk;
			///
			float _min_disk;
			///
			float _max_fds;
			///
			float _min_fds;
			///
			float _max_cpu;
			///
			float _min_cpu;
			///
			int _total_procs;


		public:


			/**
			 * Build a uid_blob with a unique string and integer label
			 * @param vector<string>& tokens 
			 * @param string& in (what will be hash index in db)
			 * @param uint64_t& label integer 
			 */	
			uid_blob(vector<string>& tokens, string &in, uint64_t &label);

			/// update

			/**
			 * Update a uid_blob from input tokens and hostname
			 * @param string hostname
			 * @param vector<string> tokens input data
			 */	
			void set(string host_info,vector<string> tokens);
			/// return the header as a string
			string get_header() const;
			/// 
			uid_blob(){};
			/// bye bye
			~uid_blob(){};
	
			/// timestamp this thing
			void update_time();

			/// get the time	
			uint64_t get_time() const;

			/// format the stream operator		
			friend ostream& operator<<(ostream &out, const uid_blob& in);


			friend class clpr_proc_db;
	};

};
#endif
