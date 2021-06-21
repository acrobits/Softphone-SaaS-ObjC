/*
 *  ali_typed_int.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_typed_number_forward.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
struct typed_number
// ******************************************************************
{
    constexpr typed_number( void ) {}
    
    constexpr explicit typed_number( number_type value )
    :   value{value}
    {}

    constexpr void swap( typed_number& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    constexpr friend void swap( typed_number& a, typed_number& b )
    {
        a.swap(b);
    }

    number_type value{};
};

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator++(
    typed_number<number_type, enum_type, type>& a )
// ******************************************************************
{
    ++a.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator+=(
    typed_number<number_type, enum_type, type>& a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    a.value += b.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator--(
    typed_number<number_type, enum_type, type>& a )
// ******************************************************************
{
    --a.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator-=(
    typed_number<number_type, enum_type, type>& a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    a.value -= b.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator*=(
    typed_number<number_type, enum_type, type>& a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    a.value *= b.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator/=(
    typed_number<number_type, enum_type, type>& a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    a.value /= b.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type>& operator%=(
    typed_number<number_type, enum_type, type>& a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    a.value %= b.value;
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator+(
    typed_number<number_type, enum_type, type> a )
// ******************************************************************
{
    return a;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator+(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator-(
    typed_number<number_type, enum_type, type> a )
// ******************************************************************
{
    a.value = -a.value;
    return a;
}


// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator-(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator*(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a *= b;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator/(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a /= b;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline typed_number<number_type, enum_type, type> operator%(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a %= b;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline int compare(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    using ali::compare;
    return compare(a.value, b.value);
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator==(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value == b.value;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator!=(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value != b.value;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator<(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value < b.value;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator>(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value > b.value;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator<=(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value <= b.value;
}

// ******************************************************************
template <typename number_type, typename enum_type, enum_type type>
constexpr inline bool operator>=(
    typed_number<number_type, enum_type, type> a,
    typed_number<number_type, enum_type, type> b )
// ******************************************************************
{
    return a.value >= b.value;
}

}   //  namespace ali

