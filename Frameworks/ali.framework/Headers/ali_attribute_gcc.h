/*
 *  ali_attribute_gcc.h
 *  ali library
 *
 *  Copyright 2012 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_features.h"


// Warnings and errors
#define ALI_ATTRIBUTE_DEPRECATED \
    __attribute__(( deprecated ))

#define ALI_ATTRIBUTE_WARNING(message) \
    __attribute__(( warning(message) ))

#define ALI_ATTRIBUTE_ERROR(message) \
    __attribute__(( error(message) ))


// Function attributes
#if defined(ALI_FEATURE_CONSTEXPR)
#   define ALI_ATTRIBUTE_CONSTEXPR \
        constexpr
#else
#   define ALI_ATTRIBUTE_CONSTEXPR
#endif

#define ALI_ATTRIBUTE_CONSTRUCTOR \
    __attribute__(( constructor ))

#define ALI_ATTRIBUTE_NOINLINE \
    __attribute__(( noinline ))

#if defined(ALI_FEATURE_OVERRIDE)
#   define ALI_ATTRIBUTE_OVERRIDE \
        override
#else
#   define ALI_ATTRIBUTE_OVERRIDE
#endif

// Structure and class attributes
#define ALI_ATTRIBUTE_USED \
    __attribute__(( used ))

#define ALI_ATTRIBUTE_PACKED \
    __attribute__(( packed ))

// Attributes for everything
#define ALI_ATTRIBUTE_EXPORT \
    __attribute__((visibility("default")))

#define ALI_ATTRIBUTE_HIDDEN \
    __attribute__((visibility("hidden")))

// Multiple definitions handling
#if defined(_WIN32)
#   define ALI_ATTRIBUTE_SELECTANY \
        __attribute__(( selectany ))
#else
#   define ALI_ATTRIBUTE_SELECTANY
#endif

#if defined(ALI_FEATURE_DELETED_FUNCTIONS)
#   define ALI_ATTRIBUTE_DELETED \
        = delete
#else
#   define ALI_ATTRIBUTE_DELETED
#endif

// MSVC *Full* Empty Base Class Optimization opt-in (until it's the default)
#define ALI_ATTRIBUTE_EMPTY_BASES

#if defined(__clang__) &&  __clang_major__ < 4 && __clang_minor__ < 9
#define ALI_ATTRIBUTE_FALLTHROUGH [[clang::fallthrough]]
#else   //  !Clang or Clang >= 4.9
#define ALI_ATTRIBUTE_FALLTHROUGH [[fallthrough]]
#endif  //  Clang < 4.9
