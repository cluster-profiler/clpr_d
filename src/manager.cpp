#include "manager.hpp"

namespace clpr_d{

	manager::manager(db_pointer &db):
		db_(db){

		};


	void manager::operator()(){


		while(1){

			boost::this_thread::sleep(boost::posix_time::minutes(CLPR_MANAGER_WAIT));

			db_.get()->clean_by_size();
			db_.get()->clean_by_time();

		}
	}
};
