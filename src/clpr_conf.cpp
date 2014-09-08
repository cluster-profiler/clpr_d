#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <istream>

#include "clpr_conf.hpp"
#include "clpr_log.hpp"

namespace clpr_d 
{

  // The input file has already been tested and approved
  clpr_conf::clpr_conf(const std::istream& conf_file) 
  {
    // Populate tree structure pt
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(const_cast<std::istream&>(conf_file), pt);
    
    // Traverse pt
    BOOST_FOREACH( ptree::value_type const& v, pt.get_child("conf") ) 
      {
	if ( v.first == "log" ) 
	  {
	    this->log_level = v.second.get<int>("level",0);
	    this->log_wait = v.second.get<int>("wait",121);
	    this->log_filename = v.second.get<std::string>("file","/var/log/cliprd.log");
	    this->log_file = clpr_d::log_ptr(new clpr_d::clpr_log(this->log_filename));
	  } else if ( v.first == "db" ) 
	  {
	    this->db_max_entries = v.second.get<int>("max_entries",10);
	    this->db_filename = v.second.get<std::string>("file", "/var/spool/clipr_db_tmp/db");
	    this->db_dir = v.second.get<std::string>("dir", "/var/spool/clipr_db");
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
  } 


  clpr_conf::~clpr_conf() 
  {
    this->log_file->write(CLPR_LOG_DEBUG,"Calling clpr_conf destructor");
  }

  bool clpr_conf::is_debug() 
  {
    return ( (this->log_level == CLPR_DEBUG_ON ) ? true : false );
  }

  int const& clpr_conf::get_db_max_entries() const 
  {
    return this->db_max_entries;
  }

  clpr_d::log_ptr const& clpr_conf::get_log_ptr() const 
  {
    return this->log_file;
  }

  std::string const& clpr_conf::get_db_filename() const 
  {
    return this->db_filename;
  }


  std::string const& clpr_conf::get_db_dir() const 
  {
    return this->db_dir;
  }



  
} 
