/*
 *  ali_endianness.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_meta.h"
#include "ali/ali_wchar.h"

namespace ali
{

namespace hidden
{

// ******************************************************************
template <typename I,
    typename U = typename meta::integer::
        unsigned_of_exact_size_in_bits<8 * sizeof(I)>::type>
struct to_other_endian_;
// ******************************************************************

// ******************************************************************
template <typename U>
struct to_other_endian_<U, U>
// ******************************************************************
{
    static constexpr U convert( U data ) noexcept
    {
        static_assert(
            sizeof(data) == 2 || sizeof(data) == 4 || sizeof(data) == 8,
            "to_other_endian needs adjustment for new integer size.");

        if constexpr ( sizeof(data) == 2 )
        {
            return  ((data & 0x00ffU) <<  8)
                |   ((data & 0xff00U) >>  8);
        }
        else if constexpr ( sizeof(data) == 4 )
        {
            return  ((data & 0x000000ffU) << 24)
                |   ((data & 0x0000ff00U) <<  8)
                |   ((data & 0x00ff0000U) >>  8)
                |   ((data & 0xff000000U) >> 24);
        }
        else if constexpr ( sizeof(data) == 8 )
        {
            return  ((data & 0x00000000000000ffULL) << 56)
                |   ((data & 0x000000000000ff00ULL) << 40)
                |   ((data & 0x0000000000ff0000ULL) << 24)
                |   ((data & 0x00000000ff000000ULL) <<  8)
                |   ((data & 0x000000ff00000000ULL) >>  8)
                |   ((data & 0x0000ff0000000000ULL) >> 24)
                |   ((data & 0x00ff000000000000ULL) >> 40)
                |   ((data & 0xff00000000000000ULL) >> 56);
        }
    }
};

// ******************************************************************
template <typename I, typename U>
struct to_other_endian_
// ******************************************************************
{
    static constexpr I convert( I data ) noexcept
    {
        return static_cast<I>(to_other_endian_<
            U>::convert(static_cast<U>(data)));
    }
};

// ******************************************************************
template <typename I>
inline constexpr I to_other_endian( I data )
// ******************************************************************
{
    return to_other_endian_<I>::convert(data);
}

// ******************************************************************
// ******************************************************************

static_assert(
    to_other_endian(meta::integer::signed_of_exact_size_in_bits<16>::type{/*0x8765*/-30875})
        == meta::integer::signed_of_exact_size_in_bits<16>::type{0x6587},
    "bad to_other_endian (16-bit signed int)");

static_assert(
    to_other_endian(meta::integer::signed_of_exact_size_in_bits<32>::type{/*0x87654321*/-2023406815})
        == meta::integer::signed_of_exact_size_in_bits<32>::type{0x21436587},
    "bad to_other_endian (32-bit signed int)");

static_assert(
    to_other_endian(meta::integer::signed_of_exact_size_in_bits<64>::type{/*0x8765432198765432LL*/-8690466094370630606LL})
        == meta::integer::signed_of_exact_size_in_bits<64>::type{0x3254769821436587LL},
    "bad to_other_endian (64-bit signed int)");

static_assert(
    to_other_endian(meta::integer::unsigned_of_exact_size_in_bits<16>::type{0x8765U})
        == meta::integer::unsigned_of_exact_size_in_bits<16>::type{0x6587U},
    "bad to_other_endian (16-bit unsigned int)");

static_assert(
    to_other_endian(meta::integer::unsigned_of_exact_size_in_bits<32>::type{0x87654321U})
        == meta::integer::unsigned_of_exact_size_in_bits<32>::type{0x21436587U},
    "bad to_other_endian (32-bit unsigned int)");

static_assert(
    to_other_endian(meta::integer::unsigned_of_exact_size_in_bits<64>::type{0x8765432198765432ULL})
        == meta::integer::unsigned_of_exact_size_in_bits<64>::type{0x3254769821436587ULL},
    "bad to_other_endian (64-bit unsigned int)");

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
inline constexpr short to_other_endian( short data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned short to_other_endian( unsigned short data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr ali::wchar to_other_endian( ali::wchar data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr int to_other_endian( int data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned int to_other_endian( unsigned int data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr long to_other_endian( long data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned long to_other_endian( unsigned long data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr long long to_other_endian( long long data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned long long to_other_endian( unsigned long long data ) noexcept
// ******************************************************************
{
    return hidden::to_other_endian(data);
}

}   //  namespace ali
