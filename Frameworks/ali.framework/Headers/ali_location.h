/*
 *  ali_location.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_utils_forward.h"
#include "ali/ali_string_forward.h"

#define ALI_HERE \
    ali::location{__FILE__, __LINE__}

namespace ali
{

// Use ALI_HERE to construct
// ******************************************************************
class location
// ******************************************************************
{
public:
    using line_number_type = decltype(__LINE__);

public:
    location( void ) noexcept;

    //  Using reference to array so that ali::string.c_str()
    //  (or any other temporary buffer) won't be accepted.
    template <unsigned int size>
    location( char const (&path)[size],
              line_number_type line ) noexcept
    :   _path{path},
        _path_size{size - 1 /* NULL */},
        _line{line}
    {}

    static c_string_const_ref file_from_path( c_string_const_ref path ) noexcept;

    // inline methods below are defined in ali_array_utils.h

    inline c_string_const_ref path( void ) const noexcept;

    // The last part of path()
    inline c_string_const_ref file( void ) const noexcept;

    line_number_type line( void ) const noexcept
    {
        return _line;
    }

    string format( void ) const;
    string& format( string& str ) const;
    wstring& format( wstring& str ) const;
    
private:
    const char*         _path;
    int                 _path_size;
    line_number_type    _line;
};

// ******************************************************************
int compare( location a, location b ) noexcept;
// ******************************************************************

}   //  namespace ali
