#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "clpr_config.hpp"

namespace clpr_d {
// The input file has been tested and approved
clpr_config::clpr_config(const std::istream& config_file) {
 
	// populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(const_cast<std::istream&>(config_file), pt);

	// traverse pt
	BOOST_FOREACH( ptree::value_type const& v, pt.get_child("conf") ) {
		if( v.first == "input_pipe" ) {
			this->input_pipe = v.second.get<std::string>("<xmlattr>.path","/opt/ci2/clprd/bin/clpr_fifo");
		} else if ( v.first == "log" ) {
			this->log_path = v.second.get<std::string>("<xmlattr>.path","/opt/ci2/clprd/log/clpr_d.log");
			this->log_level = v.second.get<int>("log_level",0);
			this->log_wait = v.second.get<int>("log_wait",121);
	//		this->log_max_entries = 

		} else if ( v.first == "db_max_entries" ) {
		//	this->db_max_entries = v.second.get<int>("db_max_entries",10);
			this->db_max_entries = pt.get<int>("conf.db_max_entries",10);
		}	


	}

	std::cout << "Parsed values" << std::endl;
	std::cout << "Input pipe: " << this->input_pipe << std::endl;
	std::cout << "Log path: " << this->log_path << std::endl;
	std::cout << "Log level: " << this->log_level << std::endl;
	std::cout << "Log wait: " << this->log_wait << std::endl;
	std::cout << "Log max entries: " << this->log_max_entries << std::endl;
	std::cout << "Db max entries: " << this->db_max_entries << std::endl;


/*
		if( v.first == "flight" ) {
			Flight f;
			f.carrier = v.second.get<std::string>("carrier");
			f.number = v.second.get<unsigned>("number");
			f.date = v.second.get<Date>("date");
			f.cancelled = v.second.get("<xmlattr>.cancelled", false);
			ans.push_back(f);
		}
*/		
}

clpr_config::~clpr_config() {
	std::cout << "INFO Destructor called" << std::endl;
}	

}
