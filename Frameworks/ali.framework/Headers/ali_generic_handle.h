#pragma once
#include "ali/ali_atomic.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_features.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_utility.h"
#include "ali/ali_exception_if.h"

namespace ali
{

// ******************************************************************
// ******************************************************************
//  The following classes are parametrized on a wrapper class
//  which must expose the following minimal interface:
// ******************************************************************
/*

class wrapper
{
public:     //  Struct
    struct handle_traits
    {
        using native_handle = ...

        static bool is_null( native_handle h );

        static native_handle null( void );

        static void destroy( native_handle h );
    };

public:
    wrapper( handle_traits::native_handle h )
    :   _h{h}
    {
        ali_assert(!handle_traits::is_null(_h));
    }

    wrapper* operator->( void ) { return this; }

private:    //  Data members
    handle_traits::native_handle    _h;
};

*/
// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename wrapper>
class auto_generic_handle
// ******************************************************************
{
    using traits = typename wrapper::handle_traits;

public:
    using native_handle = typename traits::native_handle;

    auto_generic_handle( void ) = default;

    auto_generic_handle( auto_generic_handle&& b )
	:   _h{b.release()}
	{}

    auto_generic_handle( ali::nullptr_type ) {}

    explicit auto_generic_handle( native_handle h )
    :   _h{h}
    {}

    ~auto_generic_handle( void )
    {
        traits::destroy(_h);

        _h = traits::null();
    }

    auto_generic_handle& operator=(
        auto_generic_handle&& b )
    {
		return reset(b.release());
    }

    wrapper operator->( void ) const
    {
        ali_assert(!is_null());

        return wrapper{_h};
    }

    bool is_null( void ) const
    {
        return traits::is_null(_h);
    }

    bool operator==( ali::nullptr_type ) const
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const
    {
        return !is_null();
    }

    native_handle get( void ) const
    {
        return _h;
    }

    auto_generic_handle& reset( void )
    {
        return reset(traits::null());
    }

    auto_generic_handle& reset( native_handle h )
    {
        using ali::swap;
        swap(_h, h);

        if ( _h != h )
            traits::destroy(h);

        return *this;
    }

    native_handle release( void )
    {
        native_handle const t{_h};

        _h = traits::null();

        return t;
    }

    void swap( auto_generic_handle& b )
    {
        using ali::swap;
        swap(_h, b._h);
    }

    friend void swap(
        auto_generic_handle& a,
        auto_generic_handle& b )
    {
        a.swap(b);
    }

private:    //  Methods
    auto_generic_handle(
        auto_generic_handle const& ) = delete;
    auto_generic_handle& operator=(
        auto_generic_handle const& ) = delete;

private:    //  Data members
    native_handle   _h{traits::null()};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename wrapper>
class shared_generic_handle
// ******************************************************************
//  Using instances of this class in multiple threads
//  is safe, provided all threads have their own "copy"
//  of the shared_generic_handle class.
//  If two or more threads share a pointer or a reference to
//  a single instance of the shared_generic_handle then the
//  behavior is undefined.
// ******************************************************************
{
    using traits = typename wrapper::handle_traits;

public:
    using native_handle = typename traits::native_handle;

    shared_generic_handle( void ) = default;

    shared_generic_handle( shared_generic_handle const& b )
    :   _h{b._h},
        _rc{b._rc}
    {
        if ( _rc != nullptr )
            _rc->inc();
    }

    shared_generic_handle( shared_generic_handle&& b )
    :   _h{b._h},
        _rc{b._rc}
    {
        b._h = traits::null();

        b._rc = nullptr;
    }

    shared_generic_handle( nullptr_type ) {}

    explicit shared_generic_handle( auto_generic_handle<wrapper> b )
    {
        if ( b.is_null() ) return;

        _rc = new_auto_ptr<atomic::counter>(1).release();

        _h = b.release();
    }

    explicit shared_generic_handle( native_handle h )
    :   shared_generic_handle{auto_generic_handle<wrapper>{h}}
    {}

    ~shared_generic_handle( void )
    {
        reset();
    }

    shared_generic_handle& operator=( nullptr_type )
    {
        return reset();
    }

    shared_generic_handle& operator=( shared_generic_handle const& b )
    {
        ali_assert((_rc == b._rc) == (_h == b._h));

        if ( _rc != b._rc )
        {
            reset();

            _h = b._h;

            _rc = b._rc;

            if ( _rc != nullptr )
                _rc->inc();
        }
        return *this;
    }

    shared_generic_handle& operator=( shared_generic_handle&& b )
    {
        ali_assert((_rc == b._rc) == (_h == b._h));

        if ( _rc != b._rc )
        {
            reset();

            _h = b._h;

            _rc = b._rc;

            b._h = traits::null();

            b._rc = nullptr;
        }
        return *this;
    }

    shared_generic_handle& operator=( auto_generic_handle<wrapper> b )
    {
        return operator=(shared_generic_handle{ali::move(b)});
    }

    wrapper operator->( void ) const
    {
        ali_assert(!is_null());

        return wrapper{_h};
    }

    bool is_null( void ) const
    {
        return traits::is_null(_h);
    }

    explicit operator bool( void ) const
    {
        return !is_null();
    }

    bool operator !( void ) const
    {
        return is_null();
    }

    native_handle get( void ) const
    {
        return _h;
    }

    shared_generic_handle& reset( void )
    {
        if ( _rc != nullptr && _rc->dec() == 0 )
        {
            traits::destroy(_h);

            delete_scalar(_rc);

            _h = traits::null();

            _rc = nullptr;
        }
        return *this;
    }

    void swap( shared_generic_handle& b )
    {
        using ali::swap;
        swap(_h, b._h);
        swap(_rc, b._rc);
    }

    friend void swap(
        shared_generic_handle& a,
        shared_generic_handle& b )
    {
        a.swap(b);
    }
    
    bool operator==( shared_generic_handle const& b ) const
    {
        ali_assert((_h == b._h) == (_rc == b._rc));
        return _h == b._h;
    }
    
    bool operator!=( shared_generic_handle const& b ) const
    {
        return !operator==(b);
    }

private:    //  Data members
    native_handle       _h{traits::null()};
    atomic::counter*    _rc{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename wrapper>
class non_owning_generic_handle
// ******************************************************************
{
    using traits = typename wrapper::handle_traits;

public:
    using native_handle = typename traits::native_handle;

    non_owning_generic_handle( void ) = default;

    non_owning_generic_handle( ali::nullptr_type ) {}

    explicit non_owning_generic_handle( native_handle h )
    :   _h{h}
    {}

    wrapper operator->( void ) const
    {
        ali_assert(!is_null());

        return wrapper{_h};
    }

    bool is_null( void ) const
    {
        return traits::is_null(_h);
    }

    bool operator==( ali::nullptr_type ) const
    {
        return is_null();
    }

    bool operator!=( ali::nullptr_type ) const
    {
        return !is_null();
    }

    native_handle get( void ) const
    {
        return _h;
    }

    non_owning_generic_handle& reset( void )
    {
        return reset(traits::null());
    }

    non_owning_generic_handle& reset( native_handle h )
    {
        _h = h;
        return *this;
    }

    native_handle release( void )
    {
        native_handle const t{_h};

        _h = traits::null();

        return t;
    }

    void swap( non_owning_generic_handle& b )
    {
        using ali::swap;
        swap(_h, b._h);
    }

    friend void swap(
        non_owning_generic_handle& a,
        non_owning_generic_handle& b )
    {
        a.swap(b);
    }

private:    //  Data members
    native_handle   _h{traits::null()};
};

// ******************************************************************
// ******************************************************************


}   //  namespace ali
