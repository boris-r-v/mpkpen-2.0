#include <udp_client.h> 
#include <udp_client_manager.h> 
#include <message_dispatcher.h>
#include <boost/bind.hpp> 
#define MAX_SEND_ATTEMPT 3

MpkPen::Public::UdpClient::UdpClient( boost::asio::ip::address const& address, int port, boost::asio::io_service& io_service, std::string const& msg, UdpClientManager& _cmgr ):
    endpoint_( address, port),
    socket_( io_service, boost::asio::ip::udp::v4() ), //endpoint_.protocol() ),	
    timer_( io_service ),
    done_( false ),
    message_( msg ),
    attempt_ ( MAX_SEND_ATTEMPT ),
    client_manager_(_cmgr )
{
    socket_.non_blocking( true );    
    if ( !msg.empty() )
        socket_.async_send_to( boost::asio::buffer( msg ), endpoint_, boost::bind(&UdpClient::handle_send_to, this, boost::asio::placeholders::error) );
}

void MpkPen::Public::UdpClient::handle_send_to(const boost::system::error_code& error)
{
    std::cout << "void MpkPen::Public::UdpClient::handle_send_to" << std::endl;
    if ( !error && !done_ )
    {
	timer_.expires_from_now(boost::posix_time::seconds(1));
        timer_.async_wait( boost::bind(&UdpClient::handle_timeout, this, boost::asio::placeholders::error));
    }
    else
    {
	client_manager_.stop(shared_from_this());
    }
}

void MpkPen::Public::UdpClient::handle_timeout(const boost::system::error_code& error)
{
    std::cout << "void MpkPen::Public::UdpClient::handle_timeout" << std::endl;
    read_ticket_from_socket();
    if ( !error && !done_ && --attempt_ )
    {	//FIX ME - разбить на разные сообщения в лог
	socket_.async_send_to( boost::asio::buffer(message_), endpoint_, boost::bind(&UdpClient::handle_send_to, this, boost::asio::placeholders::error));
    }
    else
    {
	client_manager_.stop(shared_from_this());
    }
}

void MpkPen::Public::UdpClient::read_ticket_from_socket()
{
    boost::asio::ip::udp::endpoint sender_endpoint;
    boost::system::error_code er;
    size_t size = socket_.receive_from( boost::asio::buffer(rec_buffer_), sender_endpoint, 0, er );
    if ( !er and size )
    {
	std::string rec;
	std::copy (rec_buffer_.begin(), rec_buffer_.begin()+size, std::back_inserter( rec ) );
	done_= client_manager_.message_dispatcher().check_tu_ticket( rec );
    }
}

void MpkPen::Public::UdpClient::stop()
{
    socket_.close();	
    timer_.cancel();
}

bool MpkPen::Public::UdpClient::delivered() const
{
    return done_;
}

void MpkPen::Public::UdpClient::delivered( bool _d )
{
    done_ = _d;
}
bool MpkPen::Public::UdpClient::timeout() const
{
    return !attempt_;
}
