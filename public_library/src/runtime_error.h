#ifndef MPKPEN_PUBLIC_RUNTIME_ERROR
#define MPKPEN_PUBLIC_RUNTIME_ERROR

/**
* Class RuntimeError - ошибка исполнения, в конструктор описание ошибки
*/
#include <stdexcept>

namespace MpkPen
{
    namespace Public
    {
	class RuntimeError: public std::runtime_error
	{
	    public:		
		RuntimeError( std::string const& );
	};
    }
}

#endif // MPKPEN_PUBLIC_RUNTIME_ERROR
