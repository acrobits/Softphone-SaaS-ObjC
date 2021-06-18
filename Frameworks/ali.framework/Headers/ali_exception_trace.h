#pragma once
#include "ali/ali_exception.h"
#include "ali/ali_printf.h"
#include "ali/ali_utility.h"

//namespace ali
//{
//
//namespace hidden
//{
//
//// ******************************************************************
//template <typename... T>
//inline string legacy_tracef( string_const_ref format,
//                             T const&... params )
//// ******************************************************************
//{
//    formatter fmt(format);
//    ali::for_each(fmt, params...);
//    return fmt.str();
//}
//
//}   //  namespace hidden
//
//}   //  namespace ali

#define ALI_TRACE(msg) \
    ALI_CATCH(ali::exception, e) \
    { \
        e.trace(ALI_HERE, msg); \
        ALI_RETHROW; \
    }

//#define ALI_LEGACY_TRACEF(...) \
//    ALI_TRACE(ali::hidden::legacy_tracef(__VA_ARGS__))

#define ALI_TRACEF(...) \
    ALI_TRACE(ali::printf(__VA_ARGS__))
