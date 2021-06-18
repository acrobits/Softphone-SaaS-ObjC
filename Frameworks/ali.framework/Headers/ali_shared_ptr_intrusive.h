/*
 *  ali_shared_ptr_intrusive.h
 *  ali Library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_auto_ptr.h"
#include "ali/ali_debug.h"
#include "ali/ali_exception_memory.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_typeof.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
template <typename T>
class shared_ptr_intrusive
// ******************************************************************
{
public:     // Typedefs
    typedef T element_type;

public:
    shared_ptr_intrusive( void ) = default;

    template <typename U>
    shared_ptr_intrusive( shared_ptr_intrusive<U> const& b )
    :   shared_ptr_intrusive{b._ptr}
    {}

    shared_ptr_intrusive( shared_ptr_intrusive const& b )
    :   shared_ptr_intrusive{b._ptr}
    {}

    template <typename U>
    shared_ptr_intrusive( shared_ptr_intrusive<U>&& b )
    :   _ptr{b._ptr}
    {
        b._ptr = nullptr;
    }

    shared_ptr_intrusive( shared_ptr_intrusive&& b )
    :   _ptr{b._ptr}
    {
        b._ptr = nullptr;
    }

    shared_ptr_intrusive( ali::nullptr_type ) {}

    explicit shared_ptr_intrusive( T* ptr )
    :   _ptr{ptr}
    {
        if ( _ptr != nullptr )
            _ptr->retain();
    }

    template <typename U>
    explicit shared_ptr_intrusive( ali::auto_ptr<U> ptr )
    :   shared_ptr_intrusive{ptr.get()}
    {
        ptr.release();
    }

    ~shared_ptr_intrusive( void )
    {
        if ( _ptr != nullptr )
            _ptr->release();
    }

    template <typename U>
    shared_ptr_intrusive& operator=( shared_ptr_intrusive<U> const& b )
    {
        shared_ptr_intrusive{b}.swap(*this);
        return *this;
    }

    shared_ptr_intrusive& operator=( shared_ptr_intrusive b )
    {
        swap(b);
        return *this;
    }

    T* get( void ) const
    {
        return _ptr;
    }

    void reset( void )
    {
        shared_ptr_intrusive{}.swap(*this);
    }

    void reset( ali::auto_ptr<T> ptr )
    {
        shared_ptr_intrusive{ali::move(ptr)}.swap(*this);
    }

    T& operator*( void ) const
    {
        ali_assert(_ptr != nullptr);
        return *_ptr;
    }

    T* operator->( void ) const
    {
        ali_assert(_ptr != nullptr);
        return _ptr;
    }

    bool is_null( void ) const
    {
        return _ptr == nullptr;
    }

    bool operator==( ali::nullptr_type ) const
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const
    {
        return !is_null();
    }

    template <typename U>
    bool operator==( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr == b.get();
    }

    template <typename U>
    bool operator!=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr != b.get();
    }

    template <typename U>
    bool operator<( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr < b.get();
    }

    template <typename U>
    bool operator<=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr <= b.get();
    }

    template <typename U>
    bool operator>( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr > b.get();
    }

    template <typename U>
    bool operator>=( shared_ptr_intrusive<U> const& b ) const
    {
        return _ptr >= b.get();
    }

    template <typename U>
    friend int compare(
        shared_ptr_intrusive const& a,
        shared_ptr_intrusive<U> const& b )
    {
        using ali::compare;
        return compare(a._ptr, b._ptr);
    }

    void swap( shared_ptr_intrusive& b )
    {
        using ali::swap;
        swap(_ptr, b._ptr);
    }

    friend void swap( shared_ptr_intrusive& a, shared_ptr_intrusive& b )
    {
        a.swap(b);
    }

private:    // Data members
    T*  _ptr{};

template <typename U>
friend class shared_ptr_intrusive;
};

// ******************************************************************
template <typename T, typename... Params>
inline ali::shared_ptr_intrusive<T> new_shared_ptr_intrusive( Params&&... params )
// ******************************************************************
{
    return ali::shared_ptr_intrusive<T>{
        new_auto_ptr<T>(ali::forward<Params>(params)...)};

}

}   //  namespace ali
