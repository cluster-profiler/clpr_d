#include "tcp_server.hpp"

namespace clpr_d{
	tcp_pointer tcp_connection::create(boost::asio::io_service& io_service)
	{
		return tcp_pointer(new tcp_connection(io_service));
	}

	tcp::socket& tcp_connection::socket()
	{
		return socket_;
	}



	void tcp_connection::serve(db_pointer& db)
	{

		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << *db.get();
		//we use a blocking write, and make sure it succeeds...
		bool suc = boost::asio::write(socket_, request); 
		//in order that the db object doesn't have entries deleted without being written
		if (suc) db.get()->update_write_time();
	}

	tcp_connection::tcp_connection(boost::asio::io_service& io_service)
		: socket_(io_service)
	{
	}


	tcp_server::tcp_server(boost::asio::io_service& io_service, db_pointer &db)
		: acceptor_(io_service, tcp::endpoint(tcp::v4(), 80)),
		db_(db)
	{
		start_accept();
	}

	void tcp_server::start_accept()
	{
		tcp_pointer new_connection =
			tcp_connection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_connection->socket(),
				boost::bind(&tcp_server::handle_accept, this, new_connection, 
					boost::asio::placeholders::error));
	}

	void tcp_server::handle_accept(tcp_pointer new_connection, 
			const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->serve(db_);
		}

		start_accept();
	}

};
