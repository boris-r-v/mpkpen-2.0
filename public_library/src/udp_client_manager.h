#ifndef COMMON_UDP_CLIENT_MANAGER_H
#define COMMON_UDP_CLIENT_MANAGER_H


#include <set>
#include <udp_client.h>


namespace MpkPen
{
    namespace Public
    {
	class MessageDispatcher;
	class UdpClientManager
	{
		std::set<UdpClient_ptr> udp_clients_;
		MessageDispatcher& message_dispatcher_;

	    public:
		UdpClientManager() = delete;
		virtual ~UdpClientManager() = default;

		UdpClientManager( MessageDispatcher& );
		void start( UdpClient_ptr );
		void stop( UdpClient_ptr );
		MpkPen::Public::MessageDispatcher& message_dispatcher();

	};
    }
}
#endif //COMMON_UDP_CLIENT_MANAGER_H
