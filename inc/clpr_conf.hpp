#ifndef _CLPR_CONF_HPP_
#define _CLPR_CONF_HPP_

#include <istream>
#include <string>

#include "clpr_log.hpp"

// Path to the configuration file
//#define CLPR_CONF_PATH "/gpfs/work/p/pzt5044/Github/clpr_d/clprd.xml"
//#define CLPR_CONF_PATH "/opt/clprd/clprd.xml"
#define CLPR_CONF_PATH "/gpfs/home/ivy1/Projects/Clpr/clpr_d/clprd.xml"


#define CLPR_DEBUG_ON 1

namespace clpr_d {

class clpr_log;
typedef boost::shared_ptr<clpr_log> log_ptr;

class clpr_conf { 

	private:
		// Dependency injection -- smart pointer for the log file
		clpr_d::log_ptr log_file;

		// Logging
		int log_level;
		int log_wait;
                std::string log_filename;

		int db_max_entries;
                std::string db_filename;
		
	public:
		// Construct this object from the config file
		clpr_conf(const std::istream& conf_file);
		~clpr_conf();

		bool is_debug();	

		// Getters
		int const& get_db_max_entries() const; 
                clpr_d::log_ptr const& get_log_ptr() const;
                std::string const& get_db_filename() const;

}; // End of class clpr_config

typedef boost::shared_ptr<clpr_conf> conf_ptr;

}; // End of namespace clprd_d

#endif
