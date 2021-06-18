//
//  ali_simd_detect.h
//  asmtest
//
//  Copyright (c) 2014 - 2018 Acrobits, s.r.o. All rights reserved.
//

#pragma once

#define ALI_SIMD_NONE           0
#define ALI_SIMD_NEON_ARMV7     1
#define ALI_SIMD_NEON_ARM64     2
#define ALI_SIMD_SSE3           3

// defines ALI_SIMD macro to one of the values above

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#   if defined(__arm64__) || defined(__aarch64__)
#       define ALI_SIMD ALI_SIMD_NEON_ARM64
#   else
#       define ALI_SIMD ALI_SIMD_NEON_ARMV7
#   endif
#elif defined(__SSE3__)
#   define ALI_SIMD ALI_SIMD_SSE3
#else
#   define ALI_SIMD ALI_SIMD_NONE
#endif
