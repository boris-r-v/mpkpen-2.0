#include <message_dispatcher.h>
#include <message_processor_order.h>		//for dispatcher fabric
#include <message_processor_ticket.h>		//for dispatcher fabric

namespace mp = MpkPen::Public;		

void mp::MessageDispatcher::addProcessor( MpkPen::Public::MessageProcessorBasePtr processor)
{
    mImpl_[processor->id()] = processor;
}

		
mp::Message mp::MessageDispatcher::dispatch( MpkPen::Public::Message const& query )
{

    auto fnd = mImpl_.find( query.id() );
    if ( fnd == mImpl_.end() )
    {
	throw mp::RuntimeError ( "Unsupported message type:\""+query.id()+"\"" );
    }
    return fnd->second->process( query );
}
	

mp::MessageDispatcher mp::create_message_dispatcher()
{

    mp::MessageDispatcher md;
    md.addProcessor ( std::make_shared<MessageOrderProcessor>() );
    md.addProcessor ( std::make_shared<MessageTicketProcessor>() );

    return md;
}

mp::Message mp::MessageDispatcher::create_tu_message( std::string& const _arm_tu )
{
    MpkPen::Public::Order ord;
    ord.set_order_number( mImpl_[mp::Order::default_instance().message_type()].get_next_tu_id() );
    ord.set_order_data( _arm_tu );
    
    return MpkPen::Public::pack_message( ord );

}
