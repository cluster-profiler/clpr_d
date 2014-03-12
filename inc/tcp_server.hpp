/**
 * @file tcp_server.hpp
 * @author  Bill Brouwer <whiskeyjulietb@gmail.com>
 * @version 1.0
 *
 * adapted from :
 *
 * server.cpp
 * 
 * @section LICENSE
 * Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *
 * @section DESCRIPTION
 *
 * server for archiver class/thread
 */


#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include "clpr_proc_db.hpp"
#include "key_defines.hpp"


using boost::asio::ip::tcp;

namespace clpr_d{

	/// establish a socket using tcp/ipv4/port 80
	class tcp_connection
		: public boost::enable_shared_from_this<tcp_connection>
	{
		public:

			/// setup new service
			static tcp_pointer create(boost::asio::io_service& io_service);
			/// the socket
			tcp::socket& socket();

			/// serve up the database
			void serve(db_pointer &db);

		private:

			/// the tcp connection
			tcp_connection(boost::asio::io_service& io_service);

			tcp::socket socket_;
	};

	/// serve up clpr_proc_db ostream using tcp/ipv4/port 80
	class tcp_server
	{
		public:
			tcp_server(boost::asio::io_service& io_service, db_pointer &db);

		private:

			///
			void start_accept();
			///
			void handle_accept(tcp_pointer new_connection, 
					const boost::system::error_code& error);
			///
			tcp::acceptor acceptor_;
			/// the database
			db_pointer db_;
	};

};
#endif
