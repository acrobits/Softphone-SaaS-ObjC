/*
 *  ali_string.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_string_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_array_utils.h"
#include "ali/ali_attribute.h"
#include "ali/ali_stable_string.h"
#include "ali/ali_str_utils.h"

#ifndef ALI_STRING_IS_ARRAY

#include "ali/ali_debug.h"

// Debug buffer overflow.
#if defined(ALI_DEBUG_OVERFLOW)

#   define ALI_STRING_DEBUG_OVERFLOW
    //  Globally controlled define.

#else

//#   define ALI_STRING_DEBUG_OVERFLOW
    //  Locally controlled define (comment/uncomment).

#endif  //  ALI_DEBUG_OVERFLOW

// ******************************************************************

#if defined(ALI_STRING_DEBUG_OVERFLOW)
#   define ALI_STRING_IF_DEBUG_OVERFLOW(...) __VA_ARGS__
#else
#   define ALI_STRING_IF_DEBUG_OVERFLOW(...)
#endif

// ******************************************************************

namespace ali
{

namespace hidden
{

/*

class string_data
{
public:     // Typedefs
    using value_type = char;
    using E = value_type;
    using iterator = value_type*;
    using const_iterator = value_type const*;

public:
    string_data( void );
    string_data( string_data const& b, int pos, int n );

    bool is_empty( void ) const;
    int size( void ) const;
    int capacity( void ) const;

    iterator begin( void );
    const_iterator begin( void ) const;

    iterator end( void );
    const_iterator end( void ) const;

    E const& operator[]( int i ) const;
    E& operator[]( int i );

    E const* data( void ) const;
    E const* c_str( void ) const;

protected:  // Methods
    ~string_data( void );

    void assign( string_data_sso_rc const& b, int pos, int n );
    void assign( string_const_ref b );
    void assign( int n, E c );

    void reserve( int n );
    void resize( int n, E c );
    void erase( void );

    void swap( string_data& b );

    void ensure_unique( void );

    is_internal_pointer_result is_internal_pointer( E const* str, int n ) const;
    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const;
    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const;

    void set( int pos, string_const_ref str );
    void set( int pos, int n, E c );
    void move( int first1, int first2, int n );

private:    //  Methods
    string_data& operator=( string_data const& b );
};

*/

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_data_sso
// ******************************************************************
{
public:     // Typedefs
    using value_type = char;
    using E = value_type;
    using iterator = value_type*;
    using const_iterator = value_type const*;

public:
    string_data_sso( void ) noexcept
    {
        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        _begin[_size] = '\0';

        ali_assert(is_small());
        ALI_IF_DEBUG(assert_invariant());
    }

    string_data_sso(
        string_data_sso const& b,
        int pos, int n );

    explicit string_data_sso(
        string_const_ref str );

    bool is_empty( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _size == 0;
    }

    int size( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _size;
    }

    int capacity( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        if ( is_small() )
            return _small_capacity;

        return _capacity;
    }

    iterator begin( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _begin;
    }

    friend iterator begin( string_data_sso& str ) noexcept
    {
        return str.begin();
    }

    const_iterator begin( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _begin;
    }

    friend const_iterator begin( string_data_sso const& str ) noexcept
    {
        return str.begin();
    }

    iterator end( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _begin + _size;
    }

    friend iterator end( string_data_sso& str ) noexcept
    {
        return str.end();
    }

    const_iterator end( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());

        return _begin + _size;
    }

    friend const_iterator end( string_data_sso const& str ) noexcept
    {
        return str.end();
    }

    E const& operator[]( int i ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }

    E& operator[]( int i ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }

    E const* data( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());
        return _begin;
    }

    E const* c_str( void ) const noexcept
    {
        return data();
    }

    is_internal_pointer_result is_internal_pointer(
        E const* str, int ALI_IF_DEBUG(n) ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards());
        ali_assert(0 <= n);

        bool const is_internal{
            _begin <= str && str <= _begin + _size};

        ali_assert(!is_internal || n <= _size - (str - _begin));

        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( string_const_ref str ) const noexcept
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( string_const_ptr str ) const noexcept
    {
        return is_internal_pointer(str->data(), str->size());
    }

    void assert_invariant( void ) const noexcept
    {
        ali_assert(0 <= _size);
        ali_assert(!is_small() || _size <= _small_capacity);
        ali_assert(is_small() || _size <= _capacity);
        ali_assert(is_small() || _small_capacity < _capacity);
        ali_assert(_begin != nullptr);
        ali_assert(_begin != invalid_pointer_value);
        ali_assert(_begin[_size] == '\0');
    }

protected:  // Methods
    ~string_data_sso( void ) noexcept;

    void assign(
        string_data_sso const& b,
        int pos, int n )
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        ali_assert(0 <= pos);
        ali_assert(pos <= b._size);
        ali_assert(0 <= n);
        ali_assert(n <= b._size - pos);

        assign(string_const_ref{b.data() + pos, n});
    }

    void assign( string_const_ref b );
    void assign( int n, E c );

    void reserve( int n )
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        ali_assert(0 <= n);

        private_reserve(n, _size);
    }

    void resize( int n, E c );

    void erase( void ) noexcept
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        _size = 0;

        _begin[_size] = '\0';
    }

    void swap( string_data_sso& b ) noexcept;

    void ensure_unique( void ) noexcept {}

    void set( int pos, string_const_ref str ) noexcept
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        ali_sanoex((string_ref{_begin, _size}
            .copy(pos, ali::move(str))));
    }

    void set( int pos, int n, E c ) noexcept
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        ali_sanoex((string_ref{_begin, _size}
            .mutable_ref(pos, n).fill(c)));
    }

    void move( int first1, int first2, int n ) noexcept
    {
        ALI_IF_DEBUG(
            assert_invariant_now_and_on_exit const inv{*this});

        ALI_STRING_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit const guards{*this});

        auto const self = ali_sanoex((string_ref{_begin, _size}));

        ali_sanoex(self.mutable_ref(first1, n) = self.ref(first2, n));
    }

private:    // Methods
    string_data_sso& operator=(
        string_data_sso const& b ) = delete;

    bool is_small( void ) const noexcept
    {
        return _begin == _small.begin;
    }

    void release( void ) noexcept;

    void private_reserve(
        int min_capacity,
        int retained_size );

    static void swap_ll(
        string_data_sso& a,
        string_data_sso& b ) noexcept
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());

        using ali::swap;
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._capacity, b._capacity);
    }

    static void swap_ss(
        string_data_sso& a,
        string_data_sso& b ) noexcept
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());

        using ali::swap;
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        string_data_sso& a,
        string_data_sso& b ) noexcept;

    using assert_invariant_now_and_on_exit
        = debug::assert_invariant_now_and_on_exit<string_data_sso>;

#ifdef  ALI_STRING_DEBUG_OVERFLOW

    struct check_overflow_guards_now_and_on_exit
    {
        string_data_sso const&  s;

        explicit check_overflow_guards_now_and_on_exit(
            string_data_sso const& s ) noexcept
        :   s{s}
        {
            s.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void ) noexcept
        {
            s.check_overflow_guards();
        }
    };

    void uninitialize_overflow_guards( void ) const noexcept;

    bool check_overflow_guards( void ) const noexcept;

    static constexpr int guard_element_count( void ) noexcept
    {
        return 1 + (sizeof(_correct_guard) - 1) / sizeof(E);
    }

#endif  //  ALI_STRING_DEBUG_OVERFLOW

private:    // Data members
ALI_STRING_IF_DEBUG_OVERFLOW(
    static constexpr ali::uint32    _correct_guard{0xaaaaaaaaU};
);
    static constexpr int            _nice_object_size{16};
    
    static constexpr int            _min_object_size{
                                        sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                            sizeof(int), sizeof(E)>::result};
    
    static constexpr int            _object_size{meta::maxi<
                                        _min_object_size, _nice_object_size>::result};

    static constexpr int            _small_capacity{
                                        (_object_size - sizeof(E*) - sizeof(unsigned int))
                                            / sizeof(E) - 1};

    struct small_data
    {
        E               begin[_small_capacity + 1];
    };

ALI_STRING_IF_DEBUG_OVERFLOW(
    mutable ali::uint32 _guard1{_correct_guard};
);

    E*                  _begin{_small.begin};
    int                 _size{};

ALI_STRING_IF_DEBUG_OVERFLOW(
    mutable ali::uint32 _guard2{_correct_guard};
);

    union
    {
        int             _capacity;
        small_data      _small;
    };

ALI_STRING_IF_DEBUG_OVERFLOW(
    mutable ali::uint32 _guard3{_correct_guard};
);

};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class wstring_data_sso
// ******************************************************************
{
public:     // Typedefs
    using value_type = wchar;
    using E = value_type;
    using iterator = value_type*;
    using const_iterator = value_type const*;

public:
    wstring_data_sso( void ) noexcept
    {
        _begin[_size] = L'\0';

        ali_assert(is_small());
    }

    wstring_data_sso(
        wstring_data_sso const& b,
        int pos, int n );

    explicit wstring_data_sso( wstring_const_ref str );

    bool is_empty( void ) const noexcept
    {
        return _size == 0;
    }

    int size( void ) const noexcept
    {
        return _size;
    }

    int capacity( void ) const noexcept
    {
        if ( is_small() )
            return _small_capacity;
        return _capacity;
    }

    iterator begin( void ) noexcept
    {
        return _begin;
    }

    friend iterator begin( wstring_data_sso& str ) noexcept
    {
        return str.begin();
    }

    const_iterator begin( void ) const noexcept
    {
        return _begin;
    }

    friend const_iterator begin( wstring_data_sso const& str ) noexcept
    {
        return str.begin();
    }

    iterator end( void ) noexcept
    {
        return _begin + _size;
    }

    friend iterator end( wstring_data_sso& str ) noexcept
    {
        return str.end();
    }

    const_iterator end( void ) const noexcept
    {
        return _begin + _size;
    }

    friend const_iterator end( wstring_data_sso const& str ) noexcept
    {
        return str.end();
    }

    E const& operator[]( int i ) const noexcept
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }

    E& operator[]( int i ) noexcept
    {
        ali_assert(0 <= i);
        ali_assert(i < _size);
        return _begin[i];
    }

    E const* data( void ) const noexcept
    {
        ali_assert(_begin[_size] == L'\0');
        return _begin;
    }

    E const* c_str( void ) const noexcept
    {
        return data();
    }

    is_internal_pointer_result is_internal_pointer(
        E const* str, int ALI_IF_DEBUG(n) ) const noexcept
    {
        ali_assert(0 <= n);

        bool const is_internal{
            _begin <= str && str <= _begin + _size};

        ali_assert(!is_internal || n <= _size - (str - _begin));

        return is_internal_pointer_result{
            is_internal ? static_cast<int>(str - _begin) : -1};
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ref str ) const noexcept
    {
        return is_internal_pointer(str.data(), str.size());
    }

    is_internal_pointer_result is_internal_pointer( wstring_const_ptr str ) const noexcept
    {
        return is_internal_pointer(str->data(), str->size());
    }

protected:  // Methods
    ~wstring_data_sso( void ) noexcept;

    void assign(
        wstring_data_sso const& b,
        int pos, int n )
    {
        ali_assert(0 <= pos);
        ali_assert(pos <= b._size);
        ali_assert(0 <= n);
        ali_assert(n <= b._size - pos);

        assign(wstring_const_ref{b.data() + pos, n});
    }
    void assign( wstring_const_ref b );
    void assign( int n, E c );

    void reserve( int n )
    {
        ali_assert(0 <= n);

        private_reserve(n, _size);
    }

    void resize( int n, E c );

    void erase( void ) noexcept
    {
        _size = 0;

        _begin[_size] = L'\0';
    }

    void swap( wstring_data_sso& b ) noexcept;

    void ensure_unique( void ) noexcept {}

    void set( int pos, wstring_const_ref str ) noexcept
    {
        ali_sanoex((wstring_ref{_begin, _size}
            .copy(pos, ali::move(str))));
    }

    void set( int pos, int n, E c ) noexcept
    {
        ali_sanoex((wstring_ref{_begin, _size}
            .mutable_ref(pos, n).fill(c)));
    }

    void move( int first1, int first2, int n ) noexcept
    {
        auto const self = ali_sanoex((wstring_ref{_begin, _size}));

        ali_sanoex(self.mutable_ref(first1, n) = self.ref(first2, n));
    }

private:    // Methods
    wstring_data_sso& operator=(
        wstring_data_sso const& b ) = delete;

    bool is_small( void ) const noexcept
    {
        return _begin == _small.begin;
    }

    void release( void ) noexcept;

    void private_reserve(
        int min_capacity,
        int retained_size );

    static void swap_ll(
        wstring_data_sso& a,
        wstring_data_sso& b ) noexcept
    {
        ali_assert(!a.is_small());
        ali_assert(!b.is_small());

        using ali::swap;
        swap(a._begin, b._begin);
        swap(a._size, b._size);
        swap(a._capacity, b._capacity);
    }

    static void swap_ss(
        wstring_data_sso& a,
        wstring_data_sso& b ) noexcept
    {
        ali_assert(a.is_small());
        ali_assert(b.is_small());

        using ali::swap;
        swap(a._size, b._size);
        swap_by_copy(a._small, b._small);
    }

    static void swap_ls(
        wstring_data_sso& a,
        wstring_data_sso& b ) noexcept;

private:    // Data members
    static constexpr int    _nice_object_size{16};

    static constexpr int    _min_object_size{
                                sizeof(E*) + sizeof(unsigned int) + meta::maxi<
                                    sizeof(int), sizeof(E)>::result};
    
    static constexpr int    _object_size{meta::maxi<
                                _min_object_size, _nice_object_size>::result};

    static constexpr  int   _small_capacity{
                                (_object_size - sizeof(E*) - sizeof(unsigned int))
                                    / sizeof(E) - 1};

    struct small_data
    {
        E                   begin[_small_capacity + 1];
    };

    E*                      _begin{_small.begin};
    int                     _size{};

    union
    {
        int                 _capacity;
        small_data          _small;
    };
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
using string_data = string_data_sso;
using wstring_data = wstring_data_sso;
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
class ALI_ATTRIBUTE_EMPTY_BASES string
// ******************************************************************
    :   public hidden::string_data,
        public exposes_string_ref<string>
{
public:
    static constexpr int npos{
        meta::integer::max_value<int>::result};

public:
    string( void ) = default;

    string( string&& b ) noexcept
    {
        ali_sanoex(swap(b));
    }

    string( string const& b, int pos = 0, int n = npos )
    :   hidden::string_data{b, pos, n}
    {}

    string( E const* str ) = delete;
    
    string( E const* str, int n ) = delete;

    string( string_const_ref str )
    :   hidden::string_data{ali::move(str)}
    {}

    ~string( void ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
    }

    string& operator=( string const& b )
    {
        return assign(b);
    }

    string& operator=( string&& b ) noexcept
    {
        return ali_sanoex(assign(ali::move(b)));
    }

    string& operator=( E const* b ) = delete;
    
    string& operator=( string_const_ref b )
    {
        return assign(ali::move(b));
    }

ALI_DEPRECATED(
    int length( void ) const noexcept
    {
        return ali_sanoex(size());
    }
);

    string& assign(
        string const& b,
        int pos = 0,
        int n = npos );

    string& assign(
        string&& b,
        int pos = 0,
        int n = npos ) noexcept;

    string& assign( E const* b, int n ) = delete;
    
    string& assign( E const* b ) = delete;

    string& assign( string_const_ref b );

    string& assign( int n, E c ) = delete;
        //  deprecated in favor of assign_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    string& assign_( E c, int n = 1 );

    string& reserve( int n );

    string& resize( int n, E c );

    string& resize( int n )
    {
        return resize(n, E{});
    }

    E const& at( int i ) const noexcept
    {
        return ali_sanoex(operator[](i));
    }

    E& at( int i ) noexcept
    {
        return ali_sanoex(operator[](i));
    }

    string& operator+=( E const* str ) = delete;
    
    string& operator+=( string_const_ref str )
    {
        return append(ali::move(str));
    }

    string& operator+=( E c )
    {
        return append_(c);
    }

    // append
    
    string& append(
        string const& str,
        int pos,
        int n = npos );

    string& append( E const* str ) = delete;
    
    string& append( E const* str, int n ) = delete;

    string& append( string_const_ref str );

    string& append( int n, E c ) = delete;
        //  deprecated in favor of append_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    string& append_( E c, int n );

    string& append_( E c )
    {
        return append_(c, 1);
    }

    string& push_back( E c, int n )
    {
        return append_(c, n);
    }

    string& push_back( E c )
    {
        return append_(c);
    }

    string& push_front( E c )
    {
        return insert_(0, c);
    }

    // replace

    string& replace(
        int pos1, int n1,
        string const& str,
        int pos2, int n2 );

    string& replace( int pos1, int n1, E const* str ) = delete;
    
    string& replace(
        int pos1, int n1,
        E const* str, int n2 ) = delete;

    string& replace(
        int pos1, int n1,
        string_const_ref str );

    string& replace(
        int pos1, int n1,
        int n2, E c ) = delete;
            //  deprecated in favor of replace_ (to be renamed back later)
            //  in order to unify string and array interfaces.
            //  BEWARE: the order of parameters 'n2' and 'c' is different.

    string& replace_(
        int pos1, int n1,
        E c, int n2 = 1 );

    string& replace(
        string_const_ref src,
        string_const_ref target );

    string& nocase_replace(
        string_const_ref src,
        string_const_ref target );

    // erase
    
    string& erase( int pos, int n = npos ) noexcept;

    string& erase( void ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        ali_sanoex(hidden::string_data::erase());
        return *this;
    }

    // insert

    string& insert(
        int pos1, string const& str,
        int pos2, int n );

    string& insert( int pos1, E const* str, int n ) = delete;

    string& insert( int pos1, E const* str ) = delete;
    
    string& insert( int pos1, string_const_ref str );

    string& insert( int pos1, int n, E c ) = delete;
        //  deprecated in favor of insert_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    string& insert_( int pos1, E c, int n = 1 );

    //  transform

    template <typename char_transform_in_place>
    string& transform_chars_in_place( char_transform_in_place t ) noexcept(
        noexcept(string_ref{begin(), size()}.for_each(t)))
    {
        string_ref{begin(), size()}.for_each(t);
        return *this;
    }

    //  misc

    void swap( string& b ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        ali_sanoex(hidden::string_data::swap(b));
    }

    friend void swap( string& a, string& b ) noexcept
    {
        a.swap(b);
    }

    string& ensure_unique( void ) noexcept(
        noexcept(hidden::string_data::ensure_unique()))
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        hidden::string_data::ensure_unique();
        return *this;
    }

    //  utility
    
    string substr( int pos = 0, int n = npos ) const
    {
        return string{*this, pos, n};
    }

    bool begins_with( E const* prefix, int n ) const = delete;

    bool begins_with( E const* prefix ) const = delete;

    bool begins_with( string_const_ref prefix ) const noexcept
    {
        return ali_sanoex(ref().begins_with_n(ali::move(prefix)));
    }

    bool begins_with( E prefix ) const noexcept
    {
        return ali_sanoex(ref().begins_with(prefix));
    }

    bool nocase_begins_with( E const* prefix, int n ) const = delete;

    bool nocase_begins_with( E const* prefix ) const = delete;
    
    bool nocase_begins_with( string_const_ref prefix ) const noexcept
    {
        return ali_sanoex(ref().nocase_begins_with_n(ali::move(prefix)));
    }

    bool nocase_begins_with( E prefix ) const noexcept
    {
        return ali_sanoex(ref().nocase_begins_with(prefix));
    }

    bool ends_with( E const* suffix, int n ) const = delete;

    bool ends_with( E const* suffix ) const = delete;
    
    bool ends_with( string_const_ref suffix ) const noexcept
    {
        return ali_sanoex(ref().ends_with_n(ali::move(suffix)));
    }

    bool ends_with( E suffix ) const noexcept
    {
        return ali_sanoex(ref().ends_with(suffix));
    }

    bool nocase_ends_with( E const* suffix, int n ) const = delete;

    bool nocase_ends_with( E const* suffix ) const = delete;
    
    bool nocase_ends_with( string_const_ref suffix ) const noexcept
    {
        return ali_sanoex(ref().nocase_ends_with_n(ali::move(suffix)));
    }

    bool nocase_ends_with( E suffix ) const noexcept
    {
        return ali_sanoex(ref().nocase_ends_with(suffix));
    }

    int compare( string const& b ) const noexcept
    {
        return ali_sanoex(ref().compare(b.ref()));
    }

    int compare(
        int pos1, int n1,
        string const& b ) const noexcept
    {
        return ali_sanoex(compare(pos1, n1, b.ref()));
    }

    int compare(
        int pos1, int n1,
        string const& b,
        int pos2, int n2 ) const noexcept;

    int compare( E const* str ) const = delete;

    int compare( string_const_ref str ) const noexcept
    {
        return ali_sanoex(ref().compare(ali::move(str)));
    }

    int compare( int pos1, int n1, E const* str ) const = delete;

    int compare(
        int pos1, int n1,
        string_const_ref str ) const noexcept;

    int compare(
        int pos1, int n1,
        E const* str, int n2 ) const = delete;

    int nocase_compare( string const& b ) const noexcept
    {
        return ali_sanoex(ref().nocase_compare(b.ref()));
    }

    int nocase_compare(
        int pos1, int n1,
        string const& b ) const noexcept
    {
        return ali_sanoex(nocase_compare(pos1, n1, b.ref()));
    }

    int nocase_compare(
        int pos1, int n1,
        string const& b,
        int pos2, int n2 ) const noexcept;

    int nocase_compare( E const* str ) const = delete;

    int nocase_compare( string_const_ref str ) const noexcept
    {
        return ali_sanoex(ref().nocase_compare(ali::move(str)));
    }

    int nocase_compare(
        int pos1,
        int n1,
        E const* str ) const = delete;

    int nocase_compare(
        int pos1, int n1,
        string_const_ref str ) const noexcept;

    int nocase_compare(
        int pos1, int n1,
        E const* str, int n2 ) const = delete;


    int find( E const* str, int pos1 = 0 ) const = delete;

    int find( string_const_ref str, int pos1 = 0 ) const noexcept;
    
    int find( E const* str, int pos1, int n2 ) const = delete;
    
    int find( E c, int pos1 = 0 ) const noexcept;


    int nocase_find( E const* str, int pos1 = 0 ) const = delete;
    
    int nocase_find( string_const_ref str, int pos1 = 0 ) const noexcept;
    
    int nocase_find( E const* str, int pos1, int n2 ) const = delete;
    
    int nocase_find( E c, int pos1 = 0 ) const noexcept;


    int rfind( E const* str, int pos1 = npos ) const = delete;
    
    int rfind( string_const_ref str, int pos1 = npos ) const noexcept;
    
    int rfind( E const* str, int pos1, int n2 ) const = delete;
    
    int rfind( E c, int pos1 = npos ) const noexcept;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)

    
    int nocase_rfind( E const* str, int pos1 = npos ) const = delete;
    
    int nocase_rfind( string_const_ref str, int pos1 = npos ) const noexcept;
    
    int nocase_rfind( E const* str, int pos1, int n2 ) const = delete;
    
    int nocase_rfind( E c, int pos1 = npos ) const noexcept;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)

    
    int find_first_of( string_const_ref set, int pos1 = 0 ) const noexcept;
    int find_first_not_of( string_const_ref set, int pos1 = 0 ) const noexcept;
    int find_last_not_of( string_const_ref set, int pos1 = npos ) const noexcept;

    // deprecated
    int find_any( string_const_ref set, int pos1 = 0 ) const noexcept
    {
        return ali_sanoex(find_first_of(ali::move(set), pos1));
    }

    // deprecated
    int find_neither( string_const_ref set, int pos1 = 0 ) const noexcept
    {
        return ali_sanoex(find_first_not_of(ali::move(set), pos1));
    }

    //  ref

    string_const_ref ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    c_string_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_string_const_ref{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            c_str(), size()}));
    }

    //  mutable_ref

    string_ref mutable_ref( int pos, int n ) noexcept
    {
        return ali_sanoex(mutable_ref().mutable_ref(pos, n));
    }

    string_ref mutable_ref( void ) noexcept
    {
        return ali_sanoex((string_ref{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            begin(), size()}));
    }

    blob_const_ref as_blob( void ) const noexcept
    {
        return ali_sanoex(ref().as_blob());
    }

    blob_ref as_mutable_blob( void ) noexcept
    {
        return ali_sanoex(mutable_ref().as_mutable_blob());
    }

private:    // Methods
    //static void check_range( int cur, int b, int e );

    //  If the compiler complains about this method being private, you have
    //  an error in your program's logic. Maybe you are using ali::optional<>
    //  directly instead of dereferencing it?
    string& operator +=(bool) = delete;

ALI_IF_DEBUG_WEAK_REF(

    bool check_no_weak_ref( void ) const noexcept
    {
        bool const success{_weak_rc.get() == 0};

        if ( !success )
            debug::abort();

        return success;
    }

private:    //  Data members
    mutable debug::weak_reference::counter  _weak_rc{};

)   //  ALI_IF_DEBUG_WEAK_REF

};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class ALI_ATTRIBUTE_EMPTY_BASES wstring
// ******************************************************************
     :  public hidden::wstring_data,
        public exposes_wstring_ref<wstring>
{
public:
    static constexpr int npos{
        ali::meta::integer::max_value<int>::result};

public:
    wstring( void ) = default;

    wstring( wstring&& b ) noexcept
    {
        ali_sanoex(swap(b));
    }

    wstring( wstring const& b, int pos = 0, int n = npos )
    :   hidden::wstring_data{b, pos, n}
    {}

    wstring( E const* str ) = delete;

    wstring( E const* str, int n ) = delete;

    wstring( wstring_const_ref str )
    :   hidden::wstring_data{ali::move(str)}
    {}

    ~wstring( void ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
    }

    wstring& operator=( wstring const& b )
    {
        return assign(b);
    }

    wstring& operator=( wstring&& b ) noexcept
    {
        return ali_sanoex(assign(ali::move(b)));
    }

    wstring& operator=( E const* b ) = delete;

    wstring& operator=( wstring_const_ref b )
    {
        return assign(ali::move(b));
    }

ALI_DEPRECATED(
    int length( void ) const noexcept
    {
        return ali_sanoex(size());
    }
);

    wstring& assign(
        wstring const& b,
        int pos = 0,
        int n = npos );

    wstring& assign(
        wstring&& b,
        int pos = 0,
        int n = npos ) noexcept;

    wstring& assign( E const* b, int n ) = delete;

    wstring& assign( E const* b ) = delete;

    wstring& assign( wstring_const_ref b );

    wstring& assign( int n, E c ) = delete;
        //  deprecated in favor of assign_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    wstring& assign_( E c, int n = 1 );

    wstring& reserve( int n );

    wstring& resize( int n, E c );

    wstring& resize( int n )
    {
        return resize(n, E{});
    }

    E const& at( int i ) const noexcept
    {
        return ali_sanoex(operator[](i));
    }

    E& at( int i ) noexcept
    {
        return ali_sanoex(operator[](i));
    }

    wstring& operator+=( E const* str ) = delete;

    wstring& operator+=( wstring_const_ref str )
    {
        return append(ali::move(str));
    }

    wstring& operator+=( E c )
    {
        return append_(c);
    }

    // append
    wstring& append(
        wstring const& str,
        int pos,
        int n = npos );

    wstring& append( E const* str ) = delete;

    wstring& append( E const* str, int n ) = delete;

    wstring& append( wstring_const_ref str );

    wstring& append( int n, E c ) = delete;
        //  deprecated in favor of append_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    wstring& append_( E c, int n );

    wstring& append_( E c )
    {
        return append_(c, 1);
    }

    wstring& push_back( E c, int n )
    {
        return append_(c, n);
    }

    wstring& push_back( E c )
    {
        return append_(c);
    }

    wstring& push_front( E c )
    {
        return insert_(0, c);
    }

    // replace

    wstring& replace(
        int pos1, int n1,
        wstring const& str,
        int pos2, int n2 );

    wstring& replace( int pos1, int n1, E const* str ) = delete;

    wstring& replace(
        int pos1, int n1,
        E const* str, int n2 ) = delete;

    wstring& replace(
        int pos1, int n1,
        wstring_const_ref str );

    wstring& replace(
        int pos1, int n1,
        int n2, E c ) = delete;
            //  deprecated in favor of replace_ (to be renamed back later)
            //  in order to unify string and array interfaces.
            //  BEWARE: the order of parameters 'n2' and 'c' is different.

    wstring& replace_(
        int pos1, int n1,
        E c, int n2 = 1 );

    wstring& replace(
        wstring_const_ref src,
        wstring_const_ref target );

    wstring& nocase_replace(
        wstring_const_ref src,
        wstring_const_ref target );

    // erase

    wstring& erase( int pos, int n = npos ) noexcept;

    wstring& erase( void ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        ali_sanoex(hidden::wstring_data::erase());
        return *this;
    }

    // insert

    wstring& insert(
        int pos1, wstring const& str,
        int pos2, int n );

    wstring& insert( int pos1, E const* str, int n ) = delete;

    wstring& insert( int pos1, E const* str ) = delete;

    wstring& insert( int pos1, wstring_const_ref str );

    wstring& insert( int pos1, int n, E c ) = delete;
        //  deprecated in favor of insert_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    wstring& insert_( int pos1, E c, int n = 1 );

    //  transform

    template <typename char_transform_in_place>
    wstring& transform_chars_in_place( char_transform_in_place t ) noexcept(
        noexcept(wstring_ref{begin(), size()}.for_each(t)))
    {
        wstring_ref{begin(), size()}.for_each(t);
        return *this;
    }

    //  misc

    void swap( wstring& b ) noexcept
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        ali_sanoex(hidden::wstring_data::swap(b));
    }

    friend void swap( wstring& a, wstring& b ) noexcept
    {
        a.swap(b);
    }

    wstring& ensure_unique( void ) noexcept(
        noexcept(hidden::wstring_data::ensure_unique()))
    {
        ALI_IF_DEBUG_WEAK_REF(check_no_weak_ref());
        hidden::wstring_data::ensure_unique();
        return *this;
    }

    //  utility

    wstring substr( int pos = 0, int n = npos ) const
    {
        return wstring{*this, pos, n};
    }

    bool begins_with( E const* prefix, int n ) const = delete;

    bool begins_with( E const* prefix ) const = delete;

    bool begins_with( wstring_const_ref prefix ) const noexcept
    {
        return ali_sanoex(ref().begins_with_n(ali::move(prefix)));
    }

    bool begins_with( E prefix ) const noexcept
    {
        return ali_sanoex(ref().begins_with(prefix));
    }

    bool nocase_begins_with( E const* prefix, int n ) const = delete;

    bool nocase_begins_with( E const* prefix ) const = delete;

    bool nocase_begins_with( wstring_const_ref prefix ) const noexcept
    {
        return ali_sanoex(ref().nocase_begins_with_n(ali::move(prefix)));
    }

    bool nocase_begins_with( E prefix ) const noexcept
    {
        return ali_sanoex(ref().nocase_begins_with(prefix));
    }

    bool ends_with( E const* suffix, int n ) const = delete;

    bool ends_with( E const* suffix ) const = delete;

    bool ends_with( wstring_const_ref suffix ) const noexcept
    {
        return ali_sanoex(ref().ends_with_n(ali::move(suffix)));
    }

    bool ends_with( E suffix ) const noexcept
    {
        return ali_sanoex(ref().ends_with(suffix));
    }

    bool nocase_ends_with( E const* suffix, int n ) const = delete;

    bool nocase_ends_with( E const* suffix ) const = delete;

    bool nocase_ends_with( wstring_const_ref suffix ) const noexcept
    {
        return ali_sanoex(ref().nocase_ends_with_n(ali::move(suffix)));
    }

    bool nocase_ends_with( E suffix ) const noexcept
    {
        return ali_sanoex(ref().nocase_ends_with(suffix));
    }

    int compare( wstring const& b ) const noexcept
    {
        return ali_sanoex(ref().compare(b.ref()));
    }

    int compare(
        int pos1, int n1,
        wstring const& b ) const noexcept
    {
        return ali_sanoex(compare(pos1, n1, b.ref()));
    }

    int compare(
        int pos1, int n1,
        wstring const& b,
        int pos2, int n2 ) const noexcept;

    int compare( E const* str ) const = delete;

    int compare( wstring_const_ref str ) const noexcept
    {
        return ali_sanoex(ref().compare(ali::move(str)));
    }

    int compare( int pos1, int n1, E const* str ) const = delete;

    int compare(
        int pos1, int n1,
        wstring_const_ref str ) const noexcept;

    int compare(
        int pos1, int n1,
        E const* str, int n2 ) const noexcept;

    int nocase_compare( wstring const& b ) const noexcept
    {
        return ali_sanoex(ref().nocase_compare(b.ref()));
    }

    int nocase_compare(
        int pos1, int n1,
        wstring const& b ) const noexcept
    {
        return ali_sanoex(nocase_compare(pos1, n1, b.ref()));
    }

    int nocase_compare(
        int pos1, int n1,
        wstring const& b,
        int pos2, int n2 ) const noexcept;

    int nocase_compare( E const* str ) const = delete;

    int nocase_compare( wstring_const_ref str ) const noexcept
    {
        return ali_sanoex(ref().nocase_compare(ali::move(str)));
    }

    int nocase_compare(
        int pos1,
        int n1,
        E const* str ) const = delete;

    int nocase_compare(
        int pos1, int n1,
        wstring_const_ref str ) const noexcept;

    int nocase_compare(
        int pos1, int n1,
        E const* str, int n2 ) const = delete;

    int find( E const* str, int pos1 = 0 ) const = delete;

    int find( wstring_const_ref str, int pos1 = 0 ) const noexcept;
    int find( E const* str, int pos1, int n2 ) const = delete;
    int find( E c, int pos1 = 0 ) const noexcept;

    int nocase_find( E const* str, int pos1 = 0 ) const = delete;

    int nocase_find( wstring_const_ref str, int pos1 = 0 ) const noexcept;
    int nocase_find( E const* str, int pos1, int n2 ) const = delete;
    int nocase_find( E c, int pos1 = 0 ) const noexcept;

    int rfind( E const* str, int pos1 = npos ) const = delete;

    int rfind( wstring_const_ref str, int pos1 = npos ) const noexcept;
    int rfind( E const* str, int pos1, int n2 ) const = delete;
    int rfind( E c, int pos1 = npos ) const noexcept;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)

    int nocase_rfind( E const* str, int pos1 = npos ) const = delete;

    int nocase_rfind( wstring_const_ref str, int pos1 = npos ) const noexcept;
    int nocase_rfind( E const* str, int pos1, int n2 ) const = delete;
    int nocase_rfind( E c, int pos1 = npos ) const noexcept;
        //  post:   result == npos  //  Not found.
        //      ||  (0 <= result && result <= pos1)

    int find_first_of( wstring_const_ref set, int pos1 = 0 ) const noexcept;
    int find_first_not_of( wstring_const_ref set, int pos1 = 0 ) const noexcept;
    int find_last_not_of( wstring_const_ref set, int pos1 = npos ) const noexcept;

    // deprecated
    int find_any( wstring_const_ref set, int pos1 = 0 ) const noexcept
    {
        return ali_sanoex(find_first_of(ali::move(set), pos1));
    }

    // deprecated
    int find_neither( wstring_const_ref set, int pos1 = 0 ) const noexcept
    {
        return ali_sanoex(find_first_not_of(ali::move(set), pos1));
    }

    //  ref

    wstring_const_ref ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    c_wstring_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_wstring_const_ref{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            c_str(), size()}));
    }

    //  mutable_ref

    wstring_ref mutable_ref( int pos, int n ) noexcept
    {
        return ali_sanoex(mutable_ref().mutable_ref(pos, n));
    }

    wstring_ref mutable_ref( void ) noexcept
    {
        return ali_sanoex((wstring_ref{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            begin(), size()}));
    }

    blob_const_ref as_blob( void ) const noexcept
    {
        return ali_sanoex(ref().as_blob());
    }

    blob_ref as_mutable_blob( void ) noexcept
    {
        return ali_sanoex(mutable_ref().as_mutable_blob());
    }

private:    // Methods
    //static void check_range( int cur, int b, int e );

    //  If the compiler complains about this method being private, you have
    //  an error in your program's logic. Maybe you are using ali::optional<>
    //  directly instead of using operator *()?
    wstring& operator +=(bool) = delete;

ALI_IF_DEBUG_WEAK_REF(

    bool check_no_weak_ref( void ) const noexcept
    {
        bool const success{_weak_rc.get() == 0};

        if ( !success )
            debug::abort();

        return success;
    }

private:    //  Data members
    mutable debug::weak_reference::counter  _weak_rc{};

)   //  ALI_IF_DEBUG_WEAK_REF

};

// ******************************************************************
// ******************************************************************

}   //  namespace ali

#endif  //  !ALI_STRING_IS_ARRAY

namespace ali
{

namespace str
{

// ******************************************************************
template <int _capacity>
class  ALI_ATTRIBUTE_EMPTY_BASES fixed
// ******************************************************************
    :   public exposes_string_ref<fixed<_capacity>>
{
public:     //  Data members
    static constexpr int static_capacity{_capacity};

public:
    fixed( void ) noexcept
    {
        _str.push_back('\0');
    }

    char const* c_str( void ) const noexcept
    {
        ali_assert(_str.back() == '\0');
        return &_str[0];
    }

    static constexpr int capacity( void ) noexcept
    {
        return _capacity;
    }

    int size( void ) const noexcept
    {
        return _str.size() - 1;
    }

    fixed& append( string_const_ref str )
    {
        ali_assert(str.size() <= _capacity - size());

        string_const_ptr ptr = str.pointer_front(
            ali::mini(str.size(), _capacity - size()));

        _str.erase_back();
            //  Pop the terminating zero.

        for ( ; !ptr->is_empty(); ++ptr )
            _str.push_back(ptr->front());

        _str.push_back('\0');

        return *this;
    }

    fixed& append( int n, char c ) = delete;
        //  deprecated in favor of append_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    fixed& append_( char c )
    {
        ali_assert(1 <= _capacity - size());

        _str.back() = c;
        _str.push_back('\0');

        return *this;
    }

    fixed& append_( char c, int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= _capacity - size());

        n = ali::maxi(0, ali::mini(n, _capacity - size()));

        _str.erase_back();
            //  Pop the terminating zero.
        _str.resize(_str.size() + n, c);
        _str.push_back('\0');

        return *this;
    }

    fixed& resize( int n, char c = ' ' )
    {
        ali_assert(0 <= n);
        ali_assert(n <= _capacity);

        n = ali::maxi(0, ali::mini(n, _capacity));

        _str.erase_back();
            //  Pop the terminating zero.
        _str.resize(n, c);
        _str.push_back('\0');

        return *this;
    }

    fixed& erase( void ) noexcept
    {
        _str.erase().push_back('\0');
        return *this;
    }

    c_string_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_string_const_ref{c_str(), size()}));
    }

    auto ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    string_ref mutable_ref( void ) noexcept
    {
        return ali_sanoex((string_ref{&_str[0], size()}));
    }

    string_ref mutable_ref( int pos, int n ) noexcept
    {
        return ali_sanoex(mutable_ref().mutable_ref(pos, n));
    }

private:    // Data members
    fixed_capacity_array<char, _capacity + 1> _str;
};

// ******************************************************************
// ******************************************************************

}   //  namespace str

namespace wstr
{

// ******************************************************************
template <int _capacity>
class ALI_ATTRIBUTE_EMPTY_BASES fixed
// ******************************************************************
    :   public exposes_wstring_ref<fixed<_capacity>>
{
public:     //  Data members
    static constexpr int static_capacity{_capacity};

public:
    fixed( void ) noexcept
    {
        _str.push_back(L'\0');
    }

    ali::wchar const* c_str( void ) const noexcept
    {
        ali_assert(_str.back() == L'\0');
        return &_str[0];
    }

    static constexpr int capacity( void ) noexcept
    {
        return _capacity;
    }

    int size( void ) const noexcept
    {
        return _str.size() - 1;
    }

    fixed& append( wstring_const_ref str )
    {
        ali_assert(str.size() <= _capacity - size());

        wstring_const_ptr ptr = str.pointer_front(
            ali::mini(str.size(), _capacity - size()));

        _str.erase_back();
            //  Pop the terminating zero.

        for ( ; !ptr->is_empty(); ++ptr )
            _str.push_back(ptr->front());

        _str.push_back(L'\0');

        return *this;
    }

    fixed& append( int n, ali::wchar c ) = delete;
        //  deprecated in favor of append_ (to be renamed back later)
        //  in order to unify string and array interfaces.
        //  BEWARE: the order of parameters 'n' and 'c' is different.

    fixed& append_( ali::wchar c )
    {
        ali_assert(1 <= _capacity - size());

        _str.back() = c;
        _str.push_back(L'\0');

        return *this;
    }

    fixed& append_( ali::wchar c, int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= _capacity - size());

        n = ali::maxi(0, ali::mini(n, _capacity - size()));

        _str.erase_back();
            //  Pop the terminating zero.
        _str.resize(_str.size() + n, c);
        _str.push_back(L'\0');

        return *this;
    }

    fixed& resize( int n, ali::wchar c = L' ' )
    {
        ali_assert(0 <= n);
        ali_assert(n <= _capacity);

        n = ali::maxi(0, ali::mini(n, _capacity));

        _str.erase_back();
            //  Pop the terminating zero.
        _str.resize(n, c);
        _str.push_back(L'\0');

        return *this;
    }

    fixed& erase( void ) noexcept
    {
        _str.erase().push_back(L'\0');
        return *this;
    }

    c_wstring_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_wstring_const_ref{c_str(), size()}));
    }

    auto ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    wstring_ref mutable_ref( void ) noexcept
    {
        return ali_sanoex((wstring_ref{&_str[0], size()}));
    }

    wstring_ref mutable_ref( int pos, int n ) noexcept
    {
        return ali_sanoex(mutable_ref().mutable_ref(pos, n));
    }

private:    // Data members
    fixed_capacity_array<ali::wchar, _capacity + 1> _str;
};

// ******************************************************************
// ******************************************************************

}   //  namespace wstr

namespace functor_types
{

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename is_quoted_type>
struct unquote_in_place_ : private is_quoted_type
// ******************************************************************
{
    unquote_in_place_( is_quoted_type const& is_quoted ) noexcept(
        noexcept(is_quoted_type(is_quoted)))
    :   is_quoted_type(is_quoted)
    {}

    template <typename T>
    T& operator()( T& str ) const noexcept(
        noexcept(this->transform(str)))
    {
        this->transform(str);
        return str;
    }

private:    //  Methods
    void transform( ali::string& str ) const noexcept(
        noexcept(is_quoted_type::operator()(str)))
    {
        if ( is_quoted_type::operator()(str) )
            ali_sanoex(str.erase(str.size() - 1).erase(0, 1));
    }

    void transform( ali::wstring& str ) const noexcept(
        noexcept(is_quoted_type::operator()(str)))
    {
        if ( is_quoted_type::operator()(str) )
            ali_sanoex(str.erase(str.size() - 1).erase(0, 1));
    }

    void transform( string_const_ptr& str ) const noexcept(
        noexcept(is_quoted_type::operator()(*str)))
    {
        if ( is_quoted_type::operator()(*str) )
            ali_sanoex(str = str->pointer(1, str->size() - 2));
    }

    void transform( wstring_const_ptr& str ) const noexcept(
        noexcept(is_quoted_type::operator()(*str)))
    {
        if ( is_quoted_type::operator()(*str) )
            ali_sanoex(str = str->pointer(1, str->size() - 2));
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unquote_in_place : unquote_in_place_<functor_types::is_quoted>
// ******************************************************************
{
    unquote_in_place( void ) noexcept(
        noexcept(unquote_in_place_<functor_types::is_quoted>{ali::is_quoted}))
    :   unquote_in_place_<functor_types::is_quoted>{ali::is_quoted}
    {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct quote_in_place
// ******************************************************************
{
    template <typename T>
    T& operator()( T& str ) const
    {
        this->transform(str);
        return str;
    }

private:    //  Methods
    void transform( ali::string& str ) const
    {
        str.insert_(0, '\"').append_('\"');
    }

    void transform( ali::wstring& str ) const
    {
        str.insert_(0, L'\"').append_(L'\"');
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct quote_in_place_1
// ******************************************************************
{
    quote_in_place_1( char quote ) noexcept
    :   quote{quote},
        wquote{static_cast<wchar>(quote)}
    {}

    quote_in_place_1( char quote, wchar wquote ) noexcept
    :   quote{quote},
        wquote{wquote}
    {}

    template <typename T>
    T& operator()( T& str ) const
    {
        this->transform(str);
        return str;
    }

private:
    void transform( ali::string& str ) const
    {
        str.insert_(0, quote).append_(quote);
    }

    void transform( ali::wstring& str ) const
    {
        str.insert_(0, wquote).append_(wquote);
    }

    char        quote;
    wchar       wquote;
};

// ******************************************************************
// ******************************************************************

}   //  namespace functor_types

// ******************************************************************
extern functor_types::unquote_in_place const unquote_in_place;
// ******************************************************************

// ******************************************************************
template <typename is_quoted_type>
inline functor_types::unquote_in_place_<is_quoted_type>
    make_unquote_in_place( is_quoted_type const& is_quoted ) noexcept(
        noexcept(functor_types::unquote_in_place_<is_quoted_type>{is_quoted}))
// ******************************************************************
{
    return functor_types::unquote_in_place_<is_quoted_type>{is_quoted};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
extern functor_types::quote_in_place const quote_in_place;
// ******************************************************************

// ******************************************************************
inline functor_types::quote_in_place_1 make_quote_in_place(
    char quote ) noexcept
// ******************************************************************
{
    return ali_sanoex(functor_types::quote_in_place_1{quote});
}

// ******************************************************************
inline functor_types::quote_in_place_1 make_quote_in_place(
    char quote, wchar wquote ) noexcept
// ******************************************************************
{
    return ali_sanoex((functor_types::quote_in_place_1{quote, wquote}));
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
// use ali::to_upper from ali_ctype.h
ALI_DEPRECATED(
inline ali::string& to_upper_case( ali::string& str ) noexcept(
    noexcept(to_upper_in_place(str)))
// ******************************************************************
{
    return to_upper_in_place(str);
});

// ******************************************************************
// use ali::to_lower from ali_ctype.h
ALI_DEPRECATED(
inline ali::string& to_lower_case( ali::string& str ) noexcept(
    noexcept(to_lower_in_place(str)))
// ******************************************************************
{
    return to_lower_in_place(str);
});

// ******************************************************************
inline ali::string operator+( ali::string s1, string_const_ref s2 )
// ******************************************************************
{
    s1.append(ali::move(s2));
    return s1;
}

// ******************************************************************
inline ali::string operator+( ali::string s, ali::string::value_type c )
// ******************************************************************
{
    s.push_back(c);
    return s;
}

// ******************************************************************
inline ali::string operator+( ali::string::value_type c, ali::string s )
// ******************************************************************
{
    s.push_front(c);
    return s;
}

// ******************************************************************
inline ali::wstring operator+( ali::wstring s1, wstring_const_ref s2 )
// ******************************************************************
{
    s1.append(ali::move(s2));
    return s1;
}

// ******************************************************************
inline ali::wstring operator+( ali::wstring s, ali::wstring::value_type c )
// ******************************************************************
{
    s.push_back(c);
    return s;
}

// ******************************************************************
inline ali::wstring operator+( ali::wstring::value_type c, ali::wstring s )
// ******************************************************************
{
    s.push_front(c);
    return s;
}

// ******************************************************************
ali::string& prepad(
    ali::string& s, int n,
    ali::string::value_type c );
// ******************************************************************

// ******************************************************************
ali::wstring& prepad(
    ali::wstring& s, int n,
    ali::wstring::value_type c );
// ******************************************************************

// ******************************************************************
inline ali::string& trim_right( ali::string& str ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(str.ref().trim_right().size())};
    return ali_sanoex(str.erase(trimmed_size));
}

// ******************************************************************
inline ali::string& trim_left( ali::string& str ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(str.ref().trim_left().size())};
    return ali_sanoex(str.erase(0, str.size() - trimmed_size));
}

// ******************************************************************
inline ali::string& trim( ali::string& str ) noexcept
// ******************************************************************
{
    return ali_sanoex(trim_left(trim_right(str)));
}

// ******************************************************************
inline ali::wstring& trim_right( ali::wstring& str ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(str.ref().trim_right().size())};
    return ali_sanoex(str.erase(trimmed_size));
}

// ******************************************************************
inline ali::wstring& trim_left( ali::wstring& str ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(str.ref().trim_left().size())};
    return ali_sanoex(str.erase(0, str.size() - trimmed_size));
}

// ******************************************************************
inline ali::wstring& trim( ali::wstring& str ) noexcept
// ******************************************************************
{
    return ali_sanoex(trim_left(trim_right(str)));
}
	
// ******************************************************************
inline ali::string& trim_right(
    ali::string& str,
    string_const_ref chars ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(
        str.ref().trim_right(ali::move(chars)).size())};
    return ali_sanoex(str.erase(trimmed_size));
}

// ******************************************************************
inline ali::string& trim_left(
    ali::string& str,
    string_const_ref chars ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(
        str.ref().trim_left(ali::move(chars)).size())};
    return ali_sanoex(str.erase(0, str.size() - trimmed_size));
}

// ******************************************************************
inline ali::string& trim(
    ali::string& str,
    string_const_ref chars ) noexcept
// ******************************************************************
{
	return ali_sanoex(trim_left(trim_right(str, chars), chars));
}

// ******************************************************************
inline ali::wstring& trim_right(
    ali::wstring& str,
    wstring_const_ref chars ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(
        str.ref().trim_right(ali::move(chars)).size())};
    return ali_sanoex(str.erase(trimmed_size));
}

// ******************************************************************
inline ali::wstring& trim_left(
    ali::wstring& str,
    wstring_const_ref chars ) noexcept
// ******************************************************************
{
    int const trimmed_size{ali_sanoex(
        str.ref().trim_left(ali::move(chars)).size())};
    return ali_sanoex(str.erase(0, str.size() - trimmed_size));
}

// ******************************************************************
inline ali::wstring& trim(
    ali::wstring& str,
    wstring_const_ref chars ) noexcept
// ******************************************************************
{
	return ali_sanoex(trim_left(trim_right(str, chars), chars));
}
    
ali::string& strip_diacritics_in_place(ali::string & str);
ali::string& strip_diacritics(ali::string & output, ali::string_const_ref input);

ali::wstring& strip_diacritics_in_place(ali::wstring & str);
ali::wstring& strip_diacritics(ali::wstring & output, ali::wstring_const_ref input);

ali::string& transliterate_to_latin_in_place(ali::string & str);
ali::string& transliterate_to_latin(ali::string & output, ali::string_const_ref input);

ali::wstring& transliterate_to_latin_in_place(ali::wstring & str);
ali::wstring& transliterate_to_latin(ali::wstring & output, ali::wstring_const_ref input);


// ******************************************************************
inline int find_non_quoted(
    ali::string const& str, char c, int pos ) noexcept
// ******************************************************************
{
    return ali_sanoex(pos + str.pointer_right(pos)
        ->index_of_first_unquoted(c));
}

// ******************************************************************
inline int find_non_quoted(
    ali::wstring const& str, wchar c, int pos ) noexcept
// ******************************************************************
{
    return ali_sanoex(pos + str.pointer_right(pos)
        ->index_of_first_unquoted(c));
}

// ******************************************************************
array<bool> test_string_in_place_transforms( void );
// ******************************************************************

// ******************************************************************
array<bool> test_string( void );
// ******************************************************************

}   //  namespace ali

