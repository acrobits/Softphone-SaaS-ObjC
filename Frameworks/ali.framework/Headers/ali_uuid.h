#pragma once
#include "ali/ali_array_utils.h"
#include "ali/ali_integer.h"
#include "ali/ali_meta.h"
#include "ali/ali_random_forward.h"
#include "ali/ali_string.h"

namespace ali
{

// ******************************************************************
struct uuid
// ******************************************************************
{
    uuid& generate( void );

    uuid& generate_from_name( uuid const& ns, blob_const_ref name );
    
    uuid& generate_from_name( uuid const& ns, string_const_ref name )
    {
        return generate_from_name(ns, name.as_blob());
    }

    uuid& generate_from_name( blob_const_ref name );
    
    uuid& generate_from_name( string_const_ref name )
    {
        return generate_from_name(name.as_blob());
    }

    uuid& generate_random( random::uniform::generator& rng );
    uuid& generate_random( void );

    bool is_null( void ) const;

    ali::string& format( ali::string& str ) const;
        //  str += 8*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 12*XDIGIT
    
    ali::string format( void ) const
    {
        ali::string str;
        format(str);
        return str;
    }

    friend ali::string& format(
        ali::string& str,
        uuid const& value,
        ali::string_const_ref )
    {
        return value.format(str);
    }

    bool parse( string_const_ref str );
        //  str     := '{' uuid '}' | '[' uuid ']' | uuid
        //  uuid    := 8*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 4*XDIGIT '-' 12*XDIGIT

    void swap( uuid& b )
    {
        using ali::swap;
        swap(data1, b.data1);
        swap(data2, b.data2);
        swap(data3, b.data3);
        swap(data4, b.data4);
    }

    friend void swap( uuid& a, uuid& b )
    {
        a.swap(b);
    }

    ali::uint32 data1;
    ali::uint16 data2;
    ali::uint16 data3;
    ali::uint8  data4[8];

    static uuid const null;
    static uuid const namespace_dns;
    static uuid const namespace_url;
    static uuid const namespace_oid;
    static uuid const namespace_x500;
};

// ******************************************************************
ali_static_assert(meta::is_trivially_copyable<uuid>::result);
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
int compare( uuid const& a, uuid const& b );
// ******************************************************************

// ******************************************************************
inline bool operator==( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) == 0;
}

// ******************************************************************
inline bool operator!=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) != 0;
}

// ******************************************************************
inline bool operator<( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
inline bool operator<=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
inline bool operator>( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
inline bool operator>=( uuid const& a, uuid const& b )
// ******************************************************************
{
    return compare(a, b) >= 0;
}

// ******************************************************************
inline bool uuid::is_null( void ) const
// ******************************************************************
{
    return *this == null;
}

}   //  namespace ali
