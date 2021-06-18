/*
 *  ali_exception_if.h
 *  ali Library
 *
 *  Copyright (c) 2012 - 2018 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_array_utils.h"
#include "ali/ali_attribute.h"
#include "ali/ali_location.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_exception_memory.h"


namespace ali
{

// ******************************************************************
[[noreturn]]
void general_error(
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
void general_error_if(
    bool error,
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
template <typename T>
inline T general_error_if_null(
    T value,
    string_const_ref descr,
    ali::location location )
// ******************************************************************
{
    general_error_if(value == nullptr, descr, location);
    return value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
[[noreturn]]
void syntax_error(
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
void syntax_error_if(
    bool error,
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
// ******************************************************************

//  Currently this will link as long as T is int.
//  For other Ts another explicit instantiation
//  is needed in the cpp file.

// ******************************************************************
template <typename T>
[[noreturn]]
void range_error(
    T cur, T begin, T end,
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
template <typename T>
void range_error_if(
    T cur, T begin, T end,
    string_const_ref descr,
    ali::location location );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
[[noreturn]]
void optional_is_null(
    ali::location location );
// ******************************************************************

// ******************************************************************
inline void optional_is_null_if(
    bool error,
    ali::location location )
// ******************************************************************
{
    if ( error )
        optional_is_null(location);
}

}   //  namespace ali

#define ALI_X_GENERAL_IF(expression)    \
    ali::general_error_if(expression, ali::string_literal{#expression}, ALI_HERE)

#define ALI_X_GENERAL_IF_NULL(expression)   \
    ali::general_error_if_null(expression, ali::string_literal{#expression}, ALI_HERE)
