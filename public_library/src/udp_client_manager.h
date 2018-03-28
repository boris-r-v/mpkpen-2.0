#ifndef COMMON_UDP_CLIENT_MANAGER_H
#define COMMON_UDP_CLIENT_MANAGER_H


#include <set>
#include <udp_client.h>


namespace MpkPen
{
    namespace Public
    {

	class UdpClientManager
	{
	    std::set<UdpClient_ptr> udp_clients_;
	    
	    public:
		void start( UdpClient_ptr );
		void stop( UdpClient_ptr );


	};
    }
}
#endif //COMMON_UDP_CLIENT_MANAGER_H
