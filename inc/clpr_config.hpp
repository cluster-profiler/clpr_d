#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <istream>
#include <string>


// Path to the configuration file
#define CLPR_CONF_PATH "/gpfs/work/p/pzt5044/Github/clpr_d/clprd.xml"

namespace clpr_d {

class clpr_config { 

	private:
		// Path to the FIFO
		std::string input_pipe;

		// Logging
		std::string log_path;
		int log_level;
		int log_wait;
		int log_max_entries;	

		int db_max_entries;

		
	public:
		// Construct this object from the config file
		clpr_config(const std::istream& config_file);
		~clpr_config();

}; // End of class clpr_config

typedef boost::shared_ptr<clpr_config> p_conf;

}; // End of namespace clprd_d

#endif
