/**
 * @file process_grp.hpp
 * @author Bill Brouwer <whiskeyjulietb@gmail.com>
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
 *
 * @section DESCRIPTION
 *
 * Main aggregate object for a process collection
 */


#ifndef _GRP_PROC_ 
#define _GRP_PROC_ 

#include <boost/spirit/include/phoenix.hpp>

#include "clpr_log.hpp"
#include "pid_data.hpp"

#include "utilities.hpp"

using namespace boost;
using boost::phoenix::arg_names::arg1;

namespace clpr_d {

//////////////////////////////////////////
////////////// TYPEDEFS //////////////////
//////////////////////////////////////////
// Key for history unordered_map in process_grp
typedef struct{
	string command;
	string pid;
	string hostname;
} history_key;

// Hash for history unordered_map in process_grp
typedef struct{
	size_t operator()(const history_key &in ) const
	{
		return boost::hash<string>()(in.command) ^ \
			boost::hash<string>()(in.pid) ^ boost::hash<string>()(in.hostname);
	};

} key_hash;

/// Equality operator for history unordered_map in process_grp
typedef struct{
	bool operator()(const history_key& l, const history_key& r) const
	{
		return (l.command.compare(r.command) == 0) \
						      && (l.pid.compare(r.pid)==0) && \
						      (l.hostname.compare(r.hostname)==0);
	};

} key_eq;

// Hash map for the aggregator of PIDs
typedef unordered_map<history_key,
			vector<pid_data>,
			key_hash,
			key_eq > HASH_MAP;


//////////////////////////////////////////
////////// CLASS DECLARATION /////////////
//////////////////////////////////////////
/// An aggregator of pidstat data, i.e. a group of processes
// v0.1: Clustered by start date/time and uid
// v0.2: Clustered by group ID (GID), and user ID
class process_grp {

	private:
		// History of the aggregated processes
		HASH_MAP history;

		//
		string hash_index;
		// A global label, sort of
		uint64_t label;
		// Last time this was touched
		uint64_t tstamp;

		// Aggregator index
		string uid;
		string start_time;
		string gid;

		/// Some overall data for the group
		//
		float max_mem;
		//
		float min_mem;
		// 
		float max_disk;
		//
		float min_disk;
		//
		float max_fds;
		//
		float min_fds;
		//
		float max_cpu;
		//
		float min_cpu;

		// Total number of processes
		int total_process;

		// Log file
		clpr_d::p_log p_log_file;

	public:
		/**
		 * Build a group of processes process_grp with a unique string and integer label
		 * @param vector<string>& tokens 
		 * @param string& in (what will be hash index in db)
		 * @param uint64_t& label integer 
		 */	
		process_grp(const vector<string>& tokens, const string& in,const uint64_t& label, const clpr_d::p_log& p_log_file);

		/// bye bye
		~process_grp();

		void push_back(const string& host_info, const vector<string> &tokens);
		// Update
		/**
		 * Update a process_grp from input tokens and hostname
		 * @param string hostname
		 * @param vector<string> tokens input data
		 */	
//		void update(const string& host_info,vector<string>& tokens);
		/// return the header as a string
		string get_header() const;

		/// timestamp this thing
		void update_time();

		/// get the time	
		uint64_t const& get_time() const;

		//// Getters
		std::string get_hash_index() const;
		uint64_t get_label() const;
		float get_max_mem() const;
		float get_min_mem() const;

		float get_max_disk() const;
		float get_min_disk() const;

		float get_max_fds() const;
		float get_min_fds() const;
		
		float get_max_cpu() const;
		float get_min_cpu() const;




		/// format the stream operator		
		friend ostream& operator<<(ostream &out, const process_grp& in);

//		friend class clpr_db;
};

typedef boost::shared_ptr<process_grp> p_pgrp;

}; // End of namespace clpr_d

#endif
