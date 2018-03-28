#ifndef MPKPEN_PUBLIC_MESSAGE_ORDER
#define MPKPEN_PUBLIC_MESSAGE_ORDER

#include <message_processor.h>
#include <order_counter.h>
#include <Order.pb.h>
#include <Ticket.pb.h>

namespace MpkPen
{
    namespace Public
    {
	class MessageOrderProcessor: public MessageProcessorBase <MpkPen::Public::Order, MpkPen::Public::Ticket>
	{
	    public:		
		MessageOrderProcessor() = delete;
		~MessageOrderProcessor() = default;
		MessageOrderProcessor( OrderCounter& );

		virtual std::string id() const;

	    private:
		virtual MpkPen::Public::Ticket doProcess( MpkPen::Public::Order const& );
		OrderCounter& order_counter_;
	};
    }
}

#endif //MPKPEN_PUBLIC_MESSAGE_ORDER
