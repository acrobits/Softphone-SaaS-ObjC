/*
 *  ali_placement_new.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#ifdef new
#error "bad header inclusion order"
#endif

#include <new>
/*
void* operator new( unsigned, void* p );
void operator delete( void*, void* );
*/
