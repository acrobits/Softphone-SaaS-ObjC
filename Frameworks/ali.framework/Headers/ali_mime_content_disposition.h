#pragma once

#include "ali/ali_mime_parameters.h"
#include "ali/ali_string.h"

namespace ali
{

namespace mime
{

// ******************************************************************
struct content_disposition
// ******************************************************************
{
    struct disposition_type
    {
        disposition_type( void ) {}

        explicit disposition_type( ali::string const& value )
        :   value(value)
        {}

        friend bool operator==(
            disposition_type const& a,
            disposition_type const& b )
        {
            return a == b.value;
        }

        friend bool operator==(
            disposition_type const& a,
            string_const_ref b )
        {
            return b.nocase_is_equal_to(a.value);
        }

        friend bool operator==(
            string_const_ref a,
            disposition_type const& b )
        {
            return a.nocase_is_equal_to(b.value);
        }

        friend bool operator!=(
            disposition_type const& a,
            disposition_type const& b )
        {
            return !(a == b);
        }

        friend bool operator!=(
            disposition_type const& a,
            string_const_ref b )
        {
            return !(a == b);
        }

        friend bool operator!=(
            string_const_ref a,
            disposition_type const& b )
        {
            return !(a == b);
        }

        void swap( disposition_type& b )
        {
            using ali::swap;
            swap(value, b.value);
        }

        friend void swap( disposition_type& a, disposition_type& b )
        {
            a.swap(b);
        }

        bool parse( ali::string_const_ref str );

        bool is_valid( void ) const;

        disposition_type& reset( void )
        {
            value.erase();
            return *this;
        }

        ali::string& format_and_append( ali::string& str ) const;

        ali::string format( void ) const
        {
            ali::string str;
            format_and_append(str);
            return str;
        }

        ali::string value;
    };

    content_disposition( void ) {}

    content_disposition( ali::string_const_ref str )
    {
        parse(str);
    }

    bool operator==( content_disposition const& b ) const
    {
        return  type == b.type
            &&  params == b.params;
    }

    bool operator!=( content_disposition const& b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return type.value.is_empty();
    }

    bool is( string_const_ref query_type ) const
    {
        return type == query_type;
    }

    bool is_inline( void ) const;

    bool is_attachment( void ) const;

    content_disposition& set( ali::string const& another_type )
    {
        type.value = another_type;
        return *this;
    }

    content_disposition& set_inline( void );
    
    content_disposition& set_attachment( void );

    content_disposition& reset( void )
    {
        type.reset();
        params.erase();
        return *this;
    }

    bool parse( ali::string_const_ref str );
        //  str := disposition-type (; parameter)*"

    ali::string& format_and_append(
        ali::string& str ) const;
            //  Appends "disposition-type (; parameter)*"

    ali::string format( void ) const
            //  Returns "disposition-type (; parameter)*"
    {
        ali::string str;
        format_and_append(str);
        return str;
    }

    void swap( content_disposition& b )
    {
        using ali::swap;
        swap(type, b.type);
        swap(params, b.params);
    }

    friend void swap(
        content_disposition& a,
        content_disposition& b )
    {
        a.swap(b);
    }

    disposition_type    type;
    parameters          params;
};

}   //  namespace mime

}   //  namespace ali
