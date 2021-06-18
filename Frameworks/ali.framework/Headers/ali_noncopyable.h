/*
 *  ali_noncopyable.h
 *  ali library
 *
 *  Copyright (c) 2011 - 2017 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

namespace ali
{

// ******************************************************************
class noncopyable
// ******************************************************************
// A class that cannot be copied
// ******************************************************************
{
public:
    noncopyable( void )
    {}

    noncopyable( noncopyable const& ) = delete;
    noncopyable& operator =( noncopyable const& ) = delete;
};

}
