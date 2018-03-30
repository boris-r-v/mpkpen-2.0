#ifndef MPKPEN_PUBLIC_MESSAGE_TICKET
#define MPKPEN_PUBLIC_MESSAGE_TICKET

#include <message_processor.h>
#include <Ticket.pb.h>

namespace MpkPen
{
    namespace Public
    {
	class MessageTicketProcessor: public MessageProcessorBase < MpkPen::Public::Ticket, MpkPen::Public::Ticket >
	{
	    public:		
		MessageTicketProcessor() = default;
		~MessageTicketProcessor() = default;

		virtual std::string id() const;

	    private:
		virtual MpkPen::Public::Ticket doProcess( MpkPen::Public::Ticket const& );
	};
    }
}

#endif //MPKPEN_PUBLIC_MESSAGE_TICKET