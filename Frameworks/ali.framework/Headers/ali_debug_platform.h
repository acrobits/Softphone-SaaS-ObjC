/*
 *  ali_debug.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_debug_common.h"
#include "ali/ali_location.h"

#ifdef  __APPLE__
//#define ali_assert

#define ali_assert_always(expr) {if (expr) {} else {ali::mac::assertion_failure(#expr, ALI_HERE);}}

#if defined(DEBUG)
#   define ali_assert(expr) do {if (expr) {} else { ali::mac::assertion_failure(#expr, ALI_HERE);}} while (false)
#   define ali_constexpr_assert ali_assert
#   define ali_printf(...) do {printf(__VA_ARGS__);} while (false)
#   define ALI_DEBUG
#   define ALI_IF_DEBUG(...) __VA_ARGS__
#else
#   define ali_assert(expr) ali::noop()
#   define ali_constexpr_assert ali_assert
#   define ali_printf(...) ali::noop()
//#   define ALI_IF_DEBUG(...) ali::noop()
//  The above version doesn't always work -- ALI_IF_DEBUG
//  might be used to declare data members.
#   define ALI_IF_DEBUG(...)
#endif

#else   //  !__APPLE__

#include <cassert>

#define ali_assert assert
#define ali_constexpr_assert ali_assert

#ifndef NDEBUG
#   define ALI_IF_DEBUG(...) __VA_ARGS__
#else
#   define ALI_IF_DEBUG(...) ali::noop()
#endif

#endif  //  __APPLE__

namespace ali
{

namespace mac
{

// ******************************************************************
void assertion_failure( const char *expr, const ali::location &loc );
//void assertion_failure( const ali::wchar *expr, const ali::location &loc );
// ******************************************************************

} // namespace mac

} // namespace ali

#include "ali/ali_new.h"
