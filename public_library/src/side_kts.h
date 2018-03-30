#ifndef KTS_SIDE_H
#define KTS_SIDE_H
#include <Order.pb.h>
#include <udp_server.h>
#include <udp_client.h>
#include <boost/asio.hpp>
#include <udp_client_manager.h>
#include <boost/circular_buffer.hpp>
#include <boost/program_options.hpp>		

namespace MpkPen
{
    namespace Public
    {
	namespace Kts
	{
	    class Provider
	    {
		    bool test_mode;
		    boost::asio::io_service io_service_;
		    int kts_port_, service_port_;
		    std::string mcast_group_;
		    UdpServer service_server_;
		    UdpClientManager client_manager_;
    		    boost::circular_buffer<MpkPen::Public::Order> orders_;
		    std::string plan_;
		    std::vector<unsigned> stations_;		    

		    void service_callback(std::string const& _s1, std::string& _s2);	//Обрабатывает принятые квитанции от КТС УК
		public:
		    Provider( boost::program_options::variables_map const& );
		    void run( );
	    
	    };
	}
	bool operator==( MpkPen::Public::Order const&, MpkPen::Public::Order const& );
    }
}
#endif //KTS_SIDE_H