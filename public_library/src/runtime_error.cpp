#include <runtime_error.h>

namespace mp = MpkPen::Public;

mp::RuntimeError::RuntimeError( std::string const& _s): std::runtime_error( _s )
{
}
