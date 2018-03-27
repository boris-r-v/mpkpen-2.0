#include <logger.h>

MpkPen::Public::Logger::Logger():
    std::ostream( this )	
{
}

int MpkPen::Public::Logger::overflow( int c )
{
    return c;
}


std::streamsize MpkPen::Public::Logger::xsputn(const char *_str, std::streamsize _size)
{
    return _size;
}


MpkPen::Public::Logger& MpkPen::Public::Logger::instance()
{
    static Logger self_logger_;
    return self_logger_;
}
