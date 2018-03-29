#ifndef MPKPEN_PUBLIC_MESSAGE_ORDER
#define MPKPEN_PUBLIC_MESSAGE_ORDER

#include <message_processor.h>
#include <order_counter.h>
#include <Order.pb.h>
#include <Ticket.pb.h>
#include <memory>
#include <set>

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

		/*методы стороны АРМа*/
		MpkPen::Public::Order const& create_order( std::string const& );


	    private:
		virtual MpkPen::Public::Ticket doProcess( MpkPen::Public::Order const& );
		OrderCounter& order_counter_;

		class OrderWrapper: public boost::noncopyable, public std::enable_shared_from_this<OrderWrapper>
		{
			MpkPen::Public::Order order_;
		    public:
			OrderWrapper( std::string const&, unsigned );
	    
			MpkPen::Public::Order const& order() const;
			unsigned id();
		};
		typedef std::shared_ptr<OrderWrapper> OrderWrapper_ptr;
		std::set <OrderWrapper_ptr> orders_;	    		
	};


    }
}

#endif //MPKPEN_PUBLIC_MESSAGE_ORDER
