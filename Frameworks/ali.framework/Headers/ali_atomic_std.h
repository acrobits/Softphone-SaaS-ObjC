#pragma once
#include <atomic>

namespace ali
{

namespace atomic
{

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

    int get( void ) const noexcept
    {
        return _value.load();
    }

    int dec( void ) noexcept
    {
        return --_value;
    }

    int inc( void ) noexcept
    {
        return ++_value;
    }

    bool compare_exchange( int& expected, int desired ) noexcept
    {
        return _value.compare_exchange_strong(expected, desired);
    }

private:    //  Data members
    std::atomic<int>    _value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class pointer
// ******************************************************************
{
public:
    constexpr pointer( T* value = nullptr ) noexcept
    :   _value{value}
    {}

    pointer( pointer const& ) = delete;
    pointer& operator=( pointer const& ) = delete;

    T* get( void ) const noexcept
    {
        return _value.load();
    }
    
    bool compare_and_set( T* expected, T* desired ) noexcept
        //  Deprecated.
    {
        return _value.compare_exchange_strong(expected, desired);
    }

    bool compare_exchange( T*& expected, T* desired ) noexcept
    {
        return _value.compare_exchange_strong(expected, desired);
    }

private:    //  Data members
    std::atomic<T*> _value;
};

}   //  namespace atomic

}   //  namespace ali
