/*
 *  ali_auto_ptr.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_auto_ptr_forward.h"
#include "ali/ali_debug.h"
#include "ali/ali_deprecated.h"
#include "ali/ali_exception_memory.h"
#include "ali/ali_features.h"
#include "ali/ali_memory.h"
#include "ali/ali_meta.h"
#include "ali/ali_noncopyable.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_utility.h"

namespace ali
{

namespace hidden
{

// ******************************************************************
template <typename T>
inline void* most_derived_object( T* ptr )
// ******************************************************************
{
    if constexpr ( meta::is_polymorphic<T>::result )
    {
        return dynamic_cast<void*>(ptr);
            //
            //  https://en.cppreference.com/w/cpp/language/dynamic_cast
            //
            //  dynamic_cast < new_type > ( expression )
            //
            //  If expression is a pointer to a polymorphic type,
            //  and new_type is a pointer to void, the result is
            //  a pointer to the most derived object pointed
            //  or referenced by expression.
            //
    }
    else
    {
        return ptr;
    }
}

// ******************************************************************
template <typename T>
inline void const* most_derived_object( T const* ptr )
// ******************************************************************
{
    if constexpr ( meta::is_polymorphic<T>::result )
    {
        return dynamic_cast<void const*>(ptr);
            //
            //  https://en.cppreference.com/w/cpp/language/dynamic_cast
            //
            //  dynamic_cast < new_type > ( expression )
            //
            //  If expression is a pointer to a polymorphic type,
            //  and new_type is a pointer to void, the result is
            //  a pointer to the most derived object pointed
            //  or referenced by expression.
            //
    }
    else
    {
        return ptr;
    }
}

}   //  namespace hidden

// ******************************************************************
template <typename T>
inline void delete_scalar( T* ptr ) noexcept
// ******************************************************************
{
    ALI_IF_TRACK_DYNAMIC_OBJECTS(
        if ( ptr == nullptr ) ;
        else debug::deleting_dynamic_object(
            hidden::most_derived_object(ptr),
            typeid(T).name(),
            meta::is_polymorphic<T>::result));

    delete ptr;
}

// ******************************************************************
template <typename T>
inline void delete_array( T* arr ) noexcept
// ******************************************************************
{
    ALI_IF_TRACK_DYNAMIC_OBJECTS(
        if ( arr == nullptr ) ;
        else debug::deleting_dynamic_object(
            arr, typeid(T[]).name(), false));

    delete[] arr;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class auto_ptr
// ******************************************************************
{
public:
    auto_ptr( void ) noexcept = default;

    auto_ptr( auto_ptr const& ) = delete;

    auto_ptr( auto_ptr&& b ) noexcept
    :   _ptr{b.release()}
    {
        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U>&& b ) noexcept
    :   _ptr{b.release()}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U[]>&& b ) = delete;

    template <typename U>
    auto_ptr( auto_ptr<U>&& b, meta::static_pointer_cast_tag ) noexcept
    :   _ptr{static_cast<T*>(b.release())}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U[]>&& b, meta::static_pointer_cast_tag ) = delete;

    template <typename U>
    auto_ptr( auto_ptr<U>&& b, meta::dynamic_pointer_cast_tag ) noexcept
    :   _ptr{dynamic_cast<T*>(b.release())}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U[]>&& b, meta::dynamic_pointer_cast_tag ) = delete;

    template <typename U>
    auto_ptr( auto_ptr<U>&& b, meta::const_pointer_cast_tag ) noexcept
    :   _ptr{const_cast<T*>(b.release())}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U[]>&& b, meta::const_pointer_cast_tag ) = delete;

    template <typename U>
    auto_ptr( auto_ptr<U>&& b, meta::reinterpret_pointer_cast_tag ) noexcept
    :   _ptr{reinterpret_cast<T*>(b.release())}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U[]>&& b, meta::reinterpret_pointer_cast_tag ) = delete;
    
    auto_ptr( ali::nullptr_type ) noexcept {}

    explicit auto_ptr( T* ptr ) noexcept
    :   _ptr{ptr}
    {
        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    explicit auto_ptr( U* ptr ) noexcept
    :   auto_ptr{static_cast<T*>(ptr)}
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");
    }

    ~auto_ptr( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        delete_scalar(_ptr);

        _ptr = invalid_pointer_value;
    }

    auto_ptr& operator=( auto_ptr const& ) = delete;

    template <typename U>
    auto_ptr& operator=( auto_ptr<U> const& ) = delete;

    auto_ptr& operator=( auto_ptr&& b ) noexcept
    {
        reset(b.release());

        return *this;
    }

    template <typename U>
    auto_ptr& operator=( auto_ptr<U>&& b ) noexcept
    {
        reset(b.release());

        return *this;
    }

    template <typename U>
    auto_ptr& operator=( auto_ptr<U[]>&& b ) = delete;

    T& operator*( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ali_assert(_ptr != nullptr);
        return *_ptr;
    }

    T* operator->( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ali_assert(_ptr != nullptr);
        return _ptr;
    }

    bool is_null( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        return _ptr == nullptr;
    }

    friend bool is_null( auto_ptr const& a ) noexcept
    {
        return a.is_null();
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    T* get( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        return _ptr;
    }

    T* reset( void ) noexcept
    {
        return reset(nullptr);
    }

    T* reset( T* ptr ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        using ali::swap;

        swap(_ptr, ptr);

        if ( _ptr != ptr )
            delete_scalar(ptr);

        return _ptr;
    }

    template <typename U>
    T* reset( U* ptr ) noexcept
    {
        static_assert(
                meta::is_same_type_remove_cv<T, U>::result
            ||  meta::has_virtual_destructor<T>::result,
            "meta::has_virtual_destructor<T>::result");

        return reset(static_cast<T*>(ptr));
    }

    T* release( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        T* const t{_ptr};

        _ptr = nullptr;

        return t;
    }

    void swap( auto_ptr& b ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;

        swap(_ptr, b._ptr);
    }

    friend void swap( auto_ptr& a, auto_ptr& b ) noexcept
    {
        a.swap(b);
    }

    void assert_invariant( void ) const noexcept
    {
        ali_assert(_ptr != invalid_pointer_value);
    }

private:    //  Data members
    T*  _ptr{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class auto_ptr<T[]>
// ******************************************************************
{
public:
    auto_ptr( void ) noexcept = default;

    auto_ptr( auto_ptr const& ) = delete;

    template <typename U>
    auto_ptr( auto_ptr<U> const& ) = delete;

    auto_ptr( auto_ptr&& b ) noexcept
    :   _arr{b.release()}
    {
        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename U>
    auto_ptr( auto_ptr<U>&& ) = delete;

    auto_ptr( ali::nullptr_type ) noexcept {}

    explicit auto_ptr( T* arr ) noexcept
    :   _arr{arr}
    {
        ALI_IF_DEBUG(assert_invariant());
    }

    ~auto_ptr( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        delete_array(_arr);

        _arr = invalid_pointer_value;
    }

    auto_ptr& operator=( auto_ptr const& ) = delete;

    template <typename U>
    auto_ptr& operator=( auto_ptr<U> const& ) = delete;

    auto_ptr& operator=( auto_ptr&& b ) noexcept
    {
        reset(b.release());

        return *this;
    }

    template <typename U>
    auto_ptr& operator=( auto_ptr<U>&& ) = delete;

    T& operator[]( int i ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ali_assert(_arr != nullptr);
        return _arr[i];
    }

    bool is_null( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        return _arr == nullptr;
    }

    friend bool is_null( auto_ptr const& a ) noexcept
    {
        return a.is_null();
    }

    bool operator==( ali::nullptr_type ) const noexcept
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const noexcept
    {
        return !is_null();
    }

    T* get( void ) const noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        return _arr;
    }

    T* reset( void ) noexcept
    {
        return reset(nullptr);
    }

    T* reset( T* arr ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        using ali::swap;
        
        swap(_arr, arr);

        if ( _arr != arr )
            delete_array(arr);

        return _arr;
    }

    T* release( void ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());

        T* const t{_arr};

        _arr = nullptr;

        return t;
    }

    void swap( auto_ptr& b ) noexcept
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;
        swap(_arr, b._arr);
    }

    friend void swap( auto_ptr& a, auto_ptr& b ) noexcept
    {
        a.swap(b);
    }

    void assert_invariant( void ) const noexcept
    {
        ali_assert(_arr != invalid_pointer_value);
    }

private:    //  Data members
    T*  _arr{};
};

// ******************************************************************
// ******************************************************************

namespace meta
{

// ******************************************************************
template <typename T>
struct effective<ali::auto_ptr<T>>
// ******************************************************************
    :   define_type<ali::auto_ptr<T>> {};

// ******************************************************************
// ******************************************************************

}   //  namespace meta

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class auto_ptr_lvalue
// ******************************************************************
{
public:
    auto_ptr_lvalue( T*& ptr ) noexcept
    :   _ptr(ptr)
    {}

    operator T*( void ) const noexcept { return _ptr; }
    T* operator->( void ) const noexcept { return _ptr; }

    auto_ptr_lvalue<T>& operator=( ali::auto_ptr<T> b ) noexcept
    {
        return this->operator=(b.release());
    }

    auto_ptr_lvalue<T>& operator=( T* b ) noexcept
    {
        delete_scalar(_ptr);
        _ptr = b;
        return *this;
    }

private:
    auto_ptr_lvalue<T>& operator=(
        auto_ptr_lvalue<T> const& ) = delete;

private:
    T*& _ptr;
};

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T, typename... Params>
inline ali::auto_ptr<T> new_auto_ptr(
    meta::define_type<T>, Params&&... params )
// ******************************************************************
{
#ifndef ALI_TRACK_DYNAMIC_OBJECTS

    return ali::auto_ptr<T>{out_of_memory_if_null(
        new T{ali::forward<Params>(params)...}, ALI_HERE)};

#else   //  ALI_TRACK_DYNAMIC_OBJECTS

    ali::auto_ptr<T> temp{out_of_memory_if_null(
        new T{ali::forward<Params>(params)...}, ALI_HERE)};

    debug::created_dynamic_object(
        temp.get(), sizeof(T), typeid(T).name(),
        meta::is_polymorphic<T>::result);

    return temp;

#endif  //  ALI_TRACK_DYNAMIC_OBJECTS
}

// ******************************************************************
template <typename T>
inline ali::auto_ptr<T[]> new_auto_arr( size_t size )
// ******************************************************************
{
#ifndef ALI_TRACK_DYNAMIC_OBJECTS

    return ali::auto_ptr<T[]>{out_of_memory_if_null(
        new T[size], ALI_HERE)};

#else   //  ALI_TRACK_DYNAMIC_OBJECTS

    ali::auto_ptr<T[]> temp{out_of_memory_if_null(
        new T[size], ALI_HERE)};

    debug::created_dynamic_object(
        temp.get(), sizeof(T) * size,
        typeid(T[]).name(), false);

    return temp;

#endif  //  ALI_TRACK_DYNAMIC_OBJECTS

}

// ******************************************************************
template <typename T, typename... Params>
inline ali::auto_ptr<T[]> new_auto_ptr(
    meta::define_type<T[]>, Params&&... params )
// ******************************************************************
{
    return new_auto_arr<T>(ali::forward<Params>(params)...);
}

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename T, typename... Params>
inline ali::auto_ptr<T> new_auto_ptr( Params&&... params )
// ******************************************************************
{
    return hidden::new_auto_ptr(
        meta::define_type<T>{},
        ali::forward<Params>(params)...);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
inline ali::auto_ptr<T> static_pointer_cast( ali::auto_ptr<U> ptr )
// ******************************************************************
{
    return ali::auto_ptr<T>{ali::move(ptr),
        meta::static_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::auto_ptr<T> dynamic_pointer_cast( ali::auto_ptr<U> ptr )
// ******************************************************************
{
    return ali::auto_ptr<T>{ali::move(ptr),
        meta::dynamic_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::auto_ptr<T> const_pointer_cast( ali::auto_ptr<U> ptr )
// ******************************************************************
{
    return ali::auto_ptr<T>{ali::move(ptr),
        meta::const_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
ali::auto_ptr<T> reinterpret_pointer_cast( ali::auto_ptr<U> ptr )
// ******************************************************************
{
    return ali::auto_ptr<T>{ali::move(ptr),
        meta::reinterpret_pointer_cast_tag{}};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename F>
class private_auto_ptr_factory
// ******************************************************************
//  This class is inteded to be a target of (mutual)
//  friendship with F.
//  Useful when T's constructor accessibility is limited.
// ******************************************************************
{
private:    //  Class
    template <typename T = F>
    class auto_ptr
    {
    public:
        auto_ptr( void ) noexcept = default;

        auto_ptr( auto_ptr const& ) = delete;

        auto_ptr( auto_ptr&& b ) noexcept
        :   _ptr{b.release()}
        {
            ALI_IF_DEBUG(assert_invariant());
        }

        template <typename U>
        auto_ptr( auto_ptr<U>&& b ) noexcept
        :   _ptr{b.release()}
        {
            static_assert(
                    meta::is_same_type_remove_cv<T, U>::result
                ||  meta::has_virtual_destructor<T>::result,
                "meta::has_virtual_destructor<T>::result");

            ALI_IF_DEBUG(assert_invariant());
        }

        template <typename U>
        auto_ptr( auto_ptr<U[]>&& b ) = delete;

        auto_ptr( ali::nullptr_type ) noexcept {}

        explicit auto_ptr( T* ptr ) noexcept
        :   _ptr{ptr}
        {
            ALI_IF_DEBUG(assert_invariant());
        }

        template <typename U>
        explicit auto_ptr( U* ptr ) noexcept
        :   auto_ptr{static_cast<T*>(ptr)}
        {
            static_assert(
                    meta::is_same_type_remove_cv<T, U>::result
                ||  meta::has_virtual_destructor<T>::result,
                "meta::has_virtual_destructor<T>::result");
        }

        ~auto_ptr( void ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            private_auto_ptr_factory::delete_scalar(_ptr);

            _ptr = invalid_pointer_value;
        }

        auto_ptr& operator=( auto_ptr const& ) = delete;

        template <typename U>
        auto_ptr& operator=( auto_ptr<U> const& ) = delete;

        auto_ptr& operator=( auto_ptr&& b ) noexcept
        {
            reset(b.release());

            return *this;
        }

        template <typename U>
        auto_ptr& operator=( auto_ptr<U>&& b ) noexcept
        {
            reset(b.release());

            return *this;
        }

        template <typename U>
        auto_ptr& operator=( auto_ptr<U[]>&& b ) = delete;

        T& operator*( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            ali_assert(_ptr != nullptr);
            return *_ptr;
        }

        T* operator->( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            ali_assert(_ptr != nullptr);
            return _ptr;
        }

        bool is_null( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            return _ptr == nullptr;
        }

        friend bool is_null( auto_ptr const& a ) noexcept
        {
            return a.is_null();
        }

        bool operator==( ali::nullptr_type ) const noexcept
        {
            return is_null();
        }

        bool operator!=( ali::nullptr_type ) const noexcept
        {
            return !is_null();
        }

        T* get( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            return _ptr;
        }

        T* reset( void ) noexcept
        {
            return reset(nullptr);
        }

        T* reset( T* ptr ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            using ali::swap;

            swap(_ptr, ptr);

            if ( _ptr != ptr )
                private_auto_ptr_factory::delete_scalar(ptr);

            return _ptr;
        }

        template <typename U>
        T* reset( U* ptr ) noexcept
        {
            static_assert(
                    meta::is_same_type_remove_cv<T, U>::result
                ||  meta::has_virtual_destructor<T>::result,
                "meta::has_virtual_destructor<T>::result");

            return reset(static_cast<T*>(ptr));
        }

        T* release( void ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            T* const t{_ptr};

            _ptr = nullptr;

            return t;
        }

        void swap( auto_ptr& b ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            ALI_IF_DEBUG(b.assert_invariant());

            using ali::swap;

            swap(_ptr, b._ptr);
        }

        friend void swap( auto_ptr& a, auto_ptr& b ) noexcept
        {
            a.swap(b);
        }

        void assert_invariant( void ) const noexcept
        {
            ali_assert(_ptr != invalid_pointer_value);
        }

        template <typename U>
        operator ali::auto_ptr<U>( void ) noexcept
        {
            return ali::auto_ptr<U>{release()};
        }

    private:    //  Data members
        T*  _ptr{};
    };

private:    //  Class
    template <typename T>
    class auto_ptr<T[]>
    {
    public:
        auto_ptr( void ) noexcept = default;

        auto_ptr( auto_ptr const& ) = delete;

        template <typename U>
        auto_ptr( auto_ptr<U> const& ) = delete;

        auto_ptr( auto_ptr&& b ) noexcept
        :   _arr{b.release()}
        {
            ALI_IF_DEBUG(assert_invariant());
        }

        template <typename U>
        auto_ptr( auto_ptr<U>&& ) = delete;

        auto_ptr( ali::nullptr_type ) noexcept {}

        explicit auto_ptr( T* arr ) noexcept
        :   _arr{arr}
        {
            ALI_IF_DEBUG(assert_invariant());
        }

        ~auto_ptr( void ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            delete_array(_arr);

            _arr = invalid_pointer_value;
        }

        auto_ptr& operator=( auto_ptr const& ) = delete;

        template <typename U>
        auto_ptr& operator=( auto_ptr<U> const& ) = delete;

        auto_ptr& operator=( auto_ptr&& b ) noexcept
        {
            reset(b.release());

            return *this;
        }

        template <typename U>
        auto_ptr& operator=( auto_ptr<U>&& ) = delete;

        T& operator[]( int i ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            ali_assert(_arr != nullptr);
            return _arr[i];
        }

        bool is_null( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            return _arr == nullptr;
        }

        friend bool is_null( auto_ptr const& a ) noexcept
        {
            return a.is_null();
        }

        bool operator==( ali::nullptr_type ) const noexcept
        {
            return is_null();
        }

        bool operator!=( ali::nullptr_type ) const noexcept
        {
            return !is_null();
        }

        T* get( void ) const noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            return _arr;
        }

        T* reset( void ) noexcept
        {
            return reset(nullptr);
        }

        T* reset( T* arr ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            using ali::swap;
        
            swap(_arr, arr);

            if ( _arr != arr )
                delete_array(arr);

            return _arr;
        }

        T* release( void ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());

            T* const t{_arr};

            _arr = nullptr;

            return t;
        }

        void swap( auto_ptr& b ) noexcept
        {
            ALI_IF_DEBUG(assert_invariant());
            ALI_IF_DEBUG(b.assert_invariant());

            using ali::swap;
            swap(_arr, b._arr);
        }

        friend void swap( auto_ptr& a, auto_ptr& b ) noexcept
        {
            a.swap(b);
        }

        void assert_invariant( void ) const noexcept
        {
            ali_assert(_arr != invalid_pointer_value);
        }

    private:    //  Data members
        T*  _arr{};
    };

private:    //  Struct
    struct hidden
    {
        template <typename T, typename... Params>
        static private_auto_ptr_factory::auto_ptr<T> new_auto_ptr(
            meta::define_type<T>, Params&&... params )
        {
#ifndef ALI_TRACK_DYNAMIC_OBJECTS

            return private_auto_ptr_factory::auto_ptr<T>{
                out_of_memory_if_null(
                    new T{ali::forward<Params>(params)...},
                    ALI_HERE)};

#else   //  ALI_TRACK_DYNAMIC_OBJECTS

            private_auto_ptr_factory::auto_ptr<T> temp{
                out_of_memory_if_null(
                    new T{ali::forward<Params>(params)...},
                    ALI_HERE)};

            debug::created_dynamic_object(
                temp.get(), sizeof(T), typeid(T).name(),
                meta::is_polymorphic<T>::result);

            return temp;

#endif  //  ALI_TRACK_DYNAMIC_OBJECTS
        }

        template <typename T>
        static private_auto_ptr_factory::auto_ptr<T[]> new_auto_arr( size_t size )
        {
#ifndef ALI_TRACK_DYNAMIC_OBJECTS

            return private_auto_ptr_factory::auto_ptr<T[]>{
                out_of_memory_if_null(new T[size], ALI_HERE)};

#else   //  ALI_TRACK_DYNAMIC_OBJECTS

            private_auto_ptr_factory::auto_ptr<T[]> temp{
                out_of_memory_if_null(new T[size], ALI_HERE)};

            debug::created_dynamic_object(
                temp.get(), sizeof(T) * size,
                typeid(T[]).name(), false);

            return temp;

#endif  //  ALI_TRACK_DYNAMIC_OBJECTS
        }

        template <typename T, typename... Params>
        static private_auto_ptr_factory::auto_ptr<T[]> new_auto_ptr(
            meta::define_type<T[]>, Params&&... params )
        {
            return private_auto_ptr_factory::hidden::
                new_auto_arr<T>(ali::forward<Params>(params)...);
        }
    };

private:    //  Methods
    template <typename T, typename... Params>
    static private_auto_ptr_factory::auto_ptr<T> new_auto_ptr( Params&&... params )
    {
        return private_auto_ptr_factory::hidden::new_auto_ptr(
            meta::define_type<T>{},
            ali::forward<Params>(params)...);
    }

    template <typename... Params>
    static private_auto_ptr_factory::auto_ptr<F> new_auto_ptr( Params&&... params )
    {
        return private_auto_ptr_factory::new_auto_ptr<F>(
            ali::forward<Params>(params)...);
    }

    template <typename T>
    static void delete_scalar( T* ptr ) noexcept
    {
        ALI_IF_TRACK_DYNAMIC_OBJECTS(
            if ( ptr == nullptr ) ;
            else debug::deleting_dynamic_object(
                ali::hidden::most_derived_object(ptr),
                typeid(T).name(),
                meta::is_polymorphic<T>::result));

        delete ptr;
    }

    template <typename T>
    static void delete_array( T* arr ) noexcept
    {
        ALI_IF_TRACK_DYNAMIC_OBJECTS(
            if ( arr == nullptr ) ;
            else debug::deleting_dynamic_object(
                arr, typeid(T[]).name(), false));

        delete[] arr;
    }

    friend F;
        //  Legal since C++11.
};

// ******************************************************************
// ******************************************************************

}   //  namespace ali
