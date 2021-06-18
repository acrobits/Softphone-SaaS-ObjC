#pragma once
#if 1

#include "ali/ali_atomic_std.h"

#else

namespace ali
{

namespace atomic
{

//  http://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Atomic-Builtins.html

// ******************************************************************
class counter
// ******************************************************************
{
public:
    constexpr counter( int value = 0 ) noexcept
    :   _value{value}
    {}

    counter( counter const& ) = delete;
    counter& operator=( counter const& ) = delete;
    
    constexpr int get( void ) const noexcept
    {
        return _value;
    }

    int dec( void ) noexcept
    {
        return __sync_sub_and_fetch(&_value, 1);
    }

    int inc( void ) noexcept
    {
        return __sync_add_and_fetch(&_value, 1);
    }

private:    //  Data members
    alignas(4) int volatile _value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class pointer
// ******************************************************************
{
public:
    constexpr pointer( T* ptr = nullptr ) noexcept
    :   _ptr{ptr}
    {}

    pointer( pointer const& ) = delete;
    pointer& operator=( pointer const& ) = delete;

    constexpr T* get( void ) const noexcept
    {
        return _ptr;
    }

    bool compare_and_set(
        T* compare_with,
        T* set_to ) noexcept
    {
        return __sync_bool_compare_and_swap(
            &_ptr, compare_with, set_to);
    }

private:    //  Data members
    alignas(sizeof(T*)) T* volatile _ptr;
};

}   //  namespace atomic

}   //  namespace ali
#endif
