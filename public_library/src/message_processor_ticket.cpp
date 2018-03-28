#include <message_processor_ticket.h>

namespace mp = MpkPen::Public;

mp::MessageTicketProcessor::MessageTicketProcessor( OrderCounter& _oc ):
    order_counter_( _oc )
{
}

std::string mp::MessageTicketProcessor::id() const
{
    return mp::Ticket::default_instance().message_type();
}

mp::Ticket mp::MessageTicketProcessor::doProcess( mp::Ticket const& _ticket )
{
    return _ticket;
}
