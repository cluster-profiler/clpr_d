#include "process.hpp"
#include "clpr_conf.hpp"

extern clpr_d::conf_ptr conf_file;

namespace clpr_d 
{
  process::process(const std::string& cmd, 
		   const std::map<std::string, std::string>& env,
		   const int& pid,
		   const int& ppid,
		   const uint64_t& bdate) : cmd(cmd), env(env), pid(pid), ppid(ppid), bdate(bdate) {} // End of process::process

  void process::push_back(const clpr_d::snapshot_ptr& snap_ptr) 
  {
    this->time_series.push_back(snap_ptr);
  } // End of process::push_back	
  
  process::~process() {}

  std::vector<clpr_d::snapshot_ptr> const& process::get_time_series() const 
  {
    return this->time_series;
  } // End of process::get_time_series()	

  void process::dump(std::string& line_header, std::ostream& out, int pgid, std::string hostname) 
  {
    std::string env_string = "[";	
    for(auto it = (this->env).begin(); it != (this->env).end(); ++it) 
      {
	env_string += it->first + "=" + it->second + ","; 
      }
    env_string.pop_back();
    env_string += "]";
    
    for(auto it = (this->time_series).begin(); it != (this->time_series).end(); ++it) 
      {
	//		out << line_header << " " << this->bdate << " " << this->pid << " " << this->ppid << " " << *it << " " << this->cmd << " " << env_string << std::endl;
	out << this->pid << "_" << this->bdate << "_" << hostname << conf_file->get_db_field_separator() << 
	  line_header << conf_file->get_db_field_separator() << this->bdate << conf_file->get_db_field_separator() << 
	  this->pid << conf_file->get_db_field_separator() << this->ppid << conf_file->get_db_field_separator() << 
	  pgid << conf_file->get_db_field_separator() << *it << this->cmd << std::endl;
      }
  }


  std::ostream& operator<<(std::ostream& out, boost::shared_ptr<process>& in) 
  {
    for(auto it = (in->time_series).begin(); it != (in->time_series).end(); ++it) 
      {
	//out << in->bdate << " " << in->pid << " " << in->ppid << " " << *it << " " << in->cmd << in->env << std::endl;
	//out << in->bdate << " " << in->pid << " " << in->ppid << " "  << in->cmd << " " << in->env << std::endl;
	out << in->bdate << " " << in->pid << " " << in->ppid << " " << *it << " " << in->cmd << " " << std::endl;
      }		
    return out;
  } // End of operator<<
  
} // End of namespace clpr_d


