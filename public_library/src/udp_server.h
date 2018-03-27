#ifndef COMMON_UDP_SERVER_H
#define COMMON_UDP_SERVER_H
#define MAX_SIZE 1024

#include <functional>
#include <boost/asio.hpp>
#include <array>

namespace MpkPen
{
    namespace Public
    {
    	class UdpServer
	{
	    public:
		typedef std::function<void(std::string const&, std::string&)> Callback;
		UdpServer( int port, Callback cb, boost::asio::io_service& io_service );	
		void send( std::string const& );

		void join_to_group( const boost::asio::ip::address& multicast_address  );
	
	    
	    private:
		Callback cb_;
		boost::asio::ip::udp::socket socket_;
	        boost::asio::ip::udp::endpoint remote_endpoint_;
		std::array<uint8_t, MAX_SIZE> recv_buffer_;
		void receive();

	};
    }
}
#endif //COMMON_UDP_SERVER_H

