#pragma once

#include "ali/ali_tstring_forward.h"

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

// ******************************************************************
// ******************************************************************
#if 0
// ******************************************************************
class core_dump_file_owner
// ******************************************************************
//  The destructor of the last object referencing a given
//  core file deletes the file, unless told not to.
// ******************************************************************
{
public:
    core_dump_file_owner( void );
    explicit core_dump_file_owner( ali::tchar const* path );
    core_dump_file_owner( core_dump_file_owner const& b );
    core_dump_file_owner( core_dump_file_owner&& b );

    ~core_dump_file_owner( void );

    core_dump_file_owner& operator=( core_dump_file_owner b );

    bool is_null( void ) const
    {
        return _data == nullptr;
    }

    ali::tchar const* path( void ) const;

    bool keep_file( void ) const;
    
    void set_keep_file( bool value = true );

private:    //  Methods
    static void try_delete_file( ali::tchar const* path );
        //  To be implemented in ali_debug_platform.cpp.

private:    //  Data members
    struct data;
    data*   _data{};
};

// ******************************************************************
core_dump_file_owner generate_core_dump_file( void );
// ******************************************************************
#endif
}   //  namespace debug

}   //  namespace ali
