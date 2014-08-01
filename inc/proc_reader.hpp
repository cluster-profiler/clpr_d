#ifndef _PROC_READER_HPP_
#define _PROC_READER_HPP_

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "clpr_log.hpp"
#include "clpr_db.hpp"
#include "proc_structures.hpp"

//#define CLPR_SAMPLE_RATE 30
#define CLPR_SAMPLE_RATE 5

namespace clpr_d {

// Forward declaration of clpr_db
class clpr_db;
typedef boost::shared_ptr<clpr_db> db_ptr;

class proc_reader {
	public:
		proc_reader(const clpr_d::p_log p_log_file);
		~proc_reader();

		void read(clpr_d::db_ptr p_db);

		// Read content of /proc/<pid>
		void read_pid_stat(clpr_d::proc_stat& pstat);

		void read_pid_io(clpr_d::proc_io& pio);

		void read_pid_status(clpr_d::proc_status& pstatus);

		void read_pid_wchan(std::string& wchan);

//		void read_pid_cmdline(std::string& cmd, const clpr_d::proc_stat& pstat);

		void read_pid_environ(std::map<std::string, std::string>& env);

		void read_pid_fd(std::map<int,std::string>& fd_list);

		void read_pid_bdate(uint64_t& bdate); 

		void read_stat(uint64_t& uptime_c); 
		


	private:
		clpr_d::p_log p_log_file;
		typename std::vector<boost::filesystem::path>::iterator pid_dir_it; 

}; // End of class prod_reader



} // End of namespace clpr_d

#endif

