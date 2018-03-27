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
		    boost::asio::io_service io_service_;
		    int arm_port_, kts_port_, service_port_;
		    UdpServer udp_server_, service_server_;
		    unsigned tu_cntr_;
		    std::map <unsigned, std::shared_ptr<MpkPen::Public::UdpClient> > order_map_;		
		    std::mutex mutex_;
		    MpkPen::Public::MessageDispatcher md_;

		    void udp_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает команду ТУ для отправки по сервисному протоколу
		    void service_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает принятые квитанции от КТС УК

		    void delete_delivered_orders();					//Удаляет из очереди доставленные приказы
		public:
		    Provider( boost::program_options::variables_map const& );
		    void run( );
	    
	    };
	}
    }
}
#endif //ARM_SIDE_H