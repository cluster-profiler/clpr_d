#ifndef _SNAPSHOT_HPP_
#define _SNAPSHOT_HPP_

//// STD
#include <string>
#include <vector>
#include <map>

//// Boost
#include <boost/shared_ptr.hpp>

//// CLPR
#include "proc_structures.hpp"

namespace clpr_d 
{

  class snapshot 
  {
  public:
    snapshot(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus, const clpr_d::proc_io& pio, 
	     const std::map<int, std::string>& fd_list, const std::string& wchan, const uint64_t& total_mem, 
	     const clpr_d::cpu_usage& cpu_usage_c, const clpr_d::cpu_usage& cpu_usage_p, const float& delta_cpu, const uint64_t& tstamp);
    //		~snapshot();

    friend std::ostream& operator<<(std::ostream& out, boost::shared_ptr<snapshot>& in);
    cpu_usage const& get_cpu_usage_p() const; 
    
  private:
    int tty; // tty in which process is currently running
    
    //// CPU usage
    // Percentages calculated between two samples
    float pusr; // percent user
    float psys; // percent system
    float pgst; // percent guest
    float pcpu; // percent cpu
    
		// Previous cpu_usage structure
    cpu_usage cpu_usage_p;
    
    //// Core usage
    int core_num; // core number on which proc. is running
    int n_th; // number of threads
    
    //// FLT
    uint64_t minflt;
    uint64_t majflt;
    
    //// Memory usage
    uint64_t vsz;
    int64_t rss;
    float pmem; // percent memory
    
		//// Cumulative io counters
    uint64_t rchar;
    uint64_t wchar;
    uint64_t syscr;
    uint64_t syscw;
    uint64_t read_bytes;
    uint64_t write_bytes;
    uint64_t cancelled_write_bytes;
    uint64_t delay; // io delay
    
    //// Context switches, cumulative
    uint64_t cswch; // voluntary
    uint64_t nvcswch; // non voluntary
    
    //// File descriptors
    unsigned int n_fd; // Number of fd
    std::map<int,std::string> fd_list; // list of file descriptors
    
    //// WCHAN
    std::string wchan;
    std::string state; //state (R,Z,S,etc.)
    
    //// Tstamp
    uint64_t tstamp;


  }; // End of class snapshot

  typedef boost::shared_ptr<snapshot> snapshot_ptr;
} // End of namespace clpr_d

#endif
