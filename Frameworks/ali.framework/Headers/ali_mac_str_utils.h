/*
 *  ali_mac_str_utils.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#import <Foundation/Foundation.h>
#include "ali/ali_string.h"

namespace ali
{

namespace mac
{

// ******************************************************************
c_string_const_ref make_string_const_ref( NSString* str );
// ******************************************************************

namespace str
{

// ******************************************************************
string from_nsstring( NSString* str );
// ******************************************************************

// ******************************************************************
NSString* to_nsstring( string_const_ref str );
// ******************************************************************

}   //  namespace str

namespace wstr
{

// ******************************************************************
wstring from_nsstring( NSString* str );
// ******************************************************************

// ******************************************************************
NSString* to_nsstring( wstring_const_ref str );
// ******************************************************************

}   //  namespace wstr

}   //  namespace mac

}   //  namespace ali

