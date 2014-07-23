#include <cmath>

#include "snapshot.hpp"

namespace clpr_d {

snapshot::snapshot(const clpr_d::proc_stat& pstat, const clpr_d::proc_status& pstatus, const clpr_d::proc_io& pio, const std::string& wchan, const uint64_t& total_mem, const clpr_d::cpu_usage& cpu_usage_c, const clpr_d::cpu_usage& cpu_usage_p, const float& delta_cpu){

	//// TTY
	tty = pstat.tty;

	//// CPU Usage
	// Current usage
	clpr_d::cpu_usage cpu_usage_c;
	cpu_usage_c.utime = pstat.utime;
	cpu_usage_c.stime = pstat.stime;
	cpu_usage_c.gtime = pstat.gtime;

	pusr = fabs((float)( (cpu_usage_c.utime - cpu_usage_p.gtime) - (cpu_usage_p.utime - cpu_usage_p.gtime) ))/delta_cpu*100.0;
	psys = fabs((float)(cpu_usage_c.stime - cpu_usage_p.stime))/delta_cpu*100.0;
	pgst = fabs((float)(cpu_usage_c.gtime - cpu_usage_p.gtime))/delta_cpu*100.0;
	pcpu = fabs((float)(cpu_usage_c.utime + cpu_usage_c.stime - cpu_usage_p.utime - cpu_usage_p.stime))/delta_cpu*100.0;

	core_num = pstat.proc;
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
	n_fd = pstatus.fdsize;

	//// Context switching
	cswch = pstatus.cswch;
	nvcswch = pstatus.nvcswch;

	//// WCHAN
	this->wchan = wchan;

	//// State
	state = pstat.state;

} // End of snapshot::snapshot



} // End of namespace clpr_d 
