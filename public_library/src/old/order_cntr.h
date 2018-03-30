#ifndef ORDER_CNTR_H
#define ORDER_CNTR_H

#include <atomic>
#include <boost/circular_buffer.hpp>

namespace MpkPen
{
    namespace Public
    {
	class Order_Cntr
	{

		std::atomic <unsigned> cntr_;
		boost::circular_buffer<unsigned> ids_;

	    public:
		Order_Cntr();

		unsigned get_next_id();			//возвращает следующий номер приказа

		void set_recieved_id( unsigned );

		bool check_order_id( unsigned );


	};
    }
}
#endif // ORDER_CNTR_H
