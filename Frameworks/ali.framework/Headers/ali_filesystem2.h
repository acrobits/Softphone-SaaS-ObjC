/*
 *  ali_filesystem2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_forward.h"
#include "ali/ali_filesystem2_platform.h"
#include "ali/ali_serializer.h"

namespace ali
{

namespace filesystem2
{

namespace file
{

// ******************************************************************
template <typename file_handle>
class serializer_ : public ali::seekable, public ali::serializer
// ******************************************************************
{
public:
    template <typename T>
    explicit serializer_( T&& h )
    :   _h(ali::forward<T>(h))
    {
        ali_assert(!_h.is_null());
    }

    virtual int write( blob_const_ref buf ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->write(buf.data(), buf.size());
    }
    virtual ali::int64 avail_to_write( void ) const ALI_ATTRIBUTE_OVERRIDE
    {
        return meta::integer::max_value<ali::int64>::result;
    }
    virtual ali::int64 beg( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_begin(off);
    }
    virtual ali::int64 cur( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_current(off);
    }
    virtual ali::int64 end( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_end(off);
    }
    virtual seekable* seek( void ) ALI_ATTRIBUTE_OVERRIDE
    {
        return this;
    }

private:    //  Methods
    serializer_( serializer_ const& );
    serializer_& operator=( serializer_ const& );

private:    //  Data members
    file_handle _h;
};

// ******************************************************************
typedef serializer_<file::auto_handle> owning_serializer;
// ******************************************************************

// ******************************************************************
typedef serializer_<file::auto_handle const&> non_owning_serializer;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename file_handle>
class deserializer_ : public ali::seekable, public ali::deserializer
// ******************************************************************
{
public:
    template <typename T>
    explicit deserializer_( T&& h )
    :   _h(ali::forward<T>(h))
    {
        ali_assert(!_h.is_null());
    }

    virtual int read( blob_ref buf ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->read(buf.data(), buf.size());
    }
    virtual ali::int64 avail_to_read( void ) const ALI_ATTRIBUTE_OVERRIDE
    {
        ali::int64 const size = _h->size();
        ali::int64 const pos = _h->pos();
        return ali::maxi(size, pos) - pos;
    }
    virtual ali::int64 beg( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_begin(off);
    }
    virtual ali::int64 cur( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_current(off);
    }
    virtual ali::int64 end( ali::int64 off ) ALI_ATTRIBUTE_OVERRIDE
    {
        return _h->set_pos_from_end(off);
    }
    virtual seekable* seek( void ) ALI_ATTRIBUTE_OVERRIDE
    {
        return this;
    }

private:    //  Methods
    deserializer_( deserializer_ const& );
    deserializer_& operator=( deserializer_ const& );

private:    //  Data members
    file_handle _h;
};

// ******************************************************************
typedef deserializer_<file::auto_handle> owning_deserializer;
// ******************************************************************

// ******************************************************************
typedef deserializer_<file::auto_handle const&> non_owning_deserializer;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
class circular_log_file
// ******************************************************************
//  The file begins with an 8-character hex string offset
//  of the end of the circular content (content_end).
//  Together with the actual file size (file_size) it can be used
//  to unwrap the content:
//  
//  Example 1:
//
//  |---------------------|
//  |0                    |content_end = file_size
//
//  content = file[8..content_end)
//
//  Example 2:
//
//  |---------------------|---------------------------|
//  |0                    |content_end                |file_size
//
//  content = file[content_end..file_size) + file[8..content_end)
//
// ******************************************************************
{
public:
    circular_log_file(
        filesystem2::path const& path,
        int max_file_size );

    void write( string_const_ref str );

    ali::string& read( ali::string& content ) const;
    ali::blob& read( ali::blob& content ) const;
    
    ali::string read( void ) const
    {
        ali::string content{};
        read(content);
        return content;
    }

    void erase( void );

private:    //  Methods
    circular_log_file(
        circular_log_file const& ) ALI_ATTRIBUTE_DELETED;
    circular_log_file& operator=(
        circular_log_file const& ) ALI_ATTRIBUTE_DELETED;

    template <typename operation>
    void with_open_file( operation op ) const;

    file::auto_handle open_file( void ) const;

private:    //  Data members
    int                 _max_file_size;
    filesystem2::path   _path{};
};

// ******************************************************************
// ******************************************************************

}   //  namespace file

array<bool> test( void );

}   //  namespace filesystem2

}   //  namespace ali
