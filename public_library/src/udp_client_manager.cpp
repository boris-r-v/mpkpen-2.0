#include <udp_client_manager.h>
//#include <message_dispatcher.h>


MpkPen::Public::UdpClientManager::UdpClientManager( MessageDispatcher& _md ):
    message_dispatcher_( _md )
{
}

void MpkPen::Public::UdpClientManager::start( UdpClient_ptr _p )
{
    udp_clients_.insert( _p );
}

void MpkPen::Public::UdpClientManager::stop( UdpClient_ptr _p )
{
    _p -> stop();
    udp_clients_.erase( _p );
}

MpkPen::Public::MessageDispatcher& MpkPen::Public::UdpClientManager::message_dispatcher()
{
    return message_dispatcher_;
}