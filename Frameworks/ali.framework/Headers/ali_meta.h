#pragma once

#include "ali/ali_meta_platform.h"
#include "ali/ali_nullptr.h"


namespace ali
{

template <typename T,
          typename E,
          E v>
struct typed_int;


namespace meta
{


// ******************************************************************
template <typename T>
struct remove_cv
// ******************************************************************
    :   remove_v<typename remove_c<T>::type> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct remove_cv_ref
// ******************************************************************
    :   remove_cv<typename remove_ref<T>::type> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
using base = remove_cv_ref<T>;
// ******************************************************************

// ******************************************************************
// ******************************************************************

namespace detail {
// ******************************************************************
template <typename T>
struct is_fundamental_helper
// ******************************************************************
    :   define_false_result {};
// ******************************************************************
// ******************************************************************

template <typename T>
struct is_fundamental_helper<T*>
    : define_true_result {};

template <>
struct is_fundamental_helper<bool>
    : define_true_result {};

template <>
struct is_fundamental_helper<char>
    : define_true_result {};

template <>
struct is_fundamental_helper<char signed>
    : define_true_result {};

template <>
struct is_fundamental_helper<char unsigned>
    : define_true_result {};

template <>
struct is_fundamental_helper<wchar_t>
    : define_true_result {};

template <>
struct is_fundamental_helper<short signed>
    : define_true_result {};

template <>
struct is_fundamental_helper<short unsigned>
    : define_true_result {};

template <>
struct is_fundamental_helper<int signed>
    : define_true_result {};

template <>
struct is_fundamental_helper<int unsigned>
    : define_true_result {};

template <>
struct is_fundamental_helper<long signed>
    : define_true_result {};

template <>
struct is_fundamental_helper<long unsigned>
    : define_true_result {};

template <>
struct is_fundamental_helper<long long signed>
    : define_true_result {};

template <>
struct is_fundamental_helper<long long unsigned>
    : define_true_result {};

template <>
struct is_fundamental_helper<float>
    : define_true_result {};

template <>
struct is_fundamental_helper<double>
    : define_true_result {};

template <>
struct is_fundamental_helper<double long>
    : define_true_result {};
}

// ******************************************************************
template <typename T>
struct is_fundamental
// ******************************************************************
    : detail::is_fundamental_helper<typename base<T>::type> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Type, typename True, typename False>
struct if_fundamental
// ******************************************************************
    : select<is_fundamental<Type>::result, True, False> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Type, template <typename> class True, template <typename> class False>
struct template_if_fundamental
// ******************************************************************
    : template_select<is_fundamental<Type>::result, Type, True, False> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_typed_int
// ******************************************************************
    : define_false_result {};
// ******************************************************************

// ******************************************************************
// ******************************************************************
template <typename T,
          typename E,
          E v>
struct is_typed_int<ali::typed_int<T, E, v> >
// ******************************************************************
    : define_true_result {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct is_fundamental_or_typed_int
// ******************************************************************
    : _or<is_fundamental<T>::result, is_typed_int<T>::result> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Type, typename True, typename False>
struct if_fundamental_or_typed_int
// ******************************************************************
    : select<is_fundamental_or_typed_int<Type>::result, True, False> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Type, template <typename> class True, template <typename> class False>
struct template_if_fundamental_or_typed_int
// ******************************************************************
    : template_select<is_fundamental_or_typed_int<Type>::result, Type, True, False> {};
// ******************************************************************
// ******************************************************************



// ******************************************************************
// ******************************************************************
// Effective type to be passed as an argument of a function
// ******************************************************************
template <typename T>
struct effective
// ******************************************************************
    :  select<is_fundamental<T>::result, T, T const&> {};

template <class T>
struct effective<T&>
	:   define_type<T&> {};
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
struct is_same_type_remove_cv
// ******************************************************************
    :   is_same_type<
            typename remove_cv<T>::type,
            typename remove_cv<U>::type> {};

// ******************************************************************
// ******************************************************************

template <int>
struct is_constructible_helper {};

template <typename T>
yes_type is_copy_constructible_test( is_constructible_helper<sizeof(T(make<T const&>()))> const* );

template <typename T>
no_type is_copy_constructible_test( ... );

template <typename T>
struct is_copy_constructible
    : define_bool_result<sizeof(is_copy_constructible_test<
        typename base<T>::type>(0)) == sizeof(yes_type)>
            //  Note: Intentionally using 0 intead of nullptr.
            //  In WinRT nullptr cannot be "converted" to (...).
{};

// ******************************************************************
// ******************************************************************

template <typename T>
yes_type is_default_constructible_test( is_constructible_helper<sizeof(T())> const* );

template <typename T>
no_type is_default_constructible_test( ... );

template <typename T>
struct is_default_constructible
    : define_bool_result<sizeof(is_default_constructible_test<typename base<T>::type>(nullptr))
                             == sizeof(yes_type)>
{};

// ******************************************************************
// ******************************************************************

no_type is_default_all_zeros_test( ... );
    //  Default not all zeros.

yes_type is_default_all_zeros_test( bool );
yes_type is_default_all_zeros_test( char );
yes_type is_default_all_zeros_test( signed char );
yes_type is_default_all_zeros_test( unsigned char );
yes_type is_default_all_zeros_test( wchar_t );
yes_type is_default_all_zeros_test( short );
yes_type is_default_all_zeros_test( unsigned short );
yes_type is_default_all_zeros_test( int );
yes_type is_default_all_zeros_test( unsigned int );
yes_type is_default_all_zeros_test( long );
yes_type is_default_all_zeros_test( unsigned long );
yes_type is_default_all_zeros_test( long long );
yes_type is_default_all_zeros_test( unsigned long long );
yes_type is_default_all_zeros_test( float );
yes_type is_default_all_zeros_test( double );
yes_type is_default_all_zeros_test( long double );

template <typename T>
yes_type is_default_all_zeros_test( T* );

template <typename T>
struct is_default_all_zeros
    :   define_bool_result<
            sizeof(is_default_all_zeros_test(make<T>()))
                == sizeof(yes_type)> {};

template <typename T>
struct is_default_all_zeros_test_returns
    :   select<is_default_all_zeros<T>::result, yes_type, no_type> {};

// ******************************************************************
// ******************************************************************

/*
//  MSVC11 chokes on this.
template <typename T1,
          typename T2>
struct is_convertible_test {
	static yes_type test( const T2& );
	static no_type test( ... );
};
*/

template <typename T>
yes_type is_convertible_test( T const& );

template <typename T>
no_type is_convertible_test( ... );


template <typename T1,
          typename T2>
struct is_convertible
	: define_bool_result<sizeof(is_convertible_test<T2>(make<T1>()))
		== sizeof(yes_type)>
{};


// ******************************************************************
// ******************************************************************

namespace detail
{

//  for ( int n = x - 1, log2i_x = 0; n != 0; n /= 2 )
//      ++log2i_x;

template <int N>
struct log2i_loop
    : define_result<int, 1 + log2i_loop<N / 2>::result> {};

template <>
struct log2i_loop<0>
    : define_result<int, 0> {};

}   //  namespace detail

// ******************************************************************
//  Calculates log2(x)
template <int X>
struct log2i
// ******************************************************************
    :   define_result<int, detail::log2i_loop<X - 1>::result> {};
    
// ******************************************************************
// ******************************************************************

namespace detail
{

// ******************************************************************
template <int Y, int X, bool stop = (Y >= X)>
struct power_of_two_gte_loop
// ******************************************************************
    : define_result<int, power_of_two_gte_loop<2 * Y, X>::result> {};
    
// ******************************************************************
template <int Y, int X>
struct power_of_two_gte_loop<Y, X, true>
// ******************************************************************
    : define_result<int, Y> {};

}   //  namespace detail

// ******************************************************************
//  Calculates power of two (Y) greater than or equal to X.
template <int X>
struct power_of_two_gte
// ******************************************************************
    :   define_result<int, detail::power_of_two_gte_loop<1, X>::result> {};

// ******************************************************************
// ******************************************************************
    
// ******************************************************************
template <int X>
struct is_power_of_two
// ******************************************************************
    :   define_bool_result<(1 << log2i<X>::result) == X> {};
    
// ******************************************************************
// ******************************************************************

namespace integer
{

// ******************************************************************
template <int size>
struct signed_of_exact_size_in_bits
// ******************************************************************
    :   select<size_in_bits<signed char>::result
            == size, define_type<signed char>,
        typename select<size_in_bits<signed short>::result
            == size, define_type<signed short>,
        typename select<size_in_bits<signed int>::result
            == size, define_type<signed int>,
        typename select<size_in_bits<signed long>::result
            == size, define_type<signed long>,
        typename select<size_in_bits<signed long long>::result
            == size, define_type<signed long long>,
        empty_type>::type>::type>::type>::type>::type {};

// ******************************************************************
template <int size>
struct unsigned_of_exact_size_in_bits
// ******************************************************************
    :   select<size_in_bits<unsigned char>::result
            == size, define_type<unsigned char>,
        typename select<size_in_bits<unsigned short>::result
            == size, define_type<unsigned short>,
        typename select<size_in_bits<unsigned int>::result
            == size, define_type<unsigned int>,
        typename select<size_in_bits<unsigned long>::result
            == size, define_type<unsigned long>,
        typename select<size_in_bits<unsigned long long>::result
            == size, define_type<unsigned long long>,
        empty_type>::type>::type>::type>::type>::type {};
        
}   //  namespace integer

}   //  namespace meta

}   //  namespace ali
