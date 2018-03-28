#ifndef ARM_SIDE_H
#define ARM_SIDE_H
#include <boost/program_options.hpp>		//Спрятать в итоговой версии ПО
#include <boost/asio.hpp>
#include <message_dispatcher.h>
#include <udp_server.h>
#include <udp_client.h>
#include <udp_client_manager.h>

namespace MpkPen
{
    namespace Public
    {
	namespace Arm
	{
	    class Provider
	    {
		    bool test_mode;
		    boost::asio::io_service io_service_;
		    int arm_port_, kts_port_, service_port_;
		    UdpServer udp_server_;
		    UdpServer service_server_;
		    MpkPen::Public::MessageDispatcher message_dispatcher_;
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