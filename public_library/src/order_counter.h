#ifndef MPKPEN_PUBLIC_ORDER_COUNTER
#define MPKPEN_PUBLIC_ORDER_COUNTER

#include <mutex>
#include <time.h>
#include <boost/circular_buffer.hpp>

namespace MpkPen
{
    namespace Public
    {
	class Number
	{
		unsigned id_;
		bool deleted_;
	    public:
		Number()=delete;
		Number( unsigned );
		unsigned id() const;
		bool deleted() const;
		void deleted( bool ) ;
	};
	class OrderCounter
	{
		unsigned  id_;
	        boost::circular_buffer<Number>  numbers_;
		std::mutex mutex_;
	    public:
		OrderCounter();

		unsigned get_next_order_id();
		bool check_order_number( unsigned );

	};
    }
}
#endif // MPKPEN_PUBLIC_ORDER_COUNTER
