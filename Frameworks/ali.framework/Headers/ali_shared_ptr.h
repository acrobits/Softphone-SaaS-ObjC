#pragma once
#include "ali/ali_shared_ptr_forward.h"
#include "ali/ali_atomic.h"
#include "ali/ali_attribute.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_in_place_buffer.h"

namespace ali
{

namespace hidden
{

// ******************************************************************
class basic_shared_info
// ******************************************************************
{
public:
    basic_shared_info( void ) = default;

    basic_shared_info(
        basic_shared_info const& ) = delete;
    
    virtual ~basic_shared_info( void ) {}

    basic_shared_info& operator=(
        basic_shared_info const& ) = delete;

    void inc_strong_ref( void )
    {
        ALI_IF_DEBUG(auto const rc =)
            _strong_ref.inc();
        ali_assert(rc > 1);
    }

    bool inc_strong_ref_if_not_zero( void )
    {
        for ( auto count = _strong_ref.get(); count != 0; )
        {
            if ( _strong_ref.compare_exchange(count, count + 1) )
                return true;
            //  Here, count contains the new "initial" value.
        }

        return false;
    }

    void dec_strong_ref( void )
    {
        auto const rc = _strong_ref.dec();

        ali_assert(rc >= 0);

        if ( rc == 0 )
        {
            destroy_managed_resource();
            
            dec_weak_ref();
        }
    }

    void inc_weak_ref( void )
    {
        ALI_IF_DEBUG(auto const rc =)
            _weak_ref.inc();
        ali_assert(rc > 1);
    }

    void dec_weak_ref( void )
    {
        auto const rc = _weak_ref.dec();

        ali_assert(rc >= 0);

        if ( rc == 0 )
            delete this;
    }

    long use_count( void ) const
    {
        return _strong_ref.get();
    }

private:    // Methods
    virtual void destroy_managed_resource( void ) = 0;
    
private:    // Data members
    atomic::counter _strong_ref{1};
    atomic::counter _weak_ref{1};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class shared_info : public basic_shared_info
// ******************************************************************
{
public:
    explicit shared_info( ali::auto_ptr<T> ptr )
    :   _ptr{ali::move(ptr)}
    {}

    shared_info( shared_info const& ) = delete;

    virtual ~shared_info( void ) override = default;
    
    shared_info& operator=( shared_info const& ) = delete;

private:    //  Methods
    virtual void destroy_managed_resource( void ) override
    {
        this->_ptr.reset();
    }

private:    //  Data members
    ali::auto_ptr<T>    _ptr{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class in_place_shared_info : public basic_shared_info
// ******************************************************************
{
public:
    template <typename... Params>
    explicit in_place_shared_info( Params&&... params )
    {
        this->_buf.template emplace<T>(
            ali::forward<Params>(params)...);
    }

    in_place_shared_info(
        in_place_shared_info const& ) = delete;

    virtual ~in_place_shared_info( void ) override = default;
    
    in_place_shared_info& operator=(
        in_place_shared_info const& ) = delete;

    T* get( void )
    {
        return this->_buf.template begin<T>();
    }

private:    //  Typedefs
    using in_place_buffer = ali::in_place_buffer<sizeof(T), alignof(T)>;

private:    //  Methods
    virtual void destroy_managed_resource( void ) override
    {
        this->get()->~T();
    }

private:    //  Data members
    in_place_buffer _buf{};
};

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename T>
class shared_ptr
// ******************************************************************
{
public:     // Typedefs
    typedef T element_type;

public:
    shared_ptr( void ) {}

    shared_ptr( ali::nullptr_type ) {}

    template <typename U>
    explicit shared_ptr( U* ptr )
    :   shared_ptr{ali::auto_ptr<U>{ptr}}
    {}

    template <typename U>
    shared_ptr( shared_ptr<U> const& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    shared_ptr( shared_ptr const& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U>&& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }

    shared_ptr( shared_ptr&& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U> const& b, meta::static_pointer_cast_tag )
    :   _ptr{static_cast<T*>(b._ptr)},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U>&& b, meta::static_pointer_cast_tag )
    :   _ptr{static_cast<T*>(b._ptr)},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }
    
    template <typename U>
    shared_ptr( shared_ptr<U> const& b, meta::dynamic_pointer_cast_tag )
    :   _ptr{dynamic_cast<T*>(b._ptr)},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U>&& b, meta::dynamic_pointer_cast_tag )
    :   _ptr{dynamic_cast<T*>(b._ptr)},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }
    
    template <typename U>
    shared_ptr( shared_ptr<U> const& b, meta::const_pointer_cast_tag )
    :   _ptr{const_cast<T*>(b._ptr)},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U>&& b, meta::const_pointer_cast_tag )
    :   _ptr{const_cast<T*>(b._ptr)},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }
    
    template <typename U>
    shared_ptr( shared_ptr<U> const& b, meta::reinterpret_pointer_cast_tag )
    :   _ptr{reinterpret_cast<T*>(b._ptr)},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_strong_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( shared_ptr<U>&& b, meta::reinterpret_pointer_cast_tag )
    :   _ptr{reinterpret_cast<T*>(b._ptr)},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }
    
    template <typename U>
    shared_ptr( ali::auto_ptr<U> b )
    :   _ptr{b.get()}
    {
        if ( this->_ptr != nullptr )
        {
            this->_info = new_auto_ptr<
                hidden::shared_info<U>>(
                    ali::move(b)).release();
                        //  May throw.
        }

        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    shared_ptr( ali::auto_ptr<U[]> b ) = delete;

    ~shared_ptr( void )
    {
        ALI_IF_DEBUG(this->assert_invariant());

        if ( this->_info == nullptr )
            return;

        this->_info->dec_strong_ref();
        
        ALI_IF_DEBUG(this->_ptr = invalid_pointer_value);
        
        ALI_IF_DEBUG(this->_info = invalid_pointer_value);
    }

    template <typename U>
    shared_ptr& operator=( shared_ptr<U> const& b )
    {
        shared_ptr{b}.swap(*this);
        return *this;
    }

    shared_ptr& operator=( shared_ptr const& b )
    {
        shared_ptr{b}.swap(*this);
        return *this;
    }

    template <typename U>
    shared_ptr& operator=( shared_ptr<U>&& b )
    {
        shared_ptr{ali::move(b)}.swap(*this);
        return *this;
    }

    shared_ptr& operator=( shared_ptr&& b )
    {
        shared_ptr{ali::move(b)}.swap(*this);
        return *this;
    }

    T* get( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        return this->_ptr;
    }

    template <typename U>
    void reset( U* ptr )
    {
        shared_ptr{ptr}.swap(*this);
    }

    void reset( void )
    {
        shared_ptr{}.swap(*this);
    }

    T& operator*( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ali_assert(this->_ptr != nullptr);
        return *this->_ptr;
    }

    T* operator->( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ali_assert(this->_ptr != nullptr);
        return this->_ptr;
    }

    bool is_null( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        return this->_ptr == nullptr;
    }

    friend bool is_null( shared_ptr const& a )
    {
        return a.is_null();
    }

    long use_count( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        return this->_info != nullptr ? this->_info->use_count() : 0;
    }

    bool operator==( ali::nullptr_type ) const
    {
        return this->is_null();
    }

    bool operator!=( ali::nullptr_type ) const
    {
        return !this->is_null();
    }

    template <typename U>
    bool operator==( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info == b._info;
    }

    template <typename U>
    bool operator!=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info != b._info;
    }

    template <typename U>
    bool operator<( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info < b._info;
    }

    template <typename U>
    bool operator<=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info <= b._info;
    }

    template <typename U>
    bool operator>( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info > b._info;
    }

    template <typename U>
    bool operator>=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info >= b._info;
    }

    template <typename U>
    bool operator==( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info == b._info;
    }

    template <typename U>
    bool operator!=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info != b._info;
    }

    template <typename U>
    bool operator<( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info < b._info;
    }

    template <typename U>
    bool operator<=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info <= b._info;
    }

    template <typename U>
    bool operator>( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info > b._info;
    }

    template <typename U>
    bool operator>=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info >= b._info;
    }

    void swap( shared_ptr& b )
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;
        swap(this->_ptr, b._ptr);
        swap(this->_info, b._info);
    }

    friend void swap( shared_ptr& a, shared_ptr& b )
    {
        a.swap(b);
    }

    void assert_invariant( void ) const
    {
        ali_assert(this->_ptr != invalid_pointer_value);
        ali_assert(this->_info != invalid_pointer_value);
    }

private:    //  Methods
    explicit shared_ptr(
        ali::auto_ptr<hidden::in_place_shared_info<T>> info )
    :   _ptr{info->get()},
        _info{info.release()}
    {
        ALI_IF_DEBUG(this->assert_invariant());
    }

private:    // Data members
    T*                          _ptr{};
    hidden::basic_shared_info*  _info{};

    friend handle2;

    friend handle2_id;

    template <typename>
    friend class shared_ptr;

    template <typename>
    friend class weak_ptr;

    template <typename U, typename... Params>
    friend shared_ptr<U> new_shared_ptr( Params&&... params );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class weak_ptr
// ******************************************************************
{
public:     // Typedefs
    typedef T element_type;

public:
    weak_ptr( void ) {}

    weak_ptr( ali::nullptr_type ) {}

    template <typename U>
    weak_ptr( weak_ptr<U> const& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_weak_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    weak_ptr( weak_ptr const& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_weak_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    weak_ptr( weak_ptr<U>&& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }

    weak_ptr( weak_ptr&& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    weak_ptr( shared_ptr<U> const& b )
    :   _ptr{b._ptr},
        _info{b._info}
    {
        if ( this->_info != nullptr )
            this->_info->inc_weak_ref();
        
        ALI_IF_DEBUG(this->assert_invariant());
    }

    template <typename U>
    weak_ptr( shared_ptr<U[]> const& b ) = delete;

    ~weak_ptr( void )
    {
        ALI_IF_DEBUG(this->assert_invariant());

        if ( this->_info == nullptr )
            return;

        this->_info->dec_weak_ref();
        
        ALI_IF_DEBUG(this->_ptr = invalid_pointer_value);
        
        ALI_IF_DEBUG(this->_info = invalid_pointer_value);
    }

    template <typename U>
    weak_ptr& operator=( weak_ptr<U> const& b )
    {
        weak_ptr{b}.swap(*this);
        return *this;
    }

    weak_ptr& operator=( weak_ptr const& b )
    {
        weak_ptr{b}.swap(*this);
        return *this;
    }

    template <typename U>
    weak_ptr& operator=( weak_ptr<U>&& b )
    {
        weak_ptr{ali::move(b)}.swap(*this);
        return *this;
    }

    weak_ptr& operator=( weak_ptr&& b )
    {
        weak_ptr{ali::move(b)}.swap(*this);
        return *this;
    }

    template <typename U>
    weak_ptr& operator=( shared_ptr<U> const& b )
    {
        weak_ptr{b}.swap(*this);
        return *this;
    }

    template <typename U>
    weak_ptr& operator=( shared_ptr<U[]> const& b ) = delete;

    void reset( void )
    {
        weak_ptr{}.swap(*this);
    }

    long use_count( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        return this->_info != nullptr ? this->_info->use_count() : 0;
    }

    bool is_expired( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        return this->_info == nullptr || this->_info->use_count() == 0;
    }
    
    shared_ptr<T> lock( void ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        
        shared_ptr<T> temp{};

        if (    this->_info != nullptr
            &&  this->_info->inc_strong_ref_if_not_zero() )
        {
            temp._ptr = this->_ptr;
            temp._info = this->_info;
        }

        return temp;
    }

    template <typename U>
    bool operator==( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info == b._info;
    }

    template <typename U>
    bool operator!=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info != b._info;
    }

    template <typename U>
    bool operator<( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info < b._info;
    }

    template <typename U>
    bool operator<=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info <= b._info;
    }

    template <typename U>
    bool operator>( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info > b._info;
    }

    template <typename U>
    bool operator>=( shared_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info >= b._info;
    }

    template <typename U>
    bool operator==( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info == b._info;
    }

    template <typename U>
    bool operator!=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info != b._info;
    }

    template <typename U>
    bool operator<( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info < b._info;
    }

    template <typename U>
    bool operator<=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info <= b._info;
    }

    template <typename U>
    bool operator>( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info > b._info;
    }

    template <typename U>
    bool operator>=( weak_ptr<U> const& b ) const
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());
        return this->_info >= b._info;
    }

    void swap( weak_ptr& b )
    {
        ALI_IF_DEBUG(this->assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;
        swap(this->_ptr, b._ptr);
        swap(this->_info, b._info);
    }

    friend void swap( weak_ptr& a, weak_ptr& b )
    {
        a.swap(b);
    }

    void assert_invariant( void ) const
    {
        ali_assert(this->_ptr != invalid_pointer_value);
        ali_assert(this->_info != invalid_pointer_value);
    }

private:    // Data members
    T*                          _ptr{};
    hidden::basic_shared_info*  _info{};

    template <typename>
    friend class shared_ptr;

    template <typename>
    friend class weak_ptr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename... Params>
inline ali::shared_ptr<T> new_shared_ptr( Params&&... params )
// ******************************************************************
{
    return ali::shared_ptr<T>{
        new_auto_ptr<hidden::in_place_shared_info<T>>(
            ali::forward<Params>(params)...)};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> static_pointer_cast(
    ali::shared_ptr<U> const& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ptr,
        meta::static_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> static_pointer_cast(
    ali::shared_ptr<U>&& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ali::move(ptr),
        meta::static_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> dynamic_pointer_cast(
    ali::shared_ptr<U> const& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ptr,
        meta::dynamic_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> dynamic_pointer_cast(
    ali::shared_ptr<U>&& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ali::move(ptr),
        meta::dynamic_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> const_pointer_cast(
    ali::shared_ptr<U> const& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ptr,
        meta::const_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> const_pointer_cast(
    ali::shared_ptr<U>&& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ali::move(ptr),
        meta::const_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> reinterpret_pointer_cast(
    ali::shared_ptr<U> const& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ptr,
        meta::reinterpret_pointer_cast_tag{}};
}

// ******************************************************************
template <typename T, typename U>
inline ali::shared_ptr<T> reinterpret_pointer_cast(
    ali::shared_ptr<U>&& ptr )
// ******************************************************************
{
    return ali::shared_ptr<T>{ali::move(ptr),
        meta::reinterpret_pointer_cast_tag{}};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
class handle2
// ******************************************************************
//  This is basically a *move-only* shared_ptr<void>.
// ******************************************************************
{
public:
    handle2( void ) = default;
    
    handle2( handle2 const& ) = delete;
    
    handle2( handle2&& b )
    :   _info{b._info}
    {
        b._info = nullptr;

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename T>
    handle2( shared_ptr<T>&& b )
    :   _info{b._info}
    {
        b._ptr = nullptr;
        b._info = nullptr;

        ALI_IF_DEBUG(assert_invariant());
    }
    
    ~handle2( void )
    {
        ALI_IF_DEBUG(assert_invariant());

        if ( _info == nullptr )
            return;

        _info->dec_strong_ref();
        
        ALI_IF_DEBUG(_info = invalid_pointer_value);
    }

    friend bool operator==( handle2 const& a, handle2_id const& b );
    friend bool operator==( handle2_id const& a, handle2 const& b );

    handle2& operator=( handle2 const& ) = delete;
    
    handle2& operator=( handle2&& b )
    {
        handle2{ali::move(b)}.swap(*this);
        return *this;
    }

    bool is_null( void ) const
    {
        ALI_IF_DEBUG(assert_invariant());

        return _info == nullptr;
    }

    void reset( void )
    {
        handle2{}.swap(*this);
    }

    void swap( handle2& b )
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;
        swap(_info, b._info);
    }

    void assert_invariant( void ) const
    {
        ali_assert(_info != invalid_pointer_value);
    }

private:    //  Data members
    hidden::basic_shared_info*  _info{};

    friend handle2_id;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class handle2_id
// ******************************************************************
//  A "weak_ptr" counterpart for handle2.
//  Can be used to identify a particular handle2 object.
// ******************************************************************
{
public:
    handle2_id( void ) = default;
    
    handle2_id( handle2_id const& b )
    :   _info{b._info}
    {
        if ( _info != nullptr )
            _info->inc_weak_ref();

        ALI_IF_DEBUG(assert_invariant());
    }
    
    handle2_id( handle2_id&& b )
    :   _info{b._info}
    {
        b._info = nullptr;

        ALI_IF_DEBUG(assert_invariant());
    }

    explicit handle2_id( handle2 const& b )
    :   _info{b._info}
    {
        if ( _info != nullptr )
            _info->inc_weak_ref();

        ALI_IF_DEBUG(assert_invariant());
    }

    template <typename T>
    explicit handle2_id( shared_ptr<T> const& b )
    :   _info{b._info}
    {
        if ( _info != nullptr )
            _info->inc_weak_ref();

        ALI_IF_DEBUG(assert_invariant());
    }

    ~handle2_id( void )
    {
        ALI_IF_DEBUG(assert_invariant());

        if ( _info == nullptr )
            return;

        _info->dec_weak_ref();
        
        ALI_IF_DEBUG(_info = invalid_pointer_value);
    }

    friend bool operator==( handle2 const& a, handle2_id const& b );
    friend bool operator==( handle2_id const& a, handle2 const& b );

    friend bool operator==( handle2_id const& a, handle2_id const& b )
    {
        return a._info == b._info;
    }

    friend bool operator!=( handle2_id const& a, handle2_id const& b )
    {
        return !(a == b);
    }

    handle2_id& operator=( handle2_id const& b )
    {
        handle2_id{b}.swap(*this);
        return *this;
    }
    
    handle2_id& operator=( handle2_id&& b )
    {
        handle2_id{ali::move(b)}.swap(*this);
        return *this;
    }

    void swap( handle2_id& b )
    {
        ALI_IF_DEBUG(assert_invariant());
        ALI_IF_DEBUG(b.assert_invariant());

        using ali::swap;
        swap(_info, b._info);
    }

    friend void swap( handle2_id& a, handle2_id& b )
    {
        a.swap(b);
    }

    void assert_invariant( void ) const
    {
        ali_assert(_info != invalid_pointer_value);
    }

private:    //  Data members
    hidden::basic_shared_info*  _info{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline bool operator==( handle2 const& a, handle2_id const& b )
// ******************************************************************
{
    return a._info == b._info;
}

// ******************************************************************
inline bool operator==( handle2_id const& a, handle2 const& b )
// ******************************************************************
{
    return a._info == b._info;
}

// ******************************************************************
inline bool operator!=( handle2 const& a, handle2_id const& b )
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=( handle2_id const& a, handle2 const& b )
// ******************************************************************
{
    return !(a == b);
}
    
}   //  namespace ali
