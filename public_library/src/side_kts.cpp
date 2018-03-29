#include <thread>
#include <boost/bind.hpp>

#include <tu.h>
#include <logger.h>
#include <side_kts.h>
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
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ )
{
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
    std::srand(unsigned(std::time(0)));
}


void MpkPen::Public::Kts::Provider::service_callback(std::string const& _arm_order, std::string& _kts_ticket )
{
std::cout << "void MpkPen::Public::Kts::Provider::service_callback" << std::endl;    
    /*random ticket*/
    if ( (std::rand() % 100) > 50 )
    {
        _kts_ticket = _arm_order;
    }

    auto cmd = std::make_shared<UdpClient>( boost::asio::ip::address::from_string( mcast_group_ ), kts_port_, io_service_, _arm_order, client_manager_ );
    cmd->delivered( true );
    client_manager_.start( cmd );
}

void MpkPen::Public::Kts::Provider::run()
{
    std::thread udp( boost::bind( &boost::asio::io_service::run, &io_service_ ) );
    udp.join();
}

