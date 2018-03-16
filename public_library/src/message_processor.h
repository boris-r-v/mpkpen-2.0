#ifndef MPKPEN_PUBLIC_MESSAGE_PROCESSOR_BASE
#define MPKPEN_PUBLIC_MESSAGE_PROCESSOR_BASE

#include <memory>
#include <Message.pb.h>
#include <runtime_error.h>
#include <boost/noncopyable.hpp>

namespace MpkPen
{
    namespace Public
    {
	class ProcessorBase: public boost::noncopyable
	{
	    public:		
		virtual ~ProcessorBase() = default;

		virtual std::string id() const = 0;

		virtual MpkPen::Public::Message process( MpkPen::Public::Message const& ) = 0;
	};

	typedef std::shared_ptr<ProcessorBase> MessageProcessorBasePtr;

	template <class QueryT, class AnswerT >
	class MessageProcessorBase: public ProcessorBase
	{
	    public:
	        virtual MpkPen::Public::Message process( MpkPen::Public::Message const& _query )
		{
		    QueryT packed_query;
		    if ( !packed_query.ParseFromString( _query.data() ) )
		    {
			throw RuntimeError ( "Failed to parse query: \""+_query.ShortDebugString()+"\"" );
		    }
		    
		    AnswerT packed_answer = doProcess( packed_query );
		    MpkPen::Public::Message answer;
		    answer.set_id( _query.id() );

		    if ( !packed_answer.SerializeToString( answer.mutable_data() ) )
		    {
			throw RuntimeError ( "Failed to prepare answer: \""+packed_answer.ShortDebugString()+"\"" );
		    }
		    return answer;
		
	        }
	    private:
		virtual AnswerT doProcess( QueryT const& ) = 0;
	};
    }
}


#endif //MPKPEN_PUBLIC_MESSAGE_PROCESSOR_BASE