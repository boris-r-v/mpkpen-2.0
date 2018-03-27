#include <udp_client.h> 
#include <boost/bind.hpp> 
#define MAX_SEND_ATTEMPT 3

MpkPen::Public::UdpClient::UdpClient( boost::asio::ip::address const& address, int port, boost::asio::io_service& io_service, std::string const& msg ):
    endpoint_( address, port),
    socket_( io_service, endpoint_.protocol() ),	
    timer_( io_service ),
    done_( false ),
    message_( msg ),
    attempt_ ( MAX_SEND_ATTEMPT )
{
    if ( !msg.empty() )
        socket_.async_send_to( boost::asio::buffer( msg ), endpoint_, boost::bind(&UdpClient::handle_send_to, this, boost::asio::placeholders::error) );
}


void MpkPen::Public::UdpClient::send( std::string const& _msg )
{
    message_ = _msg;
    socket_.async_send_to( boost::asio::buffer( _msg ), endpoint_, boost::bind(&UdpClient::handle_send_to, this, boost::asio::placeholders::error) );
}

void MpkPen::Public::UdpClient::send_once( std::string const& _msg)
{
    done_ = true;
    send( _msg );
}


void MpkPen::Public::UdpClient::handle_send_to(const boost::system::error_code& error)
{
    if ( !error && !done_ )
    {
	timer_.expires_from_now(boost::posix_time::seconds(1));
        timer_.async_wait( boost::bind(&UdpClient::handle_timeout, this, boost::asio::placeholders::error));
    }
}

void MpkPen::Public::UdpClient::handle_timeout(const boost::system::error_code& error)
{
    if ( !error && !done_ && --attempt_ )
    {
	socket_.async_send_to( boost::asio::buffer(message_), endpoint_, boost::bind(&UdpClient::handle_send_to, this, boost::asio::placeholders::error));
    }
}


bool MpkPen::Public::UdpClient::delivered() const
{
    return done_;
}

void MpkPen::Public::UdpClient::delivered( bool _d )
{
    done_ = _d;
}
