#ifndef MPKPEN_PUBLIC_MAIN
#define MPKPEN_PUBLIC_MAIN
#include <boost/program_options.hpp>		//Спрятать в итоговой версии ПО


/**
* Class Main - запускает программу используя опции командной строки для определения:
*	1. Роли программы: mpkpen_arm - запущена на АРМа, mpkpen_kts - Запущена на контроллере 
*	2. Разбора списка параметров командной строки, для своей роли
*/
namespace MpkPen
{
    namespace Public
    {
	class Main
	{
		void run_arm_side( boost::program_options::variables_map const& );
		void run_ktsuk_side( boost::program_options::variables_map const& );
	        boost::program_options::options_description desc;

	    public:		
		Main();
		~Main()=default;

		void run(int argc, char** argv);
	};
    }
}
#endif //MPKPEN_PUBLIC_MAIN
