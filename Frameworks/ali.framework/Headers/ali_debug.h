#pragma once
#include "ali/ali_debug_platform.h"
#include "ali/ali_atomic.h"

#if     !defined(ALI_DEBUG_WEAK_REF)    \
    &&  !defined(ALI_NDEBUG_WEAK_REF)   \
    &&  defined(ALI_DEBUG)
            //  Enable DEBUG_WEAK_REF in debug builds by default,
            //  unless external tools specify otherwise.

// ******************************************************************
#define ALI_DEBUG_WEAK_REF
// ******************************************************************

#endif  //  ALI_DEBUG

// ******************************************************************
// ******************************************************************

#ifndef  ALI_TRACK_DYNAMIC_OBJECTS

#define ALI_IF_TRACK_DYNAMIC_OBJECTS(...)

#else   //  ALI_TRACK_DYNAMIC_OBJECTS

#include <stddef.h> //  size_t
#include <typeinfo>

#define ALI_IF_TRACK_DYNAMIC_OBJECTS(...) __VA_ARGS__

#endif  //  ALI_TRACK_DYNAMIC_OBJECTS

// ******************************************************************
// ******************************************************************

namespace ali
{

namespace debug
{

#ifndef ALI_DEBUG_WEAK_REF

// ******************************************************************
#define ALI_IF_DEBUG_WEAK_REF(...)
// ******************************************************************

#else   //  ALI_DEBUG_WEAK_REF

// ******************************************************************
#define ALI_IF_DEBUG_WEAK_REF(...) __VA_ARGS__
// ******************************************************************


// ******************************************************************
class weak_reference
// ******************************************************************
{
public:     //  Class
    class counter
        //  The counter is intended to be embedded inside
        //  a resource owning class (owner) along with the
        //  managed resource.
        //  The counter assumes that the owner makes a copy
        //  of the resource in its copy constructor, and that
        //  assignment is allowed only if the counter is zero.
    {
    public:
        counter( void ) noexcept;

        counter( counter const& ) noexcept
            //  We intentionally don't copy the value.
        :   counter{}
        {}

        ~counter( void ) noexcept;

        counter& operator=( counter const& ) noexcept
            //  We intentionally don't copy the value.
        {
            return *this;
        }

        int get( void ) const noexcept
        {
            return _count->get();
        }

        void swap( counter& b ) noexcept
        {
            auto const temp = _count;
            _count = b._count;
            b._count = temp;
        }

        friend void swap( counter& a, counter& b ) noexcept
        {
            a.swap(b);
        }

    private:    //  Data members
        atomic::counter*    _count{};
        friend weak_reference;
    };

public:
    weak_reference( void ) = default;

    weak_reference( counter& count ) noexcept
    :   weak_reference{count._count}
    {}

    weak_reference( weak_reference const& b ) noexcept
    :   weak_reference{b._count}
    {}

    weak_reference( weak_reference&& b ) noexcept
    :   _count{b._count}
    {
        b._count = nullptr;
    }

    ~weak_reference( void ) noexcept
    {
        if ( _count == nullptr ) return;
        auto const decremented_value = _count->dec();
        ali_assert(decremented_value >= 0);
    }

    weak_reference& operator=( weak_reference b ) noexcept
    {
        swap(b);
        return *this;
    }

    weak_reference& reset( void ) noexcept
    {
        weak_reference{}.swap(*this);
        return *this;
    }

    void swap( weak_reference& b ) noexcept
    {
        auto const temp = _count;
        _count = b._count;
        b._count = temp;
    }

    friend void swap(
        weak_reference& a,
        weak_reference& b ) noexcept
    {
        a.swap(b);
    }

    bool operator==(
        weak_reference const& b ) const noexcept
    {
        return _count == b._count;
            //  Intentionally compares the POINTERS.
    }

    bool operator==(
        counter const& b ) const noexcept
    {
        return _count == b._count;
            //  Intentionally compares the POINTERS.
    }

    friend bool operator==(
        counter const& a,
        weak_reference const& b ) noexcept
    {
        return b == a;
    }

    friend bool operator!=(
        weak_reference const& a,
        weak_reference const& b ) noexcept
    {
        return !(a == b);
    }

    friend bool operator!=(
        weak_reference const& a,
        counter const& b ) noexcept
    {
        return !(a == b);
    }

    friend bool operator!=(
        counter const& a,
        weak_reference const& b ) noexcept
    {
        return !(a == b);
    }

private:    //  Methods
    explicit weak_reference( atomic::counter* count ) noexcept
    :   _count{count}
    {
        if ( count == nullptr ) return;
        auto const incremented_value = _count->inc();
        ali_assert(incremented_value > 0);
    }

private:    //  Data members
    atomic::counter*    _count{};
};

// ******************************************************************
// ******************************************************************

#endif  //  ALI_DEBUG_WEAK_REF

// ******************************************************************
template <typename T>
class assert_invariant_now_and_on_exit
// ******************************************************************
{
public:
    assert_invariant_now_and_on_exit( T const& object ) noexcept
    :   _object{object}
    {
        _object.assert_invariant();
    }

    ~assert_invariant_now_and_on_exit( void ) noexcept
    {
        _object.assert_invariant();
    }

private:    //  Methods
    assert_invariant_now_and_on_exit(
        assert_invariant_now_and_on_exit const& ) = delete;
    assert_invariant_now_and_on_exit& operator=(
        assert_invariant_now_and_on_exit const& ) = delete;

private:    //  Data members
    T const&    _object;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline T& assert_invariant( T& object ) noexcept
// ******************************************************************
//  T&: Intentionally non-const.
// ******************************************************************
{
    object.assert_invariant();
    return object;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline T* assert_not_nullptr( T* p ) noexcept
// ******************************************************************
{
    ali_assert(p != nullptr);
    return p;
}

// ******************************************************************
// ******************************************************************

ALI_IF_TRACK_DYNAMIC_OBJECTS(

    void created_dynamic_object(
        void const* begin, size_t size, char const* type, bool is_polymorphic );

    void deleting_dynamic_object(
        void const* begin, char const* type, bool is_polymorphic ) noexcept;

);

}   //  namespace debug

}   //  namespace ali
