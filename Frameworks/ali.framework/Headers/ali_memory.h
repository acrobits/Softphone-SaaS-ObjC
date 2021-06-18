/*
 *  ali_memory.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#if 0   //  AAA
#include "ali/ali_features.h"
#include "ali/ali_integer.h"
#include "ali/ali_meta.h"
#include "ali/ali_new.h"
#include "ali/ali_utility.h"
#else   //  AAA
#include "ali/ali_integer.h"
#endif  //  AAA

namespace ali
{
#if 0   //  AAA
// ******************************************************************
void* allocate( size_t size );
// ******************************************************************

// ******************************************************************
void* reallocate( void* p, size_t size );
// ******************************************************************

// ******************************************************************
void deallocate( void* p );
// ******************************************************************
#endif  //  AAA

#if 0
#ifdef  ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline typename meta::base<T>::type* construct( void* p, T&& val )
// ******************************************************************
{
    return new(p) typename meta::base<T>::type(ali::forward<T>(val));
}

#else   //  !ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline T* construct( void* p, T const& val )
// ******************************************************************
{
    return new(p) T(val);
}

#endif  //  ALI_FEATURE_RVALUE_REF

// ******************************************************************
template <typename T>
inline T* construct( void* p )
// ******************************************************************
{
    return new(p) T();
}

// ******************************************************************
template <typename T>
inline void destroy( T const& p )
// ******************************************************************
{
    p.~T();
}

// ******************************************************************
template <typename T>
inline void destroy( T* const& ) {}
// ******************************************************************
#endif
// ******************************************************************
// ******************************************************************

// ******************************************************************
struct
// ******************************************************************
{
    template <typename T>
    operator T*( void ) const noexcept
    {
        return reinterpret_cast<T*>(~uint_of_pointer_size{});
    }

} const invalid_pointer_value{};

}   //  namespace ali
