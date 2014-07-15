#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <istream>
#include <string>

#include "clpr_log.hpp"

// Path to the configuration file
#define CLPR_CONF_PATH "/gpfs/work/p/pzt5044/Github/clpr_d/clprd.xml"
#define CLPR_DEBUG_ON 1

namespace clpr_d {

class clpr_log;
typedef boost::shared_ptr<clpr_log> p_log;

class clpr_conf { 

	private:
		// Path to the FIFO
		std::string input_pipe;

		// Dependency injection -- smart pointer for the log file
		clpr_d::p_log p_log_file;

		// Logging
		int log_level;
		int log_wait;
		int log_max_entries;	

		int db_max_entries;

		
	public:
		// Construct this object from the config file
		clpr_conf(const std::istream& conf_file, const clpr_d::p_log& p_log_file);
		~clpr_conf();

		bool is_debug();	

		// Getters
		int const& get_db_max_entries() const; 


}; // End of class clpr_config

typedef boost::shared_ptr<clpr_conf> p_conf;

}; // End of namespace clprd_d

#endif
