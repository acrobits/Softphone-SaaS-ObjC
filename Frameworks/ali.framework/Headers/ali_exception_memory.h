/*
*  ali_exception_memory.h
*  ali Library
*
*  Copyright (c) 2012 - 2018 Acrobits, s.r.o. All rights reserved.
*
*/

#pragma once

#include "ali/ali_attribute.h"
#include "ali/ali_location.h"


namespace ali
{


// ******************************************************************
[[noreturn]]
void out_of_memory( ali::location location );
// ******************************************************************

// ******************************************************************
inline void out_of_memory_if( bool error, ali::location location )
// ******************************************************************
{
	if ( error )
		out_of_memory(location);
}

// ******************************************************************
template <typename T>
inline T out_of_memory_if_null( T t, ali::location location )
// ******************************************************************
{
	out_of_memory_if(t == nullptr, location);
	return t;
}

}   //  namespace ali

#define ALI_X_OUT_OF_MEMORY_IF_NULL(expression) \
    ali::out_of_memory_if_null(expression, ALI_HERE)
