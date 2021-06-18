/*
 *  ali_filesystem2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_filesystem2_common.h"
#include "ali/ali_generic_handle.h"

namespace ali
{

namespace filesystem2
{

// ******************************************************************
query_result query( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
query_result query( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline query_result query( filesystem2::path const& path )
// ******************************************************************
{
    return query(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp last_modified(
    c_string_const_ref path );
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp last_modified(
    c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
bool set_last_modified(
    c_string_const_ref path,
    time::unix_micro_timestamp time );
// ******************************************************************

// ******************************************************************
bool set_last_modified(
    c_wstring_const_ref path,
    time::unix_micro_timestamp time );
// ******************************************************************

// ******************************************************************
// ******************************************************************

namespace file
{

// ******************************************************************
class wrapper
// ******************************************************************
//  Don't derive this from ali::(de)serializer!
//  Use ali::filesystem2::file::(de)serializer class instead.
// ******************************************************************
{
public:     //  Struct
    struct handle_traits
    {
        typedef int native_handle;

        static bool is_null( native_handle h )
        {
            return h < 0;
        }

        static native_handle null( void )
        {
            return -1;
        }

        static void destroy( native_handle h );
    };

public:
    wrapper( handle_traits::native_handle fd )
    :   _fd(fd)
    {
        ali_assert(!handle_traits::is_null(_fd));
    }

    ali::int64 size( void ) const;

    ali::int64 pos( void ) const;

    ali::int64 set_pos_from_begin( ali::int64 offset );
    ali::int64 set_pos_from_end( ali::int64 offset );
    ali::int64 set_pos_from_current( ali::int64 offset );

    int read( void* buf, int bufSize );
    int read( blob_ref data )
    {
        return read(data.data(), data.size());
    }

    int write( void const* buf, int bufSize );
    int write( ali::blob_const_ref data )
    {
        return write(data.data(), data.size());
    }

    void set_end( void );

    void flush( void );

    wrapper* operator->( void )
    {
        return this;
    }

private:    // Data members
    handle_traits::native_handle    _fd;
};

// ******************************************************************
typedef ali::auto_generic_handle<file::wrapper>  auto_handle;
// ******************************************************************

// ******************************************************************
typedef ali::shared_generic_handle<file::wrapper>  shared_handle;
// ******************************************************************

// ******************************************************************
file::auto_handle try_open(
    c_string_const_ref path,
    unsigned mode,
    open_result* result = nullptr );
// ******************************************************************

// ******************************************************************
file::auto_handle try_open(
    c_wstring_const_ref path,
    unsigned mode,
    open_result* result = nullptr );
// ******************************************************************

// ******************************************************************
inline file::auto_handle try_open(
    filesystem2::path const& path,
    unsigned mode,
    open_result* result = nullptr )
// ******************************************************************
{
    return file::try_open(
        path.format_platform_string(), mode, result);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
file::auto_handle open(
    c_string_const_ref path,
    unsigned mode );
// ******************************************************************

// ******************************************************************
file::auto_handle open(
    c_wstring_const_ref path,
    unsigned mode );
// ******************************************************************

// ******************************************************************
inline file::auto_handle open(
    filesystem2::path const& path,
    unsigned mode )
// ******************************************************************
{
    return file::open(path.format_platform_string(), mode);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
file::auto_handle try_open_if_modified_since(
    c_string_const_ref path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time,
    open_result* result = nullptr );
// ******************************************************************

// ******************************************************************
file::auto_handle try_open_if_modified_since(
    c_wstring_const_ref path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time,
    open_result* result = nullptr );
// ******************************************************************

// ******************************************************************
inline file::auto_handle try_open_if_modified_since(
    filesystem2::path const& path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time,
    open_result* result = nullptr )
// ******************************************************************
{
    return file::try_open_if_modified_since(
        path.format_platform_string(), mode,
        last_modified_time, result);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
file::auto_handle open_if_modified_since(
    c_string_const_ref path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time );
// ******************************************************************

// ******************************************************************
file::auto_handle open_if_modified_since(
    c_wstring_const_ref path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time );
// ******************************************************************

// ******************************************************************
inline file::auto_handle open_if_modified_since(
    filesystem2::path const& path,
    unsigned mode,
    time::unix_micro_timestamp& last_modified_time )
// ******************************************************************
{
    return file::open_if_modified_since(
        path.format_platform_string(), mode,
        last_modified_time);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
close_result try_close( file::auto_handle file );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
remove_result try_remove( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
remove_result try_remove( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline remove_result try_remove( filesystem2::path const& path )
// ******************************************************************
{
    return file::try_remove(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
void remove( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
void remove( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline void remove( filesystem2::path const& path )
// ******************************************************************
{
    file::remove(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
copy_result try_copy(
    c_string_const_ref existing_path,
    c_string_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
copy_result try_copy(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
inline copy_result try_copy(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt )
// ******************************************************************
{
    return file::try_copy(
        existing_path.format_platform_string(),
        new_path.format_platform_string(),
        owt);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
void copy(
    c_string_const_ref existing_path,
    c_string_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
void copy(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
inline void copy(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt )
// ******************************************************************
{
    file::copy(
        existing_path.format_platform_string(),
        new_path.format_platform_string(),
        owt);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
move_result try_move(
    c_string_const_ref existing_path,
    c_string_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
move_result try_move(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
inline move_result try_move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt )
// ******************************************************************
{
    return file::try_move(
        existing_path.format_platform_string(),
        new_path.format_platform_string(),
        owt);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
void move(
    c_string_const_ref existing_path,
    c_string_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
void move(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
inline void move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt )
// ******************************************************************
{
    file::move(
        existing_path.format_platform_string(),
        new_path.format_platform_string(),
        owt);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
get_size_result try_get_size( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
get_size_result try_get_size( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline get_size_result try_get_size( filesystem2::path const& path )
// ******************************************************************
{
    return file::try_get_size(path.format_platform_string());
}

}   //  namespace file

namespace folder
{

// ******************************************************************
ali::array<entry> try_get_entries(
    c_string_const_ref path,
    callback<bool(entry const&)> const& filter,
    get_entries_result* result );
// ******************************************************************

// ******************************************************************
ali::array<entry> try_get_entries(
    c_wstring_const_ref path,
    callback<bool(entry const&)> const& filter,
    get_entries_result* result );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::array<entry> get_entries(
    c_string_const_ref path,
    callback<bool(entry const&)> const& filter );
// ******************************************************************

// ******************************************************************
ali::array<entry> get_entries(
    c_wstring_const_ref path,
    callback<bool(entry const&)> const& filter );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
folder::create_result try_create(
    c_string_const_ref path );
// ******************************************************************

// ******************************************************************
folder::create_result try_create(
	c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline folder::create_result try_create(
    filesystem2::path const& path )
// ******************************************************************
{
    return folder::try_create(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
void create( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
void create( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline void create( filesystem2::path const& path )
// ******************************************************************
{
    folder::create(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
folder::create_all_result try_create_all(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
inline folder::create_all_result try_create_all(
    c_tstring_const_ref path_string )
// ******************************************************************
{
    filesystem2::path path;
    return path.parse_platform_string(path_string)
        ? folder::try_create_all(path)
        : folder::create_all_result::general_error;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
folder::create_all_result try_create_all_but_last(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
inline folder::create_all_result try_create_all_but_last(
    c_tstring_const_ref path_string )
// ******************************************************************
{
    filesystem2::path path;
    return path.parse_platform_string(path_string)
        ? folder::try_create_all_but_last(path)
        : folder::create_all_result::general_error;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
folder::remove_result try_remove(
    c_string_const_ref path );
// ******************************************************************

// ******************************************************************
folder::remove_result try_remove(
	c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline folder::remove_result try_remove(
    filesystem2::path const& path )
// ******************************************************************
{
    return folder::try_remove(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
void remove( c_string_const_ref path );
// ******************************************************************

// ******************************************************************
void remove( c_wstring_const_ref path );
// ******************************************************************

// ******************************************************************
inline void remove( filesystem2::path const& path )
// ******************************************************************
{
    folder::remove(path.format_platform_string());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
int try_remove_entries(
    c_string_const_ref folder,
    callback<remove_entry_action(
        filesystem2::path const& entry_path,
        folder::entry const& entry)> const& filter );
// ******************************************************************

// ******************************************************************
int try_remove_entries(
    c_wstring_const_ref folder,
    callback<remove_entry_action(
        filesystem2::path const& entry_path,
        folder::entry const& entry)> const& filter );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
folder::move_result try_move(
    c_string_const_ref existing_path,
    c_string_const_ref new_path );
// ******************************************************************

// ******************************************************************
folder::move_result try_move(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path );
// ******************************************************************

// ******************************************************************
inline folder::move_result try_move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path )
// ******************************************************************
{
    return folder::try_move(
        existing_path.format_platform_string(),
        new_path.format_platform_string());
}

// ******************************************************************
void move(
    c_string_const_ref existing_path,
    c_string_const_ref new_path );
// ******************************************************************

// ******************************************************************
void move(
    c_wstring_const_ref existing_path,
    c_wstring_const_ref new_path );
// ******************************************************************

// ******************************************************************
inline void move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path )
// ******************************************************************
{
    folder::move(
        existing_path.format_platform_string(),
        new_path.format_platform_string());
}

}	//	namespace folder

}   //  namespace filesystem2

}   //  namespace ali

