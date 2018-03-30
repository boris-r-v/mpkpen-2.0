#ifndef ARM_SIDE_H
#define ARM_SIDE_H
#include <boost/asio.hpp>
#include <udp_server.h>
#include <udp_client.h>
#include <udp_client_manager.h>
#include <boost/program_options.hpp>	

namespace MpkPen
{
    namespace Public
    {
	namespace Arm
	{
	    class Provider
	    {
		    bool test_mode;
		    bool listen_udp_;
		    boost::asio::io_service io_service_;
		    int arm_port_, service_port_;
		    UdpServer udp_server_;
		    UdpServer service_server_;
		    UdpClientManager client_manager_;

		    void udp_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает команду ТУ для отправки по сервисному протоколу
		    void service_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает принятые квитанции от КТС УК

		public:
		    Provider( boost::program_options::variables_map const& );
		    void run( );
	    
	    };
	}
    }
}
#endif //ARM_SIDE_H