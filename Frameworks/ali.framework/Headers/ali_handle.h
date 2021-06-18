/*
 *  ali_handle.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2017 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_noncopyable.h"

namespace ali
{

// ******************************************************************
class handle
    : private ali::noncopyable
// ******************************************************************
{
public:     //  Methods
    virtual ~handle( void ) {}

protected:  //  Methods
    handle( void ) {}
};

}   //  namespace ali
