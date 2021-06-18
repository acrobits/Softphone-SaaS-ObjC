#pragma once


#include "ali/ali_meta_common.h"
#include <limits.h>
#include <wchar.h>

namespace ali
{

namespace meta
{

namespace integer
{

// ******************************************************************
template <typename T>
struct max_value;
// ******************************************************************

template <>
struct max_value<char>
    :   define_result<char, CHAR_MAX> {};

template <>
struct max_value<wchar_t>
    :   define_result<wchar_t, WCHAR_MAX> {};

template <>
struct max_value<signed char>
    :   define_result<signed char, SCHAR_MAX> {};
    
template <>
struct max_value<unsigned char>
    :   define_result<unsigned char, UCHAR_MAX> {};
    
template <>
struct max_value<signed short>
    :   define_result<signed short, SHRT_MAX> {};
    
template <>
struct max_value<unsigned short>
    :   define_result<unsigned short, USHRT_MAX> {};
    
template <>
struct max_value<signed int>
    :   define_result<signed int, INT_MAX> {};
    
template <>
struct max_value<unsigned int>
    :   define_result<unsigned int, UINT_MAX> {};

template <>
struct max_value<signed long>
    :   define_result<signed long, LONG_MAX> {};
    
template <>
struct max_value<unsigned long>
    :   define_result<unsigned long, ULONG_MAX> {};

template <>
struct max_value<signed long long>
    :   define_result<signed long long, LLONG_MAX> {};
    
template <>
struct max_value<unsigned long long>
    :   define_result<unsigned long long, ULLONG_MAX> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct min_value;
// ******************************************************************

template <>
struct min_value<char>
    :   define_result<char, CHAR_MIN> {};

template <>
struct min_value<wchar_t>
    :   define_result<wchar_t, WCHAR_MIN> {};

template <>
struct min_value<signed char>
    :   define_result<signed char, SCHAR_MIN> {};
    
template <>
struct min_value<unsigned char>
    :   define_result<unsigned char, 0> {};
    
template <>
struct min_value<signed short>
    :   define_result<signed short, SHRT_MIN> {};
    
template <>
struct min_value<unsigned short>
    :   define_result<unsigned short, 0> {};
    
template <>
struct min_value<signed int>
    :   define_result<signed int, INT_MIN> {};
    
template <>
struct min_value<unsigned int>
    :   define_result<unsigned int, 0> {};

template <>
struct min_value<signed long>
    :   define_result<signed long, LONG_MIN> {};
    
template <>
struct min_value<unsigned long>
    :   define_result<unsigned long, 0> {};

template <>
struct min_value<signed long long>
    :   define_result<signed long long, LLONG_MIN> {};
    
template <>
struct min_value<unsigned long long>
    :   define_result<unsigned long long, 0> {};

}   //  namespace integer

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct size_in_bits
// ******************************************************************
    :   define_result<int, CHAR_BIT * sizeof(T)> {};
    
// ******************************************************************
// ******************************************************************

// ******************************************************************
constexpr char max_value( char )
// ******************************************************************
{
    return integer::max_value<char>::result;
}

// ******************************************************************
constexpr wchar_t max_value( wchar_t )
// ******************************************************************
{
    return integer::max_value<wchar_t>::result;
}

// ******************************************************************
constexpr signed char max_value( signed char )
// ******************************************************************
{
    return integer::max_value<signed char>::result;
}

// ******************************************************************
constexpr unsigned char max_value( unsigned char )
// ******************************************************************
{
    return integer::max_value<unsigned char>::result;
}

// ******************************************************************
constexpr signed short max_value( signed short )
// ******************************************************************
{
    return integer::max_value<signed short>::result;
}

// ******************************************************************
constexpr unsigned short max_value( unsigned short )
// ******************************************************************
{
    return integer::max_value<unsigned short>::result;
}

// ******************************************************************
constexpr signed int max_value( signed int )
// ******************************************************************
{
    return integer::max_value<signed int>::result;
}

// ******************************************************************
constexpr unsigned int max_value( unsigned int )
// ******************************************************************
{
    return integer::max_value<unsigned int>::result;
}

// ******************************************************************
constexpr signed long max_value( signed long )
// ******************************************************************
{
    return integer::max_value<signed long>::result;
}

// ******************************************************************
constexpr unsigned long max_value( unsigned long )
// ******************************************************************
{
    return integer::max_value<unsigned long>::result;
}

// ******************************************************************
constexpr signed long long max_value( signed long long )
// ******************************************************************
{
    return integer::max_value<signed long long>::result;
}

// ******************************************************************
constexpr unsigned long long max_value( unsigned long long )
// ******************************************************************
{
    return integer::max_value<unsigned long long>::result;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
constexpr char min_value( char )
// ******************************************************************
{
    return integer::min_value<char>::result;
}

// ******************************************************************
constexpr wchar_t min_value( wchar_t )
// ******************************************************************
{
    return integer::min_value<wchar_t>::result;
}

// ******************************************************************
constexpr signed char min_value( signed char )
// ******************************************************************
{
    return integer::min_value<signed char>::result;
}

// ******************************************************************
constexpr unsigned char min_value( unsigned char )
// ******************************************************************
{
    return integer::min_value<unsigned char>::result;
}

// ******************************************************************
constexpr signed short min_value( signed short )
// ******************************************************************
{
    return integer::min_value<signed short>::result;
}

// ******************************************************************
constexpr unsigned short min_value( unsigned short )
// ******************************************************************
{
    return integer::min_value<unsigned short>::result;
}

// ******************************************************************
constexpr signed int min_value( signed int )
// ******************************************************************
{
    return integer::min_value<signed int>::result;
}

// ******************************************************************
constexpr unsigned int min_value( unsigned int )
// ******************************************************************
{
    return integer::min_value<unsigned int>::result;
}

// ******************************************************************
constexpr signed long min_value( signed long )
// ******************************************************************
{
    return integer::min_value<signed long>::result;
}

// ******************************************************************
constexpr unsigned long min_value( unsigned long )
// ******************************************************************
{
    return integer::min_value<unsigned long>::result;
}

// ******************************************************************
constexpr signed long long min_value( signed long long )
// ******************************************************************
{
    return integer::min_value<signed long long>::result;
}

// ******************************************************************
constexpr unsigned long long min_value( unsigned long long )
// ******************************************************************
{
    return integer::min_value<unsigned long long>::result;
}

}   //  namespace meta

}   //  namespace ali
