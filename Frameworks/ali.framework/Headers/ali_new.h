/*
 *  ali_new.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#ifdef new
#error "foreign new macro definition"
#endif

#include "ali/ali_placement_new.h"
/*
//
// ~~~ operator new/delete ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

void* operator new( size_t size );
void operator delete( void* p );
void* operator new[]( size_t size );
void operator delete[]( void* p );

*/
#ifdef ALI_TRACEMEMORY

#include "ali/ali_location.h"

namespace ali
{


//
// ~~~ set_new_info ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Defined in ali_memory.cpp
//
void set_new_info( void* p, unsigned size, const ali::location &loc );

//
// ~~~ operator* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
template <class T>
T* operator<<( const ali::location &loc, T* p )
{
    ali::set_new_info(p, sizeof(T), info.file, info.line);
    return p;
}


} // namespace ali


//
// And here is the single macro...
//
//#define new (ALI_HERE << (new))


#endif // ALI_TRACEMEMORY



