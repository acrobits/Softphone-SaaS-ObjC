//
//  ali_format.h
//

#pragma once

#include "ali/ali_shared_ptr_intrusive.h"
#include "ali/ali_string.h"

namespace ali
{

// ******************************************************************
ali::string& format(
    ali::string& str,
    void const* value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
ali::string& format(
    ali::string& str,
    T const* value,
    string_const_ref format_string )
// ******************************************************************
{
    format(str, static_cast<void const*>(value), {});
    if ( value != nullptr )
        format(str.append(": "_s), *value, format_string);
    return str;
}

// ******************************************************************
// ******************************************************************

// *******************************************************************
template <typename T>
inline ali::string& format(
    ali::string& str,
    shared_ptr_intrusive<T> const& pointer,
    string_const_ref format_string )
// *******************************************************************
{
    return format(str, pointer.get(), format_string);
}

}   //  namespace ali
