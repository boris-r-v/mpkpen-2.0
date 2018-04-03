#ifndef COMMON_UDP_CLIENT_H
#define COMMON_UDP_CLIENT_H

#include <boost/asio.hpp>
#include <memory>
#include <Order.pb.h>

namespace MpkPen
{
    namespace Public
    {
	class UdpClientManager;
    	class UdpClient: public std::enable_shared_from_this<UdpClient>
	{
	    public:
		UdpClient( boost::asio::ip::address const& address, int port, boost::asio::io_service& io_service, std::string const& msg, UdpClientManager& );	
		UdpClient( boost::asio::ip::address const& address, int port, boost::asio::io_service& io_service, MpkPen::Public::Order const& ord, UdpClientManager&, unsigned );	
		virtual ~UdpClient( ) = default;

		bool delivered() const;
		void delivered( bool );
		bool timeout() const;
		void stop();	    

	    private:
	        boost::asio::ip::udp::endpoint endpoint_;
		boost::asio::ip::udp::socket socket_;
		boost::asio::deadline_timer timer_;
		bool done_;
		std::string message_, order_string_;
		int attempt_;
    		std::array<uint8_t, 1024> rec_buffer_;
		UdpClientManager& client_manager_;
    
		void handle_send_to(const boost::system::error_code& error);
		void handle_timeout(const boost::system::error_code& error);
		void read_ticket_from_socket();
	};
	typedef std::shared_ptr<UdpClient> UdpClient_ptr;
    }
}
#endif //COMMON_UDP_SERVER_H

