#pragma once
#include "ali/ali_debug.h"
#include "ali/ali_integer.h"
#include "ali/ali_utility.h"
#include "ali/ali_meta.h"
#include "ali/ali_new.h"

namespace ali
{

// ******************************************************************
template <int _size, int _alignment>
class in_place_buffer
// ******************************************************************
{
    ali_static_assert(_size > 0);
    ali_static_assert(_alignment > 0);
    ali_static_assert(_alignment <= _size);
    ali_static_assert(meta::is_power_of_two<_alignment>::result);

public:     //  Methods
    template <typename T>
    T* begin( void ) noexcept
    {
        return this->private_begin(meta::define_type<T>{});
    }

    template <typename T>
    T const* begin( void ) const noexcept
    {
        return this->private_begin(meta::define_type<T>{});
    }

    template <typename T, typename... Params>
    T& emplace( Params&&... params ) noexcept (
        noexcept(T(ali::forward<Params>(params)...)))
    {
        return *(new (this->begin<typename meta::remove_cv<T>::type>()) T(
            ali::forward<Params>(params)...));
    }

    static constexpr int            alignment{_alignment};
    static constexpr int            size{_size};

private:    //  Methods
    template <typename T>
    T* private_begin( meta::define_type<T> ) noexcept
    {
        return this->private_begin<
            size, sizeof(T),
            alignment, alignof(T), T>();
    }

    template <typename T>
    T const* private_begin( meta::define_type<T> ) const noexcept
    {
        return this->private_begin<
            size, sizeof(T),
            alignment, alignof(T), T>();
    }

    void* private_begin( meta::define_type<void> ) noexcept
    {
        return this->private_begin();
    }

    void const* private_begin( meta::define_type<void> ) const noexcept
    {
        return this->private_begin();
    }

    template <
        int sizeOfThis, int sizeOfT,
        int alignOfThis, int alignOfT,
        typename T>
    T* private_begin( void ) noexcept
    {
        ali_static_assert(sizeOfThis >= sizeOfT);
        ali_static_assert(alignOfThis >= alignOfT);

        return reinterpret_cast<T*>(this->private_begin());
    }

    template <
        int sizeOfThis, int sizeOfT,
        int alignOfThis, int alignOfT,
        typename T>
    T const* private_begin( void ) const noexcept
    {
        ali_static_assert(sizeOfThis >= sizeOfT);
        ali_static_assert(alignOfThis >= alignOfT);

        return reinterpret_cast<T const*>(this->private_begin());
    }

    void* private_begin( void ) noexcept
    {
        constexpr ali::uint_of_pointer_size mask{alignment - 1};

        ali::uint8* const begin{
            reinterpret_cast<ali::uint8*>(
                (reinterpret_cast<ali::uint_of_pointer_size>(
                    static_cast<void*>(_storage)) + mask) & ~mask)};

        ali_assert(is_correctly_aligned(begin));

        return begin;
    }

    void const* private_begin( void ) const noexcept
    {
        constexpr ali::uint_of_pointer_size mask{alignment - 1};

        ali::uint8 const* const begin{
            reinterpret_cast<ali::uint8 const*>(
                (reinterpret_cast<ali::uint_of_pointer_size>(
                    static_cast<void const*>(_storage)) + mask) & ~mask)};

        ali_assert(is_correctly_aligned(begin));

        return begin;
    }

    bool is_correctly_aligned( ali::uint8 const* begin ) const noexcept
    {
        return  reinterpret_cast<ali::uint_of_pointer_size>(begin) % alignment == 0
            &&  _storage <= begin && begin + size <= _storage + _size_with_padding;
    }

private:    //  Data members
    static constexpr int    _size_with_padding{size + alignment - 1};
    ali::uint8              _storage[_size_with_padding];
};

// ******************************************************************
ali_static_assert((meta::is_trivially_copyable<in_place_buffer<1, 1>>::result));
// ******************************************************************

// ******************************************************************
// ******************************************************************

}   //  namespace ali
