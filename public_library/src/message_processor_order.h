#ifndef MPKPEN_PUBLIC_MESSAGE_ORDER
#define MPKPEN_PUBLIC_MESSAGE_ORDER

#include <message_processor.h>
#include <Order.pb.h>
#include <Ticket.pb.h>

namespace MpkPen
{
    namespace Public
    {
	class MessageOrderProcessor: public MessageProcessorBase <MpkPen::Public::Order, MpkPen::Public::Ticket>
	{
	    public:		
		MessageOrderProcessor() = default;
		~MessageOrderProcessor() = default;
		virtual std::string id() const;

		size_t get_next_tu_id();
	    private:
		virtual MpkPen::Public::Ticket doProcess( MpkPen::Public::Order const& );
	};
    }
}

#endif //MPKPEN_PUBLIC_MESSAGE_ORDER
