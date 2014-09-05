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

#ifndef _PROCESS_GRP_HPP_
#define _PROCESS_GRP_HPP_

#include <string>
#include <vector>
#include <iostream>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>

#include "clpr_log.hpp"
#include "proc_structures.hpp"
#include "process.hpp"


// Total time to wait before we drop a process group that has not been touched, in seconds 
#define CLPR_PROCESS_GRP_MAX_DELTA_UPDATE 7200 


using namespace boost;
using boost::phoenix::arg_names::arg1;

namespace clpr_d 
{
  
  //////////////////////////////////////////
  ////////////// TYPEDEFS //////////////////
  //////////////////////////////////////////
  // Key for history unordered_map in process_grp
  typedef struct
  {
    std::string command;
    int pid;
    uint64_t starttime;
  } process_key;
  
  // Hash for history unordered_map in process_grp
  typedef struct
  {
    size_t operator()(const process_key &in ) const
    {
      return boost::hash<std::string>()(in.command) ^			\
	boost::hash<std::string>()(std::to_string(in.pid)) ^ boost::hash<std::string>()(std::to_string(in.starttime));
    };
    
  } key_hash;

  /// Equality operator for history unordered_map in process_grp
  typedef struct
  {
    bool operator()(const process_key& l, const process_key& r) const
    {
      return (l.command.compare(r.command) == 0)			\
	&& ((std::to_string(l.pid)).compare(std::to_string(r.pid))==0) && \
	(std::to_string(l.starttime).compare(std::to_string(r.starttime))==0);
    };
  } key_eq;

  // Hash map for the aggregator of PIDs
  typedef unordered_map<process_key,
			//std::vector<clpr_d::process_ptr>,
			clpr_d::process_ptr,
			key_hash,
			key_eq > HASH_MAP;
  
  
  //////////////////////////////////////////
  ////////// CLASS DECLARATION /////////////
  //////////////////////////////////////////
  /// An aggregator of pidstat data, i.e. a group of processes
  // v0.1: Clustered by start date/time and uid
  // v0.2: Clustered by group ID (GID), and user ID
  class process_grp 
  {
  private:
    //// Mutable data
    uint64_t tstamp; // Last time this was touched
    HASH_MAP process_list; // List of processes and their history
    int total_process; // Total number of processes in that group
    
    //// Immutable data for lifetime of process_grp
    int pgid; // Group id
    int sid; // Session id
    std::string uid; // User id
    std::string hash_index; // Corresponding index
    
  public:
    process_grp(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus); 
    bool is_present(const process_key& pkey); 
    void insert(const process_key& pkey, const clpr_d::process_ptr& pproc_insert); 
    clpr_d::process_ptr find(const process_key& pkey); 
    void dump(std::string& line_header, std::ostream& out);
    // Timestamp this thing
    void update_tstamp();
    // Get the time	
    uint64_t const& get_tstamp() const;
    //// Getters
    std::string get_hash_index() const;
    /// Format the stream operator		
    friend std::ostream& operator<<(std::ostream &out, boost::shared_ptr<process_grp>& in);
    //		friend class clpr_db;
  }; // End of class process_grp
  
  typedef boost::shared_ptr<process_grp> process_grp_ptr;
  
}; // End of namespace clpr_d

#endif
