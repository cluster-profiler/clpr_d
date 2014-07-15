#include "fifo_reader.hpp"

namespace clpr_d {

fifo_reader::fifo_reader(const std::string& fifo_path, const clpr_d::p_log p_log_file) {

	this->p_log_file = p_log_file;
	this->p_log_file->write(CLPR_LOG_DEBUG,"Calling fifo_reader constructor");


	// Open the path to the fifo
	fifo.open(fifo_path, std::ifstream::in); 

	if(!fifo.is_open()) {
		p_log_file->write(CLPR_LOG_ERROR,"Could not open fifo for reading");
	}	
} // End of fifo_reader::fifo_reader


fifo_reader::~fifo_reader() {
	p_log_file->write(CLPR_LOG_DEBUG,"Calling fifo_reader destructor");

	if(fifo.is_open()) {
		fifo.close();
	}	
} // End of fifo_reader::~fifo_reader
	

void fifo_reader::read(clpr_d::db_ptr p_db) {

	std::string line;
	std::string msg;

	while(1) {

	// Open the path to the fifo
		if( !fifo.is_open() ) 
			fifo.open(CLPR_FIFO_PATH, std::ifstream::in); 

		while(std::getline(fifo, line)) {
			msg = "Processed line " + line;
			p_log_file->write(CLPR_LOG_INFO,msg);
		}
		fifo.close();
	}

} // End of fifo_reader::read


} // End of namespace clpr_d
