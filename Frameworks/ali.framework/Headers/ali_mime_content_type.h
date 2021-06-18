#pragma once

#include "ali/ali_mime_parameters.h"
#include "ali/ali_string.h"

namespace ali
{

namespace mime
{

// ******************************************************************
struct content_type
// ******************************************************************
{
    struct type_subtype_info
    {
        type_subtype_info( void ) {}

        type_subtype_info(
            ali::string const& type,
            ali::string const& subtype )
        :   type(type),
            subtype(subtype)
        {}

        type_subtype_info& set(
            ali::string const& other_type,
            ali::string const& other_subtype )
        {
            type = other_type;
            subtype = other_subtype;
            return *this;
        }

        type_subtype_info& reset( void )
        {
            type.erase();
            subtype.erase();
            return *this;
        }

        void swap( type_subtype_info& b )
        {
            using ali::swap;
            swap(type, b.type);
            swap(subtype, b.subtype);
        }

        friend void swap( type_subtype_info& a, type_subtype_info& b )
        {
            a.swap(b);
        }

        bool parse( ali::string_const_ref value );
            //  value := "type/subtype"

        bool is_empty( void ) const
        {
            return type.is_empty() && subtype.is_empty();
        }

        bool is_valid( void ) const;

        ali::string& format_and_append( ali::string& str ) const;
            //  Appends "type/subtype".

        ali::string format( void ) const
            //  Returns "type/subtype".
        {
            ali::string str;
            format_and_append(str);
            return str;
        }

        ali::string type;
        ali::string subtype;
    };

    content_type( void ) {}

    content_type( ali::string_const_ref value )
    {
        parse(value);
    }

    //  Matching of media type and subtype
    //  is ALWAYS case-insensitive.

    bool operator==( content_type const& b ) const
    {
        return  *this == b.type_subtype
            &&  params == b.params;
    }

    bool operator!=( content_type const& b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return type_subtype.is_empty();
    }

    bool is_type( ali::string_const_ref type ) const
    {
        return type_subtype.type.nocase_compare(type) == 0;
    }

    bool is_subtype( ali::string_const_ref subtype ) const
    {
        return type_subtype.subtype.nocase_compare(subtype) == 0;
    }

    bool subtype_ends_with( ali::string_const_ref suffix ) const
    {
        return type_subtype.subtype.nocase_ends_with(suffix);
    }

    bool is(
        ali::string_const_ref type,
        ali::string_const_ref subtype ) const
    {
        return is_type(type) && is_subtype(subtype);
    }

    bool operator==( type_subtype_info const& b ) const
    {
        return is(b.type, b.subtype);
    }

    bool operator!=( type_subtype_info const& b ) const
    {
        return !operator==(b);
    }

    //  text/*
    bool is_text( void ) const;

    bool is_text( ali::string_const_ref subtype ) const
    {
        return is_text() && is_subtype(subtype);
    }

    //  image/*
    bool is_image( void ) const;

    bool is_image( ali::string_const_ref subtype ) const
    {
        return is_image() && is_subtype(subtype);
    }

    //  audio/*
    bool is_audio( void ) const;

    bool is_audio( ali::string_const_ref subtype ) const
    {
        return is_audio() && is_subtype(subtype);
    }

    //  video/*
    bool is_video( void ) const;

    bool is_video( ali::string_const_ref subtype ) const
    {
        return is_video() && is_subtype(subtype);
    }

    //  application/*
    bool is_application( void ) const;

    bool is_application( ali::string_const_ref subtype ) const
    {
        return is_application() && is_subtype(subtype);
    }

    //  message/*
    bool is_message( void ) const;

    bool is_message( ali::string_const_ref subtype ) const
    {
        return is_message() && is_subtype(subtype);
    }

    //  multipart/*
    bool is_multipart( void ) const;

    bool is_multipart( ali::string_const_ref subtype ) const
    {
        return is_multipart() && is_subtype(subtype);
    }

    ali::c_string_const_ref default_dot_extension( void ) const;
    ali::c_string_const_ref default_dotless_extension( void ) const;

    content_type& set(
        ali::string const& type,
        ali::string const& subtype )
    {
        type_subtype.set(type, subtype);
        return *this;
    }

    content_type& reset( void )
    {
        type_subtype.reset();
        params.erase();
        return *this;
    }

    bool parse( ali::string_const_ref str );
        //  str := "type/subtype (; parameter)*"

    ali::string& format_and_append(
        ali::string& str ) const;
            //  Appends "type/subtype (; parameter)*"

    ali::string format( void ) const
            //  Returns "type/subtype (; parameter)*"
    {
        ali::string str;
        format_and_append(str);
        return str;
    }

    void swap( content_type& b )
    {
        using ali::swap;
        swap(type_subtype, b.type_subtype);
        swap(params, b.params);
    }

    friend void swap( content_type& a, content_type& b )
    {
        a.swap(b);
    }

    type_subtype_info   type_subtype;
    parameters          params;
};

}   //  namespace mime

}   //  namespace ali
