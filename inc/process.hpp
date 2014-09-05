#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include "snapshot.hpp"
#include "clpr_log.hpp"

namespace clpr_d {

class process {

	public:
		process(const std::string& cmd, 
			const std::map<std::string, std::string>& env,
			const int& pid,
			const int& ppid,
			const uint64_t& bdate);

		~process();
		void push_back(const clpr_d::snapshot_ptr& snap_ptr);
                void dump(std::string& line_header, std::ostream& out, int pgid, std::string hostname);
		std::vector<clpr_d::snapshot_ptr> const& get_time_series() const; 

		// Ostream
		friend std::ostream& operator<<(std::ostream& out, boost::shared_ptr<process>& in);

	private:
		//// Mutable data
		std::vector<clpr_d::snapshot_ptr> time_series; // History of a process
		uint64_t tstamp; // Last time we added data to the process

		//// Immutable process data for lifetime of process
		std::string cmd; // Command of a process  
		std::map<std::string, std::string> env; // Environment variables of a process at start time from /proc/<pid>/environ -- key = variable, value = content of variable
		int pid; // PID
		int ppid; // Parent PID
		uint64_t bdate; // Birthdate of process


		// Log file
		clpr_d::log_ptr log_file;

};

typedef boost::shared_ptr<process> process_ptr;

} // End of namespace clpr_d


#endif
