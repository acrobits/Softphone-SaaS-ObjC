/*
 *  ali_filesystem_base2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_filesystem2_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_callback_forward.h"
#include "ali/ali_error_forward.h"
#include "ali/ali_time_forward.h"
#include "ali/ali_tstring.h"

namespace ali
{

namespace filesystem2
{

// ******************************************************************
class path_root
// ******************************************************************
{
public:
    path_root( void ) {}

    static bool is_valid_drive_letter(
        ali::tchar letter );
            //  Defined in ali_filesystem2_platform.cpp.

    static bool are_equal_drive_letters(
        ali::tchar a,
        ali::tchar b );
            //  Defined in ali_filesystem2_platform.cpp.

    static bool is_valid_server_name(
        tstring_const_ref name );
            //  Defined in ali_filesystem2_platform.cpp.

    static bool are_equal_server_names(
        tstring_const_ref a,
        tstring_const_ref b );
            //  Defined in ali_filesystem2_platform.cpp.

    path_root& set_drive( ali::tchar letter );

    path_root& set_current_drive( void );

    path_root& set_server( tstring_const_ref name );

    path_root& erase( void )
    {
        _value.erase();

        return *this;
    }

    bool operator==( path_root const& b ) const;

    bool operator!=( path_root const& b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return _value.is_empty();
    }

    bool is_drive( void ) const;

    bool is_current_drive( void ) const;

    bool is_server( void ) const;

    bool is_valid( void ) const;

    tstring_const_ref name( void ) const
    {
        if ( is_drive() )
            return _value.ref().ref(0, 1);
        else if ( is_server() )
            return _value.ref().ref(2, _value.size() - 3);
        else
            return tstring_const_ref();
    }

    void swap( path_root& b )
    {
        _value.swap(b._value);
    }

    friend void swap( path_root& a, path_root& b )
    {
        a.swap(b);
    }

    ali::string& format_platform_string(
        ali::string& platform_string ) const;

    ali::wstring& format_platform_string(
        ali::wstring& platform_string ) const;

    ali::tstring format_platform_string( void ) const
    {
        ali::tstring platform_string{};

        format_platform_string(platform_string);

        return platform_string;
    }

private:    //  Methods
    explicit path_root( tstring_const_ref value )
    :   _value{value}
    {}

private:    //  Data members
    ali::tstring _value{};

    friend class path;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct path_segment
// ******************************************************************
{
    path_segment( void )
    {}

    path_segment( tstring_const_ref value )
    :   value{value}
    {}

    path_segment& operator=( tstring_const_ref b )
    {
        value = b;
        return *this;
    }

    static bool is_valid(
        tstring_const_ref value );
            //  Defined in ali_filesystem2_platform.cpp.

    static bool are_equal(
        tstring_const_ref a,
        tstring_const_ref b );
            //  Defined in ali_filesystem2_platform.cpp.

    bool operator==( path_segment const& b ) const
    {
        return are_equal(value, b.value);
    }

    bool operator==( tstring_const_ref b ) const
    {
        return are_equal(value, b);
    }

    bool operator!=( path_segment const& b ) const
    {
        return !operator==(b);
    }

    bool operator!=( tstring_const_ref b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return value.is_empty();
    }

    bool is_valid( void ) const
    {
        return  is_dot()
            ||  is_dot_dot()
            ||  is_valid(value);
    }

    static bool is_dot( tstring_const_ref value )
    {
        return value == ALI_T("."_s);
    }

    bool is_dot( void ) const
    {
        return is_dot(value);
    }

    static bool is_dot_dot( tstring_const_ref value )
    {
        return value == ALI_T(".."_s);
    }

    bool is_dot_dot( void ) const
    {
        return is_dot_dot(value);
    }

    int dot_extension_position( void ) const
    {
        return  is_dot() || is_dot_dot()
            ?   value.size()
            :   value.ref().index_of_last(ALI_T('.'));
    }

    int dotless_extension_position( void ) const
    {
        int const pos = dot_extension_position();
        return pos == value.size() ? pos : pos + 1;
    }

    tstring_const_ref name_without_extension( void ) const
    {
        return value.ref().ref_left(dot_extension_position());
    }

    tstring_const_ref dot_extension( void ) const
    {
        return value.ref().ref_right(dot_extension_position());
    }

    tstring_const_ref dotless_extension( void ) const
    {
        return value.ref().ref_right(dotless_extension_position());
    }

    path_segment& set_name_without_extension(
        tstring_const_ref name )
    {
        value.replace(0, dot_extension_position(), name);
        return *this;
    }

    path_segment& set_extension(
        tstring_const_ref extension );
            //  The extension parameter may begin with a dot or not.
            //  There is a logic to handle both cases correctly.

    void swap( path_segment& b )
    {
        value.swap(b.value);
    }

    friend void swap(
        path_segment& a,
        path_segment& b )
    {
        a.swap(b);
    }

    ali::string& format_platform_string(
        ali::string& platform_string ) const;

    ali::wstring& format_platform_string(
        ali::wstring& platform_string ) const;

    ali::tstring format_platform_string( void ) const
    {
        ali::tstring platform_string{};

        format_platform_string(platform_string);

        return platform_string;
    }

    friend ali::string& format(
        ali::string& str,
        path_segment const& value,
        string_const_ref /*format_string*/ )
    {
        return value.format_platform_string(str);
    }

    friend ali::wstring& format(
        ali::wstring& str,
        path_segment const& value,
        wstring_const_ref /*format_string*/ )
    {
        return value.format_platform_string(str);
    }

    ali::tstring value{};
};

// ******************************************************************
// ******************************************************************

typedef ali::array<path_segment> path_segments;

// ******************************************************************
// ******************************************************************

// ******************************************************************
class path
// ******************************************************************
//          Path := Root Segment ('\' Segment)*
//       Segment := Name DotExtension
//  DotExtension := '.' DotlessExtension
//          Root := Server | Drive | CurrentDrive | epsilon
//        Server := '\\' Name '\'
//         Drive := Letter ':\'
//  CurrentDrive := '\'
// ******************************************************************
{
public:     //  Methods
    path( void ) {}

    explicit path( tstring_const_ref platform_string )
    {
        parse_platform_string(platform_string);
    }

    path( ali::filesystem2::path_segments segments )
    :   segments{ali::move(segments)}
    {}

    path( ali::filesystem2::path_root root,
          ali::filesystem2::path_segments segments )
    :   root{ali::move(root)},
        segments{ali::move(segments)}
    {}

    static bool is_separator( ali::tchar c );
        //  Defined in ali_filesystem2_platform.cpp.

    bool operator==( path const& b ) const
    {
        return  root == b.root
            &&  segments == b.segments;
    }

    bool operator!=( path const& b ) const
    {
        return !operator==(b);
    }

    bool is_empty( void ) const
    {
        return  root.is_empty()
            &&  segments.is_empty();
    }

    bool is_valid( void ) const;

    path& collapse_dots( void );

    path& erase( void )
    {
        root.erase();
        segments.erase();
        return *this;
    }

    bool parse_platform_string(
        tstring_const_ref platform_string );

    ali::string& format_platform_string(
        ali::string& platform_string ) const
    {
        return format_platform_string(
            platform_string,
            segments.size());
    }

    ali::wstring& format_platform_string(
        ali::wstring& platform_string ) const
    {
        return format_platform_string(
            platform_string,
            segments.size());
    }

    ali::tstring format_platform_string( void ) const
    {
        ali::tstring platform_string{};

        format_platform_string(platform_string);

        return platform_string;
    }

    ali::string& format_platform_string(
        ali::string& platform_string,
        int segment_count ) const;

    ali::wstring& format_platform_string(
        ali::wstring& platform_string,
        int segment_count ) const;

    ali::tstring format_platform_string(
        int segment_count ) const
    {
        ali::tstring platform_string{};

        format_platform_string(platform_string, segment_count);

        return platform_string;
    }

    ali::string& format_platform_string_without_last_segment(
        ali::string& platform_string ) const
    {
        ali_assert(!segments.is_empty());

        return format_platform_string(
            platform_string,
            segments.size() - 1);
    }

    ali::wstring& format_platform_string_without_last_segment(
        ali::wstring& platform_string ) const
    {
        ali_assert(!segments.is_empty());

        return format_platform_string(
            platform_string,
            segments.size() - 1);
    }

    ali::tstring format_platform_string_without_last_segment( void ) const
    {
        ali::tstring platform_string{};

        format_platform_string_without_last_segment(platform_string);

        return platform_string;
    }

    bool begins_with( path const& prefix ) const
    {
        return  root == prefix.root
            &&  begins_with(segments, prefix.segments);
    }

    void swap( path& b )
    {
        root.swap(b.root);
        segments.swap(b.segments);
    }

    friend void swap( path& a, path& b )
    {
        a.swap(b);
    }

    path& operator /=( tstring_const_ref s )
    {
        segments.push_back(path_segment(s));
        return *this;
    }

    path& operator /=( path_segment s )
    {
        segments.push_back(ali::move(s));
        return *this;
    }

    path& operator /=( path_segments s )
    {
        for (auto & segment : s)
            segments.push_back(ali::move(segment));
        return *this;
    }

    friend ali::string& format(
        ali::string& str,
        path const& value,
        string_const_ref /*format_string*/ )
    {
        return value.format_platform_string(str);
    }

    friend ali::wstring& format(
        ali::wstring& str,
        path const& value,
        wstring_const_ref /*format_string*/ )
    {
        return value.format_platform_string(str);
    }

private:    //  Methods
    static bool begins_with(
        path_segments const& path,
        path_segments const& prefix );

public:     //  Data members
    path_root                   root;
    path_segments               segments;
    static ali::tchar const     default_separator;
        //  Defined in ali_filesystem2_platform.cpp.
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline path operator/( path p, tstring_const_ref s )
// ******************************************************************
{
    p /= s;
    return p;
}

// ******************************************************************
inline path operator/( path p, path_segment s )
// ******************************************************************
{
    p /= ali::move(s);
    return p;
}

// ******************************************************************
inline path operator/( path p, path_segments s )
// ******************************************************************
{
    p /= ali::move(s);
    return p;
}

// ******************************************************************
inline path operator/( path a, path b )
// ******************************************************************
{
    if ( b.root.is_empty() )
    {
        a /= ali::move(b.segments);
        return a;
    }
    else
    {
        return b;
    }
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct query_result
// ******************************************************************
{
    enum type {error, not_found, file, folder};

    query_result( type value )
    :   value(value)
    {}

    bool is_error( void ) const
    {
        return value == error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_file( void ) const
    {
        return value == file;
    }

    bool is_folder( void ) const
    {
        return value == folder;
    }

    type value;
};

// ******************************************************************
query_result query( c_tstring_const_ref path );
query_result query( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp last_modified(
    c_tstring_const_ref path );
time::unix_micro_timestamp last_modified(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
bool set_last_modified(
    c_tstring_const_ref path,
    time::unix_micro_timestamp time );
bool set_last_modified(
    filesystem2::path const& path,
    time::unix_micro_timestamp time );
// ******************************************************************

// ******************************************************************
bool set_last_modified(
    c_tstring_const_ref path,
    time::unix_timestamp time );
bool set_last_modified(
    filesystem2::path const& path,
    time::unix_timestamp time );
// ******************************************************************
//  Implemented in ali_filesystem2_common.cpp.
// ******************************************************************

namespace file
{

// ******************************************************************
struct overwrite
// ******************************************************************
{
    enum type {no, yes};

    overwrite( type value )
    :   value{value}
    {}

    friend bool operator==( overwrite a, overwrite b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( overwrite a, overwrite b )
    {
        return !(a == b);
    }

    bool is_no( void ) const
    {
        return value == no;
    }

    bool is_yes( void ) const
    {
        return value == yes;
    }

    type value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
namespace open_mode
// ******************************************************************
{

namespace hidden
{

namespace flag
{

unsigned const create_new       = 0;
unsigned const create_always    = 1;
unsigned const open_existing    = 2;
unsigned const open_always      = 3;
unsigned const access_read      = 4;
unsigned const access_write     = 8;
unsigned const share_read       = 16;
unsigned const share_write      = 32;

unsigned const create_mask      = 3;
unsigned const access_mask      = access_read|access_write;
unsigned const share_mask       = share_read|share_write;

}   //  namespace flag

// ******************************************************************
template <unsigned mode>
struct constructor
// ******************************************************************
{
    typedef constructor<mode
        | flag::access_read>    access_read;

    typedef constructor<mode
        | flag::access_write>   access_write;

    typedef constructor<mode
        | flag::access_read
        | flag::access_write>   access_all;

    typedef constructor<mode
        | flag::share_read>     share_read;

    typedef constructor<mode
        | flag::share_write>    share_write;

    typedef constructor<mode
        | flag::share_read
        | flag::share_write>    share_all;

    static unsigned const value = mode;
};

}   //  namespace hidden

// ******************************************************************
typedef hidden::constructor<hidden::flag::create_new>       create_new;
typedef hidden::constructor<hidden::flag::create_always>    create_always;
typedef hidden::constructor<hidden::flag::open_existing>    open_existing;
typedef hidden::constructor<hidden::flag::open_always>      open_always;
// ******************************************************************
//                        |            When the file...
//  This argument:        |          Exists      Does not exist
//  ----------------------+------------------------------------
//  CREATE_ALWAYS         |         Truncates       Creates
//  CREATE_NEW      +-----------+     Fails         Creates
//  OPEN_ALWAYS   ==| does this |==>  Opens         Creates
//  OPEN_EXISTING   +-----------+     Opens          Fails
// ******************************************************************

}   //  namespace open_mode

// ******************************************************************
struct open_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        invalid_mode,
        not_found,
        already_exists,
        access_denied,
        sharing_violation,
    };

    open_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_invalid_mode( void ) const
    {
        return value == invalid_mode;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_already_exists( void ) const
    {
        return value == already_exists;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    c_string_const_ref display_name( void ) const;

    friend error_domain const& domain_of( open_result );

    operator error_code( void ) const;

    bool from_error_code( error_code code );

    type    value;

private:    //  Class
    class _domain;
};

// ******************************************************************
struct close_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
    };

    close_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    type    value;
};

//  The implementation is required to define:
//
//  Data Types:
//      auto_handle
//      shared_handle
//
//  Functions:
//
//      auto_handle try_open( c_tstring_const_ref path, unsigned mode,
//          open_result* result = nullptr );
//
//      auto_handle try_open( filesystem2::path const& path, unsigned mode,
//          open_result* result = nullptr );
//
//      auto_handle open( c_tstring_const_ref path, unsigned mode );
//
//      auto_handle open( filesystem2::path const& path, unsigned mode );
//
//      close_result try_close( auto_handle file );
//
//  The implementation may define:
//
//      auto_handle try_open_if_modified_since(
//          c_tstring_const_ref path,
//          unsigned mode,
//          time::unix_micro_timestamp& last_modified_time,
//          open_result* result = nullptr );
//              --  This function must update the last_modified_time
//              --  parameter if it opens the file.
//
//      auto_handle try_open_if_modified_since(
//          filesystem2::path const& path,
//          unsigned mode,
//          time::unix_micro_timestamp& last_modified_time,
//          open_result* result = nullptr );
//              --  This function must update the last_modified_time
//              --  parameter if it opens the file.
//
//      auto_handle open_if_modified_since(
//          c_tstring_const_ref path,
//          unsigned mode,
//          time::unix_micro_timestamp& last_modified_time );
//              --  This function must update the last_modified_time
//              --  parameter if it opens the file.
//
//      auto_handle open_if_modified_since(
//          filesystem2::path const& path,
//          unsigned mode,
//          time::unix_micro_timestamp& last_modified_time );
//              --  This function must update the last_modified_time
//              --  parameter if it opens the file.

// ******************************************************************
struct remove_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        access_denied,
        sharing_violation
    };

    remove_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    type    value;
};

// ******************************************************************
remove_result try_remove( c_tstring_const_ref path );
remove_result try_remove( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
void remove( c_tstring_const_ref path );
void remove( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
struct copy_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        already_exists,
        access_denied,
        sharing_violation
    };

    copy_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_already_exists( void ) const
    {
        return value == already_exists;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    type    value;
};

// ******************************************************************
copy_result try_copy(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path,
    overwrite::type owt );
copy_result try_copy(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
void copy(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path,
    overwrite::type owt );
void copy(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
struct move_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        already_exists,
        access_denied,
        sharing_violation
    };

    move_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_already_exists( void ) const
    {
        return value == already_exists;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    type    value;
};

// ******************************************************************
move_result try_move(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path,
    overwrite::type owt );
move_result try_move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
void move(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path,
    overwrite::type owt );
void move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path,
    overwrite::type owt );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct get_size_result
// ******************************************************************
{
	enum type
	{
		success,
		general_error,
		not_file,
		not_found,
		access_denied,
		sharing_violation
	};

	get_size_result( type value, ali::int64 size = 0 )
	:	value(value),
		_size(size)
	{}

	bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_file( void ) const
    {
        return value == not_file;
    }

	bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

	ali::int64 size( void ) const;
        //  pre:    this->is_success()

	type 		value;

private:	//	Data members
	ali::int64	_size;
};

// ******************************************************************
get_size_result try_get_size( c_tstring_const_ref path );
get_size_result try_get_size( filesystem2::path const& path );
// ******************************************************************

}   //  namespace file

namespace folder
{

// ******************************************************************
struct entry
// ******************************************************************
{
    struct kind
    {
        enum type
        {
            unspecified,
            file,
            folder
        };

        kind( type value = unspecified )
        :   value(value)
        {}

        friend bool operator==( kind a, kind b )
        {
            return a.value == b.value;
        }

        friend bool operator!=( kind a, kind b )
        {
            return !(a == b);
        }

        bool is_unspecified( void ) const
        {
            return value == unspecified;
        }

        bool is_file( void ) const
        {
            return value == file;
        }

        bool is_folder( void ) const
        {
            return value == folder;
        }

        c_string_const_ref display_name( void ) const;

        type    value;
    };

    entry( void )
    :   size(0)
    {}

    entry::kind         kind;
    path_segment        name;
    ali::int64          size;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct get_entries_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
            //  This code means that the *folder itself* was not found.
            //  Getting entries of an empty folder results in an empty
            //  array of entries and success result code.
        access_denied,
    };

    get_entries_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    type    value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline bool default_filter( folder::entry const& entry )
// ******************************************************************
{
    return  !entry.kind.is_folder()
        ||  (   !entry.name.is_dot()
            &&  !entry.name.is_dot_dot());
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::array<entry> try_get_entries(
    c_tstring_const_ref path,
    callback<bool(entry const&)> const& filter,
    get_entries_result* result );
ali::array<entry> try_get_entries(
    filesystem2::path const& path,
    callback<bool(entry const&)> const& filter,
    get_entries_result* result );
// ******************************************************************
//  The default_filter is applied if filter.is_null().
// ******************************************************************

// ******************************************************************
ali::array<entry> try_get_entries(
    c_tstring_const_ref path,
    get_entries_result* result );
ali::array<entry> try_get_entries(
    filesystem2::path const& path,
    get_entries_result* result );
// ******************************************************************
//  Implemented in ali_filesystem2_common.cpp.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::array<entry> get_entries(
    c_tstring_const_ref path,
    callback<bool(entry const&)> const& filter );
ali::array<entry> get_entries(
    filesystem2::path const& path,
    callback<bool(entry const&)> const& filter );
// ******************************************************************
//  The default_filter is applied if filter.is_null().
// ******************************************************************

// ******************************************************************
ali::array<entry> get_entries(
    c_tstring_const_ref path );
ali::array<entry> get_entries(
    filesystem2::path const& path );
// ******************************************************************
//  Implemented in ali_filesystem2_common.cpp.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct create_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        already_exists,
        access_denied
    };

    create_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_already_exists( void ) const
    {
        return value == already_exists;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    type    value;
};

// ******************************************************************
folder::create_result try_create(
    c_tstring_const_ref path );
folder::create_result try_create(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
void create( c_tstring_const_ref path );
void create( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
struct create_all_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        access_denied
    };

    create_all_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    type    value;
};

// ******************************************************************
folder::create_all_result try_create_all(
    c_tstring_const_ref path );
folder::create_all_result try_create_all(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
void create_all( c_tstring_const_ref path );
void create_all( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
folder::create_all_result try_create_all_but_last(
    c_tstring_const_ref path );
folder::create_all_result try_create_all_but_last(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
void create_all_but_last( c_tstring_const_ref path );
void create_all_but_last( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
struct remove_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        not_empty,
        access_denied,
        sharing_violation
    };

    remove_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_not_empty( void ) const
    {
        return value == not_empty;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    type    value;
};


// ******************************************************************
folder::remove_result try_remove(
    c_tstring_const_ref path );
folder::remove_result try_remove(
    filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
void remove( c_tstring_const_ref path );
void remove( filesystem2::path const& path );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct remove_entry_action
// ******************************************************************
{
    enum type
    {
        skip,
            //  File:   Don't touch the file.
            //  Folder: Don't touch the folder.
        remove,
            //  File:   Remove the file.
            //  Folder: Remove the folder WITH ALL its entries
            //          and WITHOUT any filtering.
        recurse,
            //  File:   ali_assert(false)
            //  Folder: Recurse into the folder and apply
            //          the filter to its entries. Then
            //          keep the folder.
        recurse_and_remove_if_empty
            //  File:   ali_assert(false)
            //  Folder: Recurse into the folder and apply
            //          the filter to its entries. Then
            //          remove the folder if it's empty.
    };

    remove_entry_action( type value )
    :   value(value)
    {}

    bool is_skip( void ) const
    {
        return value == skip;
    }

    bool is_remove( void ) const
    {
        return value == remove;
    }

    bool is_recurse( void ) const
    {
        return value == recurse;
    }

    bool is_recurse_and_remove_if_empty( void ) const
    {
        return value == recurse_and_remove_if_empty;
    }

    type    value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
int try_remove_entries(
    c_tstring_const_ref folder,
    callback<remove_entry_action(
        filesystem2::path const& entry_path,
        filesystem2::folder::entry const& entry )> const& filter );
int try_remove_entries(
    filesystem2::path const& folder,
    callback<remove_entry_action(
        filesystem2::path const& entry_path,
        filesystem2::folder::entry const& entry )> const& filter );
// ******************************************************************
//  If filter.is_null() then remove_entry_action::remove is taken
//  for all entries.
//  Does NOT remove the "root" folder.
//  Returns number of entries that could not be removed.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
int try_remove_entries(
    c_tstring_const_ref folder );
int try_remove_entries(
    filesystem2::path const& folder );
// ******************************************************************
//  Implemented in ali_filesystem2_common.cpp.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct move_result
// ******************************************************************
{
    enum type
    {
        success,
        general_error,
        not_found,
        already_exists,
        access_denied,
        sharing_violation
    };

    move_result( type value )
    :   value(value)
    {}

    bool is_success( void ) const
    {
        return value == success;
    }

    bool is_general_error( void ) const
    {
        return value == general_error;
    }

    bool is_not_found( void ) const
    {
        return value == not_found;
    }

    bool is_already_exists( void ) const
    {
        return value == already_exists;
    }

    bool is_access_denied( void ) const
    {
        return value == access_denied;
    }

    bool is_sharing_violation( void ) const
    {
        return value == sharing_violation;
    }

    type    value;
};

// ******************************************************************
move_result try_move(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path );
move_result try_move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path );
// ******************************************************************

// ******************************************************************
void move(
    c_tstring_const_ref existing_path,
    c_tstring_const_ref new_path );
void move(
    filesystem2::path const& existing_path,
    filesystem2::path const& new_path );
// ******************************************************************

}   //  namespace folder

}   //  namespace filesystem2

}   //  namespace ali
