#include <udp_server.h>
#include <logger.h>
MpkPen::Public::UdpServer::UdpServer( int port, MpkPen::Public::UdpServer::Callback cb, boost::asio::io_service& io_service ):
    cb_ ( std::move ( cb ) ),
    socket_( io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),  port ) )
{
    receive();
}

void MpkPen::Public::UdpServer::receive()
{
std::cout << "UdpServer start_recieve" << std::endl;
    socket_.async_receive_from( boost::asio::buffer(recv_buffer_), remote_endpoint_, 
	[this]( boost::system::error_code _ec, size_t _size )
	{
	    if ( !_ec )
	    {
		std::string req;
		std::copy (recv_buffer_.begin(), recv_buffer_.begin()+_size, std::back_inserter(req) );
		std::string reply;
		this->cb_( req, reply );
		if ( !reply.empty() )
		{
		    this->send( reply );
		}
		receive();
	    }
	    else
	    {
		MpkPen::Public::Logger::instance() << "Ошибка приема: " << _ec ;
	    }
	}
    );
}

void MpkPen::Public::UdpServer::send( std::string const& _reply )
{
//std::cout << "void MpkPen::Public::UdpServer::send answer:"  << std::endl;
    socket_.async_send_to( boost::asio::buffer( _reply ), remote_endpoint_, 
	[]( boost::system::error_code _ec, size_t _size )
	{	
	    if ( _ec )
		MpkPen::Public::Logger::instance() << "Ошибка передачи: " << _ec ;

	}
    );
}

void MpkPen::Public::UdpServer::join_to_group( const boost::asio::ip::address& multicast_address  )
{
    // Join the multicast group.
    socket_.set_option( boost::asio::ip::multicast::join_group( multicast_address ) );
}
