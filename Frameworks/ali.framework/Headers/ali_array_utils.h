#pragma once
#include "ali/ali_array_utils_forward.h"
#include "ali/ali_array_forward.h"
#include "ali/ali_array_utils_platform.h"
#include "ali/ali_debug.h"
#include "ali/ali_deprecated.h"
#include "ali/ali_endianness.h"
#include "ali/ali_functor_types_common.h"
#include "ali/ali_initializer_list.h"
#include "ali/ali_integer.h"
#include "ali/ali_location.h"
#include "ali/ali_meta.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_utility.h"
#include "ali/ali_wchar.h"

// ******************************************************************
// ******************************************************************
//  Note on ref_left/ref_right and ref_front/ref_back methods.
// ******************************************************************
//
//  ref_left/ref_right
//  ------------------
//
//  These methods take as parameter *a position*
//  inside the array.
//
//  The ref_left(position) method returns
//  the left half from position *excluding*
//  the element at the given position,
//  i.e. [0, position).
//
//  The ref_right(position) method returns
//  the right half from position *including*
//  the element at the given position,
//  i.e. [position, size).
//
//  0                  position         size
//  +------------------+----------------o
//  +-----ref_left----++---ref_right---+
//
//
//  ref_front/ref_back
//  ------------------
//
//  These methods take as parameter *number of elements*
//  from the beginning/end of the array.
//
//  The ref_front(n) method returns
//  the first n elements in the array.
//
//  The ref_back(n) method returns
//  the last n elements in the array.
//
//  The ref_not_front(n) method returns
//  the last size-n elements in the array.
//
//  The ref_not_back(n) method returns
//  the first size-n elements in the array.
//
//  0                 n   size-n            size
//  +-----------------+---+-----------------o
//  +--ref_front(n)--++--ref_not_front(n)--+
//  +--ref_not_back(n)---++---ref_back(n)--+
//
// ******************************************************************
// ******************************************************************

namespace ali
{

// ******************************************************************
struct is_internal_element_result { int index_or_negative; };
// ******************************************************************

// ******************************************************************
struct is_internal_pointer_result { int distance_or_negative; };
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
namespace string_literals
// ******************************************************************
//  This namespace exists mainly as a workaround for MSVC compiler
//  bug--it is unable to find user defined literal operators
//  introduced with using declaration in templates, i.e. instead of
//
//      using ali::operator""_s;
//
//  one has to write
//
//      using namespace ali::string_literals;
//
// ******************************************************************
{

// ******************************************************************
/*constexpr*/ string_literal operator""_s(
    char const* str, size_t size ) noexcept;
// ******************************************************************

// ******************************************************************
/*constexpr*/ wstring_literal operator""_s(
    ali::wchar const* str, size_t size ) noexcept;
// ******************************************************************

// ******************************************************************
/*constexpr*/ string_literal_ptr operator""_sp(
    char const* str, size_t size ) noexcept;
// ******************************************************************

// ******************************************************************
/*constexpr*/ wstring_literal_ptr operator""_sp(
    ali::wchar const* str, size_t size ) noexcept;
// ******************************************************************

// ******************************************************************
// ******************************************************************

}   //  namespace string_literals

using namespace string_literals;

// ******************************************************************
template <typename T, typename D>
struct exposes_array_const_ref
// ******************************************************************
//  The class D MUST derive from
//
//      exposes_array_const_ref<T, D>
//
//  and define the following methods:
//
//      array_const_ref<T> ref( int pos, int n ) const;
//      array_const_ref<T> ref( void ) const;
//
// ******************************************************************
{
    //  ref

    auto ref( int pos, int n ) const noexcept
    {
        return static_cast<D const&>(*this).ref(pos, n);
    }

    auto ref( void ) const noexcept
    {
        return static_cast<D const&>(*this).ref();
    }

    auto ref_left( int pos ) const noexcept
    {
        return this->ref().ref_left(pos);
    }

    auto ref_right( int pos ) const noexcept
    {
        return this->ref().ref_right(pos);
    }

    auto ref_front( int n ) const noexcept
    {
        return this->ref().ref_front(n);
    }

    auto ref_back( int n ) const noexcept
    {
        return this->ref().ref_back(n);
    }

    auto ref_not_front( int n ) const noexcept
    {
        return this->ref().ref_not_front(n);
    }

    auto ref_not_back( int n ) const noexcept
    {
        return this->ref().ref_not_back(n);
    }

    auto ref_not_front_not_back( int n ) const noexcept
    {
        return this->ref_not_front(n).ref_not_back(n);
    }

    auto ref_not_front_not_back( int nf, int nb ) const noexcept
    {
        return this->ref_not_front(nf).ref_not_back(nb);
    }

    //  pointer

    auto pointer( int pos, int n ) const noexcept
    {
        return this->ref(pos, n).pointer();
    }

    auto pointer( void ) const noexcept
    {
        return this->ref().pointer();
    }

    auto pointer_left( int pos ) const noexcept
    {
        return this->ref_left(pos).pointer();
    }

    auto pointer_right( int pos ) const noexcept
    {
        return this->ref_right(pos).pointer();
    }

    auto pointer_front( int n ) const noexcept
    {
        return this->ref_front(n).pointer();
    }

    auto pointer_back( int n ) const noexcept
    {
        return this->ref_back(n).pointer();
    }

    auto pointer_not_front( int n ) const noexcept
    {
        return this->ref_not_front(n).pointer();
    }

    auto pointer_not_back( int n ) const noexcept
    {
        return this->ref_not_back(n).pointer();
    }

    auto pointer_not_front_not_back( int n ) const noexcept
    {
        return this->ref_not_front(n).pointer_not_back(n);
    }

    auto pointer_not_front_not_back( int nf, int nb ) const noexcept
    {
        return this->ref_not_front(nf).pointer_not_back(nb);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename D>
struct exposes_array_ref : exposes_array_const_ref<T, D>
// ******************************************************************
//  The class D MUST derive from
//
//      exposes_array_ref<T, D>
//
//  and define the following methods:
//
//      array_const_ref<T> ref( int pos, int n ) const;
//      array_const_ref<T> ref( void ) const;
//
//      array_ref<T> mutable_ref( int pos, int n );
//      array_ref<T> mutable_ref( void );
//
// ******************************************************************
{
    //  mutable_ref

    auto mutable_ref( int pos, int n ) noexcept
    {
        return static_cast<D&>(*this).mutable_ref(pos, n);
    }

    auto mutable_ref( void ) noexcept
    {
        return static_cast<D&>(*this).mutable_ref();
    }

    auto mutable_ref_left( int pos ) noexcept
    {
        return this->mutable_ref().mutable_ref_left(pos);
    }

    auto mutable_ref_right( int pos ) noexcept
    {
        return this->mutable_ref().mutable_ref_right(pos);
    }

    auto mutable_ref_front( int n ) noexcept
    {
        return this->mutable_ref().mutable_ref_front(n);
    }

    auto mutable_ref_back( int n ) noexcept
    {
        return this->mutable_ref().mutable_ref_back(n);
    }

    auto mutable_ref_not_front( int n ) noexcept
    {
        return this->mutable_ref().mutable_ref_not_front(n);
    }

    auto mutable_ref_not_back( int n ) noexcept
    {
        return this->mutable_ref().mutable_ref_not_back(n);
    }

    auto mutable_ref_not_front_not_back( int n ) const noexcept
    {
        return this->mutable_ref_not_front(n).mutable_ref_not_back(n);
    }

    auto mutable_ref_not_front_not_back( int nf, int nb ) const noexcept
    {
        return this->mutable_ref_not_front(nf).mutable_ref_not_back(nb);
    }
    

    //  mutable_pointer

    auto mutable_pointer( int pos, int n ) noexcept
    {
        return this->mutable_ref(pos, n).mutable_pointer();
    }

    auto mutable_pointer( void ) noexcept
    {
        return this->mutable_ref().mutable_pointer();
    }

    auto mutable_pointer_left( int pos ) noexcept
    {
        return this->mutable_ref_left(pos).mutable_pointer();
    }

    auto mutable_pointer_right( int pos ) noexcept
    {
        return this->mutable_ref_right(pos).mutable_pointer();
    }

    auto mutable_pointer_front( int n ) noexcept
    {
        return this->mutable_ref_front(n).mutable_pointer();
    }

    auto mutable_pointer_back( int n ) noexcept
    {
        return this->mutable_ref_back(n).mutable_pointer();
    }

    auto mutable_pointer_not_front( int n ) noexcept
    {
        return this->mutable_ref_not_front(n).mutable_pointer();
    }

    auto mutable_pointer_not_back( int n ) noexcept
    {
        return this->mutable_ref_not_back(n).mutable_pointer();
    }

    auto mutable_pointer_not_front_not_back( int n ) const noexcept
    {
        return this->mutable_ref_not_front(n).mutable_pointer_not_back(n);
    }

    auto mutable_pointer_not_front_not_back( int nf, int nb ) const noexcept
    {
        return this->mutable_ref_not_front(nf).mutable_pointer_not_back(nb);
    }
};

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T, typename U>
void copy( T* dst, U const* src, int n ) noexcept(
    noexcept(dst[0] = src[0]))
// ******************************************************************
//  Copies arrays of different types.
//  Copies elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = src[n];
}

// ******************************************************************
template <typename T>
inline void copy( T* dst, T const* src, int n,
    meta::define_bool_result<true> /*is_trivially_copyable*/ ) noexcept
// ******************************************************************
//  Copies arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void copy( T* dst, T const* src, int n,
    meta::define_bool_result<false> /*is_trivially_copyable*/ ) noexcept(
        noexcept(dst[0] = src[0]))
// ******************************************************************
//  Copies arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i{}; i != n; ++i )
            dst[i] = src[i];
    }
    else if ( dst != src )
    {
        hidden::copy<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = src[n];
*/
    }
}

// ******************************************************************
template <typename T>
inline void copy( T* dst, T const* src, int n ) noexcept(
    noexcept(hidden::copy(dst, src, n, meta::is_trivially_copyable<T>{})))
// ******************************************************************
//  Copies arrays of the same types.
//  Copies elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::copy(dst, src, n, meta::is_trivially_copyable<T>{});
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
void move( T* dst, U* src, int n ) noexcept(
    noexcept(dst[0] = ali::move(src[0])))
// ******************************************************************
//  Moves elements of arrays of different types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = ali::move(src[n]);
}

// ******************************************************************
template <typename T>
inline void move( T* dst, T* src, int n,
    meta::define_bool_result<true> /*is_trivially_copyable*/ ) noexcept
// ******************************************************************
//  Moves elements of arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void move( T* dst, T* src, int n,
    meta::define_bool_result<false> /*is_trivially_copyable*/ ) noexcept(
        noexcept(dst[0] = ali::move(src[0])))
// ******************************************************************
//  Moves elements of arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i{}; i != n; ++i )
            dst[i] = ali::move(src[i]);
    }
    else if ( dst != src )
    {
        hidden::move<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = ali::move(src[n]);
*/
    }
}

// ******************************************************************
template <typename T>
inline void move( T* dst, T* src, int n ) noexcept(
    noexcept(hidden::move(dst, src, n, meta::is_trivially_copyable<T>{})))
// ******************************************************************
//  Moves elements of arrays of the same types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::move(dst, src, n, meta::is_trivially_copyable<T>{});
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
void move_if_noexcept( T* dst, U* src, int n ) noexcept(
    noexcept(dst[0] = ali::move_if_noexcept(src[0])))
// ******************************************************************
//  Moves elements of arrays of different types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  Assumption: dst and src don't overlap.
// ******************************************************************
{
    while ( n != 0 )
        --n, dst[n] = ali::move_if_noexcept(src[n]);
}

// ******************************************************************
template <typename T>
inline void move_if_noexcept( T* dst, T* src, int n,
    meta::define_bool_result<true> /*is_trivially_copyable*/ ) noexcept
// ******************************************************************
//  Moves elements of arrays of bitwise copyable elements.
// ******************************************************************
{
    if ( n != 0 && dst != src )
        platform::memmove(dst, src, n * sizeof(T));
}

// ******************************************************************
template <typename T>
void move_if_noexcept( T* dst, T* src, int n,
    meta::define_bool_result<false> /*is_trivially_copyable*/ ) noexcept(
        noexcept(dst[0] = ali::move_if_noexcept(src[0])))
// ******************************************************************
//  Moves elements of arrays of NOT bitwise copyable elements.
// ******************************************************************
{
    if ( dst < src )
    {
        for ( int i{}; i != n; ++i )
            dst[i] = ali::move_if_noexcept(src[i]);
    }
    else if ( dst != src )
    {
        hidden::move_if_noexcept<T, T>(dst, src, n);
/*
        while ( n != 0 )
            --n, dst[n] = ali::move(src[n]);
*/
    }
}

// ******************************************************************
template <typename T>
inline void move_if_noexcept( T* dst, T* src, int n ) noexcept(
    noexcept(hidden::move_if_noexcept(dst, src, n, meta::is_trivially_copyable<T>{})))
// ******************************************************************
//  Moves elements of arrays of the same types.
//  Moves elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
//  Uses memmove if possible.
// ******************************************************************
{
    hidden::move_if_noexcept(dst, src, n, meta::is_trivially_copyable<T>{});
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct read_only : meta::select<
    meta::is_fundamental<T>::result,
    T const, T const&> {};
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
void fill( T* arr, typename read_only<T>::type value, int n,
    meta::define_bool_result<false> /*is_default_all_zeros*/ ) noexcept(
        noexcept(hidden::copy(arr, arr, 0)))
// ******************************************************************
//  Fills array with a given value.
// ******************************************************************
{
    if ( n != 0 )
    {
        int count{1};

        hidden::copy(arr, &value, count);

        while ( count != n )
        {
            int const copy_count{
                ali::mini(n - count, count)};

            hidden::copy(arr + count, arr, copy_count);

            count += copy_count;
        }
    }
}

// ******************************************************************
template <typename T>
inline void fill( T* arr, typename read_only<T>::type value, int n,
    meta::define_bool_result<true> /*is_default_all_zeros*/ ) noexcept(
        noexcept(fill(arr, value, n, meta::define_bool_result<false>{})))
// ******************************************************************
//  Fills array of a "default all zeros" type
//  with a given value using memset if possible.
// ******************************************************************
{
    if ( value != T() )
        fill(arr, value, n, meta::define_bool_result<false>{});
    else if ( n != 0 )
        platform::memset(arr, 0, n * sizeof(T));
}

// ******************************************************************
template <typename T>
inline void fill( T* arr, typename read_only<T>::type value, int n ) noexcept(
    noexcept(fill(arr, value, n, meta::is_default_all_zeros<T>{})))
// ******************************************************************
//  Fills array with a given value using memset if possible.
// ******************************************************************
{
    fill(arr, value, n, meta::is_default_all_zeros<T>{});
}

// ******************************************************************
inline void fill( char* arr, read_only<char>::type value, int n ) noexcept
// ******************************************************************
//  Fills array of chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    signed char* arr, read_only<signed char>::type value, int n ) noexcept
// ******************************************************************
//  Fills array of signed chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    unsigned char* arr, read_only<unsigned char>::type value, int n ) noexcept
// ******************************************************************
//  Fills array of unsigned chars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::memset(arr, value, n);
}

// ******************************************************************
inline void fill(
    ali::wchar* arr, read_only<ali::wchar>::type value, int n ) noexcept
// ******************************************************************
//  Fills array of wchars with a given value using memset.
// ******************************************************************
{
    if ( n != 0 )
        platform::wmemset(arr, value, n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U, typename transform>
inline int array_const_ref_compare(
    array_const_ref<T> a,
    array_const_ref<U> b,
    transform t ) noexcept(
// ******************************************************************
        noexcept(ali::mini(a.size(), b.size()))
    &&  noexcept(ali::adl_compare(t(a.at(0)), t(b.at(0))))
    &&  noexcept(ali::adl_compare(a.size(), b.size())))
// ******************************************************************
{
    int result{};

    int const n{ali::mini(a.size(), b.size())};

    for ( int i{}; result == 0 && i != n; ++i )
        result = ali::adl_compare(t(a.at(i)), t(b.at(i)));

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

// ******************************************************************
int array_const_ref_compare(
    array_const_ref<unsigned char> a,
    array_const_ref<unsigned char> b,
    functor_types::identity t ) noexcept;
// ******************************************************************
//  Can use memcmp.
// ******************************************************************

//
//  BEWARE:
//  While we would like to use memcmp for the following two
//  specializations as well, it unfortunately gives incorrect
//  results when the arrays contain negative values.
//  It's because the memcmp function interprets the data
//  as unsigned chars.
//
//  Example: (signed char)-1 < 0, but (unsigned char)-1 = 255 > 0
//
//  We cannot use strncmp either, because it stops at the first
//  zero it encounters.
//
//  Having said that, we are going to use memcmp for chars
//  as there is little harm in doing so, but not for the
//  signed chars.
//

// ******************************************************************
int array_const_ref_compare(
    array_const_ref<char> a,
    array_const_ref<char> b,
    functor_types::identity t ) noexcept;
// ******************************************************************
int array_const_ref_compare(
    array_const_ref<unsigned char> a,
    array_const_ref<char> b,
    functor_types::identity t ) noexcept;
// ******************************************************************
int array_const_ref_compare(
    array_const_ref<char> a,
    array_const_ref<unsigned char> b,
    functor_types::identity t ) noexcept;
// ******************************************************************
//  Using memcmp does little harm.
// ******************************************************************

// ******************************************************************
//int array_const_ref_compare(
//    array_const_ref<signed char> a,
//    array_const_ref<signed char> b,
//    functor_types::identity t ) const noexcept;
// ******************************************************************
//  BEWARE: Using memcmp would give INCORRECT results!
// ******************************************************************

// ******************************************************************
int array_const_ref_compare(
    array_const_ref<ali::wchar> a,
    array_const_ref<ali::wchar> b,
    functor_types::identity t ) noexcept;
// ******************************************************************
//  Can use wmemcmp.
// ******************************************************************

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename T>
class array_const_ref_common
// ******************************************************************
{
public:     //  Typedefs
    using read_only_T = typename hidden::read_only<T>::type;

public:

ALI_IF_DEBUG_WEAK_REF(
    debug::weak_reference& weak_reference( void ) const noexcept
    {
        return _wref;
    }

)   //  ALI_IF_DEBUG_WEAK_REF

    constexpr bool is_empty( void ) const noexcept
    {
        return this->_size == 0;
    }

    constexpr int size( void ) const noexcept
    {
        return this->_size;
    }

    constexpr T const& front( void ) const noexcept
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());

        return this->_begin[0];
    }

    constexpr T const& back( void ) const noexcept
        //  pre:    !is_empty()
    {
        ali_constexpr_assert(!this->is_empty());

        return this->_begin[this->_size - 1];
    }

    constexpr T const& back( int i ) const noexcept
        //  pre:    0 <= i && i < size()
    {
        ali_constexpr_assert(0 <= i);
        ali_constexpr_assert(i < this->_size);

        return this->_begin[this->_size - 1 - i];
    }

    constexpr T const& at( int i ) const noexcept
    {
        ali_constexpr_assert(0 <= i);
        ali_constexpr_assert(i < this->_size);

        return this->_begin[i];
    }

    constexpr T const& operator[]( int i ) const noexcept
    {
        return this->at(i);
    }

    constexpr T const* data( void ) const noexcept
    {
        return this->_begin;
    }

    //  ref

    array_const_ref<T> ref( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);
        ali_assert(0 <= n);
        ali_assert(n <= this->_size - pos);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin + pos, n};
    }

    array_const_ref<T> ref( void ) const noexcept
    {
        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin, this->_size};
    }

    array_const_ref<T> ref_left( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin, pos};
    }

    array_const_ref<T> ref_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin + pos, this->_size - pos};
    }

    array_const_ref<T> ref_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin, n};
    }

    array_const_ref<T> ref_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin + (this->_size - n), n};
    }

    array_const_ref<T> ref_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin + n, this->_size - n};
    }

    array_const_ref<T> ref_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin, this->_size - n};
    }

    array_const_ref<T> ref_not_front_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size() / 2
    {
        return this->ref_not_front(n).ref_not_back(n);
    }

    array_const_ref<T> ref_not_front_not_back( int nf, int nb ) const noexcept
        //  pre:    0 <= fn && nf <= size()
        //      &&  0 <= nb && nb <= size() - nf
    {
        return this->ref_not_front(nf).ref_not_back(nb);
    }


    //  pointer

    array_const_ptr<T> pointer( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        return this->ref(pos, n).pointer();
    }

    array_const_ptr<T> pointer( void ) const noexcept
    {
        return array_const_ptr<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->_begin, this->_size};
    }

    array_const_ptr<T> pointer_left( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return this->ref_left(pos).pointer();
    }

    array_const_ptr<T> pointer_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return this->ref_right(pos).pointer();
    }

    array_const_ptr<T> pointer_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->ref_front(n).pointer();
    }

    array_const_ptr<T> pointer_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->ref_back(n).pointer();
    }

    array_const_ptr<T> pointer_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->ref_not_front(n).pointer();
    }

    array_const_ptr<T> pointer_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->ref_not_back(n).pointer();
    }

    array_const_ptr<T> pointer_not_front_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size() / 2
    {
        return this->ref_not_front(n).pointer_not_back(n);
    }

    array_const_ptr<T> pointer_not_front_not_back( int nf, int nb ) const noexcept
        //  pre:    0 <= fn && nf <= size()
        //      &&  0 <= nb && nb <= size() - nf
    {
        return this->ref_not_front(nf).pointer_not_back(nb);
    }


    //  for_each

    template <typename body>
    array_const_ref<T> for_each( body&& b ) const noexcept(
        noexcept(b(this->at(0))))
    {
        for ( int i = this->_size; i != 0; )
            b(this->at(--i));

        return this->ref();
    }

    //  is_equal_to

    template <typename U, typename transform>
    bool is_equal_to( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) != t(b.at(0))}))
    {
        if ( this->_size != b._size )
            return false;
        else for ( int i = this->_size; i != 0; )
            if ( --i, bool{t(this->at(i)) != t(b.at(i))} )
                return false;
        return true;
    }

    template <typename transform>
    bool is_equal_to( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->is_equal_to<T, transform>(ali::move(b), t)))
    {
        return this->is_equal_to<T, transform>(ali::move(b), t);
    }

    template <typename U>
    bool is_equal_to( array_const_ref<U> b, functor_types::identity ) const noexcept(
        noexcept(this->_is_equal_to(ali::move(b), meta::define_bool_result<
            //  Use memcmp if:
                    meta::is_fundamental<T>::result
                &&  meta::is_same_type_remove_cv<T, U>::result>())))
    {
        return this->_is_equal_to(ali::move(b), meta::define_bool_result<
            //  Use memcmp if:
                    meta::is_fundamental<T>::result
                &&  meta::is_same_type_remove_cv<T, U>::result>());
    }

    bool is_equal_to( array_const_ref<T> b, functor_types::identity ) const noexcept(
        noexcept(this->is_equal_to<T>(ali::move(b), ali::identity)))
    {
        return this->is_equal_to<T>(ali::move(b), ali::identity);
    }

    template <typename U>
    bool is_equal_to( array_const_ref<U> b ) const noexcept(
        noexcept(this->is_equal_to(ali::move(b), ali::identity)))
    {
        return this->is_equal_to(ali::move(b), ali::identity);
    }

    bool is_equal_to( array_const_ref<T> b ) const noexcept(
        noexcept(this->is_equal_to<T>(ali::move(b))))
    {
        return this->is_equal_to<T>(ali::move(b));
    }

    //  compare

    template <typename U, typename transform>
    int compare( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(hidden::array_const_ref_compare(this->ref(), ali::move(b), t)))
    {
        return hidden::array_const_ref_compare(this->ref(), ali::move(b), t);
    }

    template <typename transform>
    int compare( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->compare<T, transform>(ali::move(b), t)))
    {
        return this->compare<T, transform>(ali::move(b), t);
    }

    template <typename U>
    int compare( array_const_ref<U> b ) const noexcept(
        noexcept(this->compare(ali::move(b), ali::identity)))
    {
        return this->compare(ali::move(b), ali::identity);
    }

    int compare( array_const_ref<T> b ) const noexcept(
        noexcept(this->compare<T>(ali::move(b))))
    {
        return this->compare<T>(ali::move(b));
    }

    //  overlaps_with

    bool overlaps_with( array_const_ref<T> b ) const noexcept
    {
        return this->_begin + this->_size > b._begin
            && b._begin + b._size > this->_begin;
    }

    //  begins_with

    template <typename transform>
    bool begins_with( read_only_T b, transform t ) const noexcept(
        noexcept(bool{t(this->front()) == t(b)}))
    {
        return !this->is_empty() && bool{t(this->front()) == t(b)};
    }

    bool begins_with( read_only_T b ) const noexcept(
        noexcept(this->begins_with(b, ali::identity)))
    {
        return this->begins_with(b, ali::identity);
    }

    template <typename U, typename transform>
    bool begins_with( U const& b, transform t ) const noexcept(
        noexcept(bool{t(this->front()) == t(b)}))
    {
        return !this->is_empty() && bool{t(this->front()) == t(b)};
    }

    template <typename U, typename transform>
    bool begins_with_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(this->ref_front(0).is_equal_to(ali::move(b), t)))
        //  Matches elements of b against b.size() front elements.
    {
        int const b_size{b._size};
        return  this->_size >= b_size
            &&  this->ref_front(b_size).is_equal_to(ali::move(b), t);
    }

    template <typename transform>
    bool begins_with_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->begins_with_n<T, transform>(ali::move(b), t)))
    {
        return this->begins_with_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    bool begins_with( U const& b ) const noexcept(
        noexcept(this->begins_with(b, ali::identity)))
    {
        return this->begins_with(b, ali::identity);
    }

    template <typename U>
    bool begins_with_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->begins_with_n(ali::move(b), ali::identity)))
        //  Matches elements of b against b.size() front elements.
    {
        return this->begins_with_n(ali::move(b), ali::identity);
    }

    bool begins_with_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->begins_with_n<T>(ali::move(b))))
    {
        return this->begins_with_n<T>(ali::move(b));
    }

    //  ends_with

    template <typename transform>
    bool ends_with( read_only_T b, transform t ) const noexcept(
        noexcept(!this->is_empty() && t(this->back()) == t(b)))
    {
        return !this->is_empty() && t(this->back()) == t(b);
    }

    bool ends_with( read_only_T b ) const noexcept(
        noexcept(this->ends_with(b, ali::identity)))
    {
        return this->ends_with(b, ali::identity);
    }

    template <typename U, typename transform>
    bool ends_with( U const& b, transform t ) const noexcept(
        noexcept(!this->is_empty() && t(this->back()) == t(b)))
    {
        return !this->is_empty() && t(this->back()) == t(b);
    }

    template <typename U, typename transform>
    bool ends_with_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(this->ref_back(0).is_equal_to(ali::move(b), t)))
        //  Matches elements of b against b.size() back elements.
    {
        int const b_size{b._size};
        return  this->_size >= b_size
            &&  this->ref_back(b_size).is_equal_to(ali::move(b), t);
    }


    template <typename transform>
    bool ends_with_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->ends_with_n<T, transform>(ali::move(b), t)))
    {
        return this->ends_with_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    bool ends_with( U const& b ) const noexcept(
        noexcept(this->ends_with(b, ali::identity)))
    {
        return this->ends_with(b, ali::identity);
    }

    template <typename U>
    bool ends_with_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->ends_with_n(ali::move(b), ali::identity)))
        //  Matches elements of b against b.size() back elements.
    {
        return this->ends_with_n(ali::move(b), ali::identity);
    }

    bool ends_with_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->ends_with_n<T>(ali::move(b))))
    {
        return this->ends_with_n<T>(ali::move(b));
    }

    //  index_of_first_if

    template <typename predicate>
    int index_of_first_if( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   p(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  !p(at(i)) )
    {
        for ( int i{}; i != this->_size; ++i )
            if ( bool{p(this->at(i))} )
                return i;

        return this->_size;
    }

    //  index_of_first_if_not

    template <typename predicate>
    int index_of_first_if_not( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   !p(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  p(at(i)) )
    {
        for ( int i{}; i != this->_size; ++i )
            if ( !bool{p(this->at(i))} )
                return i;

        return this->_size;
    }

    //  index_of_first

    template <typename transform>
    int index_of_first( read_only_T b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i{}; i != this->_size; ++i )
            if ( bool{t(this->at(i)) == t(b)} )
                return i;

        return this->_size;
    }

    int index_of_first( read_only_T b ) const noexcept(
        noexcept(this->index_of_first(b, ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_first(b, ali::identity);
    }

    template <typename U, typename transform>
    int index_of_first( U const& b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i{}; i != this->_size; ++i )
            if ( bool{t(this->at(i)) == t(b)} )
                return i;

        return this->_size;
    }

    template <typename U, typename transform>
    int index_of_first_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) != t(b.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b, t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !ref(i, b.size()).is_equal_to(b, t) )
    {
        if ( b.is_empty() )
            return 0;

        if ( this->_size < b._size )
            return this->_size;

        int const i_max = this->_size - b._size;

        int i{};

        for ( int j{b._size}; j != 0; )
            if ( --j, bool{t(this->at(i + j)) != t(b.at(j))} )
            {
                if ( i == i_max )
                    return this->_size;

                ++i;

                j = b._size;
            }

        return i;
    }

    template <typename transform>
    int index_of_first_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->index_of_first_n<T, transform>(ali::move(b), t)))
    {
        return this->index_of_first_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    int index_of_first( U const& b ) const noexcept(
        noexcept(this->index_of_first(b, ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {0 ... result - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_first(b, ali::identity);
    }

    template <typename U>
    int index_of_first_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->index_of_first_n(ali::move(b), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !ref(i, b.size()).is_equal_to(b) )
    {
        return this->index_of_first_n(ali::move(b), ali::identity);
    }

    int index_of_first_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->index_of_first_n<T>(ali::move(b))))
    {
        return this->index_of_first_n<T>(ali::move(b));
    }

    //  index_of_first_of

    template <typename U, typename transform>
    int index_of_first_of( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(set.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result), t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  !set.contains(at(i), t) )
    {
        if ( !set.is_empty() )
            for ( int i{}; i != this->_size; ++i )
                for ( int j{set._size}; j != 0; )
                    if ( --j, bool{t(this->at(i)) == t(set.at(j))} )
                        return i;

        return this->_size;
    }

    template <typename transform>
    int index_of_first_of( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->index_of_first_of<T, transform>(ali::move(set), t)))
    {
        return this->index_of_first_of<T, transform>(ali::move(set), t);
    }

    template <typename U>
    int index_of_first_of( array_const_ref<U> set ) const noexcept(
        noexcept(this->index_of_first_of(ali::move(set), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  !set.contains(at(i)) )
    {
        return this->index_of_first_of(ali::move(set), ali::identity);
    }

    int index_of_first_of( array_const_ref<T> set ) const noexcept(
        noexcept(this->index_of_first_of<T>(ali::move(set))))
    {
        return this->index_of_first_of<T>(ali::move(set));
    }

    //  prioritized_index_of_first_of
    //
    //  Treats *this and set as two ordered lists of options with the most
    //  desirable option first. Searches for a common option such that the
    //  SUM of the option indexes in the two lists IS MINIMUM.
    //  In case there are more such options the option that appears first
    //  in *this list is selected.

    template <typename U, typename transform>
    int prioritized_index_of_first_of( array_const_ref<U> set, transform t ) const noexcept(
            noexcept(set.index_of_first_of(this->ref(), t))
        &&  noexcept(set.index_of_first(this->at(0), t))
        &&  noexcept(this->index_of_first(set.at(0), t)))
    {
        int const index_of_our_preferred_option{
            this->index_of_first_of(set, t)};

        if ( index_of_our_preferred_option == this->_size )
            return this->_size;
                //  No common option.

        int const index_of_their_preferred_option{
            set.index_of_first_of(this->ref(), t)};

        int const index_of_our_preferred_option_in_their_list{
            set.index_of_first(this->at(index_of_our_preferred_option), t)};

        int const index_of_their_preferred_option_in_our_list{
            this->index_of_first(set.at(index_of_their_preferred_option), t)};

        int const penalty_of_our_option{
            index_of_our_preferred_option
                + index_of_our_preferred_option_in_their_list};

        int const penalty_of_their_option{
            index_of_their_preferred_option
                + index_of_their_preferred_option_in_our_list};

        return  penalty_of_their_option < penalty_of_our_option
            ?   index_of_their_preferred_option_in_our_list
                    //  Prefer their option.
            :   index_of_our_preferred_option;
                    //  Prefer our option.
    }

    template <typename transform>
    int prioritized_index_of_first_of( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->prioritized_index_of_first_of<T, transform>(ali::move(set), t)))
    {
        return this->prioritized_index_of_first_of<T, transform>(ali::move(set), t);
    }

    template <typename U>
    int prioritized_index_of_first_of( array_const_ref<U> set ) const noexcept(
        noexcept(this->prioritized_index_of_first_of(ali::move(set), ali::identity)))
    {
        return this->prioritized_index_of_first_of(ali::move(set), ali::identity);
    }

    int prioritized_index_of_first_of( array_const_ref<T> set ) const noexcept(
        noexcept(this->prioritized_index_of_first_of<T>(ali::move(set))))
    {
        return this->prioritized_index_of_first_of<T>(ali::move(set));
    }

    //  index_of_first_not_of

    template <typename U, typename transform>
    int index_of_first_not_of( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(set.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result), t)
        //          &&  for all i in set {0 ... result - 1} &
        //                  set.contains(at(i), t) )
    {
        //if ( set.is_empty() )
        //    return 0;
                //  Any element is not a member of the empty set.
                //  (The later code handles this case correctly.)

        if ( this->is_empty() )
            return 0;

        int const i_max{this->_size - 1};

        int i{};

        for ( int j{set._size}; j != 0; )
            if ( --j, bool{t(this->at(i)) == t(set.at(j))} )
            {
                if ( i == i_max )
                    return this->_size;

                ++i;

                j = set._size;
            }

        return i;
    }

    template <typename transform>
    int index_of_first_not_of( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->index_of_first_not_of<T, transform>(ali::move(set), t)))
    {
        return this->index_of_first_not_of<T, transform>(ali::move(set), t);
    }

    template <typename U>
    int index_of_first_not_of( array_const_ref<U> set ) const noexcept(
        noexcept(this->index_of_first_not_of(ali::move(set), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result))
        //          &&  for all i in set {0 ... result - 1} &
        //                  set.contains(at(i)) )
    {
        return this->index_of_first_not_of(ali::move(set), ali::identity);
    }

    int index_of_first_not_of( array_const_ref<T> set ) const noexcept(
        noexcept(this->index_of_first_not_of<T>(ali::move(set))))
    {
        return this->index_of_first_not_of<T>(ali::move(set));
    }

    //  index_of_last_if

    template <typename predicate>
    int index_of_last_if( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   p(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !p(at(i)) )
    {
        for ( int i{this->_size}; i != 0; )
            if ( bool{p(this->at(--i))} )
                return i;

        return this->_size;
    }

    //  index_of_last_if_not

    template <typename predicate>
    int index_of_last_if_not( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   !p(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  p(at(i)) )
    {
        for ( int i{this->_size}; i != 0; )
            if ( !bool{p(this->at(--i))} )
                return i;

        return this->_size;
    }

    //  index_of_last

    template <typename transform>
    int index_of_last( read_only_T b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i{this->_size}; i != 0; )
            if ( --i, bool{t(this->at(i)) == t(b)} )
                return i;

        return this->_size;
    }

    int index_of_last( read_only_T b ) const noexcept(
        noexcept(this->index_of_last(b, ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_last(b, ali::identity);
    }

    template <typename U, typename transform>
    int index_of_last( U const& b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  post:   result == size()    //  not found
        //      ||  (   t(at(result)) == t(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(t(at(i)) == t(b)) )
    {
        for ( int i{this->_size}; i != 0; )
            if ( --i, bool{t(this->at(i)) == t(b)} )
                return i;

        return this->_size;
    }

    template <typename U, typename transform>
    int index_of_last_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) != t(b.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b, t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !ref(i, b.size()).is_equal_to(b, t) )
    {
        if ( b.is_empty() )
            return ali::maxi(1, this->_size) - 1;

        if ( this->_size < b._size )
            return this->_size;

        int i{this->_size - b._size};

        for ( int j{b._size}; j != 0; )
            if ( --j, bool{t(this->at(i + j)) != t(b.at(j))} )
            {
                if ( i == 0 )
                    return this->_size;

                --i;

                j = b._size;
            }

        return i;
    }

    template <typename transform>
    int index_of_last_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->index_of_last_n<T, transform>(ali::move(b), t)))
    {
        return this->index_of_last_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    int index_of_last( U const& b ) const noexcept(
        noexcept(this->index_of_last(b, ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   at(result) == b
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !(at(i) == b) )
    {
        return this->index_of_last(b, ali::identity);
    }

    template <typename U>
    int index_of_last_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->index_of_last_n(ali::move(b), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   ref(result, b.size()).is_equal_to(b)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !ref(i, b.size()).is_equal_to(b) )
    {
        return this->index_of_last_n(ali::move(b), ali::identity);
    }

    int index_of_last_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->index_of_last_n<T>(ali::move(b))))
    {
        return this->index_of_last_n<T>(ali::move(b));
    }

    //  index_of_last_of

    template <typename U, typename transform>
    int index_of_last_of( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(set.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result), t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !set.contains(at(i), t) )
    {
        if ( !set.is_empty() )
            for ( int i{this->_size}; i != 0; )
            {
                --i;
                for ( int j{set._size}; j != 0; )
                    if ( --j, bool{t(this->at(i)) == t(set.at(j))} )
                        return i;
            }

        return this->_size;
    }

    template <typename transform>
    int index_of_last_of( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->index_of_last_of<T, transform>(ali::move(set), t)))
    {
        return this->index_of_last_of<T, transform>(ali::move(set), t);
    }

    template <typename U>
    int index_of_last_of( array_const_ref<U> set ) const noexcept(
        noexcept(this->index_of_last_of(ali::move(set), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   set.contains(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  !set.contains(at(i)) )
    {
        return this->index_of_last_of(ali::move(set), ali::identity);
    }

    int index_of_last_of( array_const_ref<T> set ) const noexcept(
        noexcept(this->index_of_last_of<T>(ali::move(set))))
    {
        return this->index_of_last_of<T>(ali::move(set));
    }

    //  index_of_last_not_of

    template <typename U, typename transform>
    int index_of_last_not_of( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(set.at(0))}))
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result), t)
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  set.contains(at(i), t) )
    {
        //if ( set.is_empty() )
        //    return this->_size - 1;
                //  Any element is not a member of the empty set.
                //  (The later code handles this case correctly.)

        if ( this->is_empty() )
            return this->_size;

        int i{this->_size - 1};

        for ( int j{set._size}; j != 0; )
            if ( --j, bool{t(this->at(i)) == t(set.at(j))} )
            {
                if ( i == 0 )
                    return this->_size;

                --i;

                j = set._size;
            }

        return i;
    }

    template <typename transform>
    int index_of_last_not_of( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->index_of_last_not_of<T, transform>(ali::move(set), t)))
    {
        return this->index_of_last_not_of<T, transform>(ali::move(set), t);
    }

    template <typename U>
    int index_of_last_not_of( array_const_ref<U> set ) const noexcept(
        noexcept(this->index_of_last_not_of(ali::move(set), ali::identity)))
        //  post:   result == size()    //  not found
        //      ||  (   !set.contains(at(result))
        //          &&  for all i in set {result + 1 ... size() - 1} &
        //                  set.contains(at(i)) )
    {
        return this->index_of_last_not_of(ali::move(set), ali::identity);
    }

    int index_of_last_not_of( array_const_ref<T> set ) const noexcept(
        noexcept(this->index_of_last_not_of<T>(ali::move(set))))
    {
        return this->index_of_last_not_of<T>(ali::move(set));
    }

    //  index_of_min

    template <typename comparator>
    int index_of_min( comparator&& compare ) const noexcept(
        noexcept(int{compare(this->at(0), this->at(0))}))
    {
        int result{this->_size};

        if ( result != 0 )
            for ( int i{--result}; i != 0; )
                if ( int{compare(this->at(--i), this->at(result))} <= 0 )
                    result = i;

        return result;
    }

    int index_of_min( void ) const noexcept(
        noexcept(this->index_of_min(ali::default_comparator{})))
    {
        return this->index_of_min(ali::default_comparator{});
    }

    //  index_of_max

    template <typename comparator>
    int index_of_max( comparator&& compare ) const noexcept(
        noexcept(int{compare(this->at(0), this->at(0))}))
    {
        int result{this->_size};

        if ( result != 0 )
            for ( int i{--result}; i != 0; )
                if ( int{compare(this->at(--i), this->at(result))} > 0 )
                    result = i;

        return result;
    }

    int index_of_max( void ) const noexcept(
        noexcept(this->index_of_max(ali::default_comparator{})))
    {
        return this->index_of_max(ali::default_comparator{});
    }

    //  index_of_min_max

    template <typename comparator>
    pair<int, int> index_of_min_max( comparator&& compare ) const noexcept(
        noexcept(int{compare(this->at(0), this->at(0))}))
        //  Note: This is more efficient than calling
        //  index_of_min and index_of_max separately.
    {
        int idx{this->_size};

        if ( idx <= 1 )
            return make_pair(0, 0);

        int i_max{--idx};
        int i_min{--idx};

        if ( int{compare(this->at(i_min), this->at(i_max))} > 0 )
            ali::adl_swap(i_min, i_max);

        ali_assert(
                int{compare(this->at(i_min), this->at(i_max))} < 0
            ||  (   int{compare(this->at(i_min), this->at(i_max))} == 0
                &&  i_min < i_max));

        if ( idx % 2 == 1 )
        {
            --idx;

            if ( int{compare(this->at(idx), this->at(i_min))} <= 0 )
                i_min = idx;

            if ( int{compare(this->at(idx), this->at(i_max))} > 0 )
                i_max = idx;
        }

        while ( idx != 0 )
        {
            ali_assert(idx % 2 == 0);

            int potential_max{--idx};
            int potential_min{--idx};

            if ( int{compare(this->at(potential_min), this->at(potential_max))} > 0 )
                ali::adl_swap(potential_min, potential_max);

            if ( int{compare(this->at(potential_min), this->at(i_min))} <= 0 )
                i_min = potential_min;

            if ( int{compare(this->at(potential_max), this->at(i_max))} > 0 )
                i_max = potential_max;
        }

        return make_pair(i_min, i_max);
    }

    pair<int, int> index_of_min_max( void ) const noexcept(
        noexcept(this->index_of_min_max(ali::default_comparator{})))
    {
        return this->index_of_min_max(ali::default_comparator{});
    }

    //  index_of_difference
    
    template <typename U, typename transform>
    int index_of_difference( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) != t(b.at(0))}))
        //  post:       ref_left(result).is_equal_to(b.ref_left(result), t)
        //          &&  (   result == size()
        //              ||  result == b.size()
        //              ||  t(at(result)) != t(b.at(result))
    {
        int const n{ali::mini(this->size(), b.size())};
        
        int i{};
        
        for ( ; i != n; ++i )
            if ( bool{t(this->at(i)) != t(b.at(i))} )
                break;
        
        return i;
    }

    template <typename transform>
    int index_of_difference( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->index_of_difference<T, transform>(ali::move(b), t)))
    {
        return this->index_of_difference<T, transform>(ali::move(b), t);
    }

    template <typename U>
    int index_of_difference( array_const_ref<U> b ) const noexcept(
        noexcept(this->index_of_difference(ali::move(b), ali::identity)))
    {
        return this->index_of_difference(ali::move(b), ali::identity);
    }

    int index_of_difference( array_const_ref<T> b ) const noexcept(
        noexcept(this->index_of_difference(ali::move(b), ali::identity)))
    {
        return this->index_of_difference(ali::move(b), ali::identity);
    }

    //  all

    template <typename predicate>
    bool all( predicate&& p ) const noexcept(
        noexcept(this->index_of_last_if_not(ali::forward<predicate>(p))))
        //  Returns is_empty() || (p(at(0)) && ... && p(at(size() - 1)))
    {
        return this->index_of_last_if_not(
            ali::forward<predicate>(p)) == this->_size;
    }

    //  any

    template <typename predicate>
    bool any( predicate&& p ) const noexcept(
        noexcept(this->index_of_last_if(ali::forward<predicate>(p))))
        //  Returns !is_empty() && (p(at(0)) || ... || p(at(size() - 1)))
    {
        return this->index_of_last_if(
            ali::forward<predicate>(p)) != this->_size;
    }

    //  none

    template <typename predicate>
    bool none( predicate&& p ) const noexcept(
        noexcept(this->index_of_last_if(ali::forward<predicate>(p))))
        //  Returns is_empty() || (!p(at(0)) && ... && !p(at(size() - 1)))
    {
        return this->index_of_last_if(
            ali::forward<predicate>(p)) == this->_size;
    }

    //  contains

    template <typename transform>
    bool contains( read_only_T b, transform t ) const noexcept(
        noexcept(this->index_of_last(b, t)))
        //  Returns index_of(b, t) != size()
    {
        return this->index_of_last(b, t) != this->_size;
    }

    bool contains( read_only_T b ) const noexcept(
        noexcept(this->contains(b, ali::identity)))
        //  Returns index_of(b) != size()
    {
        return this->contains(b, ali::identity);
    }

    template <typename U, typename transform>
    bool contains( U const& b, transform t ) const noexcept(
        noexcept(this->index_of_last(b, t)))
        //  Returns index_of(b, t) != size()
    {
        return this->index_of_last(b, t) != this->_size;
    }

    template <typename U, typename transform>
    bool contains_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(this->index_of_last_n(ali::move(b), t)))
        //  Returns index_of_n(b, t) != size()
    {
        return this->index_of_last_n(ali::move(b), t) != this->_size;
    }

    template <typename transform>
    bool contains_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->contains_n<T, transform>(ali::move(b), t)))
    {
        return this->contains_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    bool contains( U const& b ) const noexcept(
        noexcept(this->contains(b, ali::identity)))
        //  Returns index_of(b) != size()
    {
        return this->contains(b, ali::identity);
    }

    template <typename U>
    bool contains_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->contains_n(ali::move(b), ali::identity)))
        //  Returns index_of_n(b) != size()
    {
        return this->contains_n(ali::move(b), ali::identity);
    }

    bool contains_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->contains_n<T>(ali::move(b))))
    {
        return this->contains_n<T>(ali::move(b));
    }

    //  contains_pair

    template <typename predicate>
    bool contains_pair( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0), this->at(0))}))
        //  Note: This method examines all the possible size * (size - 1) / 2 pairs.
    {
        for ( int i{this->_size}; i > 1; )
        {
            --i;
            ali_assert(i > 0);

            for ( int j{i}; j != 0; )
                if ( bool{p(this->at(i), this->at(--j))} )
                    return true;
        }
        return false;
    }

    //  contains_duplicates

    bool contains_duplicates( void ) const noexcept(
        noexcept(meta::make<T const&>() == meta::make<T const&>()))
    {
        return this->contains_pair(
            [] ( T const& a, T const& b ) noexcept(noexcept(a == b))
            { return a == b; });
    }

    //  trim_left_if

    template <typename predicate>
    array_const_ref<T> trim_left_if( predicate&& p ) const noexcept(
        noexcept(this->ref_right(this->index_of_first_if_not(
            ali::forward<predicate>(p)))))
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())
    {
        return this->ref_right(this->index_of_first_if_not(
            ali::forward<predicate>(p)));
    }


    //  trim_left

    template <typename U, typename transform>
    array_const_ref<T> trim_left( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(this->ref_right(this->index_of_first_not_of(ali::move(set), t))))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)
    {
        return this->ref_right(this->index_of_first_not_of(ali::move(set), t));
    }


    template <typename transform>
    array_const_ref<T> trim_left( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->trim_left<T, transform>(ali::move(set), t)))
    {
        return this->trim_left<T, transform>(ali::move(set), t);
    }


    template <typename U>
    array_const_ref<T> trim_left( array_const_ref<U> set ) const noexcept(
        noexcept(this->trim_left(ali::move(set), ali::identity)))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())
    {
        return this->trim_left(ali::move(set), ali::identity);
    }


    array_const_ref<T> trim_left( array_const_ref<T> set ) const noexcept(
        noexcept(this->trim_left<T>(ali::move(set))))
    {
        return this->trim_left<T>(ali::move(set));
    }


    //  trim_right_if

    template <typename predicate>
    array_const_ref<T> trim_right_if( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
        //  post:   result.is_empty()
        //      ||  !predicate(result.back())
    {
        int trimmed_size{this->_size};

        while ( trimmed_size != 0 && bool{p(this->at(trimmed_size - 1))} )
            --trimmed_size;

        return this->ref_front(trimmed_size);
    }


    //  trim_right

    template <typename U, typename transform>
    array_const_ref<T> trim_right( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(set.contains(this->at(0), t)))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.back(), t)
    {
        int trimmed_size{this->_size};

        if ( !set.is_empty() )
            while ( trimmed_size != 0 && set.contains(this->at(trimmed_size - 1), t) )
                --trimmed_size;

        return this->ref_front(trimmed_size);
    }


    template <typename transform>
    array_const_ref<T> trim_right( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->trim_right<T, transform>(ali::move(set), t)))
    {
        return this->trim_right<T, transform>(ali::move(set), t);
    }


    template <typename U>
    array_const_ref<T> trim_right( array_const_ref<U> set ) const noexcept(
        noexcept(this->trim_right(ali::move(set), ali::identity)))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.back())
    {
        return this->trim_right(ali::move(set), ali::identity);
    }


    array_const_ref<T> trim_right( array_const_ref<T> set ) const noexcept(
        noexcept(this->trim_right<T>(ali::move(set))))
    {
        return this->trim_right<T>(ali::move(set));
    }


    //  trim_if

    template <typename predicate>
    array_const_ref<T> trim_if( predicate&& p ) const noexcept(
        noexcept(this->trim_left_if(p).trim_right_if(p)))
        //  post:   result.is_empty()
        //      ||  (   !p(result.front())
        //          &&  !p(result.back()))
    {
        return this->trim_left_if(p).trim_right_if(p);
    }


    //  trim

    template <typename U, typename transform>
    array_const_ref<T> trim( array_const_ref<U> set, transform t ) const noexcept(
        noexcept(this->trim_left(set, t).trim_right(set, t)))
        //  post:   result.is_empty()
        //      ||  (   !set.contains(result.front(), t)
        //          &&  !set.contains(result.back(), t))
    {
        return this->trim_left(set, t).trim_right(set, t);
    }


    template <typename transform>
    array_const_ref<T> trim( array_const_ref<T> set, transform t ) const noexcept(
        noexcept(this->trim<T, transform>(ali::move(set), t)))
    {
        return this->trim<T, transform>(ali::move(set), t);
    }


    template <typename U>
    array_const_ref<T> trim( array_const_ref<U> set ) const noexcept(
        noexcept(this->trim(ali::move(set), ali::identity)))
        //  post:   result.is_empty()
        //      ||  (   !set.contains(result.front())
        //          &&  !set.contains(result.back()))
    {
        return this->trim(ali::move(set), ali::identity);
    }


    array_const_ref<T> trim( array_const_ref<T> set ) const noexcept(
        noexcept(this->trim<T>(ali::move(set))))
    {
        return this->trim<T>(ali::move(set));
    }


    //  common_prefix

    template <typename U, typename transform>
    array_const_ref<T> common_prefix( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(this->ref_left(this->index_of_difference(ali::move(b), t))))
    {
        return this->ref_left(this->index_of_difference(ali::move(b), t));
    }

    template <typename transform>
    array_const_ref<T> common_prefix( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->ref_left(this->index_of_difference(ali::move(b), t))))
    {
        return this->ref_left(this->index_of_difference(ali::move(b), t));
    }

    template <typename U>
    array_const_ref<T> common_prefix( array_const_ref<U> b ) const noexcept(
        noexcept(this->ref_left(this->index_of_difference(ali::move(b)))))
    {
        return this->ref_left(this->index_of_difference(ali::move(b)));
    }

    array_const_ref<T> common_prefix( array_const_ref<T> b ) const noexcept(
        noexcept(this->ref_left(this->index_of_difference(ali::move(b)))))
    {
        return this->ref_left(this->index_of_difference(ali::move(b)));
    }


    //  count

    template <typename transform>
    int count( read_only_T b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
    {
        int n{};

        for ( int i{this->_size}; i != 0; )
            if ( --i, bool{t(this->at(i)) == t(b)} )
                ++n;

        return n;
    }

    int count( read_only_T b ) const noexcept(
        noexcept(this->count(b, ali::identity)))
    {
        return this->count(b, ali::identity);
    }

    template <typename U, typename transform>
    int count( U const& b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  Returns the number of elements that compare equal to b (after transform t).
    {
        int n{};

        for ( int i{this->_size}; i != 0; )
            if ( --i, bool{t(this->at(i)) == t(b)} )
                ++n;

        return n;
    }

    template <typename U, typename transform>
    int count_n( array_const_ref<U> b, transform t ) const noexcept(
        noexcept(this->pointer()->index_of_first_n(b, t)))
        //  Returns the number of non-overlapping occurrences of b inside this array.
    {
        int n{};

        array_const_ptr<T> ptr{this->pointer()};

        while ( !(ptr += ptr->index_of_first_n(b, t))->is_empty() )
            ++n, ptr += b.size();

        return n;
    }


    template <typename transform>
    int count_n( array_const_ref<T> b, transform t ) const noexcept(
        noexcept(this->count_n<T, transform>(ali::move(b), t)))
    {
        return this->count_n<T, transform>(ali::move(b), t);
    }

    template <typename U>
    int count( U const& b ) const noexcept(
        noexcept(this->count(b, ali::identity)))
        //  Returns the number of elements that compare equal to b.
    {
        return this->count(b, ali::identity);
    }

    template <typename U>
    int count_n( array_const_ref<U> b ) const noexcept(
        noexcept(this->count_n(ali::move(b), ali::identity)))
        //  Returns the number of non-overlapping occurrences of b inside this array.
    {
        return this->count_n(ali::move(b), ali::identity);
    }

    int count_n( array_const_ref<T> b ) const noexcept(
        noexcept(this->count_n<T>(ali::move(b))))
    {
        return this->count_n<T>(ali::move(b));
    }

    //  count_if

    template <typename predicate>
    int count_if( predicate&& p ) const noexcept(
        noexcept(bool{p(this->at(0))}))
    {
        int n{};

        for ( int i{this->_size}; i != 0; )
            if ( --i, bool{p(this->at(i))} )
                ++n;

        return n;
    }

    //  is_sorted

    template <typename comparator>
    bool is_sorted( comparator&& compare ) const noexcept(
        noexcept(int{compare(this->at(0), this->at(0))}))
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i{this->_size - 1}; i != 0; )
        {
            --i;

            int const result{compare(
                this->at(i), this->at(i + 1))};

            if ( result > 0 )
                return false;
        }

        return true;
    }

    bool is_sorted( void ) const noexcept(
        noexcept(this->is_sorted(ali::default_comparator{})))
    {
        return this->is_sorted(ali::default_comparator{});
    }

    //  is_heap

    template <typename comparator>
    bool is_heap( comparator&& compare ) const noexcept(
        noexcept(int{compare(this->at(0), this->at(0))}))
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i{this->_size}; i != 1; )
        {
            --i;

            int const result{compare(
                this->at(i),                //  Child
                this->at((i - 1) / 2))};    //  Parent

            if ( result > 0 )
                return false;
        }

        return true;
    }

    bool is_heap( void ) const noexcept(
        noexcept(this->is_heap(ali::default_comparator{})))
    {
        return this->is_heap(ali::default_comparator{});
    }

    //  is_wildcard_match

    template <typename U, typename transform>
    bool is_wildcard_match( array_const_ref<U> pattern, U qm, U star, transform t ) const noexcept;

    template <typename transform>
    bool is_wildcard_match( array_const_ref<T> pattern, T qm, T star, transform t ) const noexcept
    {
        return this->is_wildcard_match<T, transform>(ali::move(pattern), qm, star, t);
    }

    template <typename U>
    bool is_wildcard_match( array_const_ref<U> pattern, U qm, U star ) const noexcept
    {
        return this->is_wildcard_match(ali::move(pattern), qm, star, ali::identity);
    }

    bool is_wildcard_match( array_const_ref<T> pattern, T qm, T star ) const noexcept
    {
        return this->is_wildcard_match<T>(ali::move(pattern), qm, star);
    }

    //  misc

    array_const_ref<ali::uint8> as_blob( void ) const noexcept;

    is_internal_element_result
        is_internal_element( T const& a ) const noexcept
            //  post:   (0 <= result && result < this->size())
            //      ||  result < 0
    {
        bool const is_internal{
                this->_begin != nullptr
            &&  this->_begin <= &a
            &&  &a < this->_begin + this->_size};
        
        return {is_internal ? static_cast<int>(&a - this->_begin) : -1};
    }

    is_internal_pointer_result
        is_internal_pointer( T const* a ) const noexcept
            //  post:   (0 <= result && result <= this->size())
            //      ||  result < 0
            //  Note: Pointer to one past the last
            //  element is also internal pointer.
    {
        bool const is_internal{
                this->_begin != nullptr
            &&  this->_begin <= a
            &&  a <= this->_begin + this->_size};

        return {is_internal ? static_cast<int>(a - this->_begin) : -1};
    }

    is_internal_pointer_result
        is_internal_pointer( array_const_ref<T> a ) const noexcept
    {
        return this->is_internal_pointer(a.data());
    }

    is_internal_pointer_result
        is_internal_pointer( array_const_ptr<T> a ) const noexcept
    {
        return this->is_internal_pointer(a->data());
    }

protected:  //  Methods
    constexpr array_const_ref_common( void ) = default;
    
    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_const_ref_common( array_const_ref_common const& ) = default;

    array_const_ref_common( array_const_ref_common&& b ) noexcept
    :   _begin{b._begin},
        _size{b._size}
        ALI_IF_DEBUG_WEAK_REF(, _wref{ali::move(b._wref)})
    {
        b._begin = nullptr;
        b._size = 0;
        //  b._wref has been moved above.
    }

    constexpr array_const_ref_common( T const* begin, int size ) noexcept
    :   _begin{begin},
        _size{size}
    {
        ali_constexpr_assert(0 <= this->_size);
        ali_constexpr_assert(this->_size == 0 || this->_begin != nullptr);
    }

ALI_IF_DEBUG_WEAK_REF(
    array_const_ref_common(
        debug::weak_reference const& wref,
        T const* begin, int size ) noexcept
    :   _begin{begin},
        _size{size},
        _wref{wref}
    {
        ali_assert(0 <= this->_size);
        ali_assert(this->_size == 0 || this->_begin != nullptr);
    }

)   //  ALI_IF_DEBUG_WEAK_REF

private:    //  Methods
    array_const_ref_common& operator=(
        array_const_ref_common const& ) = delete;

    template <typename U>
    bool _is_equal_to(
        array_const_ref<U> b,
        meta::define_bool_result<true> /* use mem_cmp */ ) const noexcept
    {
        return  this->_size == b._size
            &&  (   this->_size == 0
                ||  platform::memcmp(
                        this->_begin,
                        b._begin,
                        this->_size * sizeof(T)) == 0);
    }

    template <typename U>
    bool _is_equal_to(
        array_const_ref<U> b,
        meta::define_bool_result<false> /* use mem_cmp */ ) const noexcept(
            noexcept(this->is_equal_to<
                U, functor_types::identity>(
                    ali::move(b), ali::identity)))
    {
        return this->is_equal_to<
            U, functor_types::identity>(
                ali::move(b), ali::identity);
    }

private:    //  Data members
    T const*                        _begin{};
    int                             _size{};
ALI_IF_DEBUG_WEAK_REF(
    mutable debug::weak_reference   _wref{};
)

    template <typename U>
    friend class array_const_ref_common;
    template <typename U>
    friend class array_const_ref;
    friend class c_string_const_ref;
    friend class c_wstring_const_ref;
    template <typename U>
    friend class array_ref_common;
    template <typename U>
    friend class array_ref;
    template <typename U>
    friend class array_const_ptr;
    friend class c_string_const_ptr;
    friend class c_wstring_const_ptr;
    template <typename U>
    friend class array_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_const_ref
    : public array_const_ref_common<T>
// ******************************************************************
{
public:
    constexpr array_const_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_const_ref( array_const_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_const_ref( array_const_ref&& ) = default;

    template <typename D>
    array_const_ref( exposes_array_const_ref<T, D> const& b ) noexcept
    :   array_const_ref{b.ref()}
    {}

    constexpr array_const_ref( T const* begin, int size ) noexcept
    :   array_const_ref_common<T>{begin, size}
    {}

    template <int n>
    constexpr array_const_ref( T const (&begin)[n] ) noexcept
    :   array_const_ref_common<T>{begin, n}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ref(
        debug::weak_reference const& wref,
        T const* begin, int size ) noexcept
    :   array_const_ref_common<T>{wref, begin, size}
    {}

    template <int n>
    array_const_ref(
        debug::weak_reference const& wref,
        T const (&begin)[n] ) noexcept
    :   array_const_ref_common<T>{wref, begin, n}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<ali::uint8>
    : public array_const_ref_common<ali::uint8>
// ******************************************************************
//  A.k.a. blob_const_ref.
// ******************************************************************
{
public:
    constexpr array_const_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_const_ref( array_const_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_const_ref( array_const_ref&& ) = default;

    template <typename D>
    array_const_ref( exposes_blob_const_ref<D> const& b ) noexcept
    :   array_const_ref{b.ref()}
    {}
    
    constexpr array_const_ref( ali::uint8 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{begin, size}
    {}

    array_const_ref( ali::uint16 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * size}
    {}

    array_const_ref( ali::uint32 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * size}
    {}

    template <int n>
    constexpr array_const_ref( ali::uint8 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{begin, n}
    {}

    template <int n>
    explicit array_const_ref( ali::uint16 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * n}
    {}

    template <int n>
    explicit array_const_ref( ali::uint32 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * n}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint8 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{wref, begin, size}
    {}

    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint16 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * size}
    {}

    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint32 const* begin, int size ) noexcept
    :   array_const_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * size}
    {}

    template <int n>
    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint8 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{wref, begin, n}
    {}

    template <int n>
    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint16 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8 const*>(begin),
            2 * n}
    {}

    template <int n>
    array_const_ref(
        debug::weak_reference const& wref,
        ali::uint32 const (&begin)[n] ) noexcept
    :   array_const_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8 const*>(begin),
            4 * n}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    //  int

    template <typename integer_type>
    integer_type int_be_at_( int pos, int byte_count ) const noexcept;
        // pre: byte_count in set {1 ... sizeof(integer_type)}

    template <typename integer_type>
    integer_type int_le_at_( int pos, int byte_count ) const noexcept;
        // pre: byte_count in set {1 ... sizeof(integer_type)}

    ali::uint32 int_be_at( int pos, int byte_count ) const noexcept;
        // pre: byte_count in set {1, 2, 3, 4}

    ali::uint32 int_le_at( int pos, int byte_count ) const noexcept;
        // pre: byte_count in set {1, 2, 3, 4}

    ali::uint8 int8_at( int pos ) const noexcept
    {
        return this->at(pos);
    }

    ali::uint16 int16_be_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 2);

        return  static_cast<ali::uint16>(
                (static_cast<ali::uint32>(this->at(pos    )) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos + 1))      ));
    }

    ali::uint16 int16_le_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 2);

        return  static_cast<ali::uint16>(
                (static_cast<ali::uint32>(this->at(pos + 1)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos    ))      ));
    }

    ali::uint16 int16_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return to_native_from_little_endian(int16_le_at(pos));
    }

    ali::uint32 int32_be_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 4);

        return  (static_cast<ali::uint32>(this->at(pos    )) << 24)
            |   (static_cast<ali::uint32>(this->at(pos + 1)) << 16)
            |   (static_cast<ali::uint32>(this->at(pos + 2)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos + 3))      );
    }

    ali::uint32 int32_le_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 4);

        return  (static_cast<ali::uint32>(this->at(pos + 3)) << 24)
            |   (static_cast<ali::uint32>(this->at(pos + 2)) << 16)
            |   (static_cast<ali::uint32>(this->at(pos + 1)) <<  8)
            |   (static_cast<ali::uint32>(this->at(pos    ))      );
    }

    ali::uint32 int32_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return to_native_from_little_endian(int32_le_at(pos));
    }

    ali::uint64 int64_be_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 8);

        return  (static_cast<ali::uint64>(this->int32_be_at(pos    )) << 32)
            |   (static_cast<ali::uint64>(this->int32_be_at(pos + 4))      );
    }

    ali::uint64 int64_le_at( int pos ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 8);

        return  (static_cast<ali::uint64>(this->int32_le_at(pos + 4)) << 32)
            |   (static_cast<ali::uint64>(this->int32_le_at(pos    ))      );
    }

    ali::uint64 int64_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return to_native_from_little_endian(int64_le_at(pos));
    }

    array_const_ref<char> as_string( void ) const noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<char>
    : public array_const_ref_common<char>
// ******************************************************************
//  A.k.a. string_const_ref.
// ******************************************************************
{
public:
    constexpr array_const_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_const_ref( array_const_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_const_ref( array_const_ref&& ) = default;
    
    //  We need to include move ctor overload
    //  for c_string_const_ref because its default
    //  constructed state is a bit different.
    array_const_ref( c_string_const_ref&& b ) noexcept;

    template <typename D>
    array_const_ref( exposes_string_const_ref<D> const& b ) noexcept
    :   array_const_ref{b.ref()}
    {}

    constexpr array_const_ref( char const* begin, int size ) noexcept
    :   array_const_ref_common<char>{begin, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ref(
        debug::weak_reference const& wref,
        char const* begin, int size ) noexcept
    :   array_const_ref_common<char>{wref, begin, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    bool nocase_is_equal_to( array_const_ref b ) const noexcept;

    int nocase_compare( array_const_ref b ) const noexcept;

    bool nocase_begins_with( char b ) const noexcept;

    bool nocase_begins_with_n( array_const_ref b ) const noexcept;

    bool nocase_ends_with( char b ) const noexcept;

    bool nocase_ends_with_n( array_const_ref b ) const noexcept;

    int nocase_index_of_first( char b ) const noexcept;

    int nocase_index_of_first_n( array_const_ref b ) const noexcept;

    int nocase_index_of_first_of( array_const_ref set ) const noexcept;

    int nocase_index_of_first_not_of( array_const_ref set ) const noexcept;

    //  index_of_first_unquoted

    template <typename transform>
    int index_of_first_unquoted( char b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  t(at(result)) = t(b)
    {
        b = t(b);

        bool quoted{false};

        for ( int i{}; i != this->_size; ++i )
        {
            if ( !quoted && bool{t(this->at(i)) == b} )
                return i;

            bool const quote{
                    this->at(i) == '\"'
                &&  (!quoted || this->at(i - 1) != '\\')};
                        //  quoted implies i > 0 and i - 1 >= 0

            //  if ( quote )
            //      quoted = !quoted;
            //
            //  pre(quoted) | quote | post(quoted)
            //      F       |   F   |   F
            //      F       |   T   |   T
            //      T       |   F   |   T
            //      T       |   T   |   F
            //
            //  XOR

            quoted = quoted != quote;
        }

        return this->_size;
    }

    int index_of_first_unquoted( char b ) const noexcept(
        noexcept(this->index_of_first_unquoted(b, ali::identity)))
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  at(result) = b
    {
        return this->index_of_first_unquoted(b, ali::identity);
    }

    int nocase_index_of_first_unquoted( char b ) const noexcept;
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  to_lower(at(result)) = to_lower(b)

    int nocase_index_of_last( char b ) const noexcept;

    int nocase_index_of_last_n( array_const_ref b ) const noexcept;

    int nocase_index_of_last_of( array_const_ref set ) const noexcept;

    int nocase_index_of_last_not_of( array_const_ref set ) const noexcept;

    bool nocase_contains( char b ) const noexcept;

    bool nocase_contains_n( array_const_ref b ) const noexcept;

    int nocase_count( char b ) const noexcept;

    int nocase_count_n( array_const_ref b ) const noexcept;

    //  is_wildcard_match
    
    using array_const_ref_common<char>::is_wildcard_match;

    template <typename transform>
    bool is_wildcard_match( array_const_ref pattern, transform t ) const noexcept
    {
        return this->is_wildcard_match(ali::move(pattern), '?', '*', t);
    }

    bool is_wildcard_match( array_const_ref pattern ) const noexcept
    {
        return this->is_wildcard_match(ali::move(pattern), ali::identity);
    }

    bool nocase_is_wildcard_match( array_const_ref pattern ) const noexcept;

    //  trim_left

    using array_const_ref_common<char>::trim_left;

    array_const_ref trim_left( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())

    array_const_ref nocase_trim_left( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !charset.nocase_contains(result.front())

    //  trim_right

    using array_const_ref_common<char>::trim_right;

    array_const_ref trim_right( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !is_space(result.back())

    array_const_ref nocase_trim_right( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.back())

    //  trim

    using array_const_ref_common<char>::trim;

    array_const_ref trim( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  (   !is_space(result.front())
        //          &&  !is_space(result.back()))

    array_const_ref nocase_trim( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  (   !set.nocase_contains(result.front())
        //          &&  !set.nocase_contains(result.back()))

    //  is_internal_pointer

    using array_const_ref_common<char>::is_internal_pointer;

    is_internal_pointer_result
        is_internal_pointer( c_string_const_ptr a ) const noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ref<ali::wchar>
    : public array_const_ref_common<ali::wchar>
// ******************************************************************
//  A.k.a. wstring_const_ref.
// ******************************************************************
{
public:
    constexpr array_const_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_const_ref( array_const_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_const_ref( array_const_ref&& ) = default;
    
    //  We need to include move ctor overload
    //  for c_wstring_const_ref because its default
    //  constructed state is a bit different.
    array_const_ref( c_wstring_const_ref&& b ) noexcept;

    template <typename D>
    array_const_ref( exposes_wstring_const_ref<D> const& b ) noexcept
    :   array_const_ref{b.ref()}
    {}

    constexpr array_const_ref( ali::wchar const* begin, int size ) noexcept
    :   array_const_ref_common<ali::wchar>{begin, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ref(
        debug::weak_reference const& wref,
        ali::wchar const* begin, int size ) noexcept
    :   array_const_ref_common<ali::wchar>{wref, begin, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    bool nocase_is_equal_to( array_const_ref b ) const noexcept;

    int nocase_compare( array_const_ref b ) const noexcept;

    bool nocase_begins_with( ali::wchar b ) const noexcept;

    bool nocase_begins_with_n( array_const_ref b ) const noexcept;

    bool nocase_ends_with( ali::wchar b ) const noexcept;

    bool nocase_ends_with_n( array_const_ref b ) const noexcept;

    int nocase_index_of_first( ali::wchar b ) const noexcept;

    int nocase_index_of_first_n( array_const_ref b ) const noexcept;

    int nocase_index_of_first_of( array_const_ref set ) const noexcept;

    int nocase_index_of_first_not_of( array_const_ref set ) const noexcept;

    //  index_of_first_unquoted

    template <typename transform>
    int index_of_first_unquoted( ali::wchar b, transform t ) const noexcept(
        noexcept(bool{t(this->at(0)) == t(b)}))
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  t(at(result)) = t(b)
    {
        b = t(b);

        bool quoted{false};

        for ( int i = 0; i != this->_size; ++i )
        {
            if ( !quoted && bool{t(this->at(i)) == b} )
                return i;

            bool const quote{
                    this->at(i) == L'\"'
                &&  (!quoted || this->at(i - 1) != L'\\')};
                        //  quoted implies i > 0 and i - 1 >= 0

            //  if ( quote )
            //      quoted = !quoted;
            //
            //  pre(quoted) | quote | post(quoted)
            //      F       |   F   |   F
            //      F       |   T   |   T
            //      T       |   F   |   T
            //      T       |   T   |   F
            //
            //  XOR

            quoted = quoted != quote;
        }

        return this->_size;
    }

    int index_of_first_unquoted( ali::wchar b ) const noexcept(
        noexcept(this->index_of_first_unquoted(b, ali::identity)))
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  at(result) = b
    {
        return this->index_of_first_unquoted(b, ali::identity);
    }

    int nocase_index_of_first_unquoted( ali::wchar b ) const noexcept;
        //  Returns index of the first b outside of quotation marks.
        //  post:   result == size()    //  not found
        //      ||  to_lower(at(result)) = to_lower(b)

    int nocase_index_of_last( ali::wchar b ) const noexcept;

    int nocase_index_of_last_n( array_const_ref b ) const noexcept;

    int nocase_index_of_last_of( array_const_ref set ) const noexcept;

    int nocase_index_of_last_not_of( array_const_ref set ) const noexcept;

    bool nocase_contains( ali::wchar b ) const noexcept;

    bool nocase_contains_n( array_const_ref b ) const noexcept;

    int nocase_count( ali::wchar b ) const noexcept;

    int nocase_count_n( array_const_ref b ) const noexcept;

    //  is_wildcard_match
    
    using array_const_ref_common<ali::wchar>::is_wildcard_match;

    template <typename transform>
    bool is_wildcard_match( array_const_ref pattern, transform t ) const noexcept
    {
        return this->is_wildcard_match(ali::move(pattern), L'?', L'*', t);
    }

    bool is_wildcard_match( array_const_ref pattern ) const noexcept
    {
        return this->is_wildcard_match(ali::move(pattern), ali::identity);
    }

    bool nocase_is_wildcard_match( array_const_ref pattern ) const noexcept;

    //  trim_left

    using array_const_ref_common<ali::wchar>::trim_left;

    array_const_ref trim_left( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())

    array_const_ref nocase_trim_left( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !charset.nocase_contains(result.front())

    //  trim_right

    using array_const_ref_common<ali::wchar>::trim_right;

    array_const_ref trim_right( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !is_space(result.back())

    array_const_ref nocase_trim_right( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.back())

    //  trim

    using array_const_ref_common<ali::wchar>::trim;

    array_const_ref trim( void ) const noexcept;
        //  post:   result.is_empty()
        //      ||  (   !is_space(result.front())
        //          &&  !is_space(result.back()))

    array_const_ref nocase_trim( array_const_ref set ) const noexcept;
        //  post:   result.is_empty()
        //      ||  (   !set.nocase_contains(result.front())
        //          &&  !set.nocase_contains(result.back()))

    //  is_internal_pointer

    using array_const_ref_common<ali::wchar>::is_internal_pointer;

    is_internal_pointer_result
        is_internal_pointer( c_wstring_const_ptr a ) const noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ref_common
    : public array_const_ref<T>
// ******************************************************************
{
    using read_only_T = typename array_const_ref<T>::read_only_T;

public:
    array_ref_common const& operator=( array_ref_common const& b ) const noexcept(
        noexcept(this->operator=(static_cast<array_const_ref<T> const&>(b))))
        //  pre:    size() == b.size()
    {
        return this->operator=(static_cast<array_const_ref<T> const&>(b));
    }

    array_ref_common const& operator=( array_const_ref<T> b ) const noexcept(
        noexcept(this->copy_front(ali::move(b))))
        //  pre:    size() == b.size()
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->copy_front(ali::move(b));

        return *this;
    }

    T& front( void ) const noexcept
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());

        return this->data()[0];
    }

    T& back( void ) const noexcept
        //  pre:    !is_empty()
    {
        ali_assert(!this->is_empty());

        return this->data()[this->_size - 1];
    }

    T& back( int i ) const noexcept
        //  pre:    0 <= i && i < size()
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        return this->data()[this->_size - 1 - i];
    }

    T& at( int i ) const noexcept
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        return this->data()[i];
    }

    T& operator[]( int i ) const noexcept
    {
        return this->at(i);
    }

    T* data( void ) const noexcept
    {
        return const_cast<T*>(this->_begin);
            //  Legal, we started with non-const array.
    }

    //  mutable_ref

    array_ref<T> mutable_ref( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);
        ali_assert(0 <= n);
        ali_assert(n <= this->_size - pos);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data() + pos, n};
    }

    array_ref<T> mutable_ref( void ) const noexcept
    {
        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data(), this->_size};
    }

    array_ref<T> mutable_ref_left( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data(), pos};
    }

    array_ref<T> mutable_ref_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data() + pos, this->_size - pos};
    }

    array_ref<T> mutable_ref_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data(), n};
    }

    array_ref<T> mutable_ref_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data() + (this->_size - n), n};
    }

    array_ref<T> mutable_ref_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data() + n, this->_size - n};
    }

    array_ref<T> mutable_ref_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data(), this->_size - n};
    }

    array_ref<T> mutable_ref_not_front_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size() / 2
    {
        return this->mutable_ref_not_front(n).mutable_ref_not_back(n);
    }

    array_ref<T> mutable_ref_not_front_not_back( int nf, int nb ) const noexcept
        //  pre:    0 <= fn && nf <= size()
        //      &&  0 <= nb && nb <= size() - nf
    {
        return this->mutable_ref_not_front(nf).mutable_ref_not_back(nb);
    }


    //  mutable_pointer

    array_ptr<T> mutable_pointer( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        return this->mutable_ref(pos, n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer( void ) const noexcept
    {
        return array_ptr<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_wref,)
            this->data(), this->_size};
    }

    array_ptr<T> mutable_pointer_left( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return this->mutable_ref_left(pos).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return this->mutable_ref_right(pos).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->mutable_ref_front(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->mutable_ref_back(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->mutable_ref_not_front(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return this->mutable_ref_not_back(n).mutable_pointer();
    }

    array_ptr<T> mutable_pointer_not_front_not_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size() / 2
    {
        return this->mutable_ref_not_front(n).mutable_pointer_not_back(n);
    }

    array_ptr<T> mutable_pointer_not_front_not_back( int nf, int nb ) const noexcept
        //  pre:    0 <= fn && nf <= size()
        //      &&  0 <= nb && nb <= size() - nf
    {
        return this->mutable_ref_not_front(nf).mutable_pointer_not_back(nb);
    }


    //  fill

    array_ref<T> const& fill( void ) const noexcept(
        noexcept(this->fill(T())))
        //  Fill with default constructed element.
    {
        return this->fill(T());
    }

    array_ref<T> const& fill( read_only_T b ) const noexcept(
        noexcept(hidden::fill(this->data(), b, this->_size)))
    {
        hidden::fill(this->data(), b, this->_size);

        return this->self();
    }

    //  copy

    template <typename U>
    array_ref<T> const& copy( int pos, array_const_ref<U> b ) const noexcept(
        noexcept(this->mutable_ref_right(pos).copy_front(ali::move(b))))
    {
        this->mutable_ref_right(pos).copy_front(ali::move(b));

        return this->self();
    }

    array_ref<T> const& copy( int pos, array_const_ref<T> b ) const noexcept(
        noexcept(this->copy<T>(pos, ali::move(b))))
    {
        return this->copy<T>(pos, ali::move(b));
    }

    //  copy_front

    template <typename U>
    array_ref<T> const& copy_front( array_const_ref<U> b ) const noexcept(
        noexcept(hidden::copy(
            this->data(), b._begin,
            ali::mini(this->_size, b._size))))
    {
        hidden::copy(
            this->data(), b._begin,
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& copy_front( array_const_ref<T> b ) const noexcept(
        noexcept(this->copy_front<T>(ali::move(b))))
    {
        return this->copy_front<T>(ali::move(b));
    }

    //  copy_back

    template <typename U>
    array_ref<T> const& copy_back( array_const_ref<U> b ) const noexcept(
        noexcept(hidden::copy(
            this->data(), b._begin,
            ali::mini(this->_size, b._size))))
    {
        int const n{ali::mini(this->_size, b._size)};

        hidden::copy(
            this->data() + (this->_size - n),
            b._begin + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& copy_back( array_const_ref<T> b ) const noexcept(
        noexcept(this->copy_back<T>(ali::move(b))))
    {
        return this->copy_back<T>(ali::move(b));
    }

    //  move

    template <typename U>
    array_ref<T> const& move( int pos, array_ref<U> b ) const noexcept(
        noexcept(this->mutable_ref_right(pos).move_front(ali::move(b))))
    {
        this->mutable_ref_right(pos).move_front(ali::move(b));

        return this->self();
    }

    array_ref<T> const& move( int pos, array_ref<T> b ) const noexcept(
        noexcept(this->move<T>(pos, ali::move(b))))
    {
        return this->move<T>(pos, ali::move(b));
    }

    //  move_front

    template <typename U>
    array_ref<T> const& move_front( array_ref<U> b ) const noexcept(
        noexcept(hidden::move(
            this->data(), b.data(),
            ali::mini(this->_size, b._size))))
    {
        hidden::move(
            this->data(), b.data(),
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& move_front( array_ref<T> b ) const noexcept(
        noexcept(this->move_front<T>(ali::move(b))))
    {
        return this->move_front<T>(ali::move(b));
    }

    //  move_back

    template <typename U>
    array_ref<T> const& move_back( array_ref<U> b ) const noexcept(
        noexcept(hidden::move(
            this->data(), b.data(),
            ali::mini(this->_size, b._size))))
    {
        int const n{ali::mini(this->_size, b._size)};

        hidden::move(
            this->data() + (this->_size - n),
            b.data() + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& move_back( array_ref<T> b ) const noexcept(
        noexcept(this->move_back<T>(ali::move(b))))
    {
        return this->move_back<T>(ali::move(b));
    }

    //  move_if_noexcept

    template <typename U>
    array_ref<T> const& move_if_noexcept( int pos, array_ref<U> b ) const noexcept(
        noexcept(this->mutable_ref_right(pos).move_front_if_noexcept(ali::move(b))))
    {
        this->mutable_ref_right(pos).move_front_if_noexcept(ali::move(b));

        return this->self();
    }

    array_ref<T> const& move_if_noexcept( int pos, array_ref<T> b ) const noexcept(
        noexcept(this->move_if_noexcept<T>(pos, ali::move(b))))
    {
        return this->move_if_noexcept<T>(pos, ali::move(b));
    }

    //  move_front_if_noexcept

    template <typename U>
    array_ref<T> const& move_front_if_noexcept( array_ref<U> b ) const noexcept(
        noexcept(hidden::move_if_noexcept(
            this->data(), b.data(),
            ali::mini(this->_size, b._size))))
    {
        hidden::move_if_noexcept(
            this->data(), b.data(),
            ali::mini(this->_size, b._size));

        return this->self();
    }

    array_ref<T> const& move_front_if_noexcept( array_ref<T> b ) const noexcept(
        noexcept(this->move_front_if_noexcept<T>(ali::move(b))))
    {
        return this->move_front_if_noexcept<T>(ali::move(b));
    }

    //  move_back_if_noexcept

    template <typename U>
    array_ref<T> const& move_back_if_noexcept( array_ref<U> b ) const noexcept(
        noexcept(hidden::move_if_noexcept(
            this->data(), b.data(),
            ali::mini(this->_size, b._size))))
    {
        int const n{ali::mini(this->_size, b._size)};

        hidden::move_if_noexcept(
            this->data() + (this->_size - n),
            b.data() + (b._size - n),
            n);

        return this->self();
    }

    array_ref<T> const& move_back_if_noexcept( array_ref<T> b ) const noexcept(
        noexcept(this->move_back_if_noexcept<T>(ali::move(b))))
    {
        return this->move_back_if_noexcept<T>(ali::move(b));
    }

    //  for_each

    template <typename body>
    array_ref<T> const& for_each( body&& b ) const noexcept(
        noexcept(b(this->at(0))))
    {
        for ( int i{this->_size}; i != 0; )
            b(this->at(--i));

        return this->self();
    }

    //  combine

    template <typename operation>
    array_ref<T> const& combine( read_only_T b, operation&& op ) const noexcept(
        noexcept(op(this->at(0), b)))
    {
        for ( int i{this->_size}; i != 0; )
            op(this->at(--i), b);

        return this->self();
    }

    template <typename operation>
    array_ref<T> const& combine( int pos, array_const_ref<T> b, operation&& op ) const noexcept(
        noexcept(this->mutable_ref_right(pos).combine_front(
            ali::move(b), ali::forward<operation>(op))))
    {
        this->mutable_ref_right(pos).combine_front(
            ali::move(b), ali::forward<operation>(op));

        return this->self();
    }


    //  combine_front

    template <typename operation>
    array_ref<T> const& combine_front( array_const_ref<T> b, operation&& op ) const noexcept(
        noexcept(op(this->at(0), b.at(0))))
    {
        for ( int i{ali::mini(this->_size, b._size)}; i != 0; )
            --i, op(this->at(i), b.at(i));

        return this->self();
    }

    //  combine_back

    template <typename operation>
    array_ref<T> const& combine_back( array_const_ref<T> b, operation&& op ) const noexcept(
        noexcept(this->mutable_ref_back(this->_size)
            .combine_front(
                b.ref_back(ali::mini(this->_size, b._size)),
                ali::forward<operation>(op))))
    {
        int const n{ali::mini(this->_size, b._size)};

        this->mutable_ref_back(n)
            .combine_front(
                b.ref_back(n),
                ali::forward<operation>(op));

        return this->self();
    }


    //  bitwise_and

    array_ref<T> const& operator&=( read_only_T b ) const noexcept(
        noexcept(this->bitwise_and(b)))
    {
        return this->bitwise_and(b);
    }

    array_ref<T> const& operator&=( array_const_ref<T> b ) const noexcept(
        noexcept(this->bitwise_and_front(ali::move(b))))
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_and_front(ali::move(b));

        return this->self();
    }

    static constexpr auto bitwise_and_operation( void ) noexcept
    {
        return []( T& a, read_only_T b ) noexcept(noexcept(a &= b)) { a &= b; };
    }

    array_ref<T> const& bitwise_and( read_only_T b ) const noexcept(
        noexcept(this->combine(b, this->bitwise_and_operation())))
    {
        return this->combine(b, this->bitwise_and_operation());
    }

    array_ref<T> const& bitwise_and( int pos, array_const_ref<T> b ) const noexcept(
        noexcept(this->combine(pos, ali::move(b), this->bitwise_and_operation())))
    {
        return this->combine(pos, ali::move(b), this->bitwise_and_operation());
    }

    //  bitwise_and_front

    array_ref<T> const& bitwise_and_front( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_front(ali::move(b), this->bitwise_and_operation())))
    {
        return this->combine_front(ali::move(b), this->bitwise_and_operation());
    }

    //  bitwise_and_back

    array_ref<T> const& bitwise_and_back( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_back(ali::move(b), this->bitwise_and_operation())))
    {
        return this->combine_back(ali::move(b), this->bitwise_and_operation());
    }

    //  bitwise_or

    static constexpr auto bitwise_or_operation( void ) noexcept
    {
        return []( T& a, read_only_T b ) noexcept(noexcept(a |= b)) { a |= b; };
    }

    array_ref<T> const& operator|=( read_only_T b ) const noexcept(
        noexcept(this->bitwise_or(b)))
    {
        return this->bitwise_or(b);
    }

    array_ref<T> const& operator|=( array_const_ref<T> b ) const noexcept(
        noexcept(this->bitwise_or_front(ali::move(b))))
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_or_front(ali::move(b));

        return this->self();
    }

    array_ref<T> const& bitwise_or( read_only_T b ) const noexcept(
        noexcept(this->combine(b, this->bitwise_or_operation())))
    {
        return this->combine(b, this->bitwise_or_operation());
    }

    array_ref<T> const& bitwise_or( int pos, array_const_ref<T> b ) const noexcept(
        noexcept(this->combine(pos, ali::move(b), this->bitwise_or_operation())))
    {
        return this->combine(pos, ali::move(b), this->bitwise_or_operation());
    }

    //  bitwise_or_front

    array_ref<T> const& bitwise_or_front( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_front(ali::move(b), this->bitwise_or_operation())))
    {
        return this->combine_front(ali::move(b), this->bitwise_or_operation());
    }

    //  bitwise_or_back

    array_ref<T> const& bitwise_or_back( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_back(ali::move(b), this->bitwise_or_operation())))
    {
        return this->combine_back(ali::move(b), this->bitwise_or_operation());
    }

    //  bitwise_xor

    static constexpr auto bitwise_xor_operation( void ) noexcept
    {
        return []( T& a, read_only_T b ) noexcept(noexcept(a ^= b)) { a ^= b; };
    }

    array_ref<T> const& operator^=( read_only_T b ) const noexcept(
        noexcept(this->bitwise_xor(b)))
    {
        return this->bitwise_xor(b);
    }

    array_ref<T> const& operator^=( array_const_ref<T> b ) const noexcept(
        noexcept(this->bitwise_xor_front(ali::move(b))))
    {
        ali_assert(this->_size == b._size);

        if ( this->_size == b._size )
            this->bitwise_xor_front(ali::move(b));

        return this->self();
    }

    array_ref<T> const& bitwise_xor( read_only_T b ) const noexcept(
        noexcept(this->combine(b, this->bitwise_xor_operation())))
    {
        return this->combine(b, this->bitwise_xor_operation());
    }

    array_ref<T> const& bitwise_xor( int pos, array_const_ref<T> b ) const noexcept(
        noexcept(this->combine(pos, ali::move(b), this->bitwise_xor_operation())))
    {
        return this->combine(pos, ali::move(b), this->bitwise_xor_operation());
    }

    //  bitwise_xor_front

    array_ref<T> const& bitwise_xor_front( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_front(ali::move(b), this->bitwise_xor_operation())))
    {
        return this->combine_front(ali::move(b), this->bitwise_xor_operation());
    }

    //  bitwise_xor_back

    array_ref<T> const& bitwise_xor_back( array_const_ref<T> b ) const noexcept(
        noexcept(this->combine_back(ali::move(b), this->bitwise_xor_operation())))
    {
        return this->combine_back(ali::move(b), this->bitwise_xor_operation());
    }

    //  reverse

    array_ref<T> const& reverse( void ) const noexcept(
        noexcept(ali::adl_swap(this->at(0), this->at(0))))
    {
        if ( this->_size > 1 )
            for ( int left{}, right{this->_size - 1};
                left < right; ++left, --right )
                    ali::adl_swap(this->at(left), this->at(right));

        return this->self();
    }

    //  swap_block

    array_ref<T> const& swap_block( int pos, array_ref<T> b ) const noexcept(
        noexcept(this->mutable_ref_right(pos).swap_block_front(ali::move(b))))
    {
        this->mutable_ref_right(pos).swap_block_front(ali::move(b));

        return this->self();
    }


    //  swap_block_front

    array_ref<T> const& swap_block_front( array_ref<T> b ) const noexcept(
        noexcept(ali::adl_swap(this->at(0), this->at(0))))
    {
        for ( int i = ali::mini(this->_size, b._size); i != 0; )
            --i, ali::adl_swap(this->at(i), b.at(i));

        return this->self();
    }

    //  swap_block_back

    array_ref<T> const& swap_block_back( array_ref<T> b ) const noexcept(
        noexcept(this->mutable_ref_back(this->_size)
            .swap_block_front(b.mutable_ref_back(
                ali::mini(this->_size, b._size)))))
    {
        int const n{ali::mini(this->_size, b._size)};

        this->mutable_ref_back(n)
            .swap_block_front(
                b.mutable_ref_back(n));

        return this->self();
    }


    //  rotate

    array_ref<T> const& rotate_left( int n ) const noexcept(
        noexcept(this->swap_block(0, this->mutable_ref(0, 0))))
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        if ( n != 0 && n != this->_size )
        {
            int const pos{n};

            int n_left{n};

            int n_right{this->_size - n_left};

            while ( n_left != n_right )
            {
                if ( n_left > n_right )
                {
                    this->swap_block(
                        pos - n_left,
                        this->mutable_ref(
                            pos, n_right));

                    n_left -= n_right;
                }
                else
                {
                    this->swap_block(
                        pos - n_left,
                        this->mutable_ref(
                            (pos - n_left) + n_right,
                            n_left));

                    n_right -= n_left;
                }
            }

            this->swap_block(
                pos - n_left,
                this->mutable_ref(
                    pos, n_left));
        }

        return this->self();
    }

    array_ref<T> const& rotate_right( int n ) const noexcept(
        noexcept(this->rotate_left(this->_size - n)))
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        return this->rotate_left(this->_size - n);
    }

    //  shift

    array_ref<T> const& shift_left( int n, read_only_T fill ) const noexcept(
            noexcept(this->copy_front(this->ref_not_front(n)))
        &&  noexcept(this->mutable_ref_back(n).fill(fill)))
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        this->copy_front(this->ref_not_front(n));
            
        this->mutable_ref_back(n).fill(fill);
        
        return this->self();
    }

    array_ref<T> const& shift_left( int n ) const noexcept(
        noexcept(this->shift_left(n, T())))
    {
        return this->shift_left(n, T());
    }

    array_ref<T> const& shift_right( int n, read_only_T fill ) const noexcept(
            noexcept(this->copy_back(this->ref_not_back(n)))
        &&  noexcept(this->mutable_ref_front(n).fill(fill)))
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_size);

        this->copy_back(this->ref_not_back(n));
            
        this->mutable_ref_front(n).fill(fill);
        
        return this->self();
    }

    array_ref<T> const& shift_right( int n ) const noexcept(
        noexcept(this->shift_right(n, T())))
    {
        return this->shift_right(n, T());
    }

    //  next_permutation

    template <typename comparator>
	bool next_permutation( comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0)))
        &&  noexcept(this->mutable_ref_right(0).reverse()))
	{
        int next{this->_size};

        if ( next == 0 || --next == 0 )
		    return false;

	    for ( ;; )
		{
            int const next1{next};

		    if ( int{compare(this->at(--next), this->at(next1))} < 0 )
            {
			    int mid{this->_size};

                while ( int{compare(this->at(next), this->at(--mid))} >= 0 )
				    ;   //  Noop

                ali::adl_swap(this->at(next), this->at(mid));

                this->mutable_ref_right(next1).reverse();

                return true;
            }

		    if ( next == 0 )
            {
			    this->reverse();

                return false;
            }
		}
	}

    bool next_permutation( void ) const noexcept(
        noexcept(this->next_permutation(ali::default_comparator{})))
    {
        return this->next_permutation(ali::default_comparator{});
    }


    //  stable_partition

    template <typename predicate>
    int stable_partition( predicate&& p ) const noexcept(
            noexcept(bool{p(this->front())})
        &&  noexcept(this->mutable_ref(0, 0).rotate_left(0)))
        //  Rearranges the array into left and right parts.
        //  Elements in the left part satisfy the given predicate
        //  and are guaranteed to have the same relative order
        //  as in the original array.
        //  Elements in the right part DON'T satisfy the given
        //  predicate and are guaranteed to have the same relative
        //  order as in the original array.
        //  Returns size of the resulting left part (or, equivalently,
        //  index of the first element of the right part).
    {
        if ( this->_size == 0 )
            return 0;

        if ( this->_size == 1 )
            return static_cast<int>(bool{p(this->front())});

        int const m{this->_size / 2};

        int const begin{this->mutable_ref_left(m).stable_partition(p)};

        int const end{m + this->mutable_ref_right(m).stable_partition(p)};

        this->mutable_ref(begin, end - begin).rotate_left(m - begin);

        return end - (m - begin);
    }

    //  stable_partition_left_only

    template <typename predicate>
    int stable_partition_left_only( predicate&& p ) const noexcept(
            noexcept(bool{p(this->at(0))})
        &&  noexcept(this->at(0) = ali::move(this->at(0))))
        //  Rearranges the array into left and right parts.
        //  Elements in the left part satisfy the given predicate
        //  and are guaranteed to have the same relative order
        //  as in the original array.
        //  The content of the right part is UNDEFINED.
        //  Returns size of the resulting left part.
    {
        int iw{};

        for ( int ir{}; ir != this->_size; ++ir )
            if ( bool{p(this->at(ir))} )
                this->at(iw++) = ali::move(this->at(ir));

        return iw;
    }

    //  insertion_sort

    template <typename comparator>
    array_ref<T> const& insertion_sort( comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else for ( int i{1}; i != this->_size; ++i )
        {
            for ( int j{i}; j != 0 && int{compare(this->at(j - 1), this->at(j))} > 0; --j )
                ali::adl_swap(this->at(j - 1), this->at(j));
        }

        return this->self();
    }

    array_ref<T> const& insertion_sort( void ) const noexcept(
        noexcept(this->insertion_sort(ali::default_comparator{})))
    {
        return this->insertion_sort(ali::default_comparator{});
    }

    //  quick_sort

    template <typename comparator>
    int unstable_partition_around_pivot(
        int index_of_pivot, comparator&& compare ) const noexcept(
                noexcept(ali::adl_swap(this->at(0), this->at(0)))
            &&  noexcept(int{compare(this->at(0), this->at(0))}))
    {
        ali_assert(0 <= index_of_pivot);
        ali_assert(index_of_pivot < this->_size);

        if ( index_of_pivot != this->_size - 1 )
        {
            //  Move pivot to the end.

            ali::adl_swap(this->at(index_of_pivot), this->at(this->_size - 1));

            index_of_pivot = this->_size - 1;
        }

        T& pivot{this->at(index_of_pivot)};

        for ( int i{this->_size - 1}; i != 0; )
        {
            int const result{compare(this->at(--i), pivot)};

            if ( result > 0 && i != --index_of_pivot )
                ali::adl_swap(this->at(i), this->at(index_of_pivot));
        }

        if ( index_of_pivot != this->_size - 1 )
            ali::adl_swap(this->at(index_of_pivot), pivot);
                //  Move pivot back to its proper place.

        return index_of_pivot;
    }

    int unstable_partition_around_pivot( int index_of_pivot ) const noexcept(
        noexcept(this->unstable_partition_around_pivot(
            index_of_pivot, ali::default_comparator{})))
    {
        return this->unstable_partition_around_pivot(
            index_of_pivot, ali::default_comparator{});
    }

    template <typename comparator>
    array_ref<T> const& quick_sort( comparator&& compare ) const noexcept(
            noexcept(this->mutable_pointer()
                ->unstable_partition_around_pivot(0, compare))
        &&  noexcept(this->mutable_pointer()
                ->insertion_sort(compare)))
    {
        constexpr int threshold{8};

        array_ptr<T> ptr{this->mutable_pointer()};

        while ( ptr->size() > threshold )
        {
            int const i{ptr->unstable_partition_around_pivot(
                ptr->size() / 2, compare)};

            array_ptr<T>
                left{ptr->mutable_pointer_left(i)},
                right{ptr->mutable_pointer_right(i + 1)};

            //  Recurse to the smaller partition.

            if ( left->size() < right->size() )
            {
                left->quick_sort(compare);

                ptr = right;
            }
            else
            {
                right->quick_sort(compare);

                ptr = left;
            }
        }

        ptr->insertion_sort(compare);

        return this->self();
    }


    array_ref<T> const& quick_sort( void ) const noexcept(
        noexcept(this->quick_sort(ali::default_comparator{})))
    {
        return this->quick_sort(ali::default_comparator{});
    }

    //  heap_sort

    //  The heap invariant:
    //  for all i in set {1 .. size() - 1} &
    //      compare(
    //          at(i),              //  Child
    //          at((i - 1) / 2))    //  Parent
    //              <= 0

    template <typename comparator>
    array_ref<T> const& up_heap( int i, comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
        //  Assumes the heap property satisfied everywhere except
        //  between element at i and its parent at (i - 1) / 2.
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        for ( ; i != 0 && int{compare(this->at(i), this->at((i - 1) / 2))} > 0;
                i = (i - 1) / 2 )
        {
            ali::adl_swap(this->at(i), this->at((i - 1) / 2));
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& down_heap( int i, comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
        //  Assumes the heap property satisfied everywhere except
        //  between element at i and its children at 2i+1 and 2i+2.
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        //  The first element without any descendant (leaf) is at _size / 2.


        for ( int const leaf{this->_size / 2}; i < leaf; )
        {
            int const left{2 * i + 1};

            ali_assert(left < this->_size);
                //  left = 2 * i + 1 is always < _size

            int const right{left + 1};

            ali_assert(right <= this->_size);

            int largest{int{compare(this->at(left), this->at(i))} > 0 ? left : i};

            if ( right != this->_size && int{compare(this->at(right), this->at(largest))} > 0 )
                largest = right;

            if ( largest == i )
                break;

            ali::adl_swap(this->at(i), this->at(largest));

            i = largest;
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& make_heap( comparator&& compare ) const noexcept(
        noexcept(this->down_heap(0, compare)))
    {
        for ( int i{this->_size / 2}; i != 0; )
            this->down_heap(--i, compare);

        return this->self();
    }

    //  The reversed_heap invariant:
    //  for all i in set {1 .. size() - 1} &
    //      compare(
    //          at(size() - 1 - i),             //  Child
    //          at(size() - 1 - (i - 1) / 2))   //  Parent
    //              <= 0

    template <typename comparator>
    array_ref<T> const& up_reversed_heap( int i, comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
        //  Assumes the heap property satisfied everywhere except
        //  between element at size() - 1 - i and its parent
        //  at size() - 1 - (i - 1) / 2.
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        for ( ; i != 0 && int{compare(
                    this->at(this->_size - 1 - i),
                    this->at(this->_size - 1 - (i - 1) / 2))} > 0;
                i = (i - 1) / 2 )
        {
            ali::adl_swap(this->at(this->_size - 1 - i),
                this->at(this->_size - 1 - (i - 1) / 2));
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& down_reversed_heap( int i, comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
        //  Assumes the heap property satisfied everywhere except
        //  between element at size() - 1 - i and its children
        //  at size() - 1 - (2i+1) and size() - 1 - (2i+2).
        //  Ensures the heap property is satisfied everywhere.
    {
        ali_assert(0 <= i);
        ali_assert(i < this->_size);

        //  The first element without any descendant (leaf) is at _size / 2.

        for ( int const leaf{this->_size / 2}; i < leaf; )
        {
            int const left{2 * i + 1};

            ali_assert(left < this->_size);
                //  left = 2 * i + 1 is always < _size

            int const right{left + 1};

            ali_assert(right <= this->_size);

            int largest{int{compare(
                this->at(this->_size - 1 - left),
                this->at(this->_size - 1 - i))} > 0 ? left : i};

            if ( right != this->_size && int{compare(
                    this->at(this->_size - 1 - right),
                    this->at(this->_size - 1 - largest))} > 0 )
            {
                largest = right;
            }

            if ( largest == i )
                break;

            ali::adl_swap(this->at(this->_size - 1 - i),
                this->at(this->_size - 1 - largest));

            i = largest;
        }

        return this->self();
    }

    template <typename comparator>
    array_ref<T> const& make_reversed_heap( comparator&& compare ) const noexcept(
        noexcept(this->down_reversed_heap(0, compare)))
    {
        for ( int i{this->_size / 2}; i != 0; )
            this->down_reversed_heap(--i, compare);

        return this->self();
    }
    
    template <typename comparator>
    array_ref<T> const& heap_sort( comparator&& compare ) const noexcept(
            noexcept(this->make_heap(compare))
        &&  noexcept(this->mutable_ref_left(0)
                .down_heap(0, compare))
        &&  noexcept(ali::adl_swap(this->front(), this->at(0))))
    {
        if ( this->_size < 2 )
            ;   //  NOOP
        else
        {
            this->make_heap(compare);

            int right{this->_size};

            ali::adl_swap(this->front(), this->at(--right));

            while ( right > 1 )
            {
                this->mutable_ref_left(right)
                    .down_heap(0, compare);

                ali::adl_swap(this->front(), this->at(--right));
            }
        }

        return this->self();
    }


    array_ref<T> const& heap_sort( void ) const noexcept(
        noexcept(this->heap_sort(ali::default_comparator{})))
    {
        return this->heap_sort(ali::default_comparator{});
    }

    //  sort

    template <typename comparator>
    array_ref<T> const& sort( comparator&& compare ) const noexcept(
        noexcept(this->heap_sort(ali::forward<comparator>(compare))))
    {
        return this->heap_sort(ali::forward<comparator>(compare));
    }

    array_ref<T> const& sort( void ) const noexcept(
        noexcept(this->heap_sort()))
    {
        return this->heap_sort();
    }

    //  nth_element
    //  Rearranges the array such that
    //  -   The element at the nth position is changed
    //      to whatever element would occur in that
    //      position if the array was sorted.
    //  -   All of the elements before this new nth
    //      element are less than or equal to the elements
    //      after the new nth element.

    template <typename comparator>
    array_ref<T> const& nth_element( int nth, comparator&& compare ) const noexcept(
            noexcept(this->mutable_pointer()
                ->partition_by_median_guess(compare))
        &&  noexcept(this->mutable_pointer()
                ->insertion_sort(compare)))
        //  pre:    0 <= nth && nth <= size()
    {
        ali_assert(0 <= nth);
        ali_assert(nth <= this->_size);

        if ( nth == this->_size )
            return this->self();

        constexpr int insertion_sort_max{32};

        array_ptr<T> ptr{this->mutable_pointer()};

        while ( ptr->size() > insertion_sort_max )
        {
            pair<int, int> const mid{
                ptr->partition_by_median_guess(compare)};

            if ( mid.second <= nth )
            {
                ptr += mid.second;
                nth -= mid.second;
            }
            else if ( mid.first <= nth )
            {
                //  Nth inside fat pivot, done.
            
                return this->self();
            }
            else
            {
                ptr = ptr->mutable_pointer_left(mid.first);
            }
        }

        ptr->insertion_sort(compare);

        return this->self();
    }


    array_ref<T> const& nth_element( int nth ) const noexcept(
        noexcept(this->nth_element(nth, ali::default_comparator{})))
    {
        return this->nth_element(nth, ali::default_comparator{});
    }

    array_ref<ali::uint8> as_mutable_blob( void ) const noexcept;

protected:  //  Methods
    constexpr array_ref_common( void ) = default;

    array_ref_common( array_ref_common const& ) = default;

    array_ref_common( array_ref_common&& ) = default;

    constexpr array_ref_common( T* begin, int size ) noexcept
    :   array_const_ref<T>{begin, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ref_common(
        debug::weak_reference const& wref,
        T* begin, int size ) noexcept
    :   array_const_ref<T>{wref, begin, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ref<T> const& self( void ) const noexcept
    {
        return static_cast<array_ref<T> const&>(*this);
    }

private:    //  Methods
    //  part of nth_element implementation
    template <typename comparator>
    pair<int, int> partition_by_median_guess( comparator&& compare ) const noexcept(
            noexcept(this->guess_median(0, compare))
        &&  noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
    {
        ali_assert(this->_size >= 3);

        int const mid{this->_size / 2};

        this->guess_median(mid, compare);

        int pivot_first{mid};
        int pivot_last{pivot_first + 1};

        while ( 0 < pivot_first
            &&  int{compare(
                    this->at(pivot_first - 1),
                    this->at(pivot_first))} == 0 )
        {
            --pivot_first;
        }

        while ( pivot_last < this->_size
            &&  int{compare(
                    this->at(pivot_first),
                    this->at(pivot_last))} == 0 )
        {
            ++pivot_last;
        }

        int first{pivot_last};
        int last{pivot_first};

        for ( ;; )
        {
            for ( ; first < this->_size; ++first )
            {
                int const result{compare(
                    this->at(pivot_first), this->at(first))};

                if ( result < 0 )
                    ;   //  Noop
                else if ( result > 0 )
                    break;
                else if ( pivot_last != first )
                {
                    ali::adl_swap(this->at(pivot_last), this->at(first));
                    ++pivot_last;
                }
                else
                {
                    ++pivot_last;
                }
            }

            for ( ; 0 < last; --last )
            {
                int const result{compare(
                    this->at(last - 1), this->at(pivot_first))};

                if ( result < 0 )
                    ;   //  Noop
                else if ( result > 0 )
                    break;
                else if ( --pivot_first != last - 1 )
                    ali::adl_swap(this->at(pivot_first), this->at(last - 1));
            }

            if ( last == 0 && first == this->_size )
                return pair<int, int>{pivot_first, pivot_last};

            if ( last == 0 )
            {
                //  No room at bottom, rotate pivot upward.

                if ( pivot_last != first )
                    ali::adl_swap(this->at(pivot_first), this->at(pivot_last));

                ++pivot_last;
                ali::adl_swap(this->at(pivot_first), this->at(first));
                ++pivot_first;
                ++first;
            }
            else if ( first == this->_size )
            {
                //  No room at top, rotate pivot downward.

                if ( --last != --pivot_first )
                    ali::adl_swap(this->at(last), this->at(pivot_first));

                ali::adl_swap(this->at(pivot_first), this->at(--pivot_last));
            }
            else
            {
                ali::adl_swap(this->at(first), this->at(--last));
                ++first;
            }
        }
    }

    template <typename comparator>
    void guess_median( int mid, comparator&& compare ) const noexcept(
        noexcept(this->guess_median3(0, 0, 0, compare)))
        //  Sorts median element to middle.
    {
        ali_assert(this->_size >= 3);

        constexpr int first{0};
        int const last{this->_size - 1};

        ali_assert(first < mid);
        ali_assert(mid < last);

        if ( this->_size > 40 )
        {
            //  Median of nine.
            int const step{this->_size / 8};
            int const two_step{2 * step};
            this->guess_median3(first, first + step, first + two_step, compare);
            this->guess_median3(mid - step, mid, mid + step, compare);
            this->guess_median3(last - two_step, last - step, last, compare);
            this->guess_median3(first + step, mid, last - step, compare);
        }
        else
        {
            this->guess_median3(first, mid, last, compare);
        }
    }

    template <typename comparator>
    void guess_median3( int first, int mid, int last, comparator&& compare ) const noexcept(
            noexcept(int{compare(this->at(0), this->at(0))})
        &&  noexcept(ali::adl_swap(this->at(0), this->at(0))))
        //  Sorts median of three elements to middle.
    { 
        ali_assert(0 <= first);
        ali_assert(first < mid);
        ali_assert(mid < last);
        ali_assert(last < this->_size);

        if ( int{compare(this->at(mid), this->at(first))} < 0 )
            ali::adl_swap(this->at(mid), this->at(first));

        if ( int{compare(this->at(last), this->at(mid))} < 0 )
        { 
            //  Swap middle and last, then test first again.
            
            ali::adl_swap(this->at(last), this->at(mid));

            if ( int{compare(this->at(mid), this->at(first))} < 0 )
                ali::adl_swap(this->at(mid), this->at(first));
        }
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ref : public array_ref_common<T>
// ******************************************************************
{
public:
    constexpr array_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_ref( array_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_ref( array_ref&& ) = default;

    template <typename D>
    array_ref( exposes_array_ref<T, D>& b ) noexcept
    :   array_ref{b.mutable_ref()}
    {}

    constexpr array_ref( T* begin, int size ) noexcept
    :   array_ref_common<T>{begin, size}
    {}

    template <int n>
    constexpr array_ref( T (&begin)[n] ) noexcept
    :   array_ref_common<T>{begin, n}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ref(
        debug::weak_reference const& wref,
        T* begin, int size ) noexcept
    :   array_ref_common<T>{wref, begin, size}
    {}

    template <int n>
    array_ref(
        debug::weak_reference const& wref,
        T (&begin)[n] ) noexcept
    :   array_ref_common<T>{wref, begin, n}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ref const& operator=( array_ref const& b ) const noexcept(
        noexcept(array_ref_common<T>::operator=(
            static_cast<array_ref_common<T> const&>(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<T>::operator=(
            static_cast<array_ref_common<T> const&>(b));

        return *this;
    }

    array_ref const& operator=( array_const_ref<T> b ) const noexcept(
        noexcept(array_ref_common<T>::operator=(ali::move(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<T>::operator=(ali::move(b));

        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<ali::uint8> : public array_ref_common<ali::uint8>
// ******************************************************************
//  A.k.a. blob_ref.
// ******************************************************************
{
public:
    array_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_ref( array_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    array_ref( array_ref&& ) = default;

    template <typename D>
    array_ref( exposes_blob_ref<D>& b ) noexcept
    :   array_ref{b.mutable_ref()}
    {}

    array_ref( ali::uint8* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{begin, size}
    {}

    array_ref( ali::uint16* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8*>(begin),
            2 * size}
    {}

    array_ref( ali::uint32* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8*>(begin),
            4 * size}
    {}

    template <int n>
    array_ref( ali::uint8 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{begin, n}
    {}

    template <int n>
    explicit array_ref( ali::uint16 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8*>(begin),
            2 * n}
    {}

    template <int n>
    explicit array_ref( ali::uint32 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{
            reinterpret_cast<ali::uint8*>(begin),
            4 * n}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ref(
        debug::weak_reference const& wref,
        ali::uint8* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{wref, begin, size}
    {}

    array_ref(
        debug::weak_reference const& wref,
        ali::uint16* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8*>(begin),
            2 * size}
    {}

    array_ref(
        debug::weak_reference const& wref,
        ali::uint32* begin, int size ) noexcept
    :   array_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8*>(begin),
            4 * size}
    {}

    template <int n>
    array_ref(
        debug::weak_reference const& wref,
        ali::uint8 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{wref, begin, n}
    {}

    template <int n>
    array_ref(
        debug::weak_reference const& wref,
        ali::uint16 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8*>(begin),
            2 * n}
    {}

    template <int n>
    array_ref(
        debug::weak_reference const& wref,
        ali::uint32 (&begin)[n] ) noexcept
    :   array_ref_common<ali::uint8>{
            wref,
            reinterpret_cast<ali::uint8*>(begin),
            4 * n}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ref const& operator=( array_ref const& b ) const noexcept(
        noexcept(array_ref_common<ali::uint8>::operator=(
            static_cast<array_ref_common<ali::uint8> const&>(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::uint8>::operator=(
            static_cast<array_ref_common<ali::uint8> const&>(b));

        return *this;
    }

    array_ref const& operator=( array_const_ref<ali::uint8> b ) const noexcept(
        noexcept(array_ref_common<ali::uint8>::operator=(ali::move(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::uint8>::operator=(ali::move(b));

        return *this;
    }

    template <typename D>
    array_ref const& operator=( exposes_blob_const_ref<D> const& b ) const noexcept(
        noexcept(array_ref_common<ali::uint8>::operator=(b.ref())))
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::uint8>::operator=(b.ref());

        return *this;
    }

    //  int

    // pre: byte_count in set {1, 2, 3, 4}
    array_ref const& set_int_be_at(
        int pos, ali::uint32 value, int byte_count ) const noexcept;

    // pre: byte_count in set {1, 2, 3, 4}
    array_ref const& set_int_le_at(
        int pos, ali::uint32 value, int byte_count ) const noexcept;

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array_ref const& set_long_be_at(
        int pos, ali::uint64 value, int byte_count ) const noexcept;

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array_ref const& set_long_le_at(
        int pos, ali::uint64 value, int byte_count ) const noexcept;

    array_ref const& set_int8_at( int pos, ali::uint8 value ) const noexcept
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= this->_size - 1);

        this->data()[pos] = value;

        return *this;
    }

    array_ref const& set_int16_be_at( int pos, ali::uint16 value ) const noexcept
    {
        return this->set_int_be_at(pos, value, 2);
    }

    array_ref const& set_int16_le_at( int pos, ali::uint16 value ) const noexcept
    {
        return this->set_int_le_at(pos, value, 2);
    }

    array_ref const& set_int16_ne_at( int pos, ali::uint16 value ) const noexcept
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 2);
    }

    array_ref const& set_int32_be_at( int pos, ali::uint32 value ) const noexcept
    {
        return this->set_int_be_at(pos, value, 4);
    }

    array_ref const& set_int32_le_at( int pos, ali::uint32 value ) const noexcept
    {
        return this->set_int_le_at(pos, value, 4);
    }

    array_ref const& set_int32_ne_at( int pos, ali::uint32 value ) const noexcept
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 4);
    }

    array_ref const& set_int64_be_at( int pos, ali::uint64 value ) const noexcept
    {
        return this->set_long_be_at(pos, value, 8);
    }

    array_ref const& set_int64_le_at( int pos, ali::uint64 value ) const noexcept
    {
        return this->set_long_le_at(pos, value, 8);
    }

    array_ref const& set_int64_ne_at( int pos, ali::uint64 value ) const noexcept
        //  Native endian.
    {
        return this->set_long_le_at(pos, to_little_endian(value), 8);
    }

    //  shift

    array_ref const& logical_shift_right( int n ) const noexcept;
    array_ref const& logical_shift_left( int n ) const noexcept;

    array_ref const& operator>>=( int n ) const noexcept
    {
        return this->logical_shift_right(n);
    }

    array_ref const& operator<<=( int n ) const noexcept
    {
        return this->logical_shift_left(n);
    }

    //  increment

    int increment_be( void ) const noexcept;
        //  Increments this blob as a big endian large integer.
        //  Returns carry.

    int increment_le( void ) const noexcept;
        //  Increments this blob as a little endian large integer.
        //  Returns carry.

    array_ref<char> as_mutable_string( void ) const noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<char>
    : public array_ref_common<char>
// ******************************************************************
//  A.k.a. string_ref.
// ******************************************************************
{
public:
    constexpr array_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_ref( array_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    //
    //  There is no c_string_ref and conversion
    //  from c_string_const_ref is not allowed.
    array_ref( array_ref&& ) = default;

    template <typename D>
    array_ref( exposes_string_ref<D>& b ) noexcept
    :   array_ref{b.mutable_ref()}
    {}

    constexpr array_ref( char* begin, int size ) noexcept
    :   array_ref_common<char>{begin, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ref(
        debug::weak_reference const& wref,
        char* begin, int size ) noexcept
    :   array_ref_common<char>{wref, begin, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ref const& operator=( array_ref const& b ) const noexcept(
        noexcept(array_ref_common<char>::operator=(
            static_cast<array_ref_common<char> const&>(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<char>::operator=(
            static_cast<array_ref_common<char> const&>(b));

        return *this;
    }

    array_ref const& operator=( array_const_ref<char> b ) const noexcept(
        noexcept(array_ref_common<char>::operator=(ali::move(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<char>::operator=(ali::move(b));

        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_ref<ali::wchar>
    : public array_ref_common<ali::wchar>
// ******************************************************************
//  A.k.a wstring_ref.
// ******************************************************************
{
public:
    constexpr array_ref( void ) = default;

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    array_ref( array_ref const& ) = default;

    //  Default move ctor is OK - a reference can
    //  be initialized from another reference and
    //  the original reference reverts to its default
    //  constructed state.
    //
    //  There is no c_wstring_ref and conversion
    //  from c_wstring_const_ref is not allowed.
    array_ref( array_ref&& ) = default;

    template <typename D>
    array_ref( exposes_wstring_ref<D>& b ) noexcept
    :   array_ref{b.mutable_ref()}
    {}

    constexpr array_ref( ali::wchar* begin, int size ) noexcept
    :   array_ref_common<ali::wchar>{begin, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ref(
        debug::weak_reference const& wref,
        ali::wchar* begin, int size ) noexcept
    :   array_ref_common<ali::wchar>{wref, begin, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ref const& operator=( array_ref const& b ) const noexcept(
        noexcept(array_ref_common<ali::wchar>::operator=(
            static_cast<array_ref_common<ali::wchar> const&>(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::wchar>::operator=(
            static_cast<array_ref_common<ali::wchar> const&>(b));

        return *this;
    }

    array_ref const& operator=( array_const_ref<ali::wchar> b ) const noexcept(
        noexcept(array_ref_common<ali::wchar>::operator=(ali::move(b))))
        //  pre:    size() == b.size()
    {
        array_ref_common<ali::wchar>::operator=(ali::move(b));

        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_string_const_ref
    : public string_const_ref
// ******************************************************************
//  Guarantees that the string is actually
//  a null-terminated C-string.
// ******************************************************************
{
public:
    constexpr c_string_const_ref( void ) noexcept
    :   string_const_ref{"", 0}
    {}

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    c_string_const_ref( c_string_const_ref const& ) = default;

    //  Default move ctor is NOT OK - our default
    //  constructed state is a bit different than
    //  that of the base class.
    c_string_const_ref( c_string_const_ref&& b ) noexcept
    :   string_const_ref{ali::move(b)}
    {
        ali_assert(b._size == 0);
        ali_assert(b._begin != nullptr);
        ali_assert(*b._begin == '\0');
            //  string_const_ref knows how to
            //  move from c_string_const_ref.
    }

    template <typename D>
    c_string_const_ref( exposes_string_const_ref<D> const& b ) noexcept
    :   c_string_const_ref{b.ref()}
    {}

    explicit c_string_const_ref( char const* str ) noexcept
    :   string_const_ref{str, str != nullptr ? str_len(str) : 0}
    {
        ali_assert(str != nullptr);
    }

    constexpr c_string_const_ref( char const* str, int size ) noexcept
    :   string_const_ref{str, size}
    {
        ali_constexpr_assert(str != nullptr);
        ali_constexpr_assert(str[size] == '\0');
    }

    c_string_const_ref( c_string_const_ref b, int pos ) noexcept
    :   string_const_ref{
            ALI_IF_DEBUG_WEAK_REF(b._wref,)
            b.data() + pos,
            b.size() - pos}
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b.size());
    }

ALI_IF_DEBUG_WEAK_REF(
    c_string_const_ref(
        debug::weak_reference const& wref,
        char const* str ) noexcept
    :   string_const_ref{wref, str, str != nullptr ? str_len(str) : 0}
    {
        ali_assert(str != nullptr);
    }

    c_string_const_ref(
        debug::weak_reference const& wref,
        char const* str, int size ) noexcept
    :   string_const_ref{wref, str, size}
    {
        ali_assert(str != nullptr);
        ali_assert(str[size] == '\0');
    }

)   //  ALI_IF_DEBUG_WEAK_REF

    //  reference

    string_const_ref ref( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        return string_const_ref::ref(pos, n);
    }

    c_string_const_ref ref( void ) const noexcept
    {
        return c_cast(string_const_ref::ref());
    }

    c_string_const_ref ref_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return c_cast(string_const_ref::ref_right(pos));
    }

    c_string_const_ref ref_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return c_cast(string_const_ref::ref_back(n));
    }

    c_string_const_ref ref_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return c_cast(string_const_ref::ref_not_front(n));
    }


    //  pointer

    array_const_ptr<char> pointer( int pos, int n ) const noexcept;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_string_const_ptr pointer( void ) const noexcept;
    c_string_const_ptr pointer_right( int pos ) const noexcept;
        //  pre:    0 <= pos && pos <= size()
    c_string_const_ptr pointer_back( int n ) const noexcept;
        //  pre:    0 <= n && n <= size()
    c_string_const_ptr pointer_not_front( int n ) const noexcept;
        //  pre:    0 <= n && n <= size()

    char const* c_str( void ) const noexcept
    {
        return this->data();
    }
    
    //  trim_left_if

    template <typename predicate>
    c_string_const_ref trim_left_if( predicate&& p ) const noexcept(
        noexcept(c_cast(string_const_ref::trim_left_if(
            ali::forward<predicate>(p)))))
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())
    {
        return c_cast(string_const_ref::trim_left_if(
            ali::forward<predicate>(p)));
    }

    //  trim_left

    template <typename transform>
    c_string_const_ref trim_left( string_const_ref set, transform t ) const noexcept(
        noexcept(c_cast(string_const_ref::trim_left(ali::move(set), t))))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)
    {
        return c_cast(string_const_ref::trim_left(ali::move(set), t));
    }

    c_string_const_ref trim_left( void ) const noexcept(
        noexcept(c_cast(string_const_ref::trim_left())))
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())
    {
        return c_cast(string_const_ref::trim_left());
    }

    c_string_const_ref trim_left( string_const_ref set ) const noexcept(
        noexcept(c_cast(string_const_ref::trim_left(ali::move(set)))))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())
    {
        return c_cast(string_const_ref::trim_left(ali::move(set)));
    }

    c_string_const_ref nocase_trim_left( string_const_ref set ) const noexcept(
        noexcept(c_cast(string_const_ref::nocase_trim_left(ali::move(set)))))
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.front())
    {
        return c_cast(string_const_ref::nocase_trim_left(ali::move(set)));
    }


private:    //  Methods
    c_string_const_ref& operator=(
        c_string_const_ref const& ) = delete;

    static c_string_const_ref c_cast(
        string_const_ref ref ) noexcept
    {
        return c_string_const_ref{
            ALI_IF_DEBUG_WEAK_REF(ref._wref,)
            ref._begin, ref._size};
    }

    static c_string_const_ptr c_cast(
        array_const_ptr<char> ptr ) noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_literal
    : public c_string_const_ref
// ******************************************************************
{
public:
    template <int size>
    constexpr string_literal( char const (&str)[size] ) noexcept
    :   c_string_const_ref{str, size - 1 /* NULL */}
    {}

private:
    constexpr string_literal(
        char const *str, int size ) noexcept
    :   c_string_const_ref{str, size}
    {}

    friend /*constexpr*/ string_literal string_literals::operator""_s(
        char const* str, size_t size ) noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class c_wstring_const_ref
    : public wstring_const_ref
// ******************************************************************
//  Guarantees that the wstring is actually
//  a null-terminated C-wstring.
// ******************************************************************
{
public:
    constexpr c_wstring_const_ref( void ) noexcept
    :   wstring_const_ref{L"", 0}
    {}

    //  Default copy ctor is OK - a reference can
    //  be initialized from another reference.
    c_wstring_const_ref( c_wstring_const_ref const& ) = default;

    //  Default move ctor is NOT OK - our default
    //  constructed state is a bit different than
    //  that of the base class.
    c_wstring_const_ref( c_wstring_const_ref&& b ) noexcept
    :   wstring_const_ref{ali::move(b)}
    {
        ali_assert(b._size == 0);
        ali_assert(b._begin != nullptr);
        ali_assert(*b._begin == L'\0');
            //  wstring_const_ref knows how to
            //  move from c_wstring_const_ref.
    }

    template <typename D>
    c_wstring_const_ref( exposes_wstring_const_ref<D> const& b ) noexcept
    :   c_wstring_const_ref{b.ref()}
    {}

    explicit c_wstring_const_ref( ali::wchar const* str ) noexcept
    :   wstring_const_ref{str, str != nullptr ? str_len(str) : 0}
    {
        ali_assert(str != nullptr);
    }

    constexpr c_wstring_const_ref( ali::wchar const* str, int size ) noexcept
    :   wstring_const_ref{str, size}
    {
        ali_constexpr_assert(str != nullptr);
        ali_constexpr_assert(str[size] == L'\0');
    }

    c_wstring_const_ref( c_wstring_const_ref b, int pos ) noexcept
    :   wstring_const_ref(
            ALI_IF_DEBUG_WEAK_REF(b._wref,)
            b.data() + pos,
            b.size() - pos)
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b.size());
    }

ALI_IF_DEBUG_WEAK_REF(
    c_wstring_const_ref(
        debug::weak_reference const& wref,
        ali::wchar const* str ) noexcept
    :   wstring_const_ref{wref, str, str != nullptr ? str_len(str) : 0}
    {
        ali_assert(str != nullptr);
    }

    c_wstring_const_ref(
        debug::weak_reference const& wref,
        ali::wchar const* str, int size ) noexcept
    :   wstring_const_ref{wref, str, size}
    {
        ali_constexpr_assert(str != nullptr);
        ali_constexpr_assert(str[size] == L'\0');
    }

)   //  ALI_IF_DEBUG_WEAK_REF

    
    //  reference

    wstring_const_ref ref( int pos, int n ) const noexcept
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    {
        return wstring_const_ref::ref(pos, n);
    }

    c_wstring_const_ref ref( void ) const noexcept
    {
        return c_cast(wstring_const_ref::ref());
    }

    c_wstring_const_ref ref_right( int pos ) const noexcept
        //  pre:    0 <= pos && pos <= size()
    {
        return c_cast(wstring_const_ref::ref_right(pos));
    }

    c_wstring_const_ref ref_back( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return c_cast(wstring_const_ref::ref_back(n));
    }

    c_wstring_const_ref ref_not_front( int n ) const noexcept
        //  pre:    0 <= n && n <= size()
    {
        return c_cast(wstring_const_ref::ref_not_front(n));
    }

    
    //  pointer

    array_const_ptr<ali::wchar> pointer( int pos, int n ) const noexcept;
        //  pre:    0 <= pos && pos <= size()
        //      &&  0 <= n && n <= size() - pos
    c_wstring_const_ptr pointer( void ) const noexcept;
    c_wstring_const_ptr pointer_right( int pos ) const noexcept;
        //  pre:    0 <= pos && pos <= size()
    c_wstring_const_ptr pointer_back( int n ) const noexcept;
        //  pre:    0 <= n && n <= size()
    c_wstring_const_ptr pointer_not_front( int n ) const noexcept;
        //  pre:    0 <= n && n <= size()

    ali::wchar const* c_str( void ) const noexcept
    {
        return data();
    }

    
    //  trim_left_if

    template <typename predicate>
    c_wstring_const_ref trim_left_if( predicate&& p ) const noexcept(
        noexcept(c_cast(wstring_const_ref::trim_left_if(
            ali::forward<predicate>(p)))))
        //  post:   result.is_empty()
        //      ||  !predicate(result.front())
    {
        return c_cast(wstring_const_ref::trim_left_if(
            ali::forward<predicate>(p)));
    }


    //  trim_left

    template <typename transform>
    c_wstring_const_ref trim_left( wstring_const_ref set, transform t ) const noexcept(
        noexcept(c_cast(wstring_const_ref::trim_left(ali::move(set), t))))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front(), t)
    {
        return c_cast(wstring_const_ref::trim_left(ali::move(set), t));
    }

    c_wstring_const_ref trim_left( void ) const noexcept(
        noexcept(c_cast(wstring_const_ref::trim_left())))
        //  post:   result.is_empty()
        //      ||  !is_space(result.front())
    {
        return c_cast(wstring_const_ref::trim_left());
    }

    c_wstring_const_ref trim_left( wstring_const_ref set ) const noexcept(
        noexcept(c_cast(wstring_const_ref::trim_left(ali::move(set)))))
        //  post:   result.is_empty()
        //      ||  !set.contains(result.front())
    {
        return c_cast(wstring_const_ref::trim_left(ali::move(set)));
    }

    c_wstring_const_ref nocase_trim_left( wstring_const_ref set ) const noexcept(
        noexcept(c_cast(wstring_const_ref::nocase_trim_left(ali::move(set)))))
        //  post:   result.is_empty()
        //      ||  !set.nocase_contains(result.front())
    {
        return c_cast(wstring_const_ref::nocase_trim_left(ali::move(set)));
    }


private:    //  Methods
    c_wstring_const_ref& operator=(
        c_wstring_const_ref const& ) = delete;

    static c_wstring_const_ref c_cast(
        wstring_const_ref ref ) noexcept
    {
        return c_wstring_const_ref{ref._begin, ref._size};
    }

    static c_wstring_const_ptr c_cast(
        array_const_ptr<ali::wchar> ptr ) noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring_literal : public c_wstring_const_ref
// ******************************************************************
{
public:
    template <int size>
    constexpr wstring_literal( ali::wchar const (&str)[size] ) noexcept
    :   c_wstring_const_ref{str, size - 1 /* NULL */}
    {}

private:
    constexpr wstring_literal(
        ali::wchar const *str, int size ) noexcept
    :   c_wstring_const_ref{str, size}
    {}

    friend /*constexpr*/ wstring_literal string_literals::operator""_s(
        ali::wchar const* str, size_t size ) noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_ptr
// ******************************************************************
{
public:
    constexpr array_ptr( ali::nullptr_type = nullptr ) noexcept {};

    //  Default copy & move ctor is OK.
    array_ptr( array_ptr const& ) = default;
    array_ptr( array_ptr&& ) = default;

    constexpr array_ptr( T* begin, int size ) noexcept
    :   _ref{begin, size}
    {}

    template <int n>
    constexpr array_ptr( T (&begin)[n] ) noexcept
    :   _ref{begin}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_ptr(
        debug::weak_reference const& wref,
        T* begin, int size ) noexcept
    :   _ref{wref, begin, size}
    {}

    template <int n>
    array_ptr(
        debug::weak_reference const& wref,
        T (&begin)[n] ) noexcept
    :   _ref{wref, begin}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_ptr& operator=( array_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_ptr& operator=( array_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_ref<T> const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    array_ref<T> const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    array_ptr& operator++( void ) noexcept
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    array_ptr operator++( int ) noexcept
    {
        array_ptr const result(*this);

        ++(*this);

        return result;
    }

    array_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend array_ptr operator+( array_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( array_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap( array_ptr& a, array_ptr& b ) noexcept
    {
        a.swap(b);
    }

private:    //  Data members
    array_ref<T>    _ref;

    template <typename U>
    friend class array_const_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_const_ptr
// ******************************************************************
{
public:     //  Methods
    constexpr array_const_ptr( ali::nullptr_type = nullptr ) noexcept {}

    //  Default copy & move ctor is OK.
    array_const_ptr( array_const_ptr const& ) = default;
    array_const_ptr( array_const_ptr&& ) = default;

    constexpr array_const_ptr( T const* begin, int size ) noexcept
    :   _ref{begin, size}
    {}

    template <int n>
    constexpr array_const_ptr( T const (&begin)[n] ) noexcept
    :   _ref{begin}
    {}

    constexpr array_const_ptr( array_ptr<T> b ) noexcept
    :   _ref{ali::move(b._ref)}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ptr(
        debug::weak_reference const& wref,
        T const* begin, int size ) noexcept
    :   _ref{wref, begin, size}
    {}

    template <int n>
    array_const_ptr(
        debug::weak_reference const& wref,
        T const (&begin)[n] ) noexcept
    :   _ref{wref, begin}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_const_ptr& operator=( array_ptr<T> const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_ptr<T>&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_const_ref<T> const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    array_const_ref<T> const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    array_const_ptr& operator++( void ) noexcept
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    array_const_ptr operator++( int ) noexcept
    {
        array_const_ptr const result(*this);

        ++(*this);

        return result;
    }

    array_const_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( array_const_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap( array_const_ptr& a, array_const_ptr& b ) noexcept
    {
        a.swap(b);
    }

private:    //  Data members
    array_const_ref<T>  _ref;
};

// ******************************************************************
// ******************************************************************

// NULL-terminated string const pointer
// Can't be nullptr because it needs at least one byte: the
// terminating NULL
// ******************************************************************
class c_string_const_ptr
// ******************************************************************
{
public:     //  Methods
    constexpr c_string_const_ptr( void ) = default;

    //  Default copy & move ctor is OK.
    c_string_const_ptr( c_string_const_ptr const& ) = default;
    c_string_const_ptr( c_string_const_ptr&& ) = default;

    explicit c_string_const_ptr( char const* str ) noexcept
    :   _ref{str}
    {}

    constexpr c_string_const_ptr( char const* str, int size ) noexcept
    :   _ref{str, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    c_string_const_ptr(
        debug::weak_reference const& wref,
        char const* str ) noexcept
    :   _ref{wref, str}
    {}

    c_string_const_ptr(
        debug::weak_reference const& wref,
        char const* str, int size ) noexcept
    :   _ref{wref, str, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    c_string_const_ptr& operator=( c_string_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    c_string_const_ptr& operator=( c_string_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = "";

        b._ref._size = 0;

        return *this;
    }

    c_string_const_ref const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    c_string_const_ref const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    c_string_const_ptr& operator++( void ) noexcept
    {
        ali_assert(!_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    c_string_const_ptr operator++( int ) noexcept
    {
        c_string_const_ptr const result(*this);

        ++(*this);

        return result;
    }

    c_string_const_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend c_string_const_ptr operator+(
        c_string_const_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( c_string_const_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap( c_string_const_ptr& a, c_string_const_ptr& b ) noexcept
    {
        a.swap(b);
    }

private:    //  Data members
    c_string_const_ref  _ref{};

    template <typename U>
    friend class array_const_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_literal_ptr
    : public c_string_const_ptr
// ******************************************************************
{
public:
    template <int size>
    constexpr string_literal_ptr( char const (&str)[size] ) noexcept
    :   c_string_const_ptr{str, size - 1 /* NULL */}
    {}

private:
    constexpr string_literal_ptr(
        char const *str, int size ) noexcept
    :   c_string_const_ptr{str, size}
    {}

    friend /*constexpr*/ string_literal_ptr string_literals::operator""_sp(
        char const* str, size_t size ) noexcept;
};


// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ptr<char>
// ******************************************************************
//  A.k.a. string_const_ptr.
// ******************************************************************
{
public:     //  Methods
    constexpr array_const_ptr( ali::nullptr_type = nullptr ) noexcept {}

    //  Default copy & move ctor is OK.
    array_const_ptr( array_const_ptr const& ) = default;
    array_const_ptr( array_const_ptr&& ) = default;

    constexpr array_const_ptr( char const* str, int size ) noexcept
    :   _ref{str, size}
    {}

    /*constexpr*/ array_const_ptr( array_ptr<char> const& b ) noexcept
    :   _ref{b._ref}
    {}

    /*constexpr*/ array_const_ptr( array_ptr<char>&& b ) noexcept
    :   _ref{ali::move(b._ref)}
    {}

    array_const_ptr( c_string_const_ptr const& b ) noexcept
    :   _ref{b._ref}
    {}

    array_const_ptr( c_string_const_ptr&& b ) noexcept
    :   _ref{ali::move(b._ref)}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ptr(
        debug::weak_reference const& wref,
        char const* str, int size ) noexcept
    :   _ref{wref, str, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_const_ptr& operator=( array_ptr<char> const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_ptr<char>&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_const_ptr& operator=( c_string_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( c_string_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = "";

        b._ref._size = 0;

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }
    
    array_const_ref<char> const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    array_const_ref<char> const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    array_const_ref<char> operator|( array_const_ref<char> other ) const noexcept
    {
        return this->is_null() ? other : this->_ref;
    }

    array_const_ptr operator|( array_const_ptr other ) const noexcept
    {
        return this->is_null() ? other : *this;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    array_const_ptr& operator++( void ) noexcept
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    array_const_ptr operator++( int ) noexcept
    {
        array_const_ptr const result(*this);

        ++(*this);

        return result;
    }

    array_const_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( array_const_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap( array_const_ptr& a, array_const_ptr& b ) noexcept
    {
        a.swap(b);
    }


private:    //  Data members
    array_const_ref<char>   _ref;
};

// ******************************************************************
// ******************************************************************

// NULL-terminated wide string const pointer
// Can't be nullptr because it needs at least one code point: the
// terminating NULL
// ******************************************************************
class c_wstring_const_ptr
// ******************************************************************
{
public:     //  Methods
    constexpr c_wstring_const_ptr( void ) = default;

    //  Default copy & move ctor is OK.
    c_wstring_const_ptr( c_wstring_const_ptr const& ) = default;
    c_wstring_const_ptr( c_wstring_const_ptr&& ) = default;

    explicit c_wstring_const_ptr( ali::wchar const* str ) noexcept
    :   _ref{str}
    {}

    constexpr c_wstring_const_ptr( ali::wchar const* str, int size ) noexcept
    :   _ref{str, size}
    {}

ALI_IF_DEBUG_WEAK_REF(
    c_wstring_const_ptr(
        debug::weak_reference const& wref,
        ali::wchar const* str ) noexcept
    :   _ref{wref, str}
    {}

    c_wstring_const_ptr(
        debug::weak_reference const& wref,
        ali::wchar const* str, int size ) noexcept
    :   _ref{wref, str, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    c_wstring_const_ptr& operator=( c_wstring_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    c_wstring_const_ptr& operator=( c_wstring_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = L"";

        b._ref._size = 0;

        return *this;
    }

    c_wstring_const_ref const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    c_wstring_const_ref const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    c_wstring_const_ptr& operator++( void ) noexcept
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    c_wstring_const_ptr operator++( int ) noexcept
    {
        c_wstring_const_ptr const result(*this);

        ++(*this);

        return result;
    }

    c_wstring_const_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend c_wstring_const_ptr operator+(
        c_wstring_const_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( c_wstring_const_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap( c_wstring_const_ptr& a, c_wstring_const_ptr& b ) noexcept
    {
        a.swap(b);
    }

private:    //  Data members
    c_wstring_const_ref  _ref{};

    template <typename U>
    friend class array_const_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring_literal_ptr : public c_wstring_const_ptr
// ******************************************************************
{
public:
    template <int size>
    constexpr wstring_literal_ptr( ali::wchar const (&str)[size] ) noexcept
    :   c_wstring_const_ptr{str, size - 1 /* NULL */}
    {}

private:
    constexpr wstring_literal_ptr(
        ali::wchar const *str, int size ) noexcept
    :   c_wstring_const_ptr{str, size}
    {}

    friend /*constexpr*/ wstring_literal_ptr string_literals::operator""_sp(
        ali::wchar const* str, size_t size ) noexcept;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class array_const_ptr<ali::wchar>
// ******************************************************************
//  A.k.a. wstring_const_ptr.
// ******************************************************************
{
public:     //  Methods
    constexpr array_const_ptr( ali::nullptr_type = nullptr ) noexcept {}

    //  Default copy & move ctor is OK.
    array_const_ptr( array_const_ptr const& ) = default;
    array_const_ptr( array_const_ptr&& ) = default;

    constexpr array_const_ptr( ali::wchar const* str, int size ) noexcept
    :   _ref{str, size}
    {}

    /*constexpr*/ array_const_ptr( array_ptr<ali::wchar> const& b ) noexcept
    :   _ref{b._ref}
    {}

    /*constexpr*/ array_const_ptr( array_ptr<ali::wchar>&& b ) noexcept
    :   _ref{ali::move(b._ref)}
    {}

    /*constexpr*/ array_const_ptr( c_wstring_const_ptr const& b ) noexcept
    :   _ref{b._ref}
    {}

    /*constexpr*/ array_const_ptr( c_wstring_const_ptr&& b ) noexcept
    :   _ref{ali::move(b._ref)}
    {}

ALI_IF_DEBUG_WEAK_REF(
    array_const_ptr(
        debug::weak_reference const& wref,
        ali::wchar const* str, int size ) noexcept
    :   _ref{wref, str, size}
    {}

)   //  ALI_IF_DEBUG_WEAK_REF

    array_const_ptr& operator=( array_ptr<ali::wchar> const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_ptr<ali::wchar>&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_const_ptr& operator=( c_wstring_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( c_wstring_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = L"";

        b._ref._size = 0;

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr const& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = b._ref._wref);

        return *this;
    }

    array_const_ptr& operator=( array_const_ptr&& b ) noexcept
    {
        this->_ref._size = b._ref._size;

        this->_ref._begin = b._ref._begin;

        ALI_IF_DEBUG_WEAK_REF(
            this->_ref._wref = ali::move(b._ref._wref));

        b._ref._begin = nullptr;

        b._ref._size = 0;

        return *this;
    }

    array_const_ref<ali::wchar> const* operator->( void ) const noexcept
    {
        return &this->_ref;
    }

    array_const_ref<ali::wchar> const& operator*( void ) const noexcept
    {
        return this->_ref;
    }

    array_const_ref<ali::wchar> operator|( array_const_ref<ali::wchar> other ) const noexcept
    {
        return this->is_null() ? other : this->_ref;
    }

    array_const_ptr operator|( array_const_ptr other ) const noexcept
    {
        return this->is_null() ? other : *this;
    }

    bool is_null( void ) const noexcept
    {
        return this->_ref._begin == nullptr;
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    array_const_ptr& operator++( void ) noexcept
    {
        ali_assert(!this->_ref.is_empty());

        --this->_ref._size;

        ++this->_ref._begin;

        return *this;
    }

    array_const_ptr operator++( int ) noexcept
    {
        array_const_ptr const result(*this);

        ++(*this);

        return result;
    }

    array_const_ptr& operator+=( int n ) noexcept
    {
        ali_assert(0 <= n);
        ali_assert(n <= this->_ref._size);

        this->_ref._size -= n;

        this->_ref._begin += n;

        return *this;
    }

    friend array_const_ptr operator+(
        array_const_ptr a, int n ) noexcept
    {
        a += n;

        return a;
    }

    void swap( array_const_ptr& b ) noexcept
    {
        ali::adl_swap(this->_ref._begin, b._ref._begin);

        ali::adl_swap(this->_ref._size, b._ref._size);

        ALI_IF_DEBUG_WEAK_REF(
            ali::adl_swap(this->_ref._wref, b._ref._wref));
    }

    friend void swap(
        array_const_ptr& a,
        array_const_ptr& b ) noexcept
    {
        a.swap(b);
    }

private:    //  Data members
    array_const_ref<ali::wchar> _ref;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  INLINES
// ******************************************************************

namespace hidden
{

// ******************************************************************
inline int array_const_ref_compare(
    array_const_ref<unsigned char> a,
    array_const_ref<unsigned char> b,
    functor_types::identity ) noexcept
// ******************************************************************
//  Can use memcmp.
// ******************************************************************
{
    int const n{ali::mini(a.size(), b.size())};

    int result{n != 0
        ? platform::memcmp(a.data(), b.data(), n)
        : 0};

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

//
//  BEWARE:
//  While we would like to use memcmp for the following two
//  specializations as well, it unfortunately gives incorrect
//  results when the arrays contain negative values.
//  It's because the memcmp function interprets the data
//  as unsigned chars.
//
//  Example: (signed char)-1 < 0, but (unsigned char)-1 = 255 > 0
//
//  We cannot use strncmp either, because it stops at the first
//  zero it encounters.
//
//  Having said that, we are going to use memcmp for chars
//  as there is little harm in doing so, but not for the
//  signed chars.
//

// ******************************************************************
inline int array_const_ref_compare(
    array_const_ref<char> a,
    array_const_ref<char> b,
    functor_types::identity ) noexcept
// ******************************************************************
//  Using memcmp does little harm.
// ******************************************************************
{
    int const n{ali::mini(a.size(), b.size())};

    int result{n != 0
        ? platform::memcmp(a.data(), b.data(), n)
        : 0};

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

// ******************************************************************
inline int array_const_ref_compare(
    array_const_ref<unsigned char> a,
    array_const_ref<char> b,
    functor_types::identity ) noexcept
// ******************************************************************
//  Using memcmp does little harm.
// ******************************************************************
{
    int const n{ali::mini(a.size(), b.size())};

    int result{n != 0
        ? platform::memcmp(a.data(), b.data(), n)
        : 0};

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

// ******************************************************************
inline int array_const_ref_compare(
    array_const_ref<char> a,
    array_const_ref<unsigned char> b,
    functor_types::identity ) noexcept
// ******************************************************************
//  Using memcmp does little harm.
// ******************************************************************
{
    int const n{ali::mini(a.size(), b.size())};

    int result{n != 0
        ? platform::memcmp(a.data(), b.data(), n)
        : 0};

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

// ******************************************************************
//int array_const_ref_compare(
//    array_const_ref<signed char> a,
//    array_const_ref<signed char> b,
//    functor_types::identity t ) const noexcept;
// ******************************************************************
//  BEWARE: Using memcmp would give INCORRECT results!
// ******************************************************************

// ******************************************************************
inline int array_const_ref_compare(
    array_const_ref<ali::wchar> a,
    array_const_ref<ali::wchar> b,
    functor_types::identity ) noexcept
// ******************************************************************
//  Can use wmemcmp.
// ******************************************************************
{
    int const n{ali::mini(a.size(), b.size())};

    int result{n != 0
        ? platform::wmemcmp(a.data(), b.data(), n)
        : 0};

    if ( result == 0 )
        result = ali::adl_compare(a.size(), b.size());

    return result;
}

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size - pos);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin + pos, n};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref( void ) const noexcept
//// ******************************************************************
//{
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin, this->_size};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_left( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin, pos};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin + pos, this->_size - pos};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin, n};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin + (this->_size - n), n};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin + n, this->_size - n};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_const_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin, this->_size - n};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_not_front_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size() / 2
//// ******************************************************************
//{
//    return this->ref_not_front(n).ref_not_back(n);
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::ref_not_front_not_back( int nf, int nb ) const noexcept
//// ******************************************************************
////  pre:    0 <= fn && nf <= size()
////      &&  0 <= nb && nb <= size() - nf
//// ******************************************************************
//{
//    return this->ref_not_front(nf).ref_not_back(nb);
//}

//  array_const_ref_common<T>::pointer

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    return this->ref(pos, n).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer( void ) const noexcept
//// ******************************************************************
//{
//    return array_const_ptr<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->_begin, this->_size};
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_left( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return this->ref_left(pos).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return this->ref_right(pos).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->ref_front(n).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->ref_back(n).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->ref_not_front(n).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->ref_not_back(n).pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_not_front_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size() / 2
//// ******************************************************************
//{
//    return this->ref_not_front(n).pointer_not_back(n);
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ptr<T> array_const_ref_common<T>
//    ::pointer_not_front_not_back( int nf, int nb ) const noexcept
//// ******************************************************************
////  pre:    0 <= fn && nf <= size()
////      &&  0 <= nb && nb <= size() - nf
//// ******************************************************************
//{
//    return this->ref_not_front(nf).pointer_not_back(nb);
//}

//// ******************************************************************
//template <typename T>
//template <typename body>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::for_each( body&& b ) const noexcept(
//        noexcept(b(this->at(0))))
//// ******************************************************************
//{
//    for ( int i = this->_size; i != 0; )
//        b(this->at(--i));
//
//    return this->ref();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//inline bool array_const_ref_common<T>::begins_with_n(
//    array_const_ref<U> b, transform t ) const noexcept(
//        noexcept(this->ref_front(b._size).is_equal_to(b, t)))
//// ******************************************************************
//{
//    return  this->_size >= b._size
//        &&  this->ref_front(b._size).is_equal_to(b, t);
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//inline bool array_const_ref_common<T>::ends_with_n(
//    array_const_ref<U> b, transform t ) const noexcept(
//        noexcept(this->ref_back(b._size).is_equal_to(b, t)))
//// ******************************************************************
//{
//    return  this->_size >= b._size
//        &&  this->ref_back(b._size).is_equal_to(b, t);
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename predicate>
//inline array_const_ref<T> array_const_ref_common<T>
//    ::trim_left_if( predicate p ) const
//// ******************************************************************
//{
//    return this->ref_right(this->index_of_first_if_not(p));
//}

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::trim_left(
//    array_const_ref<U> set, transform t ) const
//// ******************************************************************
//{
//    return this->ref_right(this->index_of_first_not_of(set, t));
//}

//// ******************************************************************
//template <typename T>
//template <typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::trim_left(
//    array_const_ref<T> set, transform t ) const
//// ******************************************************************
//{
//    return this->trim_left<T, transform>(set, t);
//}

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_const_ref<T> array_const_ref_common<T>::trim_left(
//    array_const_ref<U> set ) const
//// ******************************************************************
//{
//    return this->trim_left(set, ali::identity);
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>::trim_left(
//    array_const_ref<T> set ) const
//// ******************************************************************
//{
//    return this->trim_left<T>(set);
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename predicate>
//array_const_ref<T> array_const_ref_common<T>::trim_right_if(
//    predicate p ) const
//// ******************************************************************
//{
//    int trimmed_size = this->_size;
//
//    while ( trimmed_size != 0 && bool{p(this->at(trimmed_size - 1))} )
//        --trimmed_size;
//
//    return this->ref_front(trimmed_size);
//}

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//array_const_ref<T> array_const_ref_common<T>::trim_right(
//    array_const_ref<U> set, transform t ) const
//// ******************************************************************
//{
//    int trimmed_size = this->_size;
//
//    if ( !set.is_empty() )
//        while ( trimmed_size != 0 && set.contains(this->at(trimmed_size - 1), t) )
//            --trimmed_size;
//
//    return this->ref_front(trimmed_size);
//}

//// ******************************************************************
//template <typename T>
//template <typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::trim_right(
//    array_const_ref<T> set, transform t ) const
//// ******************************************************************
//{
//    return this->trim_right<T, transform>(set, t);
//}

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_const_ref<T> array_const_ref_common<T>::trim_right(
//    array_const_ref<U> set ) const
//// ******************************************************************
//{
//    return this->trim_right(set, ali::identity);
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>::trim_right(
//    array_const_ref<T> set ) const
//// ******************************************************************
//{
//    return this->trim_right<T>(set);
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename predicate>
//inline array_const_ref<T> array_const_ref_common<T>::trim_if(
//    predicate p ) const
//// ******************************************************************
//{
//    return this->trim_left_if(p).trim_right_if(p);
//}

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::trim(
//    array_const_ref<U> set, transform t ) const
//// ******************************************************************
//{
//    return this->trim_left(set, t).trim_right(set, t);
//}

//// ******************************************************************
//template <typename T>
//template <typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::trim(
//    array_const_ref<T> set, transform t ) const
//// ******************************************************************
//{
//    return this->trim<T, transform>(set, t);
//}

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_const_ref<T> array_const_ref_common<T>::trim(
//    array_const_ref<U> set ) const
//// ******************************************************************
//{
//    return this->trim(set, ali::identity);
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>::trim(
//    array_const_ref<T> set ) const
//// ******************************************************************
//{
//    return this->trim<T>(set);
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::common_prefix(
//    array_const_ref<U> b, transform t ) const
//// ******************************************************************
//{
//    return this->ref_left(this->index_of_difference(b, t));
//}

//// ******************************************************************
//template <typename T>
//template <typename transform>
//inline array_const_ref<T> array_const_ref_common<T>::common_prefix(
//    array_const_ref<T> b, transform t ) const
//// ******************************************************************
//{
//    return this->ref_left(this->index_of_difference(b, t));
//}

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_const_ref<T> array_const_ref_common<T>::common_prefix(
//    array_const_ref<U> b ) const
//// ******************************************************************
//{
//    return this->ref_left(this->index_of_difference(b));
//}

//// ******************************************************************
//template <typename T>
//inline array_const_ref<T> array_const_ref_common<T>::common_prefix(
//    array_const_ref<T> b ) const
//// ******************************************************************
//{
//    return this->ref_left(this->index_of_difference(b));
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U, typename transform>
//int array_const_ref_common<T>::count_n(
//    array_const_ref<U> b, transform t ) const
//// ******************************************************************
////  Returns the number of non-overlapping
////  occurrences of b inside this array.
//// ******************************************************************
//{
//    int n = 0;
//
//    array_const_ptr<T> ptr = this->pointer();
//
//    while ( !(ptr += ptr->index_of_first_n(b, t))->is_empty() )
//        ++n, ptr += b.size();
//
//    return n;
//}

// ******************************************************************
template <typename T>
template <typename U, typename transform>
bool array_const_ref_common<T>::is_wildcard_match(
    array_const_ref<U> pattern,
    U qm, U star, transform t ) const noexcept
// ******************************************************************
//  Adapted from:
//  http://developforperformance.com/MatchingWildcards_AnImprovedAlgorithmForBigData.html
// ******************************************************************
{
    array_const_ptr<T> s{this->pointer()};
    array_const_ptr<U> p{pattern.pointer()};

    array_const_ptr<T> sSeq{};
        //  Points to prospective string match.

    array_const_ptr<U> pSeq{};
        //  Points to prospective pattern match after '*'.

    //  Find first wildcard, if one exists, and the beginning
    //  of any prospectively matching sequence after it.

    for ( ;; )
    {
        //  Check for the end from the start.
        //  Get out fast, if possible.

        if ( s->is_empty() )
        {
            if ( p->is_empty() )
            {
                return true;
                    //  "abc" matches "abc".
            }
            else
            {
                while ( bool{p->front() == star} )
                {
                    ++p;

                    if ( p->is_empty() )
                    {
                        return true;
                            //  "ab" matches "ab*".
                    }
                }

                return false;
                    //  "abcd" doesn't match "abc".
            }
        }
        else if ( !p->is_empty() && bool{p->front() == star} )
        {
            //  Got wild: set up for the second loop and skip on down there.

            do
            {
                ++p;

                if ( p->is_empty() )
                {
                    return true;
                        //  "abc*" matches "abcd".
                }
            }
            while ( bool{p->front() == star} );

            //  Search for the next prospective match.
            
            if ( bool{p->front() != qm} )
            {
                while ( bool{t(p->front()) != t(s->front())} )
                {
                    ++s;

                    if ( s->is_empty() )
                    {
                        return false;
                            //  "a*bc" doesn't match "ab".
                    }
                }
            }

            //  Keep fallback positions for retry in case of incomplete match.
            
            sSeq = s;
            pSeq = p;

            break;
        }
        //else if (*pWild != *pTame && *pWild != '?')
        //[
        //    (a != b) === !(a == b)
        //]
        //else if ( !(*pWild == *pTame) && !(*pWild == '?') )
        //[
        //    (*pWild == *pTame) ===  both pointers point to the end
        //                            or neither pointer points to the end
        //                            and the characters they point to are equal
        //]
        //else if (   !(  (p->is_empty() && s->is_empty())
        //                    //  both pointers point to the end
        //            ||  (   !p->is_empty() && !s->is_empty()
        //                &&  bool{t(p->front()) == t(s->front())}))
        //                        //  neither pointer points to the end
        //                        //  and the characters they point to are equal
        //
        //        &&  (p->is_empty() || bool{p->front() != qm}) )
        //[
        //  s->is_empty() === false
        //]
        else if (   p->is_empty()
                ||  (   bool{t(p->front()) != t(s->front())}
                    &&  bool{p->front() != qm}) )

        {
            return false;
                //  "abc" doesn't match "abd".
        }

        //  Everything's a match, so far.

        ++s;
        ++p;
    }

    //  Find any further wildcards and any further matching sequences.

    for ( ;; )
    {
        if ( !p->is_empty() && bool{p->front() == star} )
        {
            //  Got wild again.
            
            do
            {
                ++p;

                if ( p->is_empty() )
                {
                    return true;
                        //  "ab*c*" matches "abcd".
                }
            }
            while ( bool{p->front() == star} );

            if ( s->is_empty() )
            {
                return false;
                    //  "*bcd*" doesn't match "abc".
            }

            //  Search for the next prospective match.

            if ( bool{p->front() != qm} )
            {
                while ( bool{t(p->front()) != t(s->front())} )
                {
                    ++s;

                    if ( s->is_empty() )
                    {
                        return false;
                            //  "a*b*c" doesn't match "ab".
                    }
                }
            }

            //  Keep the new fallback positions.
            
            sSeq = s;
            pSeq = p;
        }
        //else if (*pWild != *pTame && *pWild != '?')
        
        //else if ( !(*pWild == *pTame) && !(*pWild == '?') )
        
        else if (   !(  (p->is_empty() && s->is_empty())
                            //  *pWild == *pTame (== '\0')
                    ||  (   !p->is_empty() && !s->is_empty()
                        &&  bool{t(p->front()) == t(s->front())}))
                                //  *pWild == *pTame (!= '\0')
        
                &&  (p->is_empty() || bool{p->front() != qm}) )
        {
            //  The equivalent portion of the upper loop is really simple.
            
            if ( s->is_empty() )
            {
                return false;
                    //  "*bcd" doesn't match "abc".
            }

            //  A fine time for questions.

            while ( !pSeq->is_empty() && bool{pSeq->front() == qm} )
            {
                ++sSeq;
                ++pSeq;
            }

            p = pSeq;

            //  Fall back, but never so far again.

            //while ( *pWild != *++pTameSequence )

            //while ( !(*pWild == *++pTameSequence) )

            //while ( ++pTameSequence, !(*pWild == *pTameSequence) )

            while ( ++sSeq,
                        !(  (p->is_empty() && sSeq->is_empty())
                    ||  (   !p->is_empty() && !sSeq->is_empty()
                        &&  bool{t(p->front()) == t(sSeq->front())})) )
            {
                if ( sSeq->is_empty() )
                {
                    return false;
                        //  "*a*b" doesn't match "ac".
                }
            }

            s = sSeq;
        }

        //  Another check for the end, at the end.

        if ( s->is_empty() )
        {
            if ( p->is_empty() )
            {
                return true;
                    //  "*bc" matches "abc".
            }
            else
            {
                return false;
                    //  "*bc" doesn't match "abcd".
            }
        }

        //  Everything's a match, so far.

        ++s;
        ++p;
    }
}

// ******************************************************************
template <typename T>
inline array_const_ref<ali::uint8>
    array_const_ref_common<T>::as_blob( void ) const noexcept
// ******************************************************************
{
    return array_const_ref<ali::uint8>{
        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
        reinterpret_cast<ali::uint8 const*>(this->_begin),
        this->_size * static_cast<int>(sizeof(T))};
}

// ******************************************************************
template <>
inline array_const_ref<ali::uint8>
    array_const_ref_common<ali::uint8>
        ::as_blob( void ) const noexcept
// ******************************************************************
{
    return this->ref();
}

//// ******************************************************************
//template <typename T>
//inline is_internal_pointer_result array_const_ref_common<T>
//    ::is_internal_pointer( array_const_ref<T> a ) const noexcept
//// ******************************************************************
//{
//    return this->is_internal_pointer(a.data());
//}

//// ******************************************************************
//template <typename T>
//inline is_internal_pointer_result array_const_ref_common<T>
//    ::is_internal_pointer( array_const_ptr<T> a ) const noexcept
//// ******************************************************************
//{
//    return this->is_internal_pointer(a->data());
//}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
ali::uint32 array_const_ref<ali::uint8>
    ::int_be_at_<ali::uint32>(
        int pos, int byte_count ) const noexcept;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************

// ******************************************************************
template <>
ali::uint32 array_const_ref<ali::uint8>
    ::int_le_at_<ali::uint32>(
        int pos, int byte_count ) const noexcept;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************

// ******************************************************************
template <>
ali::uint64 array_const_ref<ali::uint8>
    ::int_be_at_<ali::uint64>(
        int pos, int byte_count ) const noexcept;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
// ******************************************************************

// ******************************************************************
template <>
ali::uint64 array_const_ref<ali::uint8>
    ::int_le_at_<ali::uint64>(
        int pos, int byte_count ) const noexcept;
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
// ******************************************************************

// ******************************************************************
inline ali::uint32 array_const_ref<ali::uint8>::int_be_at(
    int pos, int byte_count ) const noexcept
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************
{
    return this->int_be_at_<ali::uint32>(pos, byte_count);
}

// ******************************************************************
inline ali::uint32 array_const_ref<ali::uint8>::int_le_at(
    int pos, int byte_count ) const noexcept
// ******************************************************************
// pre: byte_count in set {1, 2, 3, 4}
// ******************************************************************
{
    return this->int_le_at_<ali::uint32>(pos, byte_count);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<char>
    array_const_ref<ali::uint8>::as_string( void ) const noexcept
// ******************************************************************
{
    return array_const_ref<char>{
        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
        reinterpret_cast<char const*>(_begin), _size};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<char>::array_const_ref(
    c_string_const_ref&& b ) noexcept
// ******************************************************************
:   array_const_ref_common<char>{ali::move(b)}
{
    ali_assert(b._begin == nullptr);
    ali_assert(b._size == 0);
        //  Good, but this is NOT the default constructed
        //  state of *c_string_const_ref*.
    b._begin = "";
        //  This is it.
}

// ******************************************************************
inline is_internal_pointer_result array_const_ref<char>
    ::is_internal_pointer( c_string_const_ptr a ) const noexcept
// ******************************************************************
{
    return this->is_internal_pointer(a->data());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_const_ref<ali::wchar>::array_const_ref(
    c_wstring_const_ref&& b ) noexcept
// ******************************************************************
:   array_const_ref_common<ali::wchar>{ali::move(b)}
{
    ali_assert(b._begin == nullptr);
    ali_assert(b._size == 0);
        //  Good, but this is NOT the default constructed
        //  state of *c_wstring_const_ref*.
    b._begin = L"";
        //  This is it.
}

// ******************************************************************
inline is_internal_pointer_result array_const_ref<ali::wchar>
    ::is_internal_pointer( c_wstring_const_ptr a ) const noexcept
// ******************************************************************
{
    return this->is_internal_pointer(a->data());
}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size - pos);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data() + pos, n};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref( void ) const noexcept
//// ******************************************************************
//{
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data(), this->_size};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_left( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data(), pos};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= pos);
//    ali_assert(pos <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data() + pos, this->_size - pos};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data(), n};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data() + (this->_size - n), n};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data() + n, this->_size - n};
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> array_ref_common<T>
//    ::mutable_ref_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= n);
//    ali_assert(n <= this->_size);
//
//    return array_ref<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data(), this->_size - n};
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    return this->mutable_ref(pos, n).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer( void ) const noexcept
//// ******************************************************************
//{
//    return array_ptr<T>{
//        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
//        this->data(), this->_size};
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_left( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_left(pos).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_right(pos).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_front(n).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_back(n).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_not_front(n).mutable_pointer();
//}

//// ******************************************************************
//template <typename T>
//inline array_ptr<T> array_ref_common<T>
//    ::mutable_pointer_not_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return this->mutable_ref_not_back(n).mutable_pointer();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_ref<T> const& array_ref_common<T>::copy(
//    int pos, array_const_ref<U> b ) const noexcept(
//        noexcept(this->mutable_ref_right(pos).copy_front(b)))
//// ******************************************************************
//{
//    this->mutable_ref_right(pos).copy_front(b);
//
//    return this->self();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_ref<T> const& array_ref_common<T>::move(
//    int pos, array_ref<U> b ) const noexcept(
//        noexcept(this->mutable_ref_right(pos).move_front(b)))
//// ******************************************************************
//{
//    this->mutable_ref_right(pos).move_front(b);
//
//    return this->self();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename U>
//inline array_ref<T> const& array_ref_common<T>::move_if_noexcept(
//    int pos, array_ref<U> b ) const noexcept(
//        noexcept(this->mutable_ref_right(pos).move_front_if_noexcept(b)))
//// ******************************************************************
//{
//    this->mutable_ref_right(pos).move_front_if_noexcept(b);
//
//    return this->self();
//}

//// ******************************************************************
//template <typename T>
//template <typename operation>
//inline array_ref<T> const& array_ref_common<T>::combine(
//    int pos, array_const_ref<T> b, operation&& op ) const noexcept(
//        noexcept(this->mutable_ref_right(pos).combine_front(
//            b, ali::forward<operation>(op))))
//// ******************************************************************
//{
//    this->mutable_ref_right(pos).combine_front(
//        b, ali::forward<operation>(op));
//
//    return this->self();
//}

//// ******************************************************************
//template <typename T>
//template <typename operation>
//inline array_ref<T> const& array_ref_common<T>::combine_back(
//    array_const_ref<T> b, operation&& op ) const noexcept(
//        noexcept(this->mutable_ref_back(this->_size)
//            .combine_front(
//                b.ref_back(ali::mini(this->_size, b._size)),
//                ali::forward<operation>(op))))
//// ******************************************************************
//{
//    int const n = ali::mini(this->_size, b._size);
//
//    this->mutable_ref_back(n)
//        .combine_front(
//            b.ref_back(n),
//            ali::forward<operation>(op));
//
//    return this->self();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//inline array_ref<T> const& array_ref_common<T>::swap_block(
//    int pos, array_ref<T> b ) const
//// ******************************************************************
//{
//    this->mutable_ref_right(pos).swap_block_front(b);
//
//    return this->self();
//}

//// ******************************************************************
//template <typename T>
//inline array_ref<T> const& array_ref_common<T>::swap_block_back(
//    array_ref<T> b ) const
//// ******************************************************************
//{
//    int const n = ali::mini(this->_size, b._size);
//
//    this->mutable_ref_back(n)
//        .swap_block_front(
//            b.mutable_ref_back(n));
//
//    return this->self();
//}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//template <typename T>
//template <typename comparator>
//array_ref<T> const& array_ref_common<T>::quick_sort(
//    comparator&& compare ) const noexcept(
//            noexcept(this->mutable_pointer()
//                ->unstable_partition_around_pivot(0, compare))
//        &&  noexcept(this->mutable_pointer()
//                ->insertion_sort(compare)))
//// ******************************************************************
//{
//    constexpr int threshold{8};
//
//    array_ptr<T> ptr = this->mutable_pointer();
//
//    while ( ptr->size() > threshold )
//    {
//        int const i = ptr->unstable_partition_around_pivot(
//            ptr->size() / 2, compare);
//
//        array_ptr<T>
//            left = ptr->mutable_pointer_left(i),
//            right = ptr->mutable_pointer_right(i + 1);
//
//        //  Recurse to the smaller partition.
//
//        if ( left->size() < right->size() )
//        {
//            left->quick_sort(compare);
//
//            ptr = right;
//        }
//        else
//        {
//            right->quick_sort(compare);
//
//            ptr = left;
//        }
//    }
//
//    ptr->insertion_sort(compare);
//
//    return this->self();
//}

//// ******************************************************************
//template <typename T>
//template <typename comparator>
//array_ref<T> const& array_ref_common<T>::heap_sort(
//    comparator&& compare ) const noexcept(
//            noexcept(this->make_heap(compare))
//        &&  noexcept(this->mutable_ref_left(0)
//                .down_heap(0, compare))
//        &&  noexcept(ali::adl_swap(this->front(), this->at(0))))
//// ******************************************************************
//{
//    if ( this->_size < 2 )
//        ;   //  NOOP
//    else
//    {
//        this->make_heap(compare);
//
//        int right = this->_size;
//
//        ali::adl_swap(this->front(), this->at(--right));
//
//        while ( right > 1 )
//        {
//            this->mutable_ref_left(right)
//                .down_heap(0, compare);
//
//            ali::adl_swap(this->front(), this->at(--right));
//        }
//    }
//
//    return this->self();
//}

//// ******************************************************************
//template <typename T>
//template <typename comparator>
//array_ref<T> const& array_ref_common<T>::nth_element(
//    int nth, comparator&& compare ) const noexcept(
//            noexcept(this->mutable_pointer()
//                ->partition_by_median_guess(compare))
//        &&  noexcept(this->mutable_pointer()
//                ->insertion_sort(compare)))
//// ******************************************************************
////  pre:    0 <= nth && nth <= size()
//// ******************************************************************
//{
//    ali_assert(0 <= nth);
//    ali_assert(nth <= this->_size);
//
//    if ( nth == this->_size )
//        return this->self();
//
//    constexpr int insertion_sort_max{32};
//
//    array_ptr<T> ptr{this->mutable_pointer()};
//
//    while ( ptr->size() > insertion_sort_max )
//    {
//        pair<int, int> const mid{
//            ptr->partition_by_median_guess(compare)};
//
//        if ( mid.second <= nth )
//        {
//            ptr += mid.second;
//            nth -= mid.second;
//        }
//        else if ( mid.first <= nth )
//        {
//            //  Nth inside fat pivot, done.
//            
//            return this->self();
//        }
//        else
//        {
//            ptr = ptr->mutable_pointer_left(mid.first);
//        }
//    }
//
//    ptr->insertion_sort(compare);
//
//    return this->self();
//}

// ******************************************************************
template <typename T>
inline array_ref<ali::uint8>
    array_ref_common<T>::as_mutable_blob( void ) const noexcept
// ******************************************************************
{
    return array_ref<ali::uint8>{
        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
        reinterpret_cast<ali::uint8*>(this->data()),
        this->_size * static_cast<int>(sizeof(T))};
}

// ******************************************************************
template <>
inline array_ref<ali::uint8>
    array_ref_common<ali::uint8>::as_mutable_blob( void ) const noexcept
// ******************************************************************
{
    return this->mutable_ref();
}


// ******************************************************************
// ******************************************************************

//// ******************************************************************
//inline string_const_ref c_string_const_ref
//    ::ref( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    return string_const_ref::ref(pos, n);
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref
//    ::ref( void ) const noexcept
//// ******************************************************************
//{
//    return c_cast(string_const_ref::ref());
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref
//    ::ref_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return c_cast(string_const_ref::ref_right(pos));
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref
//    ::ref_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return c_cast(string_const_ref::ref_back(n));
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref
//    ::ref_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return c_cast(string_const_ref::ref_not_front(n));
//}

// ******************************************************************
inline array_const_ptr<char> c_string_const_ref
    ::pointer( int pos, int n ) const noexcept
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return string_const_ref::pointer(pos, n);
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer( void ) const noexcept
// ******************************************************************
{
    return c_cast(string_const_ref::pointer());
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_right( int pos ) const noexcept
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_right(pos));
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_back( int n ) const noexcept
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_back(n));
}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref
    ::pointer_not_front( int n ) const noexcept
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(string_const_ref::pointer_not_front(n));
}

//// ******************************************************************
//template <typename predicate>
//inline c_string_const_ref c_string_const_ref
//    ::trim_left_if( predicate p ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !predicate(result.front())
//// ******************************************************************
//{
//    return c_cast(string_const_ref::trim_left_if(p));
//}

//// ******************************************************************
//template <typename transform>
//inline c_string_const_ref c_string_const_ref::trim_left(
//    string_const_ref set, transform t ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.contains(result.front(), t)
//// ******************************************************************
//{
//    return c_cast(string_const_ref::trim_left(set, t));
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref
//    ::trim_left( void ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !is_space(result.front())
//// ******************************************************************
//{
//    return c_cast(string_const_ref::trim_left());
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref::trim_left(
//    string_const_ref set ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.contains(result.front())
//// ******************************************************************
//{
//    return c_cast(string_const_ref::trim_left(set));
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref::nocase_trim_left(
//    string_const_ref set ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.nocase_contains(result.front())
//// ******************************************************************
//{
//    return c_cast(string_const_ref::nocase_trim_left(set));
//}

//// ******************************************************************
//inline c_string_const_ref c_string_const_ref::c_cast(
//    string_const_ref ref ) noexcept
//// ******************************************************************
//{
//    return c_string_const_ref{
//        ALI_IF_DEBUG_WEAK_REF(ref._wref,)
//        ref._begin, ref._size};
//}

// ******************************************************************
inline c_string_const_ptr c_string_const_ref::c_cast(
    array_const_ptr<char> ptr ) noexcept
// ******************************************************************
{
    return c_string_const_ptr{
        ALI_IF_DEBUG_WEAK_REF(ptr->_wref,)
        ptr->_begin, ptr->_size};
}

// ******************************************************************
// ******************************************************************

//// ******************************************************************
//inline wstring_const_ref c_wstring_const_ref
//    ::ref( int pos, int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
////      &&  0 <= n && n <= size() - pos
//// ******************************************************************
//{
//    return wstring_const_ref::ref(pos, n);
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref
//    ::ref( void ) const noexcept
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::ref());
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref
//    ::ref_right( int pos ) const noexcept
//// ******************************************************************
////  pre:    0 <= pos && pos <= size()
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::ref_right(pos));
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref
//    ::ref_back( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::ref_back(n));
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref
//    ::ref_not_front( int n ) const noexcept
//// ******************************************************************
////  pre:    0 <= n && n <= size()
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::ref_not_front(n));
//}

// ******************************************************************
inline array_const_ptr<ali::wchar> c_wstring_const_ref
    ::pointer( int pos, int n ) const noexcept
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
//      &&  0 <= n && n <= size() - pos
// ******************************************************************
{
    return wstring_const_ref::pointer(pos, n);
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer( void ) const noexcept
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer());
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_right( int pos ) const noexcept
// ******************************************************************
//  pre:    0 <= pos && pos <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_right(pos));
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_back( int n ) const noexcept
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_back(n));
}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref
    ::pointer_not_front( int n ) const noexcept
// ******************************************************************
//  pre:    0 <= n && n <= size()
// ******************************************************************
{
    return c_cast(wstring_const_ref::pointer_not_front(n));
}

//// ******************************************************************
//template <typename predicate>
//inline c_wstring_const_ref c_wstring_const_ref
//    ::trim_left_if( predicate p ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !predicate(result.front())
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::trim_left_if(p));
//}

//// ******************************************************************
//template <typename transform>
//inline c_wstring_const_ref c_wstring_const_ref::trim_left(
//    wstring_const_ref set, transform t ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.contains(result.front(), t)
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::trim_left(set, t));
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref
//    ::trim_left( void ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !is_space(result.front())
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::trim_left());
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref::trim_left(
//    wstring_const_ref set ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.contains(result.front())
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::trim_left(set));
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref::nocase_trim_left(
//    wstring_const_ref set ) const
//// ******************************************************************
////  post:   result.is_empty()
////      ||  !set.nocase_contains(result.front())
//// ******************************************************************
//{
//    return c_cast(wstring_const_ref::nocase_trim_left(set));
//}

//// ******************************************************************
//inline c_wstring_const_ref c_wstring_const_ref::c_cast(
//    wstring_const_ref ref ) noexcept
//// ******************************************************************
//{
//    return c_wstring_const_ref(ref._begin, ref._size);
//}

// ******************************************************************
inline c_wstring_const_ptr c_wstring_const_ref::c_cast(
    array_const_ptr<ali::wchar> ptr ) noexcept
// ******************************************************************
{
    return c_wstring_const_ptr(ptr->_begin, ptr->_size);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline array_ref<char>
    array_ref<ali::uint8>::as_mutable_string( void ) const noexcept
// ******************************************************************
{
    return array_ref<char>{
        ALI_IF_DEBUG_WEAK_REF(this->_wref,)
        reinterpret_cast<char*>(
            this->data()), this->_size};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline int operator-(
    array_const_ptr<T> a,
    array_const_ptr<T> b ) noexcept
// ******************************************************************
//  pre: b->is_internal_pointer(a).distance_or_negative >= 0
// ******************************************************************
{
    auto const distance = b->is_internal_pointer(
        ali::move(a)).distance_or_negative;

    ali_assert(distance >= 0);

    return distance;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator==(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(a.is_equal_to(ali::move(b))))
// ******************************************************************
{
    return a.is_equal_to(ali::move(b));
}

// ******************************************************************
template <typename T, int N>
inline bool operator==(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(ali::move(a) == array_const_ref<T>{b}))
// ******************************************************************
{
    return ali::move(a) == array_const_ref<T>{b};
}

// ******************************************************************
template <typename T, typename D>
inline bool operator==(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(ali::move(a) == b.ref()))
// ******************************************************************
{
    return ali::move(a) == b.ref();
}

// ******************************************************************
template <typename T, int N>
inline bool operator==(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(array_const_ref<T>{a} == ali::move(b)))
// ******************************************************************
{
    return array_const_ref<T>{a} == ali::move(b);
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator==(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(a == b.ref()))
// ******************************************************************
{
    return a == b.ref();
}

// ******************************************************************
template <typename T, typename D>
inline bool operator==(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(a.ref() == ali::move(b)))
// ******************************************************************
{
    return a.ref() == ali::move(b);
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator==(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(a.ref() == b))
// ******************************************************************
{
    return a.ref() == b;
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator==(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(a.ref() == b.ref()))
// ******************************************************************
{
    return a.ref() == b.ref();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator!=(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(!(ali::move(a) == ali::move(b))))
// ******************************************************************
{
    return !(ali::move(a) == ali::move(b));
}

// ******************************************************************
template <typename T, int N>
inline bool operator!=(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(!(ali::move(a) == b)))
// ******************************************************************
{
    return !(ali::move(a) == b);
}

// ******************************************************************
template <typename T, typename D>
inline bool operator!=(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(!(ali::move(a) == b)))
// ******************************************************************
{
    return !(ali::move(a) == b);
}

// ******************************************************************
template <typename T, int N>
inline bool operator!=(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(!(a == ali::move(b))))
// ******************************************************************
{
    return !(a == ali::move(b));
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator!=(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(!(a == b)))
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T, typename D>
inline bool operator!=(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(!(a == ali::move(b))))
// ******************************************************************
{
    return !(a == ali::move(b));
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator!=(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(!(a == b)))
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator!=(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(!(a == b)))
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline int compare(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(a.compare(ali::move(b))))
// ******************************************************************
{
    return a.compare(ali::move(b));
}

// ******************************************************************
template <typename T, int N>
inline int compare(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(ali::move(a), array_const_ref<T>{b})))
// ******************************************************************
{
    return compare(ali::move(a), array_const_ref<T>{b});
}

// ******************************************************************
template <typename T, typename D>
inline int compare(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(ali::move(a), b.ref())))
// ******************************************************************
{
    return compare(ali::move(a), b.ref());
}

// ******************************************************************
template <typename T, int N>
inline int compare(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(compare(array_const_ref<T>{a}, ali::move(b))))
// ******************************************************************
{
    return compare(array_const_ref<T>{a}, ali::move(b));
}

// ******************************************************************
template <typename T, typename D, int N>
inline int compare(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(array_const_ref<T>{a}, b)))
// ******************************************************************
{
    return compare(array_const_ref<T>{a}, b);
}

// ******************************************************************
template <typename T, typename D>
inline int compare(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a.ref(), ali::move(b))))
// ******************************************************************
{
    return compare(a.ref(), ali::move(b));
}

// ******************************************************************
template <typename T, typename D, int N>
inline int compare(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(a.ref(), b)))
// ******************************************************************
{
    return compare(a.ref(), b);
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline int compare(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(compare(a.ref(), b.ref())))
// ******************************************************************
{
    return compare(a.ref(), b.ref());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator<(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(ali::move(a), ali::move(b))))
// ******************************************************************
{
    return compare(ali::move(a), ali::move(b)) < 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator<(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) < 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator<(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) < 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator<(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) < 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator<(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator<(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) < 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator<(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator<(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) < 0;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator<=(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(ali::move(a), ali::move(b))))
// ******************************************************************
{
    return compare(ali::move(a), ali::move(b)) <= 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator<=(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) <= 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator<=(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) <= 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator<=(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) <= 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator<=(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator<=(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) <= 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator<=(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator<=(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) <= 0;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator>(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(ali::move(a), ali::move(b))))
// ******************************************************************
{
    return compare(ali::move(a), ali::move(b)) > 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator>(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) > 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator>(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) > 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator>(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) > 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator>(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator>(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) > 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator>(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator>(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) > 0;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline bool operator>=(
    array_const_ref<T> a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(ali::move(a), ali::move(b))))
// ******************************************************************
{
    return compare(ali::move(a), ali::move(b)) >= 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator>=(
    array_const_ref<T> a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) >= 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator>=(
    array_const_ref<T> a,
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(ali::move(a), b)))
// ******************************************************************
{
    return compare(ali::move(a), b) >= 0;
}

// ******************************************************************
template <typename T, int N>
inline bool operator>=(
    T const (&a)[N],
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) >= 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator>=(
    T const (&a)[N],
    exposes_array_const_ref<T, D> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) >= 0;
}

// ******************************************************************
template <typename T, typename D>
inline bool operator>=(
    exposes_array_const_ref<T, D> const& a,
    array_const_ref<T> b ) noexcept(
        noexcept(compare(a, ali::move(b))))
// ******************************************************************
{
    return compare(a, ali::move(b)) >= 0;
}

// ******************************************************************
template <typename T, typename D, int N>
inline bool operator>=(
    exposes_array_const_ref<T, D> const& a,
    T const (&b)[N] ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) >= 0;
}

// ******************************************************************
template <typename T, typename Da, typename Db>
inline bool operator>=(
    exposes_array_const_ref<T, Da> const& a,
    exposes_array_const_ref<T, Db> const& b ) noexcept(
        noexcept(compare(a, b)))
// ******************************************************************
{
    return compare(a, b) >= 0;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref( T const* arr, int size ) noexcept
// ******************************************************************
{
    return array_const_ref<T>{arr, size};
}

namespace hidden
{

// ******************************************************************
template <typename T, typename size_type>
inline array_const_ref<T> make_ref_(
    T const* arr, size_type size ) noexcept
// ******************************************************************
{
    ali_static_assert(sizeof(int) <= sizeof(size_type));
    
    ali_assert(size <= static_cast<size_type>(
        meta::integer::max_value<int>::result));
    
    return array_const_ref<T>{arr, static_cast<int>(
        ali::mini(size, static_cast<size_type>(
            meta::integer::max_value<int>::result)))};
}

}   //  namespace hidden

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    T const* arr, unsigned int size ) noexcept
// ******************************************************************
{
    return hidden::make_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    T const* arr, long size ) noexcept
// ******************************************************************
{
    return hidden::make_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    T const* arr, unsigned long size ) noexcept
// ******************************************************************
{
    return hidden::make_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    T const* arr, long long size ) noexcept
// ******************************************************************
{
    return hidden::make_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    T const* arr, unsigned long long size ) noexcept
// ******************************************************************
{
    return hidden::make_ref_(arr, size);
}

// ******************************************************************
template <typename T, int size>
inline array_const_ref<T> make_ref(
    T const (&arr)[size] ) noexcept
// ******************************************************************
{
    return array_const_ref<T>{arr, size};
}

// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    ali::initializer_list<T> arr ) noexcept
// ******************************************************************
{
    return make_ref(arr.begin(), arr.size());
}

ALI_IF_DEBUG_WEAK_REF(
// ******************************************************************
template <typename T>
inline array_const_ref<T> make_ref(
    debug::weak_reference const& wref,
    T const* arr, int size ) noexcept
// ******************************************************************
{
    return array_const_ref<T>{wref, arr, size};
}

// ******************************************************************
template <typename T, int size>
inline array_const_ref<T> make_ref(
    debug::weak_reference const& wref,
    T const (&arr)[size] ) noexcept
// ******************************************************************
{
    return array_const_ref<T>{wref, arr, size};
}

)   //  ALI_IF_DEBUG_WEAK_REF

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref( T* arr, int size ) noexcept
// ******************************************************************
{
    return array_ref<T>{arr, size};
}

namespace hidden
{

// ******************************************************************
template <typename T, typename size_type>
inline array_ref<T> make_mutable_ref_(
    T* arr, size_type size ) noexcept
// ******************************************************************
{
    ali_static_assert(sizeof(int) <= sizeof(size_type));
    
    ali_assert(size <= static_cast<size_type>(
        meta::integer::max_value<int>::result));
    
    return array_ref<T>{arr, static_cast<int>(
        ali::mini(size, static_cast<size_type>(
            meta::integer::max_value<int>::result)))};
}

}   //  namespace hidden

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    T* arr, unsigned int size ) noexcept
// ******************************************************************
{
    return hidden::make_mutable_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    T* arr, long size ) noexcept
// ******************************************************************
{
    return hidden::make_mutable_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    T* arr, unsigned long size ) noexcept
// ******************************************************************
{
    return hidden::make_mutable_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    T* arr, long long size ) noexcept
// ******************************************************************
{
    return hidden::make_mutable_ref_(arr, size);
}

// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    T* arr, unsigned long long size ) noexcept
// ******************************************************************
{
    return hidden::make_mutable_ref_(arr, size);
}

// ******************************************************************
template <typename T, int size>
inline array_ref<T> make_mutable_ref( T (&arr)[size] ) noexcept
// ******************************************************************
{
    return array_ref<T>{arr, size};
}

ALI_IF_DEBUG_WEAK_REF(
// ******************************************************************
template <typename T>
inline array_ref<T> make_mutable_ref(
    debug::weak_reference const& wref,
    T* arr, int size ) noexcept
// ******************************************************************
{
    return array_ref<T>{wref, arr, size};
}

// ******************************************************************
template <typename T, int size>
inline array_ref<T> make_mutable_ref(
    debug::weak_reference const& wref,
    T (&arr)[size] ) noexcept
// ******************************************************************
{
    return array_ref<T>{wref, arr, size};
}

)   //  ALI_IF_DEBUG_WEAK_REF

// ******************************************************************
namespace string_literals
// ******************************************************************
//  This namespace exists mainly as a workaround for MSVC compiler
//  bug--it is unable to find user defined literal operators
//  introduced with using declaration in templates, i.e. instead of
//
//      using ali::operator""_s;
//
//  one has to write
//
//      using namespace ali::string_literals;
//
// ******************************************************************
{

// ******************************************************************
inline /*constexpr*/ string_literal operator""_s(
    char const* str, size_t size ) noexcept
// ******************************************************************
{
    return string_literal{str, static_cast<int>(size)};
}

// ******************************************************************
inline /*constexpr*/ wstring_literal operator""_s(
    ali::wchar const* str, size_t size ) noexcept
// ******************************************************************
{
    return wstring_literal{str, static_cast<int>(size)};
}

// ******************************************************************
inline /*constexpr*/ string_literal_ptr operator""_sp(
    char const* str, size_t size ) noexcept
// ******************************************************************
{
    return string_literal_ptr{str, static_cast<int>(size)};
}

// ******************************************************************
inline /*constexpr*/ wstring_literal_ptr operator""_sp(
    ali::wchar const* str, size_t size ) noexcept
// ******************************************************************
{
    return wstring_literal_ptr{str, static_cast<int>(size)};
}

}   //  namespace string_literals

// ******************************************************************
array<bool> test_array_utils( void );
// ******************************************************************

// ******************************************************************
//  The following functions are DEPRECATED.
// ******************************************************************

#ifndef ALI_EXCLUDE_DEPRECATED

// ******************************************************************
inline void mem_copy( void* dst, void const* src, int n )
// ******************************************************************
{
    platform::memmove(dst, src, n);
}


// ******************************************************************
inline void mem_move( void* first1, void const* first2, int n )
// ******************************************************************
{
    platform::memmove(first1, first2, n);
}

// ******************************************************************
inline void mem_fill( void* dst, unsigned char val, int n )
// ******************************************************************
{
    platform::memset(dst, val, n);
}

// ******************************************************************
inline int mem_cmp( void const* a, void const* b, int n )
// ******************************************************************
{
    return platform::memcmp(a, b, n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  Copies elements from [src; src+n) to [dst; dst+n).
//  The arrays may overlap.
template <typename T, typename U>
inline void copy( T* dst, U const* src, int n )
// ******************************************************************
{
    hidden::copy(dst, src, n);
}

// ******************************************************************
//  Copies all elements from src to dst.
template <typename T, typename U, int n>
inline void copy( T (&dst)[n], U const (&src)[n] )
// ******************************************************************
{
    hidden::copy(dst, src, n);
}

// ******************************************************************
// ******************************************************************

namespace detail
{

// ******************************************************************
//  Returns true if the two arrays overlap.
template <typename T>
inline bool overlap( T const* a, T const* b, int n )
// ******************************************************************
{
    return a + n > b
        && b + n > a;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  Copies arrays of the same types in reverse order.
template <typename T>
void reverse_copy( T* dst, T const* src, int n )
// ******************************************************************
{
    if ( overlap(dst, src, n) )
    {
        copy(dst, src, n);
        reverse(dst, n);
    }
    else while ( n != 0 )
    {
        *dst++ = src[--n];
    }
}

// ******************************************************************
//  Copies arrays of different types in reverse order.
//  Assumption: dst and src don't overlap.
template <typename T, typename U>
void reverse_copy( T* dst, U const* src, int n )
// ******************************************************************
{
    while ( n != 0 )
        *dst++ = src[--n];
}

}   //  namespace detail

// ******************************************************************
//  Copies elements from [src; src+n) to [dst; dst+n) in reverse order.
//  The arrays may overlap.
template <typename T, typename U>
inline void reverse_copy( T* dst, U const* src, int n )
// ******************************************************************
{
    detail::reverse_copy(dst, src, n);
}

// ******************************************************************
//  Copies all elements from src to dst in reverse order.
//  The arrays may overlap.
template <typename T, typename U, int n>
inline void reverse_copy( T (&dst)[n], U const (&src)[n] )
// ******************************************************************
{
    detail::reverse_copy(dst, src, n);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline void fill( T* arr, T value, int n )
// ******************************************************************
{
    hidden::fill(arr, value, n);
}

// ******************************************************************
template <typename T, typename U>
inline void fill( T* arr, U value, int n )
// ******************************************************************
{
    hidden::fill(arr, static_cast<T>(value), n);
}

// ******************************************************************
template <typename T>
inline void fill( T* begin, T* end, T value )
// ******************************************************************
{
    hidden::fill(begin, value, end - begin);
}

// ******************************************************************
template <typename T, typename U>
inline void fill( T* begin, T* end, U value )
// ******************************************************************
{
    hidden::fill(begin, static_cast<T>(value), end - begin);
}

// ******************************************************************
template <typename T, int n>
inline void fill( T (&arr)[n], T value )
// ******************************************************************
{
    hidden::fill(arr, value, n);
}

// ******************************************************************
template <typename T, typename U, int n>
inline void fill( T (&arr)[n], U value )
// ******************************************************************
{
    hidden::fill(arr, static_cast<T>(value), n);
}

#endif  //  !ALI_EXCLUDE_DEPRECATED


// From ali_location.h

// ******************************************************************
inline c_string_const_ref location::file( void ) const noexcept
// ******************************************************************
{
    return file_from_path(path());
}

// ******************************************************************
inline c_string_const_ref location::path( void ) const noexcept
// ******************************************************************
{
    return c_string_const_ref{_path, _path_size};
}

}   //  namespace ali
