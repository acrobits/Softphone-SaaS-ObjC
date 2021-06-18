#pragma once

#include "ali/ali_string_map.h"

namespace ali
{

namespace mime
{

// ******************************************************************
typedef small_string_map<nocase_comparator> parameters;
// ******************************************************************

// ******************************************************************
parameters& parse_parameters(
    parameters& params,
    ali::string_const_ref str );
// ******************************************************************
//  str := [parameter] (; parameter)*
//  parameter := token ['=' (token | quoted-string)]
// ******************************************************************

// ******************************************************************
ali::string& format_parameter_and_append(
    ali::string& str,
    ali::string_const_ref name,
    ali::string_const_ref value );
// ******************************************************************
//  Appends "name=value" quoting value if neccessary.
//  NOTE: This function does NOT prepend or append
//  any separators.
// ******************************************************************

// ******************************************************************
inline ali::string format_parameter(
    ali::string_const_ref name,
    ali::string_const_ref value )
// ******************************************************************
//  Returns "name=value" quoting value if neccessary.
//  NOTE: This function does NOT prepend or append
//  any separators.
// ******************************************************************
{
    ali::string str;
    format_parameter_and_append(str, name, value);
    return str;
}

namespace functor_types
{

// ******************************************************************
struct is_token_special_char
// ******************************************************************
//  Must be in quoted-string, to use within parameter values.
// ******************************************************************
{
    bool operator()( char c ) const
    {
        return c == '(' || c == ')' || c == '<'
            || c == '>' || c == '@' || c == ','
            || c == ';' || c == ':' || c == '\\'
            || c == '\"' || c == '/' || c == '['
            || c == ']' || c == '?' || c == '=';
    }
};

}   //  namespace functor_types

// ******************************************************************
extern functor_types::is_token_special_char const is_token_special_char;
// ******************************************************************

namespace functor_types
{

// ******************************************************************
struct is_token_char
// ******************************************************************
//  token := 1*<any (US-ASCII) CHAR except SPACE, CTLs, or tspecials>
// ******************************************************************
{
    bool operator()( char c ) const
    {
        //  http://tools.ietf.org/html/rfc822#section-3.3
        //  CHAR  = <any ASCII character> {0-127}
        //  CTL   = <any ASCII control character and DEL> {0-31,127}
        //  SPACE = <ASCII SP, space> {32}

        //  CHAR - SPACE - CTLS
        //      = {0-127} - {0-31,127} - {32}
        //      = {33-126} = {'!' - '~'}

        return '!' <= c && c <= '~' && !mime::is_token_special_char(c);
    }
};

}   //  namespace functor_types

// ******************************************************************
extern functor_types::is_token_char const is_token_char;
// ******************************************************************

// ******************************************************************
inline bool is_token( ali::string_const_ref str )
// ******************************************************************
{
    return  !str.is_empty()
                //  Token is a non-empty string...
        &&  str.all(is_token_char);
                //  ...containing only token characters.
}

}   //  namespace mime

}   //  namespace ali
