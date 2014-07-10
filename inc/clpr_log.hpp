#ifndef _CLPR_LOG_HPP_
#define _CLPR_LOG_HPP_

#include <string>
#include <fstream>

#include <boost/shared_ptr.hpp>

// Path to the log file
#define CLPR_LOG_PATH  "/gpfs/work/p/pzt5044/Github/clpr_d/log/clprd.log"
#define CLPR_LOG_PATH_OLD  "/gpfs/work/p/pzt5044/Github/clpr_d/log/clprd.log."

enum { 	CLPR_LOG_UNKNOWN = -1, CLPR_LOG_INFO = 0, CLPR_LOG_WARN = 1, CLPR_LOG_ERROR = 2, CLPR_LOG_CRITICAL = 3, CLPR_LOG_DEBUG = 4 };

namespace clpr_d {

// Logging types

class clpr_log {

	private:
		std::ofstream log_file;

	public:
		clpr_log(const std::string& path);
		~clpr_log();

		void write(const int& severity, const std::string& msg); 

//		void get_log_date();


}; // End of class clpr_log

typedef boost::shared_ptr<clpr_log> p_log;

}; // End of namespace clpr_d

#endif
