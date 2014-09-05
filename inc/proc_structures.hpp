#ifndef _PROC_STRUCTURES_HPP_
#define _PROC_STRUCTURES_HPP_

#include <string>

namespace clpr_d 
{

  enum {PROC_STAT_UTIME = 1, PROC_STAT_STIME = 2, PROC_STAT_GTIME = 8};

  // Structure to parse the content of /proc/<pid>/status
  struct proc_stat 
  { 
    int pid;
    std::string comm;
    std::string state;
    int ppid;
    int pgid;
    int sid;
    int tty;
    int tpgid;
    
    unsigned int flags;
    
    uint64_t minflt;
    uint64_t cminflt;
    uint64_t majflt;
    uint64_t cmajflt;
    uint64_t utime;
    uint64_t stime;
    int64_t cutime;
    int64_t cstime;
    
    int64_t priority;
    int64_t nice;
    int64_t n_th;
    
    int64_t itrealvalue;
    uint64_t starttime;
    uint64_t vsz;
    int64_t rss;
    uint64_t rsslim;
    
    uint64_t startcode;
    uint64_t endcode;
    
    uint64_t startstack; 
    uint64_t  kstkesp;
    uint64_t kstkeip;
    uint64_t signal;
    uint64_t blocked;
    uint64_t sigignore;
    uint64_t sigcatch;
    uint64_t wchan;
    uint64_t nswap;
    uint64_t cnswap;
    
    int exit_sig;
    int proc;
    
    unsigned int rt_prio;
    unsigned int policy;
    uint64_t delay;
    uint64_t gtime;
    int64_t cgtime;
  };	


  // Structure to parse the content of /proc/<pid>/io
  struct proc_io 
  {
    uint64_t rchar;
    uint64_t wchar;
    uint64_t syscr;
    uint64_t syscw;
    uint64_t read_bytes;
    uint64_t write_bytes;
    uint64_t cancelled_write_bytes;
  };	

  
  // Structure to parse the content of /proc/<pid>/status
  struct proc_status 
  {
    unsigned int fdsize;
    uint64_t cswch;
    uint64_t nvcswch;
    unsigned int uid;
  };
  
  // CPU usage -- used to store previous state
  struct cpu_usage 
  {
    uint64_t utime;
    uint64_t stime;
    uint64_t gtime;
  };	
  
  
} // End of namespace clpr_d

#endif
