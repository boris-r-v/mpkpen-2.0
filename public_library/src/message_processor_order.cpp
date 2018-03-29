#include <message_processor_order.h>

//namespace mp = MpkPen::Public;

MpkPen::Public::MessageOrderProcessor::MessageOrderProcessor( OrderCounter& _oc ):
    order_counter_( _oc )
{
}

std::string MpkPen::Public::MessageOrderProcessor::id() const
{
    return MpkPen::Public::Order::default_instance().message_type();
}

MpkPen::Public::Ticket MpkPen::Public::MessageOrderProcessor::doProcess( MpkPen::Public::Order const& _order )
{
    MpkPen::Public::Ticket ret;
    ret.set_order_number ( _order.order_number() );
    ret.set_order_ticket ( MpkPen::Public::Ticket::ONCE );
    return ret;
}

MpkPen::Public::Order const& MpkPen::Public::MessageOrderProcessor::create_order( std::string const& _arm_tu )
{
    auto ord = std::make_shared<OrderWrapper>(_arm_tu, order_counter_.get_next_order_id() );
    orders_.insert( ord );
    return ord->order();
}

/*--------------Wrapper---------------*/
MpkPen::Public::MessageOrderProcessor::OrderWrapper::OrderWrapper( std::string const& _tu, unsigned _id )
{
    order_.set_order_number( _id );
    order_.set_order_data( _tu );
}
    
MpkPen::Public::Order const& MpkPen::Public::MessageOrderProcessor::OrderWrapper::order() const
{
    return order_;
}

unsigned MpkPen::Public::MessageOrderProcessor::OrderWrapper::id()
{
    return order_.order_number();
}
