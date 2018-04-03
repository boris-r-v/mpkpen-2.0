#include <tu.h>
#include <cstdlib>		//for std::exit
#include <sstream>
#include <logger.h>
#include <side_kts.h>
#include <boost/bind.hpp>

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
    orders_( 10 ),
    plan_( _vm.count("kts_plan") ? _vm["kts_plan"].as<std::string>() : "" )
{
    if ( plan_.empty() )
    {
	std::cerr << "Fail: 'kts_plan' not set" << std::endl;
	std::exit( -1 ); 
    }
    std::cout << "Handle circle: \""<< plan_ <<"\"" << std::endl;
    if ( _vm.count("kts_stations") )
    {
	std::string num;
	std::stringstream line (_vm["kts_stations"].as<std::string>() ); 
	while( getline( line, num, ',' ) )
	{
	    unsigned c = std::atoi( num.c_str() );
	    std::cout << "Handle station number: \"" << c << "\"" << std::endl;
    	    stations_.push_back( c );		    
	}	
    }
    else
    {
	std::cerr << "Fail: 'kts_stations' not set" << std::endl;
	std::exit( -1 ); 
    }
    service_server_.join_to_group( boost::asio::ip::address::from_string( SERVICE_GROUP ) );
    std::srand(unsigned(std::time(0)));

    std::cout << "Glad to serve the master..." << std::endl;

}


void MpkPen::Public::Kts::Provider::service_callback(std::string const& _arm_order, std::string& _kts_ticket )
{
    MpkPen::Public::Order order;
    if ( !order.ParseFromString( _arm_order ) )
    {
	MpkPen::Public::Logger::instance() << "FAIL: recieve Tu - can`t parse Order from recieved string " << std::endl;
	return;
    }

    if ( MpkPen::Private::is_my_order( order.order_data( ), plan_, stations_ )  )
    {
        auto fnd = std::find( orders_.begin(), orders_.end(), order );
	if ( fnd == orders_.end() )
	{
	    orders_.push_back( order );
	    MpkPen::Public::Logger::instance() << "OK: recieve Tu: " << MpkPen::Private::print_tu ( order.order_data( ) ) << std::endl;
	    auto cmd = std::make_shared<UdpClient>( boost::asio::ip::address::from_string( mcast_group_ ), kts_port_, io_service_, order.order_data( ), client_manager_ );
	    cmd->delivered( true );
	    client_manager_.start( cmd );

	    // - this   if ( (std::rand() % 100) > 50 )  is for test purpose	    
	    _kts_ticket = "received";
	}
	else
	{
	    MpkPen::Public::Logger::instance() << "Warning: recieve duplicate Tu: " << MpkPen::Private::print_tu ( order.order_data( ) ) << std::endl;
	    _kts_ticket = "already received";
	}
    }
    else
    {
	std::cout << "This tu not for me: " << MpkPen::Private::print_tu ( order.order_data( ) ) << std::endl;
    }
}

void MpkPen::Public::Kts::Provider::run()
{
    io_service_.run();
}

bool MpkPen::Public::operator==( MpkPen::Public::Order const& o1, MpkPen::Public::Order const& o2)
{
    //std::cout << "operator== " << o1.order_number() << ":[" << MpkPen::Private::print_tu ( o1.order_data( ) ) << "] ?= " << o2.order_number() << ":[" << MpkPen::Private::print_tu ( o2.order_data( ) ) << "]" << std::endl;
    return o1.order_number() == o2.order_number() and o1.order_data() == o2.order_data();
}


