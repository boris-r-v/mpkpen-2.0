#include <message_processor_ticket.h>

namespace mp = MpkPen::Public;


std::string mp::MessageTicketProcessor::id() const
{
    return mp::Ticket::default_instance().message_type();
}

mp::Ticket mp::MessageTicketProcessor::doProcess( mp::Ticket const& _ticket )
{
    return _ticket;
}
