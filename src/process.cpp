#include "process.hpp"

namespace clpr_d {
process::process(const std::string& cmd, 
			const std::map<std::string, std::string>& env,
			const int& pid,
			const int& ppid,
			const uint64_t& bdate) : cmd(cmd), env(env), pid(pid), ppid(ppid), bdate(bdate) {} // End of process::process

void process::push_back(clpr_d::snapshot_ptr snap_ptr) {
	this->time_series.push_back(snap_ptr);
} // End of process::push_back	

process::~process() {}


std::ostream& operator<<(std::ostream& out, boost::shared_ptr<process>& in) {

	for(auto it = (in->time_series).begin(); it != (in->time_series).end(); ++it) {
		//out << in->bdate << " " << in->pid << " " << in->ppid << " " << *it << " " << in->cmd << in->env << std::endl;
		//out << in->bdate << " " << in->pid << " " << in->ppid << " "  << in->cmd << " " << in->env << std::endl;
		out << in->bdate << " " << in->pid << " " << in->ppid << " " << *it << " " << in->cmd << " " << std::endl;
	}		

	return out;
} // End of operator<<

} // End of namespace clpr_d


