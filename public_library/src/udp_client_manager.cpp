#include<udp_client_manager.h>

void MpkPen::Public::UdpClientManager::start( UdpClient_ptr _p )
{
    udp_clients_.insert( _p );
}

void MpkPen::Public::UdpClientManager::stop( UdpClient_ptr _p )
{
    _p -> stop();
    udp_clients_.erase( _p );
}
