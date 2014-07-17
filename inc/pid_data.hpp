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

#include <string>

#include <boost/shared_ptr.hpp>

namespace clpr_d {

enum { TIME_POS = 5, PID_POS = 7, USER_POS = 8, SYSTEM_POS = 9, PCPU_POS = 11, CORE_CPU_POS = 12, MINFLT_S_POS = 13, MAJFLT_S_POS = 14, VSZ_POS = 15, RSS_POS = 16, MEM_POS = 17, KB_RD_S_POS = 18, KB_WR_S_POS = 19, KB_CCWR_S_POS = 20, IODELAY_POS = 21, CSWCH_S_POS = 22, NVCSWCH_S_POS = 23, NTH_POS = 24, FDS_POS = 25, CMD_POS = 26 };

/*
class pid_data {
	public:
		pid_data(const std::string& proc_path);
		~pid_data();

	private:
		long int time;
		std::string hostname;
		std::string birthdate;
		std::string user_id;

		int pid;
		int ppid;
		int pgid;
		int sid;

		std::string tty;

		float p_user;
		float p_system;
		float p_guest;
		float p_cpu;

		int core_number;

		float minflt_s;
		float majflt_s;

		long int vsz;
		long int rss;

		float p_mem;

		float kb_rd_s;
		float kb_wr_s;
		float kb_ccwr_s;

		int iodelay;

		float cswch_s;
		float nvcswch_s;

		int nth;
		int fd;

		std::string wchan;
		std::string cmd;
		std::string environ;

}; // End of class pid_data
*/


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

typedef boost::shared_ptr<pid_data> pid_data_ptr;

} // End of namespace

#endif
