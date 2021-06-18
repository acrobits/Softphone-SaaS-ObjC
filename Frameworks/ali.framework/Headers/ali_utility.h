/*
 *  ali_utility.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_utility_forward.h"
#include "ali/ali_utility_platform.h"
#include "ali/ali_features.h"
#include "ali/ali_iterator.h"
#include "ali/ali_meta.h"
#include "ali/ali_wchar.h"
#include "ali/ali_xrange.h"


#if defined(ALI_FEATURE_CONSTEXPR)
#   define ALI_CONSTEXPR constexpr
#else
#   define ALI_CONSTEXPR
#endif

namespace ali
{

// ******************************************************************
template <typename T>
inline constexpr T&& forward(
    typename meta::remove_ref<T>::type& t ) noexcept
// ******************************************************************
{
    return static_cast<T&&>(t);
}

// ******************************************************************
template <typename T>
inline constexpr T&& forward(
    typename meta::remove_ref<T>::type&& t ) noexcept
// ******************************************************************
{
    return static_cast<T&&>(t);
}

// ******************************************************************
template <typename T>
inline constexpr typename meta::remove_ref<T>::type&& move( T&& t ) noexcept
// ******************************************************************
{
    return static_cast<typename meta::remove_ref<T>::type&&>(t);
}

// ******************************************************************
template <typename T>
inline constexpr typename meta::remove_ref<T>::type&& move_if_noexcept( T&& t ) noexcept
// ******************************************************************
//  ISSUE:  Always move, for now.
// ******************************************************************
{
    return static_cast<typename meta::remove_ref<T>::type&&>(t);
}


// ******************************************************************
template <typename T, int n>
inline constexpr int array_size( T(&)[n] ) noexcept
// ******************************************************************
{
    return n;
}

#if defined(ALI_FEATURE_VARIADIC_TEMPLATES)
// ******************************************************************
template <typename Function, typename... T>
inline void for_each( Function&& func, T&&... values )
// ******************************************************************
{
    int dummy[sizeof...(values)] = { (func(values), 0)... };
    (void)dummy;
}
#endif

// ******************************************************************
template <typename T>
inline void swap_by_copy( T& x, T& y ) noexcept (
    noexcept(T((x))) && noexcept(x = y))
// ******************************************************************
{
    T const t((x)); x = y; y = t;
}

#define ALI_DEFINE_SWAP_FOR(Type)                   \
    inline void swap( Type& x, Type& y ) noexcept ( \
        noexcept(ali::swap_by_copy(x, y)))          \
    {                                               \
        ali::swap_by_copy(x, y);                    \
    }

ALI_DEFINE_SWAP_FOR(bool)
ALI_DEFINE_SWAP_FOR(char)
ALI_DEFINE_SWAP_FOR(signed char)
ALI_DEFINE_SWAP_FOR(unsigned char)
ALI_DEFINE_SWAP_FOR(wchar)
ALI_DEFINE_SWAP_FOR(short)
ALI_DEFINE_SWAP_FOR(unsigned short)
ALI_DEFINE_SWAP_FOR(int)
ALI_DEFINE_SWAP_FOR(unsigned int)
ALI_DEFINE_SWAP_FOR(long)
ALI_DEFINE_SWAP_FOR(unsigned long)
ALI_DEFINE_SWAP_FOR(long long)
ALI_DEFINE_SWAP_FOR(unsigned long long)
ALI_DEFINE_SWAP_FOR(float)
ALI_DEFINE_SWAP_FOR(double)
ALI_DEFINE_SWAP_FOR(long double)
template <typename T>
ALI_DEFINE_SWAP_FOR(T*)

#undef  ALI_DEFINE_SWAP_FOR


namespace hidden
{

namespace adl_swapper
{

// ******************************************************************
using ali::swap;
// ******************************************************************

// ******************************************************************
template <typename T>
inline void adl_swap( T& a, T& b ) noexcept(noexcept(swap(a, b)))
// ******************************************************************
{
    swap(a, b);
}

}   //  namespace adl_swapper

}   //  namespace hidden

// ******************************************************************
template <typename T>
inline void adl_swap( T& a, T& b ) noexcept(
    noexcept(hidden::adl_swapper::adl_swap(a, b)))
// ******************************************************************
{
    hidden::adl_swapper::adl_swap(a, b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, unsigned N>
inline void swap( T(&x)[N], T(&y)[N] ) noexcept (
    noexcept(ali::adl_swap(x[0], y[0])))
// ******************************************************************
{
    for ( unsigned i = N; i != 0; )
        --i, ali::adl_swap(x[i], y[i]);
}

namespace detail
{

// ******************************************************************
template <typename FI1, typename FI2, typename T>
inline void iter_swap( FI1 x, FI2 y, T* )
// ******************************************************************
{
    T const t(*x); *x = *y; *y = t;
}

}   //  namespace detail

// ******************************************************************
template <typename FI1, typename FI2>
inline void iter_swap( FI1 x, FI2 y )
// ******************************************************************
{
    detail::iter_swap(x, y, get_iterator_value_ptr(x));
}


// ******************************************************************
template <typename FI1, typename FI2>
inline FI2 swap_ranges( FI1 it, FI1 end, FI2 x )
// ******************************************************************
{
    for ( ; it != end; ++it, ++x )
        iter_swap(it, x);
    return x;
}


// ******************************************************************
template <typename T>
inline T const& mini( T const& x, T const& y ) noexcept (
    noexcept(y < x))
// ******************************************************************
{
    return y < x ? y : x;
}

// ******************************************************************
template <typename T>
inline T const& maxi( T const& x, T const& y ) noexcept (
    noexcept(y > x))
// ******************************************************************
{
    return y > x ? y : x;
}

// ******************************************************************
// ******************************************************************

//  A prvalue of type bool can be converted to a prvalue of type int,
//  with false becoming zero and true becoming one.
//      -- ISO/IEC 14882:2011
//
//  a < b ? -1 : b < a ? 1 : 0 == (a > b) - (a < b)
//
#define ALI_DEFINE_COMPARE_FOR(Type)                \
    inline int compare( Type a, Type b ) noexcept ( \
        noexcept((a > b) - (a < b)))                \
    {                                               \
        return (a > b) - (a < b);                   \
    }

ALI_DEFINE_COMPARE_FOR(char)
ALI_DEFINE_COMPARE_FOR(signed char)
ALI_DEFINE_COMPARE_FOR(unsigned char)
ALI_DEFINE_COMPARE_FOR(wchar)
ALI_DEFINE_COMPARE_FOR(short)
ALI_DEFINE_COMPARE_FOR(unsigned short)
ALI_DEFINE_COMPARE_FOR(int)
ALI_DEFINE_COMPARE_FOR(unsigned int)
ALI_DEFINE_COMPARE_FOR(long)
ALI_DEFINE_COMPARE_FOR(unsigned long)
ALI_DEFINE_COMPARE_FOR(long long)
ALI_DEFINE_COMPARE_FOR(unsigned long long)
ALI_DEFINE_COMPARE_FOR(float)
ALI_DEFINE_COMPARE_FOR(double)
ALI_DEFINE_COMPARE_FOR(long double)
template <typename T>
ALI_DEFINE_COMPARE_FOR(T const*)

#undef  ALI_DEFINE_COMPARE_FOR

// ******************************************************************
// ******************************************************************

namespace hidden
{

namespace adl_comparator
{

// ******************************************************************
using ali::compare;
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
inline int adl_compare( T const& a, U const& b ) noexcept(
    noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b);
}

}   //  namespace adl_comparator

}   //  namespace hidden

// ******************************************************************
template <typename T, typename U>
inline int adl_compare( T const& a, U const& b ) noexcept(
    noexcept(hidden::adl_comparator::adl_compare(a, b)))
// ******************************************************************
{
    return hidden::adl_comparator::adl_compare(a, b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct default_comparator
// ******************************************************************
{
    template <typename T, typename U>
    int operator()( T const& a, U const& b ) const noexcept(
        noexcept(ali::adl_compare(a, b)))
    {
        return ali::adl_compare(a, b);
    }

    bool operator==( default_comparator const& ) const noexcept
    {
        return true;
    }

    bool operator!=( default_comparator const& b ) const noexcept
    {
        return !operator==(b);
    }

    void swap( default_comparator& ) noexcept {}

    friend void swap(
        default_comparator&,
        default_comparator& ) noexcept {}
};

// ******************************************************************
// ******************************************************************

namespace meta
{

namespace hidden
{

// ******************************************************************
yes_type comparator_returns_int( int );
// ******************************************************************

// ******************************************************************
template <typename not_int>
no_type comparator_returns_int( not_int );
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename comparator, typename T, typename U = T>
struct comparator_returns_int
// ******************************************************************
    :   define_bool_result<sizeof(hidden::comparator_returns_int(
            make<comparator>()(make<T>(), meta::make<U>())))
                == sizeof(yes_type)> {};

}   //  namespace meta

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//struct less
//// ******************************************************************
//{
//    template <typename T, typename U>
//    bool operator()( T const& a, U const& b ) const
//    {
//        return a < b;
//    }
//
//    friend bool operator==( less, less ) { return true; }
//    friend bool operator!=( less, less ) { return false; }
//
//    void swap( less& ) {}
//    friend void swap( less&, less& ) {}
//};
//
//// ******************************************************************
//struct deref_less
//// ******************************************************************
//{
//    template <typename T, typename U>
//    bool operator()( T const& a, U const& b ) const
//    {
//        return *a < *b;
//    }
//
//    friend bool operator==( deref_less, deref_less ) { return true; }
//    friend bool operator!=( deref_less, deref_less ) { return false; }
//
//    void swap( deref_less& ) {}
//    friend void swap( deref_less&, deref_less& ) {}
//};

//// ******************************************************************
//template <typename comparator>
//struct less_from_comparator
//// ******************************************************************
//{
//    less_from_comparator( void ) {}
//
//    explicit less_from_comparator( comparator cmp )
//    :   _cmp(cmp)
//    {}
//    
//    template <typename T, typename U>
//    bool operator()( T const& a, U const& b ) const
//    {
//        return _cmp(a, b) < 0;
//    }
//
//    template <typename T, typename U>
//    bool operator()( T const* a, U const* b ) const
//    {
//        return _cmp(*a, *b) < 0;
//    }
//
//    friend bool operator==(
//        less_from_comparator,
//        less_from_comparator )
//    {
//        return true;
//    }
//
//    friend bool operator!=(
//        less_from_comparator a,
//        less_from_comparator b )
//    {
//        return !(a == b);
//    }
//
//    void swap( less_from_comparator& b )
//    {
//        using ali::swap;
//        swap(_cmp, b._cmp);
//    }
//
//    friend void swap(
//        less_from_comparator& a,
//        less_from_comparator& b )
//    {
//        a.swap(b);
//    }
//    
//    comparator _cmp;
//};


//// ******************************************************************
//template <typename T>
//struct less_from_compare
//// ******************************************************************
//{
//    explicit less_from_compare( int (*cmp)( T const&, T const& ) )
//    :   _cmp(cmp)
//    {}
//    
//    bool operator()( T const& a, T const& b ) const { return (*_cmp)(a, b) < 0; }
//    bool operator()( T const* a, T const* b ) const { return (*_cmp)(*a, *b) < 0; }
//
//    friend bool operator==(
//        less_from_compare a,
//        less_from_compare b )
//    {
//        return a._cmp == b._cmp;
//    }
//
//    friend bool operator!=(
//        less_from_compare a,
//        less_from_compare b )
//    {
//        return !(a == b);
//    }
//
//    void swap( less_from_compare& b )
//    {
//        using ali::swap;
//        swap(_cmp, b._cmp);
//    }
//
//    friend void swap( less_from_compare& a, less_from_compare& b )
//    {
//        a.swap(b);
//    }
//    
//    int (*_cmp)( T const&, T const& );
//};

//// ******************************************************************
//template <typename T>
//struct less_from_compare_ptr
//// ******************************************************************
//{
//    explicit less_from_compare_ptr( int (*cmp)( T const*, T const* ) )
//    :   _cmp(cmp)
//    {}
//    
//    bool operator()( T const& a, T const& b ) const { return (*_cmp)(&a, &b) < 0; }
//    bool operator()( T const* a, T const* b ) const { return (*_cmp)(a, b) < 0; }
//
//    friend bool operator==(
//        less_from_compare_ptr a,
//        less_from_compare_ptr b )
//    {
//        return a._cmp == b._cmp;
//    }
//
//    friend bool operator!=(
//        less_from_compare_ptr a,
//        less_from_compare_ptr b )
//    {
//        return !(a == b);
//    }
//
//    void swap( less_from_compare_ptr& b )
//    {
//        using ali::swap;
//        swap(_cmp, b._cmp);
//    }
//
//    friend void swap( less_from_compare_ptr& a, less_from_compare_ptr& b )
//    {
//        a.swap(b);
//    }
//
//    int (*_cmp)( T const*, T const* );
//};

// ******************************************************************
template <typename RefT>
class ref_holder
// ******************************************************************
{
public:
    ref_holder( RefT ref ) : _ref(ref) {}
    operator RefT( void ) const { return _ref; }

private:
    RefT _ref;
};


// ******************************************************************
template <typename T>
ref_holder<T&> by_ref( T& t )
// ******************************************************************
{
    return ref_holder<T&>(t);
}


// ******************************************************************
template <typename T>
ref_holder<T const&> by_const_ref( T const& t )
// ******************************************************************
{
    return ref_holder<T const&>(t);
}


// ******************************************************************
template <typename T1, typename T2>
struct pair
// ******************************************************************
{
    typedef T1              first_type;
    typedef T2              second_type;

    pair( void )
    :   first(),
        second()
    {}

    template <typename U1, typename U2>
    pair( U1&& val1, U2&& val2 )
    :   first(ali::forward<U1>(val1)),
        second(ali::forward<U2>(val2))
    {}

    template <typename U1, typename U2>
    pair( pair<U1, U2> const& b )
    :   first(b.first),
        second(b.second)
    {}

    template <typename U1, typename U2>
    pair( pair<U1, U2>&& b )
    :   first(ali::move(b.first)),
        second(ali::move(b.second))
    {}

    void swap( pair& b )
    {
        using ali::swap;
        swap(first, b.first);
        swap(second, b.second);
    }

    friend void swap( pair& a, pair& b )
    {
        a.swap(b);
    }

    T1 first;
    T2 second;
};

// ******************************************************************
template <typename T1, typename T2>
inline bool operator==( pair<T1, T2> const& a,
                        pair<T1, T2> const& b )
// ******************************************************************
{
    return a.first == b.first && a.second == b.second;
}

// ******************************************************************
template <typename T1, typename T2>
inline bool operator!=( pair<T1, T2> const& a,
                        pair<T1, T2> const& b )
// ******************************************************************
{
    return !(a == b);
}


// ******************************************************************
template <typename T1, typename T2>
inline pair<T1, T2> make_pair( T1 const& a, T2 const& b )
// ******************************************************************
{
    return pair<T1, T2>(a, b);
}

namespace detail
{

// ******************************************************************
template <typename T1, typename T2>
struct tier
// ******************************************************************
{
    tier( T1& a, T2& b ) : first(a), second(b) {}

    tier<T1, T2>& operator=( pair<T1, T2> const& p )
    {
        first = p.first;
        second = p.second;
        return *this;
    }

    T1& first;
    T2& second;

private:  // Methods
    tier<T1, T2> operator=( tier<T1, T2> const& );
};

}

// ******************************************************************
template <typename T1, typename T2>
inline detail::tier<T1, T2> tie( T1& a, T2& b )
// ******************************************************************
{
    return detail::tier<T1, T2>(a, b);
}


// ******************************************************************
template<typename II1, typename II2>
inline bool equal( II1 it1, II1 end1, II2 it2 )
// ******************************************************************
{
    for ( ; it1 != end1; ++it1, ++it2 )
        if ( !(*it1 == *it2) )
            return false;
    return true;
}

// ******************************************************************
inline void* advance_n_bytes( void* ptr, int n ) noexcept
// ******************************************************************
{
    return reinterpret_cast<char*>(ptr) + n;
}

// ******************************************************************
template <typename derived, typename intT>
class flags_common
// ******************************************************************
{
public:     //  Typedefs
    using int_type = intT;

public:
    constexpr flags_common( void ) = default;

    constexpr explicit flags_common( int_type value )
    :   _value{value}
    {}

    friend constexpr bool operator==( derived a, derived b )
    {
        return a._value == b._value;
    }

    friend constexpr bool operator!=( derived a, derived b )
    {
        return !(a == b);
    }

    template <int_type flags>
    constexpr bool is_any( void ) const
    {
        return (this->_value & flags) != int_type{};
    }

    constexpr bool is_any( int_type flags ) const
    {
        return (this->_value & flags) != int_type{};
    }
    
    template <int_type flags>
    constexpr bool is_all( void ) const
    {
        return (this->_value & flags) == flags;
    }

    constexpr bool is_all( int_type flags ) const
    {
        return (this->_value & flags) == flags;
    }

    template <int_type flags>
    constexpr derived& reset( void )
    {
        this->_value &= ~flags;
        return this->self();
    }

    constexpr derived& reset( int_type flags )
    {
        this->_value &= ~flags;
        return this->self();
    }

    template <int_type flags>
    constexpr derived& set( void )
    {
        this->_value |= flags;
        return this->self();
    }

    constexpr derived& set( int_type flags )
    {
        this->_value |= flags;
        return this->self();
    }

    template <int_type flags, int_type values>
    constexpr derived& set( void )
    {
        this->_value = (this->_value & ~flags) | (values & flags);
        return this->self();
    }

    template <int_type flags>
    constexpr derived& set( int_type values )
    {
        this->_value = (this->_value & ~flags) | (values & flags);
        return this->self();
    }

    constexpr derived& set( int_type flags, int_type values )
    {
        this->_value = (this->_value & ~flags) | (values & flags);
        return this->self();
    }

    constexpr int_type get( void ) const
    {
        return this->_value;
    }

    constexpr void swap( derived& b )
    {
        swap_by_copy(_value, b._value);
    }

    friend constexpr void swap( derived& a, derived& b )
    {
        a.swap(b);
    }

private:    //  Methods
    constexpr derived& self( void )
    {
        return static_cast<derived&>(*this);
    }

    constexpr derived const& self( void ) const
    {
        return static_cast<derived const&>(*this);
    }
    
private:    //  Data members
    int_type    _value{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct yes_no
// ******************************************************************
{
    enum type : unsigned char {no, yes};

    yes_no( type value )
    :   value{value}
    {}

    friend bool operator==( yes_no a, yes_no b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( yes_no a, yes_no b )
    {
        return !(a == b);
    }

    bool is_no( void ) const
    {
        return value == no;
    }

    bool is_yes( void ) const
    {
        return value == yes;
    }

    yes_no operator!( void ) const
    {
        return static_cast<type>(yes - value);
    }

    type    value;
};

}   //  namespace ali
