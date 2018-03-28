#ifndef ARM_SIDE_H
#define ARM_SIDE_H
#include <boost/program_options.hpp>		//Спрятать в итоговой версии ПО
#include <boost/asio.hpp>
#include <message_dispatcher.h>
#include <udp_server.h>
#include <udp_client.h>
#include <atomic>
#include <mutex>
#include <map>

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
		    UdpServer udp_server_, service_server_;
		    unsigned tu_cntr_;
		    std::map < unsigned, MpkPen::Public::UdpClient_ptr > order_map_;		

		    std::mutex mutex_;
		    MpkPen::Public::MessageDispatcher md_;
    		    boost::asio::deadline_timer timer_;

		    void udp_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает команду ТУ для отправки по сервисному протоколу
		    void service_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает принятые квитанции от КТС УК

		    void delete_delivered_orders();					//Удаляет из очереди доставленные приказы
		    void delete_timeout_orders();					//Удаляет из очереди приказы с истекшим количеством попыток
		    void delete_orders();					

		    void handle_timeout( boost::system::error_code _ec );		//Периодически проходит по очереди приказов и очищает ее
		public:
		    Provider( boost::program_options::variables_map const& );
		    void run( );
	    
	    };
	}
    }
}
#endif //ARM_SIDE_H