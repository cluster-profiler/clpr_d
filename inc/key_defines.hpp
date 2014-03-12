/**
 * @file key_defines.hpp
 * @author  Bill Brouwer <whiskeyjulietb@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 * Copyright 2014 William J. Brouwer
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
 * key definitions
 */


#ifndef _KEY_DEFINES
#define _KEY_DEFINES
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <sstream>
#include <iterator>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <map>


using namespace std;
using namespace boost;
namespace clpr_d{

	/// named pipe to read
#define CLPR_INPUT_PIPE "clpr_input"
	/// output log filename
#define CLPR_OUTPUT_LOG "clpr_log.txt"
	/// sleep time btwn log outputs in mins
#define CLPR_LOGGER_WAIT 1
	/// how many items to list in logger outputs
#define CLPR_LOGGER_ENTRIES 10
	/// sleep time btwn db management tasks in mins
#define CLPR_MANAGER_WAIT 121
	/// maximum acceptable hours between writes of uid_blobs,
	/// for cleaning purposes in manager thread
#define CLPR_HOURS_BTWN_WRITES 4

	/// logger output fields
	/// print top x by max_mem in logger
#define PRINT_MAX_MEM
	/// print top x by max_cpu in logger
#define PRINT_MAX_CPU
	/// print top x by max_dsk in logger
#define PRINT_MAX_DSK
	/// print top x by max_fds in logger
#define PRINT_MAX_FDS

	/// define for the manager, maximum allowed entries
#define MAX_DB_ENTRIES 32*1024

	///input field positions in stream
	/// timestamp
#define time_pos 5
	/// percent cpu on app
#define user_pos 8
	/// percent cpu on system stuff
#define system_pos 9
	///
#define perc_cpu_pos 11
	/// the core we are running
#define core_cpu_pos 12
	/// the page faults not requring disk
#define minflt_s_pos 13
	/// the page faults requiring disk
#define majflt_s_pos 14
	/// virtual memory size bytes
#define vsz_pos 15
	/// resident mem, non-swapped
#define rss_pos	16
	///
#define mem_pos 17
	/// kb read disk
#define kb_rd_s_pos 18
	///
#define kb_wr_s_pos 19
	/// canceled write
#define kb_ccwr_s_pos 20
	///
#define iodelay_pos 21
	/// context switches
#define cswch_s_pos 22
	/// non-voluntary
#define nvcswh_s_pos 23
	/// threads in my group
#define threads_pos 24
	/// open file descriptors
#define fds_pos 25





	/// forward dec
	class tcp_connection;

	/// forward dec
	class clpr_proc_db;

	/// for... well you get it
	class uid_blob;

	///
	typedef boost::shared_ptr<tcp_connection> tcp_pointer;

	///
	typedef boost::shared_ptr<clpr_proc_db> db_pointer;

	///
	typedef boost::shared_ptr<uid_blob> uid_blob_pointer;


	/// essentially a row in pidstat output
	typedef struct {
		/// timestamp
		int time;
		/// percent cpu on app
		float user;
		/// percent cpu on system stuff
		float system;
		///
		float perc_cpu;
		/// the core we are running
		float core_cpu;
		/// the page faults not requring disk
		float minflt_s;
		/// the page faults requiring disk
		float majflt_s;
		/// virtual memory size bytes
		float vsz;
		/// resident mem, non-swapped
		float rss;
		///
		float mem;
		/// kb read disk
		float kb_rd_s;
		///
		float kb_wr_s;
		/// canceled writes
		float kb_ccwr_s;
		///
		float iodelay;
		/// context switches
		float cswch_s;
		/// non-voluntary
		float nvcswh_s;
		/// threads in my group
		float threads;
		/// open file descriptors
		float fds;
	} pid_blob;

	/// key for history unordered_map in uid_blob
	typedef struct{
		string command;
		string pid;
		string hostname;
	} history_key;

	/// hash for history unordered_map in uid_blob
	typedef struct{
		size_t operator()(const history_key &in ) const
		{
			return std::hash<string>()(in.command) ^ \
				std::hash<string>()(in.pid) ^ std::hash<string>()(in.hostname);
		};

	} key_hash;


	/// equality operator for history unordered_map in uid_blob
	typedef struct{
		bool operator()(const history_key& l, const history_key& r) const
		{
			return (l.command.compare(r.command) == 0) \
							      && (l.pid.compare(r.pid)==0) && \
							      (l.hostname.compare(r.hostname)==0);
		};

	} key_eq;

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
	struct uid_blob_label{};

	///
	typedef unordered_map<history_key,vector<pid_blob>,key_hash,key_eq > HASH_MAP;
	//typedef unordered_map<string,vector<pid_blob> > HASH_MAP;

	/// fake delete a smart pointer to stack object
	struct null_deletor{

		void operator()(void const *) const{

		}
	};


};
/// must be in global namespace
BOOST_FUSION_ADAPT_STRUCT(clpr_d::pid_blob, 
		(int,time)
		(float,perc_cpu)
		(float,majflt_s)
		(float,vsz)
		(float,kb_rd_s)
		(float,kb_wr_s)
		(float,cswch_s)
		(float,nvcswh_s)
		(float,fds)
		);


#endif
