#include <message_processor_order.h>

namespace mp = MpkPen::Public;

std::string mp::MessageOrderProcessor::id() const
{
    return mp::Order::default_instance().message_type();
}

mp::Ticket mp::MessageOrderProcessor::doProcess( mp::Order const& _order )
{
    mp::Ticket ret;
    ret.set_order_number ( _order.order_number() );
    ret.set_order_ticket ( mp::Ticket::ONCE );
    return ret;
}
