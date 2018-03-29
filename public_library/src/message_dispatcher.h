#ifndef MPKPEN_PUBLIC_MESSAGE_DISPATCHER
#define MPKPEN_PUBLIC_MESSAGE_DISPATCHER

#include <message_processor.h>
#include <order_counter.h>
#include <iostream>
namespace MpkPen
{
    namespace Public
    {
	class MessageDispatcher
	{
	    public:
		MessageDispatcher();

		void addProcessor( MpkPen::Public::MessageProcessorBasePtr processor);
		
		MpkPen::Public::Message dispatch( MpkPen::Public::Message const& query );
		
		/*Со стороны АРМа*/
		MpkPen::Public::Message create_tu_message( std::string const& );
		bool check_tu_ticket( std::string const& );

	    private:
		typedef std::map <std::string, MpkPen::Public::MessageProcessorBasePtr > DispatcherImplType;
		DispatcherImplType mImpl_;
		OrderCounter order_counter_;
	};

	template <class T>
	MpkPen::Public::Message pack_message( T const& _im )
	{
	    MpkPen::Public::Message msg;
	    msg.set_id( _im.message_type() );
	    if ( !_im.SerializeToString( msg.mutable_data() ) )
	    {
		throw RuntimeError ( "Failed while pack data to message: \""+ msg.ShortDebugString()+"\"" );
		//CHECK exception messge
	    }
	    return msg;
	}

	template <class T>
	T unpack_message( MpkPen::Public::Message const& _im )
	{
	    T ret;
	    if ( !ret.ParseFromString( _im.data() ) )
	    {
		throw RuntimeError ( "Failed while unpack from message: \""+_im.ShortDebugString()+"\"" );
		//CHECK exception messge
	    }
	    return ret;
	}

    }
}


#endif //MPKPEN_PUBLIC_MESSAGE_DISPATCHER