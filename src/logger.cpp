#include "logger.hpp"

namespace clpr_d{

	logger::logger(db_pointer &db):
		db_(db){
		};


	void logger::operator()(){

		ofstream log_file(CLPR_OUTPUT_LOG);


		while (1){

			boost::this_thread::sleep(boost::posix_time::minutes(CLPR_LOGGER_WAIT));
#ifdef PRINT_MAX_MEM
			db_.get()->print_by_max_mem(log_file);
#endif
#ifdef PRINT_MAX_CPU
			//db_.get()->print_by_max_cpu(log_file);
#endif
#ifdef PRINT_MAX_DSK
			//db_.get()->print_by_max_dsk(log_file);
#endif
#ifdef PRINT_MAX_FDS
			//db_.get()->print_by_max_fds(log_file);
#endif


		}


	}


};
