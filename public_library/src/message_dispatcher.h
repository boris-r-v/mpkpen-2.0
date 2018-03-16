#ifndef MPKPEN_PUBLIC_MESSAGE_DISPATCHER
#define MPKPEN_PUBLIC_MESSAGE_DISPATCHER

#include <message_processor.h>
#include <iostream>
namespace MpkPen
{
    namespace Public
    {
	class MessageDispatcher
	{
	    public:
		MessageDispatcher() = default;

		void addProcessor( MpkPen::Public::MessageProcessorBasePtr processor);
		
		MpkPen::Public::Message dispatch( MpkPen::Public::Message const& query );
	

	    private:
		typedef std::map <std::string, MpkPen::Public::MessageProcessorBasePtr > DispatcherImplType;
		DispatcherImplType mImpl_;
	};

	MessageDispatcher create_message_dispatcher();

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