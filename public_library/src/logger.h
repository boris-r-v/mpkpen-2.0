#ifndef LOGGER_H
#define LOGGER_H

#include <ostream>
#include <streambuf>

namespace MpkPen
{
    namespace Public
    {
	class Logger: public std::ostream, std::streambuf
	{	
	    public:
		Logger();
		static Logger& instance();

	    protected:
	        virtual std::streamsize xsputn(const char *_str, std::streamsize _size);
	        virtual int overflow(int _sym);

	    private:
		
	};
    }
}
#endif // LOGGER_H


