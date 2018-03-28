#include <logger.h>
#include <message_dispatcher.h>
#include <message_processor_order.h>		//for dispatcher fabric
#include <message_processor_ticket.h>		//for dispatcher fabric

namespace mp = MpkPen::Public;		

mp::MessageDispatcher::MessageDispatcher()
{
    addProcessor ( std::make_shared<MessageOrderProcessor>( order_counter_) );
    addProcessor ( std::make_shared<MessageTicketProcessor>( order_counter_) );
}

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
	

mp::Message mp::MessageDispatcher::create_tu_message( std::string const& _arm_tu )
{
    MpkPen::Public::Order ord;
    //ord.set_order_number( std::static_pointer_cast<MessageOrderProcessor>(mImpl_[mp::Order::default_instance().message_type()])->get_next_tu_id() );
    ord.set_order_number( order_counter_.get_next_order_id() );
    ord.set_order_data( _arm_tu );
    
    return MpkPen::Public::pack_message( ord );
}

bool mp::MessageDispatcher::check_tu_ticket( std::string const& _ticket )
{
    MpkPen::Public::Message msg_in;
    if ( !msg_in.ParseFromString( _ticket ) )
    {
	MpkPen::Public::Logger::instance() << "Error(check_tu_ticket) while parse ticket message: " << _ticket << std::endl; 	
	return false;
    }
    try
    {
	//MpkPen::Public::Message kts_ticket = dispatch ( msg_in );
	mp::Ticket tic ( mp::unpack_message<mp::Ticket>( msg_in ) );
	if ( tic.order_ticket ( ) != mp::Ticket::ONCE )
	    MpkPen::Public::Logger::instance() << "Previous ticket for order: "<< tic.order_number() << ", has being missing"  << std::endl; 	

	return order_counter_.check_order_number( tic.order_number() );
    }
    catch ( std::exception const& _e )
    {
	MpkPen::Public::Logger::instance() << "Exceptinon while check_tu_ticket: " << _e.what() << std::endl; 	
    }	
    return false;
}
