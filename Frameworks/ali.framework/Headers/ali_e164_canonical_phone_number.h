#pragma once
#include "ali/ali_string.h"

namespace ali
{

namespace e164
{

// ******************************************************************
class canonical_phone_number
// ******************************************************************
{
public:
    struct functor_types
    {
        struct is_valid_char
        {
            bool operator()( char c ) const
            {
                string_literal const allowed("0123456789*#,+;");
                return allowed.contains(c);
            }
        };

        struct is_not_valid_char : private is_valid_char
        {
            bool operator()( char c ) const
            {
                return !is_valid_char::operator()(c);
            }
        };
    };

public:
    canonical_phone_number( void ) {}

    explicit canonical_phone_number( string_const_ref e164_number );

    bool is_empty( void ) const
    {
        return _number.is_empty();
    }

    ali::string const& get( void ) const
    {
        return _number;
    }

    canonical_phone_number& append( int n, char c )
    {
        if ( is_valid_char(c) )
            _number.append_(c, n);
        return *this;
    }

    bool operator==( canonical_phone_number const& b ) const
    {
        return operator==(b._number);
    }

    bool operator==( string_const_ref canonical_e164_number ) const
    {
        return _number == canonical_e164_number;
    }

    bool operator!=( canonical_phone_number const& b ) const
    {
        return !operator==(b);
    }

    bool operator!=( string_const_ref canonical_e164_number ) const
    {
        return !operator==(canonical_e164_number);
    }

    static functor_types::is_valid_char const is_valid_char;
    static functor_types::is_not_valid_char const is_not_valid_char;

private:
    ali::string _number;
};

}   //  namespace e164

}   //  namespace ali
