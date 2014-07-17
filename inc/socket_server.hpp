#ifndef _SOCKET_SERVER_
#define _SOCKET_SERVER_

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
using boost::asio::local::stream_protocol;

enum { max_length = 1024 };

namespace clpr_d {


class session : public boost::enable_shared_from_this<session>
{
	public:
		session(const boost::asio::io_service& io_service);
		stream_protocol::socket& socket();

		void start();
		

/*
		void handle_basic(const boost::system::error_code& error,
			size_t bytes_transferred) {

			if(!error) {
				std::cout << bytes_transferred << " were transferred" << std::endl;
			}				
		}	

		void handle_read(const boost::system::error_code& error,
				size_t bytes_transferred)
		{
			char reply[1024] = "PONG ";
			std::cout << "Now in handle read" << std::endl;
			if (!error)
			{
				boost::asio::async_write(socket_,
						boost::asio::buffer(reply,1024),
						boost::bind(&session::handle_write,
							shared_from_this(),
							boost::asio::placeholders::error));
			}
		}

		void handle_write(const boost::system::error_code& error)
		{
			char reply[1024] = "PONG";
			std::cout << "Now in handle write" << std::endl;

			if (!error)
			{
				socket_.async_read_some(boost::asio::buffer(reply,1024),
						boost::bind(&session::handle_read,
							shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
			}
		}
*/

	private:
		// The socket used to communicate with the client.
		stream_protocol::socket socket_;

		// Buffer used to store data received from the client.
		boost::array<char, 1024> data_;
};

typedef boost::shared_ptr<session> session_ptr;

class server
{
	public:
		server(boost::asio::io_service& io_service, const std::string& file);
		void handle_accept(session_ptr new_session, const boost::system::error_code& error);
		

	private:
		boost::asio::io_service& io_service_;
		stream_protocol::acceptor acceptor_;
};

/*
int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: stream_server <file>\n";
			std::cerr << "*** WARNING: existing file is removed ***\n";
			return 1;
		}

		boost::asio::io_service io_service;

		std::remove(argv[1]);
		server s(io_service, argv[1]);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
*/


#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

} // End of namespace clpr_d

#endif
