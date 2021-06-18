/*
 *  ali_buffer.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_buffer_forward.h"
#include "ali/ali_array_forward.h"
#include "ali/ali_array_utils.h"
#include "ali/ali_attribute.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_debug.h"
#include "ali/ali_memory.h"
#include "ali/ali_utility.h"

// Debug buffer overflow.

#if defined(ALI_DEBUG_OVERFLOW)

#   define ALI_BUFFER_DEBUG_OVERFLOW
    //  Globally controlled define.

#else

//#   define ALI_BUFFER_DEBUG_OVERFLOW
    //  Locally controlled define (comment/uncomment).

#endif  //  ALI_DEBUG_OVERFLOW

// ******************************************************************

#if defined(ALI_BUFFER_DEBUG_OVERFLOW)
#   define ALI_BUFFER_IF_DEBUG_OVERFLOW(...) __VA_ARGS__
#else
#   define ALI_BUFFER_IF_DEBUG_OVERFLOW(...)
#endif

// ******************************************************************

namespace ali
{

// ******************************************************************
template <typename T>
class ALI_ATTRIBUTE_EMPTY_BASES basic_pod_buffer
// ******************************************************************
    :   public exposes_array_ref<T, basic_pod_buffer<T>>
{
public:
    bool is_empty( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin == this->_end;
    }

    int capacity( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return static_cast<int>(this->_end - this->_begin);
    }
    
    T* begin( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }
    
    T const* begin( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }
    
    T* end( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_end;
    }
    
    T const* end( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_end;
    }
    
    T& operator[]( int idx )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= idx);
        ali_assert(idx < this->capacity());
        return this->_begin[idx];
    }
    
    T const& operator[]( int idx ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= idx);
        ali_assert(idx < this->capacity());
        return this->_begin[idx];
    }
    
    bool operator==( basic_pod_buffer const& b ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->ref() == b.ref();
    }
    
    bool operator!=( basic_pod_buffer const& b ) const
    {
        return !operator==(b);
    }

    array_const_ref<T> ref( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin, this->capacity()};
    }

    array_const_ref<T> ref( int pos, int n ) const
    {
        return this->ref().ref(pos, n);
    }

    array_ref<T> mutable_ref( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin, this->capacity()};
    }

    array_ref<T> mutable_ref( int pos, int n )
    {
        return this->mutable_ref().mutable_ref(pos, n);
    }

    blob_const_ref as_blob( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return blob_const_ref{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            reinterpret_cast<ali::uint8 const*>(this->_begin),
            static_cast<int>(sizeof(T)) * this->capacity()};
    }

    blob_ref as_mutable_blob( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return blob_ref{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            reinterpret_cast<ali::uint8*>(this->_begin),
            static_cast<int>(sizeof(T)) * this->capacity()};
    }
    
protected:  //  Method
    basic_pod_buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());
    }
    
    explicit basic_pod_buffer( int capacity )
    :   _begin{capacity == 0 ? nullptr
            : (reinterpret_cast<T*>(new_auto_ptr<ali::uint8[]>(
                (capacity
                    ALI_BUFFER_IF_DEBUG_OVERFLOW(
                        + 2 * guard_element_count())) * sizeof(T)).release())
                            ALI_BUFFER_IF_DEBUG_OVERFLOW(
                                + guard_element_count()))},
        _end{_begin + capacity}
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());
    }
    
    basic_pod_buffer( T const data[], int capacity )
    :   basic_pod_buffer{capacity}
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->mutable_ref() = make_ref(data, capacity);
    }
    
    ~basic_pod_buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards();
            this->uninitialize_overflow_guards());

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        ali::auto_ptr<ali::uint8[]>{
            reinterpret_cast<ali::uint8*>(this->_begin
                ALI_BUFFER_IF_DEBUG_OVERFLOW(
                    - (this->_begin == nullptr ? 0
                        : guard_element_count())))};
    }
    
    void swap( basic_pod_buffer& b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        using ali::swap;
        swap(this->_begin, b._begin);
        swap(this->_end, b._end);
        ALI_IF_DEBUG_WEAK_REF(
            swap(this->_weak_rc, b._weak_rc));
    }

private:    //  Methods
    basic_pod_buffer( basic_pod_buffer const& b );
    basic_pod_buffer& operator=( basic_pod_buffer const& b );

#ifdef  ALI_BUFFER_DEBUG_OVERFLOW

    struct check_overflow_guards_now_and_on_exit
    {
        basic_pod_buffer const&  b;

        explicit check_overflow_guards_now_and_on_exit(
            basic_pod_buffer const& b )
        :   b{b}
        {
            b.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void )
        {
            b.check_overflow_guards();
        }
    };

    void initialize_overflow_guards( void )
    {
        if ( _begin != nullptr )
        {
            make_mutable_ref(
                this->_begin - this->guard_element_count(),
                this->guard_element_count()).as_mutable_blob()
                    .mutable_ref_back(sizeof(this->_correct_guard))
                    .set_int32_ne_at(0, this->_correct_guard);
            make_mutable_ref(
                this->_end,
                this->guard_element_count()).as_mutable_blob()
                    .mutable_ref_front(sizeof(this->_correct_guard))
                    .set_int32_ne_at(0, this->_correct_guard);
        }
    }

    void uninitialize_overflow_guards( void )
    {
        this->_guard1 = 0U;
        this->_guard2 = 0U;
        if ( this->_begin != nullptr )
        {
            make_mutable_ref(
                this->_begin - this->guard_element_count(),
                this->guard_element_count())
                    .as_mutable_blob().fill();
            make_mutable_ref(
                this->_end,
                this->guard_element_count())
                    .as_mutable_blob().fill();
        }
    }
    
    bool check_overflow_guards( void ) const
    {
        bool const success{
                this->_guard1 == this->_correct_guard
            &&  this->_guard2 == this->_correct_guard
            &&  (   this->_begin == nullptr
                ||  (   make_ref(
                            this->_begin - this->guard_element_count(),
                            this->guard_element_count()).as_blob()
                                .ref_back(sizeof(this->_correct_guard))
                                .int32_ne_at(0) == this->_correct_guard
                    &&  make_ref(
                            this->_end,
                            this->guard_element_count()).as_blob()
                                .ref_front(sizeof(this->_correct_guard))
                                .int32_ne_at(0) == this->_correct_guard))};

        if ( !success )
            debug::abort();

        return success;
    }

    static int guard_element_count( void )
    {
        return 1 + (sizeof(_correct_guard) - 1) / sizeof(T);
    }

#endif  //  ALI_BUFFER_DEBUG_OVERFLOW

private:    //  Data members
    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        static ali::uint32 const    _correct_guard{0xaaaaaaaaU};
        ali::uint32                 _guard1{_correct_guard};
    );

    T*                              _begin{};
    T*                              _end{};

    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        ali::uint32                 _guard2{_correct_guard};
    );

    ALI_IF_DEBUG_WEAK_REF(
        bool check_no_weak_ref( void ) const
        {
            bool const success{_weak_rc.get() == 0};

            if ( !success )
                debug::abort();

            return success;
        }

        mutable debug::
            weak_reference::counter _weak_rc{};
    );
};

// ******************************************************************
template <>
inline blob_const_ref basic_pod_buffer<ali::uint8>::as_blob( void ) const
// ******************************************************************
{
    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        check_overflow_guards_now_and_on_exit _{*this});

    return blob_const_ref{
        ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
        this->_begin, this->capacity()};
}

// ******************************************************************
template <>
inline blob_ref basic_pod_buffer<ali::uint8>::as_mutable_blob( void )
// ******************************************************************
{
    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        check_overflow_guards_now_and_on_exit _{*this});

    return blob_ref{
        ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
        this->_begin, this->capacity()};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class pod_buffer : public basic_pod_buffer<T>
// ******************************************************************
{
public:
    pod_buffer( void )
    {}
    
    ~pod_buffer( void )
    {}
    
    explicit pod_buffer( int capacity )
    :   basic_pod_buffer<T>{capacity}
    {}
    
    pod_buffer( T const data[], int capacity )
    :   basic_pod_buffer<T>{data, capacity}
    {}
    
    pod_buffer( pod_buffer const& b )
    {
        pod_buffer{
            b.begin(),
            b.capacity()}
                .swap(*this);
    }
    
    pod_buffer& operator=( pod_buffer const& b )
    {
        if ( this->capacity() == b.capacity() )
        {
            this->mutable_ref() = b.ref();
        }
        else
        {
            pod_buffer{
                b.begin(),
                b.capacity()}
                    .swap(*this);
        }
        return *this;
    }

    pod_buffer& set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        if ( this->capacity() != capacity )
            pod_buffer{capacity}.swap(*this);
        return *this;
    }

    pod_buffer& reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        return this->set_capacity(
            ali::maxi(
                this->capacity(),
                capacity));
    }
    
    void swap( pod_buffer& b )
    {
        basic_pod_buffer<T>::swap(b);
    }

    friend void swap(
        pod_buffer& a,
        pod_buffer& b )
    {
        a.swap(b);
    }
};

// ******************************************************************
template <typename T>
class sensitive_pod_buffer : public basic_pod_buffer<T>
// ******************************************************************
{
public:
    sensitive_pod_buffer( void )
    {}
    
    explicit sensitive_pod_buffer( int capacity )
    :   basic_pod_buffer<T>{capacity}
    {}
    
    sensitive_pod_buffer( T const data[], int capacity )
    :   basic_pod_buffer<T>{data, capacity}
    {}
    
    sensitive_pod_buffer( sensitive_pod_buffer const& b )
    {
        sensitive_pod_buffer{
            b.begin(),
            b.capacity()}
                .swap(*this);
    }

    sensitive_pod_buffer( array_const_ref<T> b )
    {
        sensitive_pod_buffer{
            b.data(), b.size()}
                .swap(*this);
    }
    
    ~sensitive_pod_buffer( void )
    {
        this->mutable_ref().fill();
    }
    
    sensitive_pod_buffer& operator=( sensitive_pod_buffer const& b )
    {
        return this->operator=(b.ref());
    }

    sensitive_pod_buffer& operator=( array_const_ref<T> b )
    {
        if ( this->capacity() == b.size() )
        {
            this->mutable_ref() = b;
        }
        else
        {
            sensitive_pod_buffer{
                b.data(), b.size()}
                    .swap(*this);
        }
        return *this;
    }
    
    sensitive_pod_buffer& set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        if ( this->capacity() != capacity )
            sensitive_pod_buffer{capacity}.swap(*this);
        return *this;
    }

    sensitive_pod_buffer& reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        return this->set_capacity(
            ali::maxi(
                this->capacity(),
                capacity));
    }
    
    void swap( sensitive_pod_buffer& b )
    {
        basic_pod_buffer<T>::swap(b);
    }

    friend void swap(
        sensitive_pod_buffer& a,
        sensitive_pod_buffer& b )
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
template <typename T>
inline void buffer_destroy_content(
    array_ref<T> arr, meta::define_bool_result<true> /*is_trivially_copyable*/ )
// ******************************************************************
{
    arr.fill();
}

// ******************************************************************
template <typename T>
inline void buffer_destroy_content(
    array_ref<T> /*arr*/, meta::define_bool_result<false> /*is_trivially_copyable*/ )
// ******************************************************************
{
}

// ******************************************************************
template <typename T>
inline void buffer_destroy_content( array_ref<T> arr )
// ******************************************************************
{
    buffer_destroy_content(arr, meta::is_trivially_copyable<T>());
}

}   //  namespace hidden

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity>
class ALI_ATTRIBUTE_EMPTY_BASES buffer
// ******************************************************************
    :   public exposes_array_ref<T, buffer<T, _capacity>>
{
public:
    buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
    }

    buffer( buffer const& b )
    :   buffer{b.ref()}
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            b.check_overflow_guards());
    }

    explicit buffer( array_const_ref<T> b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->mutable_ref() = b;
    }

    ~buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        hidden::buffer_destroy_content(this->mutable_ref());

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->uninitialize_overflow_guards());
    }

    buffer& operator=( array_const_ref<T> b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->mutable_ref() = b;

        return *this;
    }

    static constexpr bool is_empty( void )
    {
        return false;
    }

    static constexpr int capacity( void )
    {
        return _capacity;
    }

    T const& operator[]( int pos ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= pos);
        ali_assert(pos < _capacity);

        return this->_begin[pos];
    }

    T& operator[]( int pos )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= pos);
        ali_assert(pos < _capacity);

        return this->_begin[pos];
    }

    T* data( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

    T const* data( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

    void swap( buffer& b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        using ali::swap;
        swap(this->_begin, b._begin);
    }

    friend void swap( buffer& a, buffer& b )
    {
        a.swap(b);
    }

    array_const_ref<T> ref( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin, _capacity};
    }

    array_const_ref<T> ref( int pos, int n ) const
    {
        return this->ref().ref(pos, n);
    }

    array_ref<T> mutable_ref( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin, _capacity};
    }

    array_ref<T> mutable_ref( int pos, int n )
    {
        return this->mutable_ref().mutable_ref(pos, n);
    }

#ifdef  ALI_BUFFER_DEBUG_OVERFLOW

    struct check_overflow_guards_now_and_on_exit
    {
        buffer const&  b;

        explicit check_overflow_guards_now_and_on_exit(
            buffer const& b )
        :   b{b}
        {
            b.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void )
        {
            b.check_overflow_guards();
        }
    };

    void uninitialize_overflow_guards( void )
    {
        this->_guard1 = 0U;
        this->_guard2 = 0U;
    }
    
    bool check_overflow_guards( void ) const
    {
        bool const success{
                this->_guard1 == this->_correct_guard
            &&  this->_guard2 == this->_correct_guard};

        if ( !success )
            debug::abort();

        return success;
    }

#endif  //  ALI_BUFFER_DEBUG_OVERFLOW

private:    //  Data members
    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        static ali::uint32 const    _correct_guard{0xaaaaaaaaU};
        ali::uint32                 _guard1{_correct_guard};
    );

    T                               _begin[_capacity];

    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        ali::uint32                 _guard2{_correct_guard};
    );

    ALI_IF_DEBUG_WEAK_REF(
        bool check_no_weak_ref( void ) const
        {
            bool const success{_weak_rc.get() == 0};

            if ( !success )
                debug::abort();

            return success;
        }

        mutable debug::
            weak_reference::counter _weak_rc{};
    );
};

// ******************************************************************
template <typename T>
class ALI_ATTRIBUTE_EMPTY_BASES buffer<T, 0>
// ******************************************************************
//  Dynamic buffer.
// ******************************************************************
    :   public exposes_array_ref<T, buffer<T, 0>>
{
public:
    buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());
    }

    explicit buffer( int capacity )
    :   _capacity{ali::maxi(0, capacity)}
    {
        if ( this->_capacity != 0 )
            this->_begin = new_auto_ptr<T[]>(this->_capacity);

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());

        ali_assert(0 <= capacity);
    }

    buffer( buffer const& b )
    :   buffer{b.ref()}
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            b.check_overflow_guards());
    }

    explicit buffer( array_const_ref<T> b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            check_overflow_guards_now_and_on_exit _{*this});

        buffer temp{b.size()};
        
        temp.mutable_ref() = b;
        
        this->swap(temp);
    }

    buffer( buffer&& b )
    :   buffer{}
    {
        this->swap(b);
    }

    ~buffer( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        hidden::buffer_destroy_content(this->mutable_ref());

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->uninitialize_overflow_guards());

        this->_capacity = meta::max_value(this->_capacity);
    }

    buffer& operator=( buffer const& b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->operator=(b.ref());
    }

    buffer& operator=( array_const_ref<T> b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_capacity == b.size() )
            this->mutable_ref() = b;
        else
            buffer{b}.swap(*this);

        return *this;
    }

    buffer& operator=( buffer&& b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        buffer{ali::move(b)}.swap(*this);
        
        return *this;
    }

    bool is_empty( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_capacity == 0;
    }

    int capacity( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_capacity;
    }

    T const& operator[]( int pos ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= pos);
        ali_assert(pos < this->_capacity);

        return this->_begin[pos];
    }

    T& operator[]( int pos )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
        ali_assert(0 <= pos);
        ali_assert(pos < this->_capacity);

        return this->_begin[pos];
    }

    T* data( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin.get();
    }

    T const* data( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin.get();
    }

    buffer& lossy_set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_capacity != capacity )
            buffer{capacity}.swap(*this);

        return *this;
    }

    buffer& lossy_reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is NOT guaranteed to be preserved.
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->lossy_set_capacity(
            ali::maxi(this->_capacity, capacity));
    }

    buffer& lossless_set_capacity( int capacity )
    //  post:   this->capacity() == capacity
    //  note:   The contents is guaranteed to be preserved up to the min(this->capacity(), capacity).
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_capacity != capacity )
        {
            buffer temp{capacity};
            temp.mutable_ref().copy_front(*this);
            this->swap(temp);
        }
        return *this;
    }

    buffer& lossless_reserve( int capacity )
    //  post:   this->capacity() == max(this->capacity(), capacity)
    //  note:   The contents is guaranteed to be preserved up to the min(this->capacity(), capacity).
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->lossless_set_capacity(
            ali::maxi(this->_capacity, capacity));
    }
    
    void swap( buffer& b )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        using ali::swap;
        swap(this->_capacity, b._capacity);
        swap(this->_begin, b._begin);
        ALI_IF_DEBUG_WEAK_REF(
            swap(this->_weak_rc, b._weak_rc));
    }

    friend void swap( buffer& a, buffer& b )
    {
        a.swap(b);
    }

    array_const_ref<T> ref( void ) const
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin.get(), this->_capacity};
    }

    array_const_ref<T> ref( int pos, int n ) const
    {
        return this->ref().ref(pos, n);
    }

    array_ref<T> mutable_ref( void )
    {
        ALI_BUFFER_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(this->_weak_rc,)
            this->_begin.get(), this->_capacity};
    }

    array_ref<T> mutable_ref( int pos, int n )
    {
        return this->mutable_ref().mutable_ref(pos, n);
    }

#ifdef  ALI_BUFFER_DEBUG_OVERFLOW

private:
    struct check_overflow_guards_now_and_on_exit
    {
        buffer const&  b;

        explicit check_overflow_guards_now_and_on_exit(
            buffer const& b )
        :   b{b}
        {
            b.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void )
        {
            b.check_overflow_guards();
        }
    };

    void initialize_overflow_guards( void )
    {
    }

    void uninitialize_overflow_guards( void )
    {
        this->_guard1 = 0U;
        this->_guard2 = 0U;
    }
    
    bool check_overflow_guards( void ) const
    {
        bool const success{
                this->_guard1 == this->_correct_guard
            &&  this->_guard2 == this->_correct_guard};

        if ( !success )
            debug::abort();

        return success;
    }

    static int guard_element_count( void )
    {
        return 1 + (sizeof(_correct_guard) - 1) / sizeof(T);
    }

#endif  //  ALI_BUFFER_DEBUG_OVERFLOW

private:    //  Data members
    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        static ali::uint32 const    _correct_guard{0xaaaaaaaaU};
        ali::uint32                 _guard1{_correct_guard};
    );

    int                             _capacity{};
    ali::auto_ptr<T[]>              _begin{};

    ALI_BUFFER_IF_DEBUG_OVERFLOW(
        ali::uint32                 _guard2{_correct_guard};
    );

    ALI_IF_DEBUG_WEAK_REF(
        bool check_no_weak_ref( void ) const
        {
            bool const success{_weak_rc.get() == 0};

            if ( !success )
                debug::abort();

            return success;
        }

        mutable debug::
            weak_reference::counter _weak_rc{};
    );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity>
class circular_buffer
// ******************************************************************
{
public:
    circular_buffer( void )
    :   _read(0),
        _write(0)
    {}

    //  Consumer

    array_ref<T> read_ref( void )
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return array_ref<T>(
            &_begin[_read],
            ali::mini(written_size, no_wrap_size));
    }

    bool read_ref_size_can_grow( void ) const
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return written_size < no_wrap_size;
    }

    circular_buffer& consume( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= read_ref().size());

        _read += n;

        if ( _read == _capacity )
        {
            _read = 0;
                
            _write -= _capacity;
        }

        return *this;
    }

    bool is_empty( void ) const
    {
        return _write == _read;
    }

    int size( void ) const
    {
        return _write - _read;
    }

    T& operator[]( int i )
    {
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    T const& operator[]( int i ) const
    {
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    //  Producer

    array_ref<T> write_ref( void )
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return array_ref<T>(
            &_begin[_write % _capacity],
            ali::mini(_capacity - written_size, no_wrap_size));
    }

    bool write_ref_size_can_grow( void ) const
    {
        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return _capacity - written_size < no_wrap_size;
    }

    circular_buffer& produce( int n )
    {
        ali_assert(0 <= n);
        ali_assert(n <= write_ref().size());

        _write += n;

        return *this;
    }

    circular_buffer& push_back( T const& t )
    {
        ali_assert(!write_ref().is_empty());

        write_ref().front() = t;
        
        return produce(1);
    }

    circular_buffer& force_push_back( T const& t )
    {
        if ( write_ref().is_empty() )
            consume(1);

        return push_back(t);
    }

    //  Other

    int capacity( void ) const
    {
        return _capacity;
    }

    circular_buffer& erase( void )
    {
        _read = _write = 0;

        return *this;
    }

private:    //  Data members
    int _read;
    int _write;
    T   _begin[_capacity];
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class circular_buffer<T, 0>
// ******************************************************************
{
public:
    circular_buffer( void )
    :   _capacity(0),
        _read(0),
        _write(0)
    {}

    explicit circular_buffer( int capacity )
    :   _capacity(capacity),
        _read(0),
        _write(0)
    {
        if ( _capacity != 0 )
            _begin = new_auto_ptr<T[]>(_capacity);
    }
    
    circular_buffer( circular_buffer const& b )
    :   _capacity(b._capacity),
        _read(b._read),
        _write(b._write)
    {
        if ( _capacity != 0 )
        {
            _begin = new_auto_ptr<T[]>(_capacity);

            make_mutable_ref(_begin.get(), _capacity)
                = make_ref(b._begin.get(), _capacity);
        }
    }

    ~circular_buffer( void )
    {
    }

    circular_buffer& operator=( circular_buffer const& b )
    {
        if ( this == &b )
            ;   //  NOOP
        else if ( _capacity == b._capacity )
        {
            if ( _capacity != 0 )
            {
                make_mutable_ref(_begin.get(), _capacity)
                    = make_ref(b._begin.get(), _capacity);
            
                _read = b._read;
            
                _write = b._write;
            }
        }
        else
        {
            circular_buffer(b).swap(*this);
        }
        
        return *this;
    }

    //  Consumer

    array_ref<T> read_ref( void )
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return array_ref<T>(
            &_begin[_read],
            ali::mini(written_size, no_wrap_size));
    }

    bool read_ref_size_can_grow( void ) const
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _read;

        return written_size < no_wrap_size;
    }

    circular_buffer& consume( int n )
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= n);
        ali_assert(n <= read_ref().size());

        _read += n;

        if ( _read == _capacity )
        {
            _read = 0;
                
            _write -= _capacity;
        }

        return *this;
    }

    int is_empty( void ) const
    {
        return _write == _read;
    }

    int size( void ) const
    {
        return _write - _read;
    }

    T& operator[]( int i )
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    T const& operator[]( int i ) const
    {
        ali_assert(_capacity != 0);
        ali_assert(0 <= i);
        ali_assert(i < size());

        return _begin[(_read + i) % _capacity];
    }

    //  Producer

    array_ref<T> write_ref( void )
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return array_ref<T>(
            &_begin[_write % _capacity],
            ali::mini(_capacity - written_size, no_wrap_size));
    }

    bool write_ref_size_can_grow( void ) const
    {
        ali_assert(_capacity != 0);

        int const written_size = _write - _read;

        int const no_wrap_size = _capacity - _write % _capacity;

        return _capacity - written_size < no_wrap_size;
    }

    circular_buffer& produce( int n )
    {
        ali_assert(_capacity != 0);

        ali_assert(0 <= n);
        ali_assert(n <= write_ref().size());

        _write += n;

        return *this;
    }

    circular_buffer& push_back( T const& t )
    {
        ali_assert(!write_ref().is_empty());

        write_ref().front() = t;
        
        return produce(1);
    }

    circular_buffer& force_push_back( T const& t )
    {
        if ( write_ref().is_empty() )
            consume(1);

        return push_back(t);
    }

    //  Other

    int capacity( void ) const
    {
        return _capacity;
    }

    circular_buffer& erase( void )
    {
        _read = _write = 0;

        return *this;
    }

    void swap( circular_buffer& b )
    {
        using ali::swap;
        swap(_capacity, b._capacity);
        swap(_read, b._read);
        swap(_write, b._write);
        swap(_begin, b._begin);
    }

    friend void swap(
        circular_buffer& a,
        circular_buffer& b )
    {
        a.swap(b);
    }

private:    //  Data members
    int                 _capacity;
    int                 _read;
    int                 _write;
    ali::auto_ptr<T[]>  _begin;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
array<bool> test_buffer( void );
// ******************************************************************

}   //  namespace ali

