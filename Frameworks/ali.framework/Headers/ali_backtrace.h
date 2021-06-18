/*
 *  ali_backtrace.h
 *  ali Library
 *
 *  Copyright (c) 2012 - 2014 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_array_forward.h"
#include "ali/ali_string_forward.h"

namespace ali
{

// ******************************************************************
class backtrace
// ******************************************************************
{
public:
    class storage;

    virtual ~backtrace( void ) {}

    virtual ali::array<ali::string> resolve( void ) const = 0;

    static int fill( void** storage, int limit, int skip );

    static ali::string resolve( void* frame );
};

// ******************************************************************
class backtrace::storage : public backtrace
// ******************************************************************
{
public:
    static const int limit{25};

    storage( int skip = 0 )
    {
        backtrace::fill(trace, limit, skip);
    }

    virtual ~storage( void ) override {}

    virtual ali::array<ali::string> resolve( void ) const override;
    
    /** The backtrace. */
    void*   trace[limit];
};

}   //  namespace ali
