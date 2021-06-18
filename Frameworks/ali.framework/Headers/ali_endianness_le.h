/*
 *  ali_endianness_le.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_endianness_common.h"

namespace ali
{

// ******************************************************************
inline constexpr short to_big_endian( short data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned short to_big_endian( unsigned short data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr ali::wchar to_big_endian( ali::wchar data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr int to_big_endian( int data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned int to_big_endian( unsigned int data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr long to_big_endian( long data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned long to_big_endian( unsigned long data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr long long to_big_endian( long long data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr unsigned long long to_big_endian( unsigned long long data ) noexcept
// ******************************************************************
{
    return to_other_endian(data);
}

// ******************************************************************
inline constexpr short to_little_endian( short data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned short to_little_endian( unsigned short data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr ali::wchar to_little_endian( ali::wchar data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr int to_little_endian( int data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned int to_little_endian( unsigned int data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr long to_little_endian( long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned long to_little_endian( unsigned long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr long long to_little_endian( long long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned long long to_little_endian( unsigned long long data ) noexcept
// ******************************************************************
{
    return data;
}


// ******************************************************************
inline constexpr short to_native_from_big_endian( short data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr unsigned short to_native_from_big_endian( unsigned short data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr ali::wchar to_native_from_big_endian( ali::wchar data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr int to_native_from_big_endian( int data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr unsigned int to_native_from_big_endian( unsigned int data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}


// ******************************************************************
inline constexpr long to_native_from_big_endian( long data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr unsigned long to_native_from_big_endian( unsigned long data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr long long to_native_from_big_endian( long long data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr unsigned long long to_native_from_big_endian( unsigned long long data ) noexcept
// ******************************************************************
{
    // to_big_endian applied to a big endian results in a little endian
    return to_big_endian(data);
}

// ******************************************************************
inline constexpr short to_native_from_little_endian( short data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned short to_native_from_little_endian( unsigned short data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr ali::wchar to_native_from_little_endian( ali::wchar data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr int to_native_from_little_endian( int data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned int to_native_from_little_endian( unsigned int data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr long to_native_from_little_endian( long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned long to_native_from_little_endian( unsigned long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr long long to_native_from_little_endian( long long data ) noexcept
// ******************************************************************
{
    return data;
}

// ******************************************************************
inline constexpr unsigned long long to_native_from_little_endian( unsigned long long data ) noexcept
// ******************************************************************
{
    return data;
}

} // namespace ali
