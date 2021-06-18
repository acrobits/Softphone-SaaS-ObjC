#pragma once
#include "ali/ali_array_utils.h"
#include "ali/ali_atomic.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_wchar.h"

namespace ali
{

// ******************************************************************
class ALI_ATTRIBUTE_EMPTY_BASES stable_string
// ******************************************************************
//  A stable_string object stores immutable character data.
//  However, it is possible to assign new character data
//  to an existing stable_string object.
// ******************************************************************
    : public exposes_string_const_ref<stable_string>
{
    using E = char;

public:     //  Struct
    struct pool_info;
    struct pool_info_entry;

public:
    stable_string( void ) noexcept
    :   _begin{&_empty.begin}
    {}

    stable_string( stable_string const& b ) noexcept
    :   _begin{ali_sanoex(retain(b._begin))}
    {}

    stable_string( stable_string&& b ) noexcept
    :   _begin{b._begin}
    {
        b._begin = &_empty.begin;
    }

    stable_string( E const* str ) = delete;
    stable_string( E const* str, int n ) = delete;
    
    stable_string( string_const_ref b )
    :   _begin{construct(b)}
    {}

    ~stable_string( void ) noexcept
    {
        ali_sanoex(release(_begin));
    }

    stable_string& operator=( stable_string b ) noexcept
    {
        ali_sanoex(swap(b));
        return *this;
    }

    stable_string& operator=( string_const_ref b )
    {
        return operator=(stable_string{b});
    }

    stable_string& operator=( E const* str ) = delete;

    bool is_empty( void ) const noexcept
    {
        return ali_sanoex(is_empty(_begin));
    }

    int size( void ) const noexcept
    {
        return ali_sanoex(header_of(_begin)->size);
    }

    E const* data( void ) const noexcept
    {
        return _begin;
    }

    E const* c_str( void ) const noexcept
    {
        return _begin;
    }
    
    E const& at( int i ) const noexcept
    {
        ali_assert(0 <= i);
        ali_assert(i < size());
        return _begin[i];
    }
    
    E const& operator[]( int i ) const noexcept
    {
        return ali_sanoex(at(i));
    }

    stable_string& erase( void ) noexcept
    {
        ali_sanoex(stable_string{}.swap(*this));
        return *this;
    }

    void swap( stable_string& b ) noexcept
    {
        using ali::swap;
        ali_sanoex(swap(_begin, b._begin));
    }

    friend void swap( stable_string& a, stable_string& b ) noexcept
    {
        ali_sanoex(a.swap(b));
    }

    //  ref

    string_const_ref ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    c_string_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_string_const_ref{_begin, size()}));
    }

    blob_const_ref as_blob( void ) const noexcept
    {
        return ali_sanoex(ref().as_blob());
    }

    //  misc

    void assert_invariant( void ) const noexcept {}

    static pool_info& get_pool_info( pool_info& info );

private:
    //  We implement the stable_string using
    //  a variant of shared_ptr scheme with
    //  a global (thread local) pool of N
    //  most frequent strings.

private:    //  Struct
    struct header
    {
        header( int size ) noexcept
        :   size{size}
        {}

        ali::atomic::counter    rc{1};
        int const               size{};
    };

    ali_static_assert(alignof(E) <= alignof(header));

private:    //  Struct
    struct empty
    {
        alignas(header) ali::uint8  h[sizeof(header)];
        E                           begin;
    };

    ali_static_assert(meta::is_trivially_copyable<empty>::result);

    ali_static_assert(offsetof(empty, begin) == sizeof(header));

private:    //  Class
    class pool;
    struct wrap_only{};

private:    //  Methods
    stable_string( E const* str, wrap_only ) noexcept
    :   _begin{str}
    {}

    static E const* construct( string_const_ref str );
    
    static void destroy( header* h ) noexcept
    {
        ali_sanoex(h->~header());

        ali_sanoex(delete_array(reinterpret_cast<char*>(h)));
    }

    static E const* retain( E const* str ) noexcept
    {
        if ( ali_sanoex(!is_empty(str)) )
            ali_sanoex(header_of(str)->rc.inc());

        return str;
    }

    static void release( E const* str ) noexcept
    {
        if ( ali_sanoex(is_empty(str)) )
            return;

        header* const h{ali_sanoex(header_of(str))};
    
        if ( ali_sanoex(h->rc.dec() == 0) )
            ali_sanoex(destroy(h));
    }

    static bool is_empty( E const* str ) noexcept
    {
        return str == &_empty.begin;
    }
    
    static header* header_of( E const* str ) noexcept
    {
        return const_cast<header*>(reinterpret_cast<
            header const*>(str) - 1);
    }

private:    //  Data members
    E const*            _begin{};
    static empty const  _empty;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class ALI_ATTRIBUTE_EMPTY_BASES stable_wstring
// ******************************************************************
//  A stable_string object stores immutable character data.
//  However, it is possible to assign new character data
//  to an existing stable_string object.
// ******************************************************************
    : public exposes_string_const_ref<stable_wstring>
{
    using E = ali::wchar;

public:     //  Struct
    struct pool_info;
    struct pool_info_entry;

public:
    stable_wstring( void ) noexcept
    :   _begin{&_empty.begin}
    {}

    stable_wstring( stable_wstring const& b ) noexcept
    :   _begin{ali_sanoex(retain(b._begin))}
    {}

    stable_wstring( stable_wstring&& b ) noexcept
    :   _begin{b._begin}
    {
        b._begin = &_empty.begin;
    }

    stable_wstring( E const* str ) = delete;
    stable_wstring( E const* str, int n ) = delete;
    
    stable_wstring( wstring_const_ref b )
    :   _begin{construct(b)}
    {}

    ~stable_wstring( void ) noexcept
    {
        ali_sanoex(release(_begin));
    }

    stable_wstring& operator=( stable_wstring b ) noexcept
    {
        ali_sanoex(swap(b));
        return *this;
    }

    stable_wstring& operator=( wstring_const_ref b )
    {
        return operator=(stable_wstring{b});
    }

    stable_wstring& operator=( E const* str ) = delete;

    bool is_empty( void ) const noexcept
    {
        return ali_sanoex(is_empty(_begin));
    }

    int size( void ) const noexcept
    {
        return ali_sanoex(header_of(_begin)->size);
    }

    E const* data( void ) const noexcept
    {
        return _begin;
    }

    E const* c_str( void ) const noexcept
    {
        return _begin;
    }
    
    E const& at( int i ) const noexcept
    {
        ali_assert(0 <= i);
        ali_assert(i < size());
        return _begin[i];
    }
    
    E const& operator[]( int i ) const noexcept
    {
        return ali_sanoex(at(i));
    }

    stable_wstring& erase( void ) noexcept
    {
        ali_sanoex(stable_wstring{}.swap(*this));
        return *this;
    }

    void swap( stable_wstring& b ) noexcept
    {
        using ali::swap;
        ali_sanoex(swap(_begin, b._begin));
    }

    friend void swap( stable_wstring& a, stable_wstring& b ) noexcept
    {
        ali_sanoex(a.swap(b));
    }

    //  ref

    wstring_const_ref ref( int pos, int n ) const noexcept
    {
        return ali_sanoex(ref().ref(pos, n));
    }

    c_wstring_const_ref ref( void ) const noexcept
    {
        return ali_sanoex((c_wstring_const_ref{_begin, size()}));
    }

    blob_const_ref as_blob( void ) const noexcept
    {
        return ali_sanoex(ref().as_blob());
    }

    //  misc

    void assert_invariant( void ) const noexcept {}

    static pool_info& get_pool_info( pool_info& info );

private:
    //  We implement the stable_wstring using
    //  a variant of shared_ptr scheme with
    //  a global (thread local) pool of N
    //  most frequent strings.

private:    //  Struct
    struct header
    {
        header( int size ) noexcept
        :   size{size}
        {}

        ali::atomic::counter    rc{1};
        int const               size{};
    };

    ali_static_assert(alignof(E) <= alignof(header));

private:    //  Struct
    struct empty
    {
        alignas(header) ali::uint8  h[sizeof(header)];
        E                           begin;
    };

    ali_static_assert(meta::is_trivially_copyable<empty>::result);

    ali_static_assert(offsetof(empty, begin) == sizeof(header));

private:    //  Class
    class pool;
    struct wrap_only{};

private:    //  Methods
    stable_wstring( E const* str, wrap_only ) noexcept
    :   _begin{str}
    {}

    static E const* construct( wstring_const_ref str );
    
    static void destroy( header* h ) noexcept
    {
        ali_sanoex(h->~header());

        ali_sanoex(delete_array(reinterpret_cast<char*>(h)));
    }

    static E const* retain( E const* str ) noexcept
    {
        if ( ali_sanoex(!is_empty(str)) )
            ali_sanoex(header_of(str)->rc.inc());

        return str;
    }

    static void release( E const* str ) noexcept
    {
        if ( ali_sanoex(is_empty(str)) )
            return;

        header* const h{ali_sanoex(header_of(str))};
    
        if ( ali_sanoex(h->rc.dec() == 0) )
            ali_sanoex(destroy(h));
    }

    static bool is_empty( E const* str ) noexcept
    {
        return str == &_empty.begin;
    }
    
    static header* header_of( E const* str ) noexcept
    {
        return const_cast<header*>(reinterpret_cast<
            header const*>(str) - 1);
    }

private:    //  Data members
    E const*            _begin{};
    static empty const  _empty;
};

}   //  namespace ali
