#include <thread>
#include <boost/bind.hpp>
#include <logger.h>
#include <side_arm.h>
#define SERVICE_GROUP "224.12.12.12"
#define SERVICE_PORT 18681
#define SERVICE_TEST_ARM_PORT 18681
#define SERVICE_TEST_KTS_PORT 18682

MpkPen::Public::Arm::Provider::Provider( boost::program_options::variables_map const& _vm):
    test_mode (_vm.count("test") ? true : false ),
    io_service_(),
    arm_port_( _vm.count("arm_port") ? _vm["arm_port"].as<int>() : 14310), 
    kts_port_( _vm.count("kts_port") ? _vm["kts_port"].as<int>() : 14350), 
    service_port_( _vm.count("service_port") ? _vm["service_port"].as<int>() : SERVICE_PORT ),
    udp_server_( arm_port_, [this](std::string const& _s1, std::string& _s2){ this->udp_callback( _s1, _s2 ); }, io_service_ ),
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ )
{
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
}

void MpkPen::Public::Arm::Provider::udp_callback(std::string const& _arm_tu, std::string& _empty )
{
    client_manager_.start( std::make_shared<UdpClient>( boost::asio::ip::address::from_string( SERVICE_GROUP ), (test_mode ? SERVICE_TEST_KTS_PORT : service_port_ ), io_service_, _arm_tu, client_manager_ ) );
}

void MpkPen::Public::Arm::Provider::service_callback(std::string const& _data_from_kts, std::string& _empty )
{
    //FIX ME -this is for resive serice information
}

void MpkPen::Public::Arm::Provider::run()
{
    std::thread udp1( boost::bind( &boost::asio::io_service::run, &io_service_ ) );
    udp1.join();
}

