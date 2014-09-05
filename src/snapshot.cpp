#include <cmath>

#include "snapshot.hpp"

namespace clpr_d 
{

  snapshot::snapshot(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus, const clpr_d::proc_io& pio, 
		     const std::map<int, std::string>& fd_list, const std::string& wchan, const uint64_t& total_mem, 
		     const clpr_d::cpu_usage& cpu_usage_c, const clpr_d::cpu_usage& cpu_usage_p, const float& delta_cpu, const uint64_t& tstamp)
  {
    //// TTY
    tty = pstat.tty;
    
    //// CPU Usage
    pusr = fabs((float)((cpu_usage_c.utime - cpu_usage_c.gtime) - (cpu_usage_p.utime - cpu_usage_p.gtime) ))/delta_cpu*100.0;
    psys = fabs((float)(cpu_usage_c.stime - cpu_usage_p.stime))/delta_cpu*100.0;
    pgst = fabs((float)(cpu_usage_c.gtime - cpu_usage_p.gtime))/delta_cpu*100.0;
    pcpu = fabs((float)(cpu_usage_c.utime + cpu_usage_c.stime - cpu_usage_p.utime - cpu_usage_p.stime))/delta_cpu*100.0;
    
    // Store current cpu_usage for next time step
    this->cpu_usage_p = cpu_usage_c;
    
    // Core number on which program executes 
    core_num = pstat.proc;
    // Number of threads
    n_th = pstat.n_th;
    
    //// FLT
    minflt = pstat.minflt;
    majflt = pstat.majflt;
    
    //// Memory
    vsz = pstat.vsz;
    rss = pstat.rss;
    pmem = (float)rss/(float)total_mem;
    
    //// IO
    rchar = pio.rchar;
    wchar = pio.wchar;
    syscr = pio.syscr;
    syscw = pio.syscw;
    read_bytes = pio.read_bytes;
    write_bytes = pio.write_bytes;
    cancelled_write_bytes = pio.cancelled_write_bytes;
    delay = pstat.delay;
    
    //// FD
    n_fd = fd_list.size();
    this->fd_list.insert(fd_list.begin(),fd_list.end());
    
    //// Context switching
    cswch = pstatus.cswch;
    nvcswch = pstatus.nvcswch;
    
    //// WCHAN
    this->wchan = wchan;
    
    //// State
    state = pstat.state;
    
    //// When was this snapshot taken ?
    this->tstamp = tstamp;
    
  } // End of snapshot::snapshot

  cpu_usage const& snapshot::get_cpu_usage_p() const 
  {
    return this->cpu_usage_p;
  } // End of snapshot::get_cpu_usage_p	

  std::ostream& operator<<(std::ostream& out, boost::shared_ptr<snapshot>& in) 
  {
    int tty_minor = 0xFF;
    
    std::string fd_string = "[";	
    for(auto it = (in->fd_list).begin(); it != (in->fd_list).end(); ++it) 
      {
	fd_string += std::to_string(it->first) + "=" + it->second + ","; 
      }
    fd_string.pop_back();
    fd_string += "]";
    
#if 0
    out << (in->tty & tty_minor) << " "		\
	<< in->pusr << " "			\
	<< in->psys << " "			\
	<< in->pgst << " "			\
	<< in->pcpu << " "			\
	<< in->core_num << " "			\
	<< in->n_th << " "			\
	<< in->minflt << " "			\
	<< in->majflt << " "			\
	<< in->vsz << " "			\
	<< in->rss << " "			\
	<< in->pmem << " "			\
	<< in->rchar << " "			\
	<< in->wchar << " "			\
	<< in->syscr << " "			\
	<< in->syscw << " "			\
	<< in->read_bytes << " "		\
	<< in->write_bytes << " "			\
	<< in->cancelled_write_bytes << " "		\
	<< in->delay << " "				\
	<< in->cswch << " "				\
	<< in->nvcswch << " "				\
	<< in->n_fd << " "				\
	<< in->wchan << " "				\
	<< in->state << " "				\
	<< fd_string << " ";
#endif
    out <<  in->tstamp << " "			\
	<< (in->tty & tty_minor) << " "		\
	<< in->pusr << " "			\
	<< in->psys << " "			\
	<< in->pgst << " "			\
	<< in->pcpu << " "			\
	<< in->core_num << " "			\
	<< in->n_th << " "			\
	<< in->minflt << " "			\
	<< in->majflt << " "			\
	<< in->vsz << " "			\
	<< in->rss << " "			\
	<< in->pmem << " "			\
	<< in->rchar << " "			\
	<< in->wchar << " "			\
	<< in->syscr << " "			\
	<< in->syscw << " "			\
	<< in->read_bytes << " "		\
	<< in->write_bytes << " "			\
	<< in->cancelled_write_bytes << " "		\
	<< in->delay << " "				\
	<< in->cswch << " "				\
	<< in->nvcswch << " "				\
	<< in->n_fd << " "				\
	<< in->wchan << " "				\
	<< in->state << " ";
    return out;
  } // End of operator<<
} // End of namespace clpr_d 
