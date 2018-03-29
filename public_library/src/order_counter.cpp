#include <logger.h>
#include <order_counter.h>
#define MAX_ORDER_IN_LIST 20 

MpkPen::Public::Number::Number( unsigned _id ):
    id_( _id ), deleted_( false )	
{
}
unsigned MpkPen::Public::Number::id() const
{
    return id_;
}
bool MpkPen::Public::Number::deleted() const
{
    return deleted_;
}
void MpkPen::Public::Number::deleted( bool _b )
{
    deleted_ = _b;
} 


MpkPen::Public::OrderCounter::OrderCounter():
    current_id_ ( 0 ),
    numbers_( MAX_ORDER_IN_LIST )
{
}

unsigned MpkPen::Public::OrderCounter::get_next_order_id()
{
    ++current_id_;
    unsigned id = current_id_.load();
    numbers_.push_back( id );
    return id;
}

bool MpkPen::Public::OrderCounter::check_order_number( unsigned _id )
{
    auto fnd = std::find_if(numbers_.begin(), numbers_.end(), [&_id](Number const& n){ return _id == n.id(); } );
    if ( fnd != numbers_.end() )
    {
	if ( false == fnd->deleted( ) )
	{
	    fnd->deleted( true );
	    return true;
	}
	else
	{
	    MpkPen::Public::Logger::instance() << "Duplicate ticket for order " << std::endl; 	
	    return true;
	}
    }
    MpkPen::Public::Logger::instance() << "Order with id: " <<_id << ", not found into list, may be it`s very old?OC"  << std::endl; 	
    return false;
}

