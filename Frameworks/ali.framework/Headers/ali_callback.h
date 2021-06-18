/*
 *  ali_callback.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2014 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_callback_forward.h"
#include "ali/ali_attribute.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_debug.h"
#include "ali/ali_in_place_buffer.h"
#include "ali/ali_noncopyable.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_utility.h"


namespace ali
{

namespace hidden
{

// ******************************************************************
class basic_unknown_fun : public ali::noncopyable
// ******************************************************************
{
protected:  //  Methods
    basic_unknown_fun( void );
    virtual ~basic_unknown_fun( void );
};

// ******************************************************************
class unknown_fun : public basic_unknown_fun
// ******************************************************************
{
public:
    unknown_fun*            _obj;
    void (unknown_fun::*    _fun)( void );
};

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename return_type,
          typename... param_types>
class callback<return_type(param_types...)>
// ******************************************************************
{
    using in_place_buffer
        = ali::in_place_buffer<
            4 * sizeof(void*),
            alignof(hidden::unknown_fun)>;

    using in_place_version = meta::define_bool_result<true>;
    using heap_version = meta::define_bool_result<false>;
    template <typename T>
    using is_in_place = meta::define_bool_result<
        (in_place_buffer::size >= sizeof(T))>;

protected:    // Struct
    // ******************************************************************
    class basic_fun : public ali::noncopyable
    // ******************************************************************
    {
    public:
        virtual return_type call( param_types... params ) const = 0;
        virtual basic_fun* clone( in_place_buffer& buf ) const = 0;
        virtual basic_fun* move( in_place_buffer& buf ) = 0;
        virtual void destroy( in_place_buffer& buf ) = 0;

    protected:  //  Methods
        basic_fun( void ) {}
        virtual ~basic_fun( void ) {}
    };

    // ******************************************************************
    template <typename object_type, typename member_type>
    class member_fun : public basic_fun
    // ******************************************************************
    {
    public:
        // ******************************************************************
        static basic_fun* create(
            in_place_buffer& buf,
            object_type* obj, member_type fun )
        // ******************************************************************
        {
            return create(buf, obj, fun, is_in_place<member_fun>{});
        }

        // ******************************************************************
        virtual return_type call( param_types... params ) const override
        // ******************************************************************
        {
            return (_obj->*_fun)(ali::forward<param_types>(params)...);
        }

        // ******************************************************************
        virtual basic_fun* clone(
            in_place_buffer& buf ) const override
        // ******************************************************************
        {
            return create(buf, _obj, _fun);
        }

        // ******************************************************************
        virtual basic_fun* move(
            in_place_buffer& buf ) override
        // ******************************************************************
        {
            return move(buf, is_in_place<member_fun>{});
        }

        // ******************************************************************
        virtual void destroy(
            in_place_buffer& buf ) override
        // ******************************************************************
        {
            destroy(buf, is_in_place<member_fun>{});
        }

    private:    //  Methods
        // ******************************************************************
        member_fun( object_type* obj, member_type fun )
        // ******************************************************************
        :   _obj{obj},
            _fun{fun}
        {}

        virtual ~member_fun( void ) override {}

        // ******************************************************************
        static basic_fun* create(
            in_place_buffer& buf,
            object_type* obj, member_type fun,
            in_place_version )
        // ******************************************************************
        {
            return &buf.emplace<member_fun>(obj, fun);
        }

        // ******************************************************************
        static basic_fun* create(
            in_place_buffer&,
            object_type* obj, member_type fun,
            heap_version )
        // ******************************************************************
        {
            return private_auto_ptr_factory<member_fun>::
                new_auto_ptr(obj, fun).release();
        }

        // ******************************************************************
        basic_fun* move(
            in_place_buffer& buf,
            in_place_version )
        // ******************************************************************
        {
            basic_fun* const result{
                &buf.emplace<member_fun>(_obj, _fun)};

            this->~member_fun();

            return result;
        }

        // ******************************************************************
        basic_fun* move(
            in_place_buffer&,
            heap_version )
        // ******************************************************************
        {
            return this;
        }

        // ******************************************************************
        void destroy(
            in_place_buffer& ALI_IF_DEBUG(buf),
            in_place_version )
        // ******************************************************************
        {
            ali_assert(this == buf.begin<member_fun>());

            this->~member_fun();
        }

        // ******************************************************************
        void destroy(
            in_place_buffer& ALI_IF_DEBUG(buf),
            heap_version )
        // ******************************************************************
        {
            ali_assert(this != buf.begin<void>());

            private_auto_ptr_factory<member_fun>::delete_scalar(this);
        }

    private:    //  Data members
        object_type*    _obj;
        member_type     _fun;
        friend in_place_buffer;
        friend private_auto_ptr_factory<member_fun>;
    };

    // ******************************************************************
    template <typename function_type>
    class other_fun : public basic_fun
    // ******************************************************************
    {
    public:
        // ******************************************************************
        static basic_fun* create(
            in_place_buffer& buf, function_type fun )
        // ******************************************************************
        {
            return create(buf, ali::move(fun), is_in_place<other_fun>{});
        }

        // ******************************************************************
        virtual return_type call( param_types... params ) const override
        // ******************************************************************
        {
            return _fun(ali::forward<param_types>(params)...);
        }

        // ******************************************************************
        virtual basic_fun* clone(
            in_place_buffer& buf ) const override
        // ******************************************************************
        {
            return create(buf, _fun);
        }

        // ******************************************************************
        virtual basic_fun* move(
            in_place_buffer& buf ) override
        // ******************************************************************
        {
            return move(buf, is_in_place<other_fun>{});
        }

        // ******************************************************************
        virtual void destroy(
            in_place_buffer& buf ) override
        // ******************************************************************
        {
            destroy(buf, is_in_place<other_fun>{});
        }

    private:    //  Methods
        // ******************************************************************
        other_fun( function_type fun )
        // ******************************************************************
        :   _fun{ali::move(fun)}
        {}

        virtual ~other_fun( void ) override {}

        // ******************************************************************
        static basic_fun* create(
            in_place_buffer& buf,
            function_type fun,
            in_place_version )
        // ******************************************************************
        {
            return &buf.emplace<other_fun>(ali::move(fun));
        }

        // ******************************************************************
        static basic_fun* create(
            in_place_buffer&,
            function_type fun,
            heap_version )
        // ******************************************************************
        {
            return private_auto_ptr_factory<other_fun>::
                new_auto_ptr(ali::move(fun)).release();
        }

        // ******************************************************************
        basic_fun* move(
            in_place_buffer& buf,
            in_place_version )
        // ******************************************************************
        {
            basic_fun* const result{
                &buf.emplace<other_fun>(ali::move(_fun))};

            this->~other_fun();

            return result;
        }

        // ******************************************************************
        basic_fun* move(
            in_place_buffer&,
            heap_version )
        // ******************************************************************
        {
            return this;
        }

        // ******************************************************************
        void destroy(
            in_place_buffer& ALI_IF_DEBUG(buf),
            in_place_version )
        // ******************************************************************
        {
            ali_assert(this == buf.begin<other_fun>());

            this->~other_fun();
        }

        // ******************************************************************
        void destroy(
            in_place_buffer& ALI_IF_DEBUG(buf),
            heap_version )
        // ******************************************************************
        {
            ali_assert(this != buf.begin<void>());

            private_auto_ptr_factory<other_fun>::delete_scalar(this);
        }

    private:    //  Data members
        function_type   _fun;
        friend in_place_buffer;
        friend private_auto_ptr_factory<other_fun>;
    };

public:     // Methods
    // ******************************************************************
    callback( ali::nullptr_type = nullptr )
    // ******************************************************************
    {}

    // ******************************************************************
    template <typename function_type>
    callback( function_type fun )
    // ******************************************************************
    {
        assign(ali::move(fun));
    }

    // ******************************************************************
    template <typename object_type, typename member_type>
    callback( object_type* obj, member_type fun )
    // ******************************************************************
    {
        assign(obj, fun);
    }

    // ******************************************************************
    callback( callback const& b )
    // ******************************************************************
    {
        operator=(b);
    }

    // ******************************************************************
    callback( callback&& b )
    // ******************************************************************
    {
        operator=(ali::move(b));
    }

    // ******************************************************************
    template <typename other_return_type,
              typename... other_param_types>
    callback( callback<other_return_type(other_param_types...)> const& b )
    // ******************************************************************
    {
        static_assert(sizeof...(param_types) == sizeof...(other_param_types),
                      "ali::callback can be created from another ali::callback only when both have the same number of arguments");
        assign(b);
    }

    // ******************************************************************
    ~callback( void )
    // ******************************************************************
    {
        reset();
    }

    // ******************************************************************
    callback& operator=( callback const& b )
    // ******************************************************************
    {
        if ( _fun != b._fun )
        {
            reset();

            if ( b._fun != nullptr )
                _fun = b._fun->clone(_buf);
        }

        return *this;
    }

    // ******************************************************************
    callback& operator=( callback&& b )
    // ******************************************************************
    {
        if ( _fun != b._fun )
        {
            reset();

            if ( b._fun != nullptr )
            {
                _fun = b._fun->move(_buf);
                // Do not reset b, b._fun was already moved or destroyed
                b._fun = nullptr;
            }
        }

        return *this;
    }

    // ******************************************************************
    template <typename function_type>
    callback& assign( function_type fun )
    // ******************************************************************
    {
        reset();

        _fun = other_fun<function_type>::create(_buf, ali::move(fun));

        return *this;
    }

    // ******************************************************************
    template <typename object_type, typename member_type>
    callback& assign( object_type* obj, member_type fun )
    // ******************************************************************
    {
        ali_assert(obj != nullptr);

        reset();

        _fun = member_fun<object_type, member_type>::create(_buf, obj, fun);

        return *this;
    }

    // ******************************************************************
    callback& reset( void )
    // ******************************************************************
    {
        if ( _fun != nullptr )
        {
            _fun->destroy(_buf);
            _fun = nullptr;
        }
        return *this;
    }

    // ******************************************************************
    return_type operator()( param_types... params ) const
    // ******************************************************************
    {
        ali_assert(_fun != nullptr);
        return _fun->call(ali::forward<param_types>(params)...);
    }

    // ******************************************************************
    bool is_null( void ) const
    // ******************************************************************
    {
        return _fun == nullptr;
    }

    // ******************************************************************
    friend bool is_null( callback const& a )
    // ******************************************************************
    {
        return a.is_null();
    }

    // ******************************************************************
    bool operator==( ali::nullptr_type ) const
    // ******************************************************************
    {
        return is_null();
    }

    // ******************************************************************
    bool operator!=( ali::nullptr_type ) const
    // ******************************************************************
    {
        return !is_null();
    }

private:    //  Data members
    in_place_buffer _buf{};
    basic_fun*      _fun{};
};

}
