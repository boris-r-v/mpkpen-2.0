#include <client.h>
#include <Order.pb.h>
#include <iostream>

void MpkPen::Public::Client::protobuf()
{
    MpkPen::Public::Order ord;
    ord.set_order_number( 333 );
    ord.set_order_data( "asvfrcqwjh$%%%%%34sdfhjs" );
    
    std::cout << "GET " << ord.order_number()<< std::endl;
    std::cout << "GET " << ord.order_data()<< std::endl;

    ord.SerializeToOstream( &std::cout );


}
