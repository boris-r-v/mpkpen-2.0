#include <main.h>
#include <version.h>
#include <side_arm.h>
#include <side_kts.h>
#include <iostream>
#include <boost/program_options.hpp>		


namespace mp = MpkPen::Public;
namespace po = boost::program_options;
static const int  max_treads = 4;

void mp::Main::run(int argc, char** argv)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "produce help message")
    ("arm,a", "run program as arm-side")
    ("kts,k", "run program as kts-side")
    ("version,v", "pring version and exit")
    ("test,t", "test-run, service port for arm and kts will be different")
    //("ini", po::value<std::string>(),"set path to ini-file, (use with ktsuk-option)")
    ("service_port", po::value<int>(),"The Multycast service port (default: 18681)")
    ("arm_mcast_port", po::value<int>(),"The MCAST port, witch the ARM sends the tu-command")
    ("arm_udp_port", po::value<int>(),"The UDP port, witch the ARM sends the tu-command")
    ("arm_number_attempts", po::value<int>(),"The UDP port, witch the ARM sends the tu-command (default: 3)")
    ("kts_port", po::value<int>(),"The Multycast port, witch the KTS listen for tu-commands (default: 14350) ")
    ("kts_mcast", po::value<std::string>(),"ktsuk multycast group (default: '224.1.1.1')")
    ("kts_plan", po::value<std::string>(),"Name of controlled circle, (field 'plan' from ini-file)")
    ("kts_stations", po::value<std::string>(),"List of controlled station numbers, separated by commas ('2,23,34,55')")
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
	std::cout << "  Welcome to mpkpen - delivery-program for mpk-tu " << std::endl;
	std::cout << "\tType: \"mpkpen -a --arm_port=14350\" - for run at the arm_dsp side. 14350 - arm-tu-port, after udp2mcast" << std::endl;
	//std::cout << "\tType: \"mpkpen -k --ini=path_to_ini_file\" - for run at the ktsuk side" << std::endl;
	std::cout << "\tType: \"mpkpen -k --kts_plan='Test_Plan' --kts_station='1,2,3'\" - for run at the ktsuk side. 'Test_plan' - control circle, 1,2,3 - controled stations" << std::endl;
	std::cout << "  Best regards, Boris.R <borisrozhkin@gmail.com>" << std::endl << std::endl;

	std::cout << desc << std::endl;
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

    if ( vm.count("kts") and !vm.count("arm"))
    {
	MpkPen::Public::Kts::Provider kts( vm );
	kts.run();
	return;
    }
    std::cout << "Wrong command line options, type ./mpkpen -h - for more information" << std::endl;
}
