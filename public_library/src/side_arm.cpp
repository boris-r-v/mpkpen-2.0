#include <tu.h>
#include <logger.h>
#include <side_arm.h>
#include <Order.pb.h>
#include <boost/bind.hpp>
#include <ctime>

#define MAX_SEND_ATTEMPT 3
#define SERVICE_GROUP "224.12.12.12"
#define SERVICE_PORT 18681
#define SERVICE_TEST_ARM_PORT 18681
#define SERVICE_TEST_KTS_PORT 18682
#define TRASH_ARM_PORT 250681

MpkPen::Public::Arm::Provider::Provider( boost::program_options::variables_map const& _vm):
    test_mode (_vm.count("test") ? true : false ),
    io_service_(),
    arm_port_( _vm.count("arm_udp_port") ? _vm["arm_udp_port"].as<int>() : ( _vm.count("arm_mcast_port") ? _vm["arm_mcast_port"].as<int>() : TRASH_ARM_PORT ) ), 
    service_port_( _vm.count("service_port") ? _vm["service_port"].as<int>() : SERVICE_PORT ),
    udp_server_( arm_port_, [this](std::string const& _s1, std::string& _s2){ this->udp_callback( _s1, _s2 ); }, io_service_ ),
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ ),
    attempts_( _vm.count("arm_send_attempts") ? _vm["arm_send_attempts"].as<int>() : MAX_SEND_ATTEMPT )
{
    if ( _vm.count("arm_udp_port") )
    {
	listen_udp_ = true;
	std::cout << "Listen udp tu port: \""<< arm_port_ <<"\"" << std::endl;
    }
    else if ( _vm.count("arm_mcast_port") )
    {
	listen_udp_ = false;
	std::cout << "Listen mcast tu port: \""<< arm_port_ <<"\"" << std::endl;
    }
    if ( TRASH_ARM_PORT == arm_port_ )
    {
	std::cerr << "Fail: arm_port not set, choose 'arm_udp_port' or 'arm_mcast_port' and set choosen." << std::endl;
	std::exit( -1 ); 
    }


    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
    std::cout << "Glad to serve the master..." << std::endl;
}

void MpkPen::Public::Arm::Provider::udp_callback(std::string const& _arm_tu, std::string& _empty )
{
    MpkPen::Public::Order order;
    order.set_order_number( time( nullptr ) );

    if( listen_udp_ )
    {  //Для команд приходящих по уникасту и требующие приготовления для потребления контроллером
	std::string tu ( MpkPen::Private::create_mcast_tu( _arm_tu ) );
	if ( "wrong" != tu )
	{
	    order.set_order_data( tu );
	}
	else
	{
	    MpkPen::Public::Logger::instance() << "Warning: - unknown data reciever from arm: " << _arm_tu  << std::endl;
	    return;
	}
    }
    else
    {	//Для команд приходящих уже в мультикасте - готовые к потреблению контроолером
	order.set_order_data( _arm_tu );
    }
    client_manager_.start( std::make_shared<UdpClient>( boost::asio::ip::address::from_string( SERVICE_GROUP ), (test_mode ? SERVICE_TEST_KTS_PORT : service_port_ ), io_service_, order, client_manager_, attempts_ ) );
}

void MpkPen::Public::Arm::Provider::service_callback(std::string const& _data_from_kts, std::string& _empty )
{
    //FIX ME -this is for resive serice information
}

void MpkPen::Public::Arm::Provider::run()
{
    io_service_.run();
}

