#include <thread>
#include <boost/bind.hpp>
#include <logger.h>
#include <side_arm.h>
#include <Order.pb.h>
#include <ctime>

#define SERVICE_GROUP "224.12.12.12"
#define SERVICE_PORT 18681
#define SERVICE_TEST_ARM_PORT 18681
#define SERVICE_TEST_KTS_PORT 18682

MpkPen::Public::Arm::Provider::Provider( boost::program_options::variables_map const& _vm):
    test_mode (_vm.count("test") ? true : false ),
    io_service_(),
    arm_port_( _vm.count("arm_port") ? _vm["arm_port"].as<int>() : 14310), 
    service_port_( _vm.count("service_port") ? _vm["service_port"].as<int>() : SERVICE_PORT ),
    udp_server_( arm_port_, [this](std::string const& _s1, std::string& _s2){ this->udp_callback( _s1, _s2 ); }, io_service_ ),
    service_server_( service_port_, [this](std::string const& _s1, std::string& _s2){ this->service_callback( _s1, _s2 ); }, io_service_ )
{
    std::cout << "Listen mcast tu port: \""<< arm_port_ <<"\"" << std::endl;
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
    std::cout << "Glad to serve the master..." << std::endl;
}

void MpkPen::Public::Arm::Provider::udp_callback(std::string const& _arm_tu, std::string& _empty )
{
    MpkPen::Public::Order order;
    order.set_order_number( time( nullptr ) );
    order.set_order_data( _arm_tu );

    client_manager_.start( std::make_shared<UdpClient>( boost::asio::ip::address::from_string( SERVICE_GROUP ), (test_mode ? SERVICE_TEST_KTS_PORT : service_port_ ), io_service_, order, client_manager_ ) );
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

