#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <istream>

#include "clpr_conf.hpp"
#include "clpr_log.hpp"

namespace clpr_d {

// The input file has already been tested and approved
clpr_conf::clpr_conf(const std::istream& conf_file, const clpr_d::log_ptr& log_file) {
 
	this->log_file = log_file;

	// Populate tree structure pt
	using boost::property_tree::ptree;
	ptree pt;
	read_xml(const_cast<std::istream&>(conf_file), pt);

	// Traverse pt
	BOOST_FOREACH( ptree::value_type const& v, pt.get_child("conf") ) {
		if ( v.first == "log" ) {
			this->log_level = v.second.get<int>("log_level",0);
			this->log_wait = v.second.get<int>("log_wait",121);

		} else if ( v.first == "db_max_entries" ) {
			this->db_max_entries = pt.get<int>("conf.db_max_entries",10);
		}	


	}
	
	// Logging the config file
	std::string msg;

	msg = "... success";
	this->log_file->write(CLPR_LOG_INFO,msg);

	msg = "Configuration values"; 	
	this->log_file->write(CLPR_LOG_INFO,msg);

	msg = "Log level: " + std::to_string(this->log_level);
	this->log_file->write(CLPR_LOG_INFO,msg);

	msg = "Log wait: " + std::to_string(this->log_wait);
	this->log_file->write(CLPR_LOG_INFO,msg);

	msg = "DB max entries: " + std::to_string(this->db_max_entries);
	this->log_file->write(CLPR_LOG_INFO,msg);
} // End of clpr_conf::clpr_conf


clpr_conf::~clpr_conf() {
	this->log_file->write(CLPR_LOG_DEBUG,"Calling clpr_conf destructor");
} // End of clpr_conf::~clpr_conf	


bool clpr_conf::is_debug() {
	return ( (this->log_level == CLPR_DEBUG_ON ) ? true : false );
} // End of clpr_conf::is_debug	


int const& clpr_conf::get_db_max_entries() const {
	return this->db_max_entries;
} // End of clpr_conf::get_db_max_entries	


} // End of namespace clpr_d
