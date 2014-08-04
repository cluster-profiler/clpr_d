#ifndef _SNAPSHOT_
#define _SNAPSHOT_

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "proc_structures.hpp"


namespace clpr_d {

class snapshot {

	public:
		//snapshot(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus, const clpr_d::proc_io& pio);
		snapshot(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus, const clpr_d::proc_io& pio, const std::map<int, std::string>& fd_list, const std::string& wchan, const uint64_t& total_mem, const clpr_d::cpu_usage& cpu_usage_c, const clpr_d::cpu_usage& cpu_usage_p, const float& delta_cpu);
//		~snapshot();

	friend std::ostream& operator<<(std::ostream& out, boost::shared_ptr<snapshot>& in);

	private:
		int tty; // tty in which process is currently running

		//// CPU usage
		// Percentages calculated between two samples
		float pusr; // percent user
		float psys; // percent system
		float pgst; // percent guest
		float pcpu; // percent cpu

		//// Core usage
		int core_num; // core number on which proc. is running
		int n_th; // number of threads

		//// FLT
		uint64_t minflt;
		uint64_t majflt;

		//// Memory usage
		uint64_t vsz;
		int64_t rss;
		float pmem; // percent memory

		//// Cumulative io counters
		uint64_t rchar;
		uint64_t wchar;
		uint64_t syscr;
		uint64_t syscw;
		uint64_t read_bytes;
		uint64_t write_bytes;
		uint64_t cancelled_write_bytes;
		uint64_t delay; // io delay

		//// Context switches, cumulative
		uint64_t cswch; // voluntary
		uint64_t nvcswch; // non voluntary

		//// File descriptors
		unsigned int n_fd; // Number of fd
		std::map<int,std::string> fd_list; // list of file descriptors
		
		//// WCHAN
		std::string wchan;
		std::string state; //state (R,Z,S,etc.)


}; // End of class snapshot

typedef boost::shared_ptr<snapshot> snapshot_ptr;


} // End of namespace clpr_d


#endif
