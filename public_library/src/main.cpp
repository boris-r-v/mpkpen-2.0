#include <main.h>
#include <version.h>
#include <iostream>

namespace mp = MpkPen::Public;
namespace po = boost::program_options;


mp::Main::Main():
    desc("Allowed options")
{

}

void mp::Main::run(int argc, char** argv)
{
    desc.add_options()
    ("help,h", "produce help message")
    ("tutorial,t", "produce short tutorial")
    ("ktsuk,k", "run program as ktsuk-side, (request set ini-option)")
    ("arm,a", "run program as arm-side ")
    ("version,v", "pring version and exit")
    ("ini", po::value<std::string>(),"set path to ini-file, (use with ktsuk-option)")
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
	run_arm_side( vm );
	return;
    }

    if ( vm.count("ktsuk") and !vm.count("arm"))
    {
	run_ktsuk_side( vm );
	return;
    }

    std::cout << "Wrong command line options" << std::endl;
    std::cout << desc << std::endl;
}

void mp::Main::run_arm_side( po::variables_map const& vm )
{
    std::cout << "run arm side " << std::endl;
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
