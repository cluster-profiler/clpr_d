#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <istream>

#include "clpr_config.hpp"
#include "clpr_log.hpp"

namespace clpr_d {
// The input file has been tested and approved
clpr_config::clpr_config(const std::istream& config_file, const clpr_d::p_log& p_log_file) {
 
	this->p_log_file = p_log_file;

	// Populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(const_cast<std::istream&>(config_file), pt);

	// Traverse pt
	BOOST_FOREACH( ptree::value_type const& v, pt.get_child("conf") ) {
		if( v.first == "input_pipe" ) {
			this->input_pipe = v.second.get<std::string>("<xmlattr>.path","/opt/ci2/clprd/bin/clpr_fifo");
		} else if ( v.first == "log" ) {
			this->log_level = v.second.get<int>("log_level",0);
			this->log_wait = v.second.get<int>("log_wait",121);
	//		this->log_max_entries = 

		} else if ( v.first == "db_max_entries" ) {
		//	this->db_max_entries = v.second.get<int>("db_max_entries",10);
			this->db_max_entries = pt.get<int>("conf.db_max_entries",10);
		}	


	}
	
	std::string msg;

	msg = "... success";
	this->p_log_file->write(CLPR_LOG_INFO,msg);

	msg = "Parsed values"; 	
	this->p_log_file->write(CLPR_LOG_INFO,msg);

	msg = "Input pipe: " + this->input_pipe;
	this->p_log_file->write(CLPR_LOG_INFO,msg);

	msg = "Log level: " + std::to_string(this->log_level);
	this->p_log_file->write(CLPR_LOG_INFO,msg);

	msg = "Log wait: " + std::to_string(this->log_wait);
	this->p_log_file->write(CLPR_LOG_INFO,msg);

	msg = "DB max entries: " + std::to_string(this->db_max_entries);
	this->p_log_file->write(CLPR_LOG_INFO,msg);
}

clpr_config::~clpr_config() {
	this->p_log_file->write(CLPR_LOG_DEBUG,"Calling clpr_config destructor");
}	

} // End of namespace clpr_d
