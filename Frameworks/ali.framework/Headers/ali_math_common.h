#pragma once

#include "ali/ali_array_forward.h"
#include "ali/ali_debug.h"
#include "ali/ali_integer.h"

namespace ali
{

namespace math
{

// ******************************************************************
inline int log2(ali::uint32 v)
// ******************************************************************
{
    unsigned int result;
    unsigned int shift;
    
    result =     (v > 0xFFFF) << 4; v >>= result;
    shift = (v > 0xFF  ) << 3;      v >>= shift; result |= shift;
    shift = (v > 0xF   ) << 2;      v >>= shift; result |= shift;
    shift = (v > 0x3   ) << 1;      v >>= shift; result |= shift;
    result |= (v >> 1);
    
    return result;
}

// ******************************************************************
inline int log2(ali::uint64 v)
// ******************************************************************
{
    unsigned int result;
    unsigned int shift;
    
    result =     (v > 0xFFFFFFFF) << 5; v >>= result;
    shift = (v > 0xFFFF ) << 4;    v >>= shift; result |= shift;
    shift = (v > 0xFF   ) << 3;    v >>= shift; result |= shift;
    shift = (v > 0xF    ) << 2;    v >>= shift; result |= shift;
    shift = (v > 0x3    ) << 1;    v >>= shift; result |= shift;
    result |= (v >> 1);
    
    return result;
}

// ******************************************************************
inline signed char abs( signed char a )
// ******************************************************************
{
    ali_assert(a > ali::meta::integer::min_value<signed char>::result);
    
    return a < 0 ? -a : a;
}

// ******************************************************************
inline short abs( short a )
// ******************************************************************
{
    ali_assert(a > ali::meta::integer::min_value<short>::result);
    
    return a < 0 ? -a : a;
}

// ******************************************************************
inline int abs( int a )
// ******************************************************************
{
    ali_assert(a > ali::meta::integer::min_value<int>::result);
    
    return a < 0 ? -a : a;
}

// ******************************************************************
inline long abs( long a )
// ******************************************************************
{
    ali_assert(a > ali::meta::integer::min_value<long>::result);
    
    return a < 0 ? -a : a;
}

// ******************************************************************
constexpr unsigned char unsigned_abs( signed char a )
// ******************************************************************
{
    return a < 0
        ?   static_cast<unsigned char>(-(a + 1)) + 1
                //  -128s -> -127s -> +127s -> +127u -> +128u
                //    -1s ->    0s ->    0s ->    0u ->   +1u
        :   static_cast<unsigned char>(a);
}

// ******************************************************************
constexpr unsigned short unsigned_abs( short a )
// ******************************************************************
{
	return a < 0
        ?   static_cast<unsigned short>(-(a + 1)) + 1
        :   static_cast<unsigned short>(a);
}

// ******************************************************************
constexpr unsigned int unsigned_abs( int a )
// ******************************************************************
{
	return a < 0
        ?   static_cast<unsigned int>(-(a + 1)) + 1
        :   static_cast<unsigned int>(a);
}

// ******************************************************************
constexpr unsigned long unsigned_abs( long a )
// ******************************************************************
{
	return a < 0
        ?   static_cast<unsigned long>(-(a + 1)) + 1
        :   static_cast<unsigned long>(a);
}

// ******************************************************************
constexpr unsigned long long unsigned_abs( long long a )
// ******************************************************************
{
	return a < 0
        ?   static_cast<unsigned long long>(-(a + 1)) + 1
        :   static_cast<unsigned long long>(a);
}

// ******************************************************************
// ******************************************************************

/*
    Absolute difference.
    http://stackoverflow.com/questions/8024480/computing-integer-absolute-differences-in-overflow-safe-ways

    The solution for unsigned integers is relatively straightforward,
    and works by moving the (implied) conditional outside the subtraction
    so that we are always subtracting the smaller number from the larger
    one, and are not comparing a potentially-wrapped value to zero:

    if (a > b)
      return a - b;
    else
      return b - a;

    This just leaves the question of signed integers.
    Consider the following variation:

    if (a > b)
      return (unsigned) a - (unsigned) b;
    else
      return (unsigned) b - (unsigned) a;

    We can easily prove that this works by using modulo arithmetic.
    We know that a and (unsigned) a are congruent modulo UINT_MAX.
    Further, the unsigned-integer subtraction operation is congruent
    to actual subtraction modulo UINT_MAX, so combining these facts,
    we know that (unsigned) a - (unsigned) b is congruent to the actual
    value of a - b modulo UINT_MAX. Finally, because we know that the
    actual value of a - b must be between 0 and UINT_MAX-1, we know that
    this is an exact equality.
*/

#define ALI_ABSOLUTE_DIFFERENCE(signed_type, unsigned_type) \
constexpr unsigned_type absolute_difference(                \
    unsigned_type a, unsigned_type b )                      \
{                                                           \
    return a > b ? a - b : b - a;                           \
}                                                           \
constexpr unsigned_type absolute_difference(                \
    signed_type a, signed_type b )                          \
{                                                           \
    return a > b                                            \
        ? static_cast<unsigned_type>(a)                     \
            - static_cast<unsigned_type>(b)                 \
        : static_cast<unsigned_type>(b)                     \
            - static_cast<unsigned_type>(a);                \
}

ALI_ABSOLUTE_DIFFERENCE(signed char, unsigned char)
ALI_ABSOLUTE_DIFFERENCE(short, unsigned short)
ALI_ABSOLUTE_DIFFERENCE(int, unsigned int)
ALI_ABSOLUTE_DIFFERENCE(long, unsigned long)
ALI_ABSOLUTE_DIFFERENCE(long long, unsigned long long)

#undef  ALI_ABSOLUTE_DIFFERENCE

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline T square( T x )
// ******************************************************************
{
    return x * x;
}

// ******************************************************************
template <typename T>
inline T cube( T x )
// ******************************************************************
{
    return x * x * x;
}

// ******************************************************************
template <typename T>
inline T square_abs( T x )
// ******************************************************************
{
	return square(x);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct quotient_remainder
// ******************************************************************
{
    quotient_remainder( void )
    :   q(),
        r()
    {}

    quotient_remainder( T q, T r )
    :   q(q),
        r(r)
    {}

    T   q;
    T   r;
};

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T>
T euclidean_quotient( T a, T b );
// ******************************************************************
//  pre:    b != 0
//  post:   a == result * b + r
//      &&  0 <= r && r < unsigned_abs(b)
//  Note:   Works well with negative numbers.
// ******************************************************************

// ******************************************************************
template <typename T>
T euclidean_remainder( T a, T b );
// ******************************************************************
//  pre:    b != 0
//  post:   a == q * b + result
//      &&  0 <= result && result < unsigned_abs(b)
//  Note:   Works well with negative numbers.
// ******************************************************************

// ******************************************************************
template <typename T>
quotient_remainder<T> euclidean_quotient_remainder( T a, T b );
// ******************************************************************
//  pre:    b != 0
//  post:   a == result.q * b + result.r
//      &&  0 <= result.r && result.r < unsigned_abs(b)
//  Note:   Works well with negative numbers.
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
inline int euclidean_quotient( int a, int b )
// ******************************************************************
{
    return hidden::euclidean_quotient(a, b);
}

// ******************************************************************
inline long long euclidean_quotient( long long a, long long b )
// ******************************************************************
{
    return hidden::euclidean_quotient(a, b);
}

// ******************************************************************
inline int euclidean_remainder( int a, int b )
// ******************************************************************
{
    return hidden::euclidean_remainder(a, b);
}

// ******************************************************************
inline long long euclidean_remainder( long long a, long long b )
// ******************************************************************
{
    return hidden::euclidean_remainder(a, b);
}

// ******************************************************************
inline quotient_remainder<int>
    euclidean_quotient_remainder( int a, int b )
// ******************************************************************
{
    return hidden::euclidean_quotient_remainder(a, b);
}

// ******************************************************************
inline quotient_remainder<long long>
    euclidean_quotient_remainder( long long a, long long b )
// ******************************************************************
{
    return hidden::euclidean_quotient_remainder(a, b);
}

// ******************************************************************
ali::array<bool> test_euclidean_quotient_remainder( void );
// ******************************************************************

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
// ******************************************************************
template <typename T>
constexpr T gcd_( T a, T b )
// ******************************************************************
{
    if ( a == 0U ) return b;
    if ( b == 0U ) return a;
 
    int shift{};
    
    for ( ; ((a | b) & 1U) == 0U; ++shift )
    {
         a >>= 1;
         b >>= 1;
    }
 
    while ( (a & 1U) == 0U )
        a >>= 1;
 
    do
    {
        while ( (b & 1U) == 0U )
            b >>= 1;

        if ( a > b )
        {
            T const t{a};
            a = b; b = t;
        }
       
        b -= a;
    }
    while ( b != 0U );

    return a << shift;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
constexpr typename meta::enable_if<
        meta::size_in_bits<T>::result <= 32
    &&  meta::integer::min_value<T>::result == 0,
T>::type gcd( T a, T b )
// ******************************************************************
{
    return static_cast<T>(gcd_<ali::uint32>(a, b));
}

// ******************************************************************
template <typename T>
constexpr typename meta::enable_if<
        meta::size_in_bits<T>::result <= 32
    &&  meta::integer::min_value<T>::result != 0,
T>::type gcd( T a, T b )
// ******************************************************************
{
    return static_cast<T>(gcd_<ali::uint32>(
        unsigned_abs(a), unsigned_abs(b)));
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
constexpr typename meta::enable_if<
        meta::size_in_bits<T>::result == 64
    &&  meta::integer::min_value<T>::result == 0,
T>::type gcd( T a, T b )
// ******************************************************************
{
    return static_cast<T>(gcd_<ali::uint64>(a, b));
}

// ******************************************************************
template <typename T>
constexpr typename meta::enable_if<
        meta::size_in_bits<T>::result == 64
    &&  meta::integer::min_value<T>::result != 0,
T>::type gcd( T a, T b )
// ******************************************************************
{
    return static_cast<T>(gcd_<ali::uint64>(
        unsigned_abs(a), unsigned_abs(b)));
}

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
#define ALI_DEFINE_GCD_FOR(Int)                                     \
    constexpr Int gcd( Int a, Int b )                               \
    {                                                               \
        return hidden::gcd(a, b);                                   \
    }
// ******************************************************************

ALI_DEFINE_GCD_FOR(char)
ALI_DEFINE_GCD_FOR(signed char)
ALI_DEFINE_GCD_FOR(unsigned char)
ALI_DEFINE_GCD_FOR(short)
ALI_DEFINE_GCD_FOR(unsigned short)
ALI_DEFINE_GCD_FOR(int)
ALI_DEFINE_GCD_FOR(unsigned int)
ALI_DEFINE_GCD_FOR(long)
ALI_DEFINE_GCD_FOR(unsigned long)
ALI_DEFINE_GCD_FOR(long long)
ALI_DEFINE_GCD_FOR(unsigned long long)

#undef  ALI_DEFINE_GCD_FOR

}   //  namespace math

}   //  namespace ali
