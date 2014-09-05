#include <pwd.h>
#include <utility>
#include <boost/foreach.hpp>

#include "process_grp.hpp"

namespace clpr_d 
{

  //// Constructor
  process_grp::process_grp(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus) 
  {
    // PGID, SID
    pgid = pstat.pgid;
    sid = pstat.sid;
    
    // Get uid
    struct passwd *pwd;
    pwd = getpwuid(pstatus.uid);
    
    uid = pwd->pw_name;
    
    // Hash
    hash_index = uid + " " + std::to_string(pgid);
    
    // Stamp !
    update_tstamp();
  }

  bool process_grp::is_present(const process_key& pkey) 
  {
    if (process_list.find(pkey) == process_list.end()) 
      {
	return false;
      }
    return true;
  }

  void process_grp::insert(const process_key& pkey, const clpr_d::process_ptr& pproc_insert) 
  {
    // Some C++11 uniform initialization here
    this->process_list.insert({pkey,pproc_insert});
    
    // We just inserted a new process ! Need to timestamp the process group now  
    update_tstamp();
  }

  clpr_d::process_ptr process_grp::find(const process_key& pkey) 
  {
    // Get the actual value from the returned iterator
    return process_list.find(pkey)->second;
  }	

  // Time setter
  void process_grp::update_tstamp()
  {
    tstamp = (uint64_t) time(NULL);
  }

  // Time getter
  uint64_t const& process_grp::get_tstamp() const 
  {
    return tstamp;
  }


  //// Getters	
  std::string process_grp::get_hash_index() const {return hash_index;}	

  void process_grp::dump(std::string& line_header, std::ostream& out) 
  {
    std::string hostname = line_header;
    line_header += " ";
    line_header += this->uid;
    
    // out<<this->pgid<<"*****"<<std::endl;
    
    BOOST_FOREACH(HASH_MAP::value_type v, this->process_list) 
      {
	v.second->dump(line_header,out,this->pgid, hostname);
      }
  } // End of process_grp::dump



  //// Output stream
  std::ostream& operator<<(std::ostream &out, boost::shared_ptr<process_grp>& in) 
  {
    BOOST_FOREACH( HASH_MAP::value_type v, in->process_list) 
      {
	out << in->uid << v.second << std::endl;
	//	out << in->uid << " " << in->pgid << v.second << std::endl;
      }
    return out;
    
  } // End of operator<<

} // End of namespace clpr_d
