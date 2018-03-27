#include <thread>
#include <boost/bind.hpp>

#include <tu.h>
#include <logger.h>
#include <side_arm.h>
#include <Order.pb.h>
#include <Ticket.pb.h>
#include <message_dispatcher.h>
#define SERVICE_GROUP "224.12.12.12"

MpkPen::Public::Arm::Provider::Provider( boost::program_options::variables_map const& _vm):
    io_service_(),
    arm_port_( _vm.count("arm_port") ? _vm["arm_port"].as<int>() : 14310), 
    kts_port_( _vm.count("kts_port") ? _vm["kts_port"].as<int>() : 14350), 
    service_port_( _vm.count("service_port") ? _vm["service_port"].as<int>() : 18681),
    udp_server_( arm_port_, [this](std::string const& _s1, std::string& _s2){ this->udp_callback( _s1, _s2 ); }, io_service_ ),
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ ),
    tu_cntr_( 0 ),
    md_( MpkPen::Public::create_message_dispatcher() )
{
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
}



void MpkPen::Public::Arm::Provider::udp_callback(std::string const& _arm_tu, std::string& _empty )
{
    std::lock_guard<std::mutex> lock( mutex_ );    
    ++tu_cntr_;
    MpkPen::Public::Order ord;
    ord.set_order_number( tu_cntr_ );
    ord.set_order_data( _arm_tu );

    MpkPen::Public::Message msg_out ( MpkPen::Public::pack_message( ord ) );
    std::string msg_tu;
    msg_out.SerializeToString( &msg_tu );
    order_map_[tu_cntr_] = std::make_shared<UdpClient>( boost::asio::ip::address::from_string( SERVICE_GROUP ), service_port_, io_service_, msg_tu );
}

void MpkPen::Public::Arm::Provider::service_callback(std::string const& _kts_ticket, std::string& _empty )
{
    MpkPen::Public::Message msg_in;
    msg_in.ParseFromString( _kts_ticket );
    try
    {
	MpkPen::Public::Ticket tic ( MpkPen::Public::unpack_message<MpkPen::Public::Ticket>( md_.dispatch ( msg_in ) ) );
        std::cout << "ticket number " << tic.order_number()<< std::endl;
	auto fnd = order_map_.find( tic.order_number() );
	if ( fnd != order_map_.end() )
	    fnd->second->delivered( true );
    }
    catch ( std::exception const& _e )
    {
	MpkPen::Public::Logger::instance() << "Error while 'service_callback': " << _e.what() << std::endl; 	
    }	
    delete_delivered_orders();
}

void MpkPen::Public::Arm::Provider::delete_delivered_orders()
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

void MpkPen::Public::Arm::Provider::run()
{
    std::thread udp( boost::bind( &boost::asio::io_service::run, &io_service_ ) );
    udp.join();
}

