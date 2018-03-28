#include <thread>
#include <boost/bind.hpp>

#include <tu.h>
#include <logger.h>
#include <side_kts.h>
#include <Order.pb.h>
#include <Ticket.pb.h>
#include <message_dispatcher.h>
#define SERVICE_GROUP "224.12.12.12"
#define SERVICE_PORT 18681
#define SERVICE_TEST_ARM_PORT 18681
#define SERVICE_TEST_KTS_PORT 18682


MpkPen::Public::Kts::Provider::Provider( boost::program_options::variables_map const& _vm):
    test_mode (_vm.count("test") ? true : false ),
    io_service_(),
    kts_port_( _vm.count("kts_port") ? _vm["kts_port"].as<int>() : 14350), 
    service_port_( _vm.count("service_port") ? _vm["service_port"].as<int>() : (test_mode ? SERVICE_TEST_KTS_PORT : SERVICE_PORT ) ),
    mcast_group_( _vm.count("mcast") ? _vm["mcast"].as<std::string>() : "224.1.1.1" ),
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ ),
    md_( MpkPen::Public::create_message_dispatcher() )
{
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
}


void MpkPen::Public::Kts::Provider::service_callback(std::string const& _arm_order, std::string& _kts_ticket )
{
std::cout << "void MpkPen::Public::Kts::Provider::service_callback" << std::endl;    
    delete_delivered_orders();
    MpkPen::Public::Message msg_in;
    if ( !msg_in.ParseFromString( _arm_order ) )
	std::cerr << "Error while parse message" << std::endl; 	
    try
    {
	MpkPen::Public::Message kts_ticket = md_.dispatch ( msg_in );
	kts_ticket.SerializeToString( &_kts_ticket );
	MpkPen::Public::Order arm_order ( MpkPen::Public::unpack_message<MpkPen::Public::Order>( msg_in  ) );
std::cout << "----------------------" << std::endl;
std::cout << arm_order.order_data() << std::endl;    
	auto cmd = std::make_shared<UdpClient>( boost::asio::ip::address::from_string( mcast_group_ ), kts_port_, io_service_, arm_order.order_data() );
	cmd->delivered( true );
	std::lock_guard<std::mutex> lock( mutex_ );    
	order_map_[ arm_order.order_number()] = cmd;
    }
    catch ( std::exception const& _e )
    {
	//MpkPen::Public::Logger::instance() << "Error while 'service_callback': " << _e.what() << std::endl; 	
	std::cerr << "Error while 'service_callback': " << _e.what() << std::endl; 	
    }	
}

void MpkPen::Public::Kts::Provider::delete_delivered_orders()
{
    for ( auto ord : order_map_ )
    {
	if ( ord.second->delivered() )
	{
	    std::lock_guard<std::mutex> lock( mutex_ );    
	    order_map_.erase( ord.first );
	}
    }
}

void MpkPen::Public::Kts::Provider::run()
{
    std::thread udp( boost::bind( &boost::asio::io_service::run, &io_service_ ) );
    udp.join();
}

