#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include "clpr_log.hpp"
#include "utilities.hpp"
#include "mv_exception.hpp"

using namespace boost::posix_time;

namespace clpr_d {

clpr_log::clpr_log(const std::string& filename) {
	
	// Check if a previous log file already exists
	// If it does, move it to a backup location
	if( file_exists(filename) ) {
		// Get the creation time of that file
		struct tm* info_time;
		struct stat attributes;
		stat(filename.c_str(), &attributes);

		// Get last modification time of inode, and convert to YYYYMMDD HHmmss format
		info_time = gmtime(&(attributes.st_ctime));
		ptime tstamp = from_time_t(mktime(info_time));

		// Create command based on the timestamp
		std::string cmd = "mv " + filename + " " + CLPR_LOG_PATH_OLD + to_iso_string(tstamp);

		// Execute command
		int ret = std::system(cmd.c_str());

		// If mv command fails, throw exception and return code
		if( ret != 0 ) {
			std::string err = "Command mv failed with return code ";
			err += std::to_string(ret);
			throw mv_exception(err);
		}	
	} 
	// Now, create the actual log file 
	this->log_file.open (filename, std::ofstream::out);
	// Throw error if you can not
	if(!this->log_file.is_open()) {
		std::string err = "Can not open log file";
		throw std::runtime_error(err);
	}	
}

clpr_log::~clpr_log() {
	clpr_log::write(CLPR_LOG_DEBUG, "Calling clpr_log destructor"); 

	if( this->log_file.is_open()) 
		log_file.close();
}


void clpr_log::write(const int& severity, const std::string& msg) {
	// Get the time stamp of that log message
  	time_facet *facet = new time_facet("%d-%b-%Y %H:%M:%S");
    	this->log_file.imbue(locale(this->log_file.getloc(), facet));
     	this->log_file << second_clock::local_time() << " ";

	std::string type;
	switch(severity) {
		case CLPR_LOG_INFO:
			type = "INFO ";
			break;
		case CLPR_LOG_WARN:
			type = "WARN ";
			break;
		case CLPR_LOG_ERROR:
			type = "ERROR ";
			break;
		case CLPR_LOG_CRITICAL:
			type = "CRITICAL ";
			break;
		case CLPR_LOG_DEBUG:
			type = "DEBUG ";
			break;
		default:
			type = "UNKNOWN ";
			break;
	}		
	
	this->log_file << type << msg << std::endl; 
} // End of clpr_log::write


} // End of namespace clpr_d
