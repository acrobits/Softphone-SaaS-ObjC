#pragma once

#include "ali/ali_string_forward.h"

namespace ali
{

// ******************************************************************
template <bool> struct static_assert_noexcept;
    //  Intentionally not defined.
template <> struct static_assert_noexcept<true> {};
// ******************************************************************

}   //  namespace ali

// ******************************************************************
#define ali_static_assert(x) static_assert(x, #x)
// ******************************************************************
//  Fixed in C++17
// ******************************************************************

// ******************************************************************
#define ali_sanoex(x)                                               \
    (((void)sizeof(ali::static_assert_noexcept<noexcept(x)>)), x)
// ******************************************************************
//  A static assert *expression* asserting that
//  the given expression doesn't throw an exception.
// ******************************************************************

namespace ali
{

// ******************************************************************
inline constexpr void noop( void )
// ******************************************************************
{}

// ******************************************************************
// ******************************************************************

namespace debug
{

// ******************************************************************
void abort( void );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct demangle_result
// ******************************************************************
{
    enum type
    {
        success,
        not_implemented,
        out_of_memory,
        syntax_error,
        general_error
    };

    demangle_result( type value )
    :   value{value}
    {}

    friend bool operator==( demangle_result a, demangle_result b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( demangle_result a, demangle_result b )
    {
        return !(a == b);
    }

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_not_implemented( void ) const
    {
        return value == not_implemented;
    }

    bool is_out_of_memory( void ) const
    {
        return value == out_of_memory;
    }

    bool is_syntax_error( void ) const
    {
        return value == syntax_error;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    type    value{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
demangle_result demangle_typeid_name_in_place( ali::string& name );
// ******************************************************************

}   //  namespace debug

}   //  namespace ali
