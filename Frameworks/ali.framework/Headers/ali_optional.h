/*
 *  ali_optional.h
 *  ali Library
 *
 *  Copyright (c) 2011 - 2017 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_optional_forward.h"
#include "ali/ali_array_forward.h"
#include "ali/ali_exception_if.h"
#include "ali/ali_features.h"
#include "ali/ali_in_place_buffer.h"
#include "ali/ali_meta_common.h"
#include "ali/ali_tstring.h"

namespace ali
{

namespace meta
{

namespace hidden
{

// ******************************************************************
template <typename T>
struct is_optional
// ******************************************************************
    :   define_false_result {};

// ******************************************************************
template <typename T>
struct is_optional<optional_common<T>>
// ******************************************************************
    :   define_true_result {};

// ******************************************************************
template <typename T>
struct is_optional<optional<T>>
// ******************************************************************
    :   define_true_result {};

}   //  namespace hidden

// ******************************************************************
template <typename T>
struct is_optional
// ******************************************************************
    :   hidden::is_optional<typename meta::remove_cv<T>::type> {};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
struct un_optional
// ******************************************************************
    :   define_type<T> {};

// ******************************************************************
template <typename T>
struct un_optional<optional_common<T>>
// ******************************************************************
    :   define_type<T> {};

// ******************************************************************
template <typename T>
struct un_optional<optional<T>>
// ******************************************************************
    :   define_type<T> {};

// ******************************************************************
// ******************************************************************

}   //  namespace meta


// ******************************************************************
struct in_place_tag
// ******************************************************************
{
    explicit in_place_tag( void ) = default;
};

// ******************************************************************
constexpr in_place_tag in_place{};
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class optional_common
// ******************************************************************
// Optional (can be none) value storage
// ******************************************************************
{
public:     //  Typedefs
    using value_type = T;

public:     //  Methods
    bool is_null( void ) const
    {
        return this->_ptr.is_null;
    }

    T* operator->( void )
    {
        ali_assert(!this->is_null());
        optional_is_null_if(this->is_null(), ALI_HERE);
        return this->_ptr;
    }

    T const* operator->( void ) const
    {
        ali_assert(!this->is_null());
        optional_is_null_if(this->is_null(), ALI_HERE);
        return this->_ptr;
    }

    T& operator*( void )
    {
        ali_assert(!this->is_null());
        optional_is_null_if(this->is_null(), ALI_HERE);
        return *this->_ptr;
    }

    T const& operator*( void ) const
    {
        ali_assert(!this->is_null());
        optional_is_null_if(this->is_null(), ALI_HERE);
        return *this->_ptr;
    }

    void reset( void )
    {
        if ( this->is_null() )
            return;

        (*this->_ptr).~T();

        this->_ptr.is_null = true;
    }

    T& set_default_value( void )
    {
        using ali::swap;

        if ( this->is_null() )
        {
            this->_ptr._buf.template emplace<T>();

            this->_ptr.is_null = false;
        }
        else
        {
            T def{};
                //  This initializes even built-in
                //  types to a well defined value.
            swap(*this->_ptr, def);
        }

        return *this->_ptr;
    }

    T& set_value( T const& value )
        //  Constructs if null;
        //  assigns otherwise.
    {
        return this->set_value_forward(value);
    }

    T& set_value( T&& value )
        //  Constructs if null;
        //  assigns otherwise.
    {
        return this->set_value_forward(ali::move(value));
    }

    template <typename U>
    T& set_value_forward( U&& value )
        //  Constructs if null;
        //  assigns otherwise.
    {
        if ( this->is_null() )
        {
            this->_ptr._buf.template emplace<T>(
                ali::forward<U>(value));

            this->_ptr.is_null = false;
        }
        else
        {
            *this->_ptr = ali::forward<U>(value);
        }

        return *this->_ptr;
    }

    template <typename... Params>
    T& emplace( Params&&... params )
        //  Destroys if not null,
        //  then always constructs.
    {
        this->reset();

        this->_ptr._buf.template emplace<T>(
            ali::forward<Params>(params)...);

        this->_ptr.is_null = false;

        return *this->_ptr;
    }

    T const& operator|( T const& b ) const
    {
        return this->is_null() ? b : *this->_ptr;
    }

    optional<T> const& operator|( optional_common const& b ) const
        //  Intentionally using optional_common for the method's parameter(s)
        //  (avoids implicit conversions).
    {
        return this->is_null() ? b.self() : this->self();
    }

    optional<T>& operator|=( T const& b )
    {
        if ( this->is_null() )
            this->set_value(b);

        return this->self();
    }

    optional<T>& operator|=( T&& b )
    {
        if ( this->is_null() )
            this->set_value(ali::move(b));

        return this->self();
    }

    optional<T>& operator|=( optional_common const& b )
        //  Intentionally using optional_common for the method's parameter(s)
        //  (avoids implicit conversions).
    {
        if ( this->is_null() && !b.is_null() )
            this->set_value(*b._ptr);

        return this->self();
    }

    optional<T>& operator|=( optional_common&& b )
        //  Intentionally using optional_common for the method's parameter(s)
        //  (avoids implicit conversions).
    {
        if ( this->is_null() && !b.is_null() )
            this->set_value(ali::move(*b._ptr));

        return this->self();
    }

    void swap( optional_common& b )
        //  Intentionally using optional_common for the method's parameter(s)
        //  (avoids implicit conversions).
    {
        using ali::swap;

        if ( this->is_null() )
        {
            if ( b.is_null() )
                // Both null, nothing to swap
                return;

            this->set_default_value();
            swap(*this->_ptr, *b._ptr);
            b.reset();
        }
        else if ( b.is_null() )
        {
            b.set_default_value();
            swap(*this->_ptr, *b._ptr);
            this->reset();
        }
        else
        {
            swap(*this->_ptr, *b._ptr);
        }
    }

    friend void swap( optional_common& a, optional_common& b )
        //  Intentionally using optional_common for the method's parameter(s)
        //  (avoids implicit conversions).
    {
        a.swap(b);
    }

protected:  //  Methods
    optional_common( void )
    {
        ali_assert(this->is_null());
    }

    optional_common( optional_common const& b )
    {
        ali_assert(this->is_null());

        if ( !b.is_null() )
            this->set_value(*b);
    }

    optional_common( optional_common&& b )
    {
        ali_assert(this->is_null());

        if ( !b.is_null() )
            this->set_value(ali::move(*b));
    }

    template <typename U>
    explicit optional_common( U&& value )
        //  Explicit to disable implicit conversion
        //  for function and method arguments.
    {
        ali_assert(this->is_null());

        this->set_value(ali::forward<U>(value));
    }

    template <typename... Params>
    optional_common( in_place_tag, Params&&... params )
    {
        ali_assert(this->is_null());

        this->emplace(ali::forward<Params>(params)...);
    }

    ~optional_common( void )
    {
        this->reset();
    }

    optional_common& operator=( optional_common const& b )
    {
        if ( b.is_null() )
            this->reset();
        else
            this->set_value(*b._ptr);

        return *this;
    }

    optional_common& operator=( optional_common&& b )
    {
        if ( b.is_null() )
            this->reset();
        else
            this->set_value(ali::move(*b._ptr));

        return *this;
    }

    optional_common& operator=( T const& b )
    {
        this->set_value(b);
        return *this;
    }

    optional_common& operator=( T&& b )
    {
        this->set_value(ali::move(b));
        return *this;
    }

    optional<T> const& self( void ) const
    {
        return static_cast<optional<T> const&>(*this);
    }

    optional<T>& self( void )
    {
        return static_cast<optional<T>&>(*this);
    }

private:    //  Data members
    struct in_place_storage
    {
        operator T*( void )
        {
            ali_assert(!this->is_null);
            return this->_buf.template begin<T>();
        }

        operator T const*( void ) const
        {
            ali_assert(!this->is_null);
            return this->_buf.template begin<T>();
        }

        in_place_buffer<sizeof(T), alignof(T)>  _buf{};
        bool                                    is_null{true};
    }                                           _ptr{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class optional : public optional_common<T>
// ******************************************************************
{
public:     //  Typedefs
    using value_type = typename optional_common<T>::value_type;

public:     //  Methods
    optional( nullptr_type = nullptr ) {}

    optional( optional const& ) = default;

    optional( optional&& ) = default;

    optional( T const& value )
    :   optional_common<T>{value}
    {}

    optional( T&& value )
    :   optional_common<T>{ali::move(value)}
    {}

    optional& operator=( optional const& ) = default;

    optional& operator=( optional&& ) = default;

    optional& operator=( T const& b )
    {
        optional_common<T>::operator=(b);
        return *this;
    }

    optional& operator=( T&& b )
    {
        optional_common<T>::operator=(ali::move(b));
        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class optional<ali::string> : public optional_common<ali::string>
// ******************************************************************
{
public:     //  Typedefs
    using value_type = optional_common<ali::string>::value_type;

public:     //  Methods
    optional( nullptr_type = nullptr ) {}

    optional( optional const& ) = default;
    
    optional( optional&& ) = default;
    
    optional( ali::string const& value )
    :   optional_common<ali::string>{value}
    {}

    optional( ali::string&& value )
    :   optional_common<ali::string>{ali::move(value)}
    {}

    template <typename D>
    optional( exposes_string_const_ref<D> const& value )
    :   optional_common<ali::string>{value.ref()}
    {}

    optional( string_const_ptr value )
    {
        if ( !value.is_null() )
            this->set_value(*value);
    }

    optional( char const* ) = delete;

    optional& operator=( optional const& ) = default;
    
    optional& operator=( optional&& ) = default;
    
    optional& operator=( ali::string const& b )
    {
        optional_common<ali::string>::operator=(b);
        return *this;
    }

    optional& operator=( ali::string&& b )
    {
        optional_common<ali::string>::operator=(ali::move(b));
        return *this;
    }

    template <typename D>
    optional& operator=( exposes_string_const_ref<D> const& b )
    {
        this->set_value(b.ref());
        return *this;
    }

    optional& operator=( string_const_ptr b )
    {
        if ( b.is_null() )
            this->reset();
        else
            this->set_value(*b);

        return *this;
    }

    optional& operator=( char const* ) = delete;

    using optional_common<ali::string>::operator|=;

    optional& operator|=( string_const_ptr b )
    {
        return this->is_null() ? this->operator=(b) : *this;
    }

    operator string_const_ptr( void ) const
    {
        return  this->is_null()
            ?   string_const_ptr{}
            :   this->operator*().pointer();
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class optional<ali::wstring> : public optional_common<ali::wstring>
// ******************************************************************
{
public:     //  Typedefs
    using value_type = optional_common<ali::wstring>::value_type;

public:     //  Methods
    optional( nullptr_type = nullptr ) {}

    optional( optional const& ) = default;
    
    optional( optional&& ) = default;
    
    optional( ali::wstring const& value )
    :   optional_common<ali::wstring>{value}
    {}

    optional( ali::wstring&& value )
    :   optional_common<ali::wstring>{ali::move(value)}
    {}

    template <typename D>
    optional( exposes_wstring_const_ref<D> const& value )
    :   optional_common<ali::wstring>{value.ref()}
    {}

    optional( wstring_const_ptr value )
    {
        if ( !value.is_null() )
            this->set_value(*value);
    }

    optional( ali::wchar const* ) = delete;

    optional& operator=( optional const& ) = default;
    
    optional& operator=( optional&& ) = default;
    
    optional& operator=( ali::wstring const& b )
    {
        optional_common<ali::wstring>::operator=(b);
        return *this;
    }

    optional& operator=( ali::wstring&& b )
    {
        optional_common<ali::wstring>::operator=(ali::move(b));
        return *this;
    }

    template <typename D>
    optional& operator=( exposes_wstring_const_ref<D> const& b )
    {
        this->set_value(b.ref());
        return *this;
    }

    optional& operator=( wstring_const_ptr b )
    {
        if ( b.is_null() )
            this->reset();
        else
            this->set_value(*b);

        return *this;
    }

    optional& operator=( ali::wchar const* ) = delete;

    using optional_common<ali::wstring>::operator|=;

    optional& operator|=( wstring_const_ptr b )
    {
        return this->is_null() ? this->operator=(b) : *this;
    }

    operator wstring_const_ptr( void ) const
    {
        return  this->is_null()
            ?   wstring_const_ptr{}
            :   this->operator*().pointer();
    }
};

// ******************************************************************
// ******************************************************************

//  operator==

// ******************************************************************
template <typename T>
inline bool operator==( optional_common<T> const& a, nullptr_type )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return a.is_null();
}

// ******************************************************************
template <typename T>
inline bool operator==( nullptr_type a, optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return b == a;
}

// ******************************************************************
template <typename T, typename U>
inline typename meta::enable_if<
    !meta::is_optional<U>::result,
bool>::type operator==( optional_common<T> const& a, U const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !a.is_null() && *a == b;
}

// ******************************************************************
template <typename T, typename U>
inline typename meta::enable_if<
    !meta::is_optional<U>::result,
bool>::type operator==( U const& a, optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return b == a;
}

// ******************************************************************
template <typename T>
inline bool operator==(
    optional_common<T> const& a,
    optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return  a.is_null() == b.is_null()
        &&  (a.is_null() || *a == *b);
}

//  operator!=

// ******************************************************************
template <typename T>
inline bool operator!=( optional_common<T> const& a, nullptr_type b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T>
inline bool operator!=( nullptr_type a, optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T, typename U>
inline typename meta::enable_if<
    !meta::is_optional<U>::result,
bool>::type operator!=( optional_common<T> const& a, U const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************    
template <typename T, typename U>
inline typename meta::enable_if<
    !meta::is_optional<U>::result,
bool>::type operator!=( U const& a, optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
template <typename T>    
inline bool operator!=(
    optional_common<T> const& a,
    optional_common<T> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return  !(a == b);
}

// ******************************************************************
// ******************************************************************

//  operator== for optional<string> and string_const_ptr

// ******************************************************************
inline bool operator==(
    optional_common<ali::string> const& a,
    string_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return  a.is_null() == b.is_null()
        &&  (a.is_null() || *a == *b);
}

// ******************************************************************
inline bool operator==(
    string_const_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return b == a;
}

// ******************************************************************
inline bool operator==(
    optional_common<ali::string> const& a,
    c_string_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return a == static_cast<string_const_ptr>(b);
}

// ******************************************************************
inline bool operator==(
    c_string_const_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return static_cast<string_const_ptr>(a) == b;
}

// ******************************************************************
inline bool operator==(
    optional_common<ali::string> const& a,
    string_literal_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return a == static_cast<string_const_ptr>(b);
}

// ******************************************************************
inline bool operator==(
    string_literal_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return static_cast<string_const_ptr>(a) == b;
}

//  operator!= for optional<string> and string_const_ptr

// ******************************************************************
inline bool operator!=(
    optional_common<ali::string> const& a,
    string_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    string_const_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    optional_common<ali::string> const& a,
    c_string_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    c_string_const_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    optional_common<ali::string> const& a,
    string_literal_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    string_literal_ptr a,
    optional_common<ali::string> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
// ******************************************************************

//  operator== for optional<wstring> and wstring_const_ptr

// ******************************************************************
inline bool operator==(
    optional_common<ali::wstring> const& a,
    wstring_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return  a.is_null() == b.is_null()
        &&  (a.is_null() || *a == *b);
}

// ******************************************************************
inline bool operator==(
    wstring_const_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return b == a;
}

// ******************************************************************
inline bool operator==(
    optional_common<ali::wstring> const& a,
    c_wstring_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return a == static_cast<wstring_const_ptr>(b);
}

// ******************************************************************
inline bool operator==(
    c_wstring_const_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return static_cast<wstring_const_ptr>(a) == b;
}

// ******************************************************************
inline bool operator==(
    optional_common<ali::wstring> const& a,
    wstring_literal_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return a == static_cast<wstring_const_ptr>(b);
}

// ******************************************************************
inline bool operator==(
    wstring_literal_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return static_cast<wstring_const_ptr>(a) == b;
}

//  operator!= for optional<wstring> and wstring_const_ptr

// ******************************************************************
inline bool operator!=(
    optional_common<ali::wstring> const& a,
    wstring_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    wstring_const_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    optional_common<ali::wstring> const& a,
    c_wstring_const_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    c_wstring_const_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    optional_common<ali::wstring> const& a,
    wstring_literal_ptr b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
inline bool operator!=(
    wstring_literal_ptr a,
    optional_common<ali::wstring> const& b )
// ******************************************************************
//  Intentionally using optional_common for the function's parameter(s)
//  (avoids implicit conversions).
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::array<bool> test_optional( void );
// ******************************************************************

}
