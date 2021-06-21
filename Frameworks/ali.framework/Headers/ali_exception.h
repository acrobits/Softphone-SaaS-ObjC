/*
 *  ali_exception.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_utils_forward.h"
#include "ali/ali_attribute.h"
#include "ali/ali_backtrace.h"
#include "ali/ali_nullptr.h"
#include "ali/ali_string_forward.h"
#include "ali/ali_throw.h"
#include "ali/ali_location.h"


namespace ali
{

// ******************************************************************
class exception_trace_item;
// ******************************************************************

// ******************************************************************
ali::location exception_trace_item_location(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
string_const_ref exception_trace_item_text(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
exception_trace_item const* exception_trace_item_next(
    exception_trace_item const& item );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
class exception_trace_item_holder
// ******************************************************************
{
public:
    exception_trace_item_holder( void ) {}

    exception_trace_item_holder(
        exception_trace_item_holder const& b );

    ~exception_trace_item_holder( void );

    void operator()(
        ali::location location,
        string_const_ref text );

    void operator()(
        ali::location location,
        wstring_const_ref text );

/*
#if defined(ALI_FEATURE_VARIADIC_TEMPLATES)
    // ******************************************************************
    template <typename... T>
    void operator ()( ali::location const& location,
                      string_const_ref format,
                      T... params )
    // ******************************************************************
    {
        tformatter fmt(format);
        ali::for_each(fmt, params...);
        operator ()(location, fmt.str());
    }
#endif
*/

    exception_trace_item const* get( void ) const;

protected:  // Data members
    struct data;
    data*   _data{};

private:    //  Methods
    exception_trace_item_holder& operator=(
        exception_trace_item_holder const& );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class safe_exception_string
// ******************************************************************
//  Reference counted string wrapper whose copy constructor and
//  assignment operator doesn't throw.
// ******************************************************************
{
public:
    safe_exception_string( string_const_ref str );
    safe_exception_string( wstring_const_ref str );

    safe_exception_string( safe_exception_string const& b );

    ~safe_exception_string( void );

    safe_exception_string& operator=( safe_exception_string b );

    c_string_const_ref get( void ) const;

    //void swap( safe_exception_string& b )
    //{
    //    using ali::swap;
    //    swap(_data, b._data);
    //}
    //
    //friend void swap(
    //    safe_exception_string& a,
    //    safe_exception_string& b )
    //{
    //    a.swap(b);
    //}

private:    // Data members
    struct data;
    data*   _data{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class exception
// ******************************************************************
//  The ultimate base class for all exceptions.
// ******************************************************************
{
public:
    virtual ~exception( void ) {}

    virtual ali::string& append_description( ali::string& descr ) const = 0;
    ali::string& append_located_description( ali::string& descr ) const;

    ali::string what( void ) const throw();

    mutable ali::location location; ///< Location where the exception was thrown
    mutable exception_trace_item_holder trace; ///< Application trace
    ali::backtrace::storage backtrace; ///< Backtrace when the exception was thrown
};

// ******************************************************************
ali::string& format(
    ali::string& str,
    exception const& value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename Ex>
const Ex& operator^( const Ex &ex, ali::location location )
// ******************************************************************
{
	ex.location = location;
	return ex;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
class general_exception : public exception
// ******************************************************************
{
public:
    general_exception( string_const_ref descr );

    virtual ali::string& append_description(
        ali::string& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class syntax_error_exception : public exception
// ******************************************************************
{
public:
    syntax_error_exception( string_const_ref descr );

    virtual ali::string& append_description(
        ali::string& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class basic_range_error_exception : public exception
// ******************************************************************
{
public:
    basic_range_error_exception( string_const_ref const& descr );

    virtual ali::string& append_description(
        ali::string& descr ) const override;

private:
    safe_exception_string   _descr;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class range_error_exception : public basic_range_error_exception
// ******************************************************************
{
public:
    range_error_exception(
        string_const_ref const& descr, T cur, T begin, T end )
    :   basic_range_error_exception{descr},
        _cur{cur},
        _begin{begin},
        _end{end}
    {}

    T current( void ) const
    {
        return _cur;
    }

    T begin( void ) const
    {
        return _begin;
    }

    T end( void ) const
    {
        return _end;
    }

private:    //  Data members
    T   _cur;
    T   _begin;
    T   _end;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class out_of_memory_exception : public exception
// ******************************************************************
{
public:
    virtual ali::string& append_description(
        ali::string& descr ) const override;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class optional_is_null_exception : public exception
// ******************************************************************
{
public:
    virtual ali::string& append_description(
        ali::string& descr ) const override;
};

// ******************************************************************
// ******************************************************************

}   //  namespace ali
