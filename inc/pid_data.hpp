/**
 * @file pid_data.hpp
 * @author Pierre-Yves Taunay <py.taunay@psu.edu>
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
 * key pid_data
 */

#ifndef _PID_DATA_
#define _PID_DATA_

namespace clpr_d {
/// Essentially a row in pidstat output
typedef struct {
	/// Timestamp
	long int time;

	/// TODO User name

	/// PID
	int pid;

	/// TODO Group ID 

	/// Percent cpu on app
	float p_usr;
	/// Percent cpu on system stuff
	float p_sys;
	/// Percent cpu guest
	float p_gst;
	/// Percent CPU
	float p_cpu;

	/// The core we are running
	float cpu_num;

	/// The page faults not requiring disk
	float minflt_s;
	/// The page faults requiring disk
	float majflt_s;


	/// Virtual memory size bytes
	float vsz;
	/// Resident mem, non-swapped
	float rss;
	/// Percent memory
	float p_mem;

	/// kb read disk
	float kb_rd_s;
	/// kb write disk
	float kb_wr_s;
	/// canceled writes
	float kb_ccwr_s;

	/// I/O Delay
	float iodelay;

	/// Context switches
	float cswch_s;
	/// Non-voluntary context switches
	float nvcswh_s;

	/// Threads in my group
	float threads;
	/// Open file descriptors
	float fds;

	/// TODO Command
	
	
} pid_data; 

}; // End of namespace

#endif
