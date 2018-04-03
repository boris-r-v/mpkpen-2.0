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
    boost::program_options::options_description gen_desc("General options");
    gen_desc.add_options()
    ("help,h", "Produce help message")
    ("arm,a", "Run program as arm-side")
    ("kts,k", "Run program as kts-side")
    ("version,v", "Pring version and exit")
    ("test,t", "Test-run, service port for arm and kts will be different. Use it for testing at own PC")
    ("service_port", po::value<int>(),"The MCAST service port, (default: 18681)")
    ;
    boost::program_options::options_description arm_desc("Arm-side options");
    arm_desc.add_options()
    ("arm_mcast_port", po::value<int>(),"The MCAST port, witch the ARM sends the tu-orders")
    ("arm_udp_port", po::value<int>(),"The UDP port, witch the ARM sends the tu-orders")
    ("arm_send_attempts", po::value<int>(),"The numbers of sends attempt for one tu-orders (default: 3)")
    ;
    boost::program_options::options_description kts_desc("Kts-side options");
    kts_desc.add_options()
    ("kts_port", po::value<int>(),"The MCAST port, at witch the KTS listen for tu-orders (default: 14350) ")
    ("kts_mcast", po::value<std::string>(),"KTS MCAST group (default: '224.1.1.1')")
    ("kts_plan", po::value<std::string>(),"Name of controlled circle, (field 'plan' from ini-file)")
    ("kts_stations", po::value<std::string>(),"List of controlled station numbers, separated by commas ('2,23,34,55')")
    //("ini", po::value<std::string>(),"set path to ini-file, (use with ktsuk-option)")
    ;

    boost::program_options::options_description desc;
    desc.add(gen_desc).add(arm_desc).add(kts_desc);

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
	std::cout << "\tType: \"mpkpen -a --arm_udp_port=14310\" - for run at the arm_dsp side. 14310 - udp port in which arm sends tu-order" << std::endl;
	//std::cout << "\tType: \"mpkpen -k --ini=path_to_ini_file\" - for run at the ktsuk side" << std::endl;
	std::cout << "\tType: \"mpkpen -k --kts_plan='Test_Plan' --kts_station='1,2,3'\" - for run at the ktsuk side. 'Test_plan' - control circle, 1,2,3 - controled stations" << std::endl;
	std::cout << "  Best regards, Boris.R <borisrozhkin@gmail.com>" << std::endl << std::endl;

	std::cout << desc << std::endl;
	return;
    }

    if ( vm.count("version") )
    {
	std::cout << "Version: " << MpkPen::Define::version << std::endl;
	std::cout << "Release: " << MpkPen::Define::release << std::endl;
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
