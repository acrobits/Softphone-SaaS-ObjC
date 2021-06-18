/*
 *  ali_thread_identifier.h
 *  ali Library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 *
 */


#pragma once

#include "ali/ali_array_utils_forward.h"
#include "ali/ali_string_forward.h"
#include <pthread.h>


namespace ali
{

namespace thread
{

// ******************************************************************
class identifier
// ******************************************************************
{
public:
    identifier( void )
    :   _id{::pthread_self()}
    {}

    explicit identifier( pthread_t id )
    :   _id{id}
    {}

    bool operator==( identifier b ) const
    {
        return ::pthread_equal(_id, b._id) != 0;
    }

    bool operator!=( identifier b ) const
    {
        return !operator==(b);
    }
    
    ali::string& format_for_logging( ali::string& str ) const;

private:
    pthread_t   _id;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    thread::identifier value,
    string_const_ref /*format_string*/ );
// ******************************************************************

}   //  namespace thread

}   //  namespace ali
