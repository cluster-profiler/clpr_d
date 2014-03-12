#include "archiver.hpp"

namespace clpr_d{

	archiver::archiver(db_pointer &db):
		db_(db){

		};


	void archiver::operator()(){



		try
		{

			boost::asio::io_service io_service;
			tcp_server server(io_service,db_);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}



	}


};
