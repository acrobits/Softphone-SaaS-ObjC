#pragma once

#include <string.h>
#include <wchar.h>
#include "ali/ali_debug.h"
#include "ali/ali_utility.h"
#include "ali/ali_simd_detect.h"

namespace ali
{

// ******************************************************************
inline int str_len( char const* str )
// ******************************************************************
{
    ali_assert(str != nullptr);
    return ali::maxi(static_cast<int>(::strlen(str)), 0);
}

// ******************************************************************
inline int str_len( wchar const* str )
// ******************************************************************
{
    ali_assert(str != nullptr);
    return ali::maxi(static_cast<int>(::wcslen(str)), 0);
}


namespace platform
{

//  NOTE: We always use memmove instead of memcpy!

#if ALI_SIMD == ALI_SIMD_NEON_ARMV7

inline void* memmove( void* dst, void const* src, size_t n )
{
    if ( (n & 0x0F) == 0 )
    {
        void* const result = dst;
            //  I bet the asm modifies dst.

        switch ( n >> 4 )
        {
        case 8: //  128 bytes
            __asm__ volatile
            (
             "vld1.8 {q8,q9},[%[src]]!      \n\t"
             "vld1.8 {q10,q11},[%[src]]!    \n\t"
             "vld1.8 {q12,q13},[%[src]]!    \n\t"
             "vld1.8 {q14,q15},[%[src]]     \n\t"

             "vst1.8 {q8,q9},[%[dst]]!      \n\t"
             "vst1.8 {q10,q11},[%[dst]]!    \n\t"
             "vst1.8 {q12,q13},[%[dst]]!    \n\t"
             "vst1.8 {q14,q15},[%[dst]]     \n\t"
             :[dst]"+r"(dst),[src]"+r"(src)
             :
             :"q8","q9","q10","q11","q12","q13","q14","q15","memory"
             );
            return result;

        case 6: //  96 bytes
            __asm__ volatile
            (
             "vld1.8 {q10,q11},[%[src]]!    \n\t"
             "vld1.8 {q12,q13},[%[src]]!    \n\t"
             "vld1.8 {q14,q15},[%[src]]     \n\t"

             "vst1.8 {q10,q11},[%[dst]]!    \n\t"
             "vst1.8 {q12,q13},[%[dst]]!    \n\t"
             "vst1.8 {q14,q15},[%[dst]]     \n\t"
             :[dst]"+r"(dst),[src]"+r"(src)
             :
             :"q10","q11","q12","q13","q14","q15","memory"
             );
            return result;

        case 4: //  64 bytes
            __asm__ volatile
            (
             "vld1.8 {q12,q13},[%[src]]!    \n\t"
             "vld1.8 {q14,q15},[%[src]]     \n\t"

             "vst1.8 {q12,q13},[%[dst]]!    \n\t"
             "vst1.8 {q14,q15},[%[dst]]     \n\t"
             :[dst]"+r"(dst),[src]"+r"(src)
             :
             :"q12","q13","q14","q15","memory"
             );
            return result;

        case 3: //  48 bytes
            __asm__ volatile
            (
             "vld1.8 {q13},[%[src]]!        \n\t"
             "vld1.8 {q14,q15},[%[src]]     \n\t"

             "vst1.8 {q13},[%[dst]]!        \n\t"
             "vst1.8 {q14,q15},[%[dst]]     \n\t"
             :[dst]"+r"(dst),[src]"+r"(src)
             :
             :"q13","q14","q15","memory"
             );
            return result;

        case 2: //  32 bytes
            __asm__ volatile
            (
             "vld1.8 {q14,q15},[%[src]]     \n\t"

             "vst1.8 {q14,q15},[%[dst]]     \n\t"
             :
             :[dst]"r"(dst),[src]"r"(src)
             :"q14","q15","memory"
             );
            return result;

        case 1: //  16 bytes
            __asm__ volatile
            (
             "vld1.8 {q15},[%[src]]         \n\t"

             "vst1.8 {q15},[%[dst]]         \n\t"
             :
             :[dst]"r"(dst),[src]"r"(src)
             :"q15","memory"
             );
            return result;

        case 0:
            return result;
        };
    }

    return ::memmove(dst, src, n);
}
#elif ALI_SIMD == ALI_SIMD_NEON_ARM64
    inline void* memmove( void* dst, void const* src, size_t n )
    {
        if ( (n & 0x0F) == 0 )
        {
            void* const result = dst;
            //  Asm modifies dst.

            switch ( n >> 4 )
            {
                case 8: //  128 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v24,v25},[%[src]],#32        \n\t"
                     "ld1.16B {v26,v27},[%[src]],#32        \n\t"
                     "ld1.16B {v28,v29},[%[src]],#32        \n\t"
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v24,v25},[%[dst]],#32        \n\t"
                     "st1.16B {v26,v27},[%[dst]],#32        \n\t"
                     "st1.16B {v28,v29},[%[dst]],#32        \n\t"
                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v24","v25","v26","v27","v28","v29","v30","v31","memory"
                     );
                    return result;

                case 6: //  96 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v26,v27},[%[src]],#32        \n\t"
                     "ld1.16B {v28,v29},[%[src]],#32        \n\t"
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v26,v27},[%[dst]],#32        \n\t"
                     "st1.16B {v28,v29},[%[dst]],#32        \n\t"
                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v26","v27","v28","v29","v30","v31","memory"
                     );
                    return result;

                case 5: //  80 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v27},[%[src]],#16            \n\t"
                     "ld1.16B {v28,v29},[%[src]],#32        \n\t"
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v27},[%[dst]],#16            \n\t"
                     "st1.16B {v28,v29},[%[dst]],#32        \n\t"
                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v27","v28","v29","v30","v31","memory"
                     );
                    return result;

                case 4: //  64 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v28,v29},[%[src]],#32        \n\t"
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v28,v29},[%[dst]],#32        \n\t"
                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v28","v29","v30","v31","memory"
                     );
                    return result;

                case 3: //  48 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v29},[%[src]],#16            \n\t"
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v29},[%[dst]],#16            \n\t"
                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v29","v30","v31","memory"
                     );
                    return result;

                case 2: //  32 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v30,v31},[%[src]],#32        \n\t"

                     "st1.16B {v30,v31},[%[dst]],#32        \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v30","v31","memory"
                     );
                    return result;

                case 1: //  16 bytes
                    __asm__ volatile
                    (
                     "ld1.16B {v31},[%[src]],#16            \n\t"

                     "st1.16B {v31},[%[dst]],#16            \n\t"
                     :[dst]"+r"(dst),[src]"+r"(src)
                     :
                     :"v31","memory"
                     );
                    return result;

                case 0:
                    return result;
            };
        }

        return ::memmove(dst, src, n);
    }

#else

    using ::memmove;

#endif  //  ALI_SIMD

using ::memset;
using ::wmemset;
using ::memcmp;
using ::wmemcmp;

}   //  namespace platform

}   //  namespace ali
