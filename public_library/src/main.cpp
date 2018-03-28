#include <main.h>
#include <version.h>
#include <message_dispatcher.h>
#include <side_arm.h>
#include <side_kts.h>
#include <iostream>


namespace mp = MpkPen::Public;
namespace po = boost::program_options;
static const int  max_treads = 4;

mp::Main::Main():
    desc("Allowed options")
{

}

void mp::Main::run(int argc, char** argv)
{
    desc.add_options()
    ("help,h", "produce help message")
    //("tutorial,t", "produce short tutorial")
    ("ktsuk,k", "run program as ktsuk-side, (request set ini-option)")
    ("arm,a", "run program as arm-side ")
    ("version,v", "pring version and exit")
    ("test,t", "test-run, service port for arm and nts are different")
    ("ini", po::value<std::string>(),"set path to ini-file, (use with ktsuk-option)")
    ("mcast", po::value<std::string>(),"kts multycast group (default: '224.1.1.1')")
    ("arm_port", po::value<int>(),"The UDP port, witch the ARM sends the tu-command (default: 14310)")
    ("kts_port", po::value<int>(),"The Multycast port, witch the KTS listen for tu-commands (default: 14350) ")
    ("service_port", po::value<int>(),"The Multycast service port (default: 18681)")
    ;

    po::variables_map vm;
    try
    {
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
    }
    catch (po::error& e )
    {
	std::cout << e.what() << std::endl;
	std::cout << desc << std::endl;
	return;
    }

    if ( vm.count("help") )
    {
	std::cout << desc << std::endl;
	return;
    }

    if ( vm.count("tutorial") )
    {
	std::cout << "  Welcome to mpkpen - а program that guarantees the delivery of an order " << std::endl;
	std::cout << "\tType: \"mpkpen -a\" - for run at the arm_dsp side" << std::endl;
	std::cout << "\tType: \"mpkpen -k --ini=path_to_ini_file\" - for run at the ktsuk side" << std::endl;
	std::cout << "  Best regards, Boris.R <borisrozhkin@gmail.com>" << std::endl << std::endl;
	return;
    }

    if ( vm.count("version") )
    {
	std::cout << "Version: " << MpkPen::Define::version << std::endl;
	return;
    }

    if ( vm.count("arm") and !vm.count("ktsuk") )
    {
	MpkPen::Public::Arm::Provider arm( vm );
	arm.run();
	return;
    }

    if ( vm.count("ktsuk") and !vm.count("arm"))
    {
	MpkPen::Public::Kts::Provider kts( vm );
	kts.run();
	//run_ktsuk_side( vm );
	return;
    }

    std::cout << "Wrong command line options" << std::endl;
    std::cout << desc << std::endl;
}

void mp::Main::run_ktsuk_side(  po::variables_map const& _vm )
{
    
    if ( _vm.count("ini") )
    {
	std::string path_to_ini ( _vm["ini"].as<std::string>() );
        std::cout << "run ktsuk side with ini: \"" << path_to_ini << "\"" << std::endl;
    }
    else
    {
	std::cout << "Wrong command line options" << std::endl;
	std::cout << desc << std::endl; 
    }
}

#include <Order.pb.h>
#include <Ticket.pb.h>
void mp::Main::prepare_for_run()
{
    mp::MessageDispatcher md;


    MpkPen::Public::Order ord;
    ord.set_order_number( 98745 );
    ord.set_order_data( "asvfrcqw#@%^&^dfhjs" );

    MpkPen::Public::Message msg_out ( mp::pack_message( ord ) );


    mp::Ticket tic ( mp::unpack_message<mp::Ticket>( md.dispatch ( msg_out ) ) );

    std::cout << "order number " << ord.order_number()<< std::endl;
    std::cout << "ticket number " << tic.order_number()<< std::endl;

}


