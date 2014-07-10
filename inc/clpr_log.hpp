#ifndef _CLPR_LOG_HPP_
#define _CLPR_LOG_HPP_

#include <string>


// Path to the log file
#define CLPR_LOG_PATH  "/gpfs/work/p/pzt5044/Github/clpr_d/log/clprd.log"

namespace clpr_d {

// Logging types
enum { 	LOG_UNKNOWN = -1,
	LOG_INFO = 0,
	LOG_WARN = 1,
	LOG_ERROR = 2,
	LOG_CRITICAL = 3,
	LOG_DEBUG = 4 };


class clpr_log {

	private:
		std::ofstream log_file;


	public:
		clpr_log(const std::string& path);
		~clpr_log();

		get_log_date();


}; // End of class clpr_log


typedef boost::shared_ptr<clpr_log> p_log;


}; // End of namespace clpr_d



#endif
