#include "process.hpp"

namespace clpr_d {

process::process() {


} // End of process::process

void process::push_back(clpr_d::snapshot_ptr snap_ptr) {
	this->time_series.push_back(snap_ptr);
} // End of process::push_back	



} // End of namespace clpr_d
