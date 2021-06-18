#pragma once
#include "ali/ali_deprecated_common.h"
#include <Availability.h>

#ifdef  ALI_EXCLUDE_DEPRECATED

#define ALI_DEPRECATED_class    class __AVAILABILITY_INTERNAL_DEPRECATED
#define ALI_DEPRECATED_struct   struct __AVAILABILITY_INTERNAL_DEPRECATED
#define ALI_DEPRECATED_function __AVAILABILITY_INTERNAL_DEPRECATED

#ifdef __AVAILABILITY_INTERNAL_DEPRECATED_MSG
    #define ALI_DEPRECATED_class_(msg)    class __AVAILABILITY_INTERNAL_DEPRECATED_MSG(msg)
    #define ALI_DEPRECATED_struct_(msg)   struct __AVAILABILITY_INTERNAL_DEPRECATED_MSG(msg)
    #define ALI_DEPRECATED_function_(msg) __AVAILABILITY_INTERNAL_DEPRECATED_MSG(msg)
#else
    #define ALI_DEPRECATED_class_(msg)    class __AVAILABILITY_INTERNAL_DEPRECATED
    #define ALI_DEPRECATED_struct_(msg)   struct __AVAILABILITY_INTERNAL_DEPRECATED
    #define ALI_DEPRECATED_function_(msg) __AVAILABILITY_INTERNAL_DEPRECATED
#endif

#else   //  !ALI_EXCLUDE_DEPRECATED

#define ALI_DEPRECATED_class    class
#define ALI_DEPRECATED_struct   struct
#define ALI_DEPRECATED_function

#define ALI_DEPRECATED_class_(msg)    class
#define ALI_DEPRECATED_struct_(msg)   struct
#define ALI_DEPRECATED_function_(msg)

#endif  //  ALI_EXCLUDE_DEPRECATED
