#include <logger.h>
#include <iostream>
MpkPen::Public::Logger::Logger():
    std::ostream( this )	
{
}

int MpkPen::Public::Logger::overflow( int _sym )
{
    std::cout << std::string(1, static_cast<char>(_sym));
    return _sym;
}


std::streamsize MpkPen::Public::Logger::xsputn(const char *_str, std::streamsize _size)
{
    std::cout << std::string( _str, _size );
    return _size;
}


MpkPen::Public::Logger& MpkPen::Public::Logger::instance()
{
    static Logger self_logger_;
    return self_logger_;
}
