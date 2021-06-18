#include "ali/ali_filesystem2_platform_posix.h"

namespace ali
{

namespace filesystem2
{

// ******************************************************************
bool exclude_from_backup( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
bool exclude_from_backup( c_wstring_const_ref path );
// ******************************************************************
    
// ******************************************************************
inline bool exclude_from_backup( filesystem2::path const& path )
// ******************************************************************
{
    return exclude_from_backup(path.format_platform_string());
}

}   //  namespace filesystem2

}   //  namespace ali
