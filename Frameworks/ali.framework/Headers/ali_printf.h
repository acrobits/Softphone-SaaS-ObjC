#pragma once
#include "ali/ali_array_forward.h"
#include "ali/ali_format.h"
#include "ali/ali_location.h"
#include "ali/ali_math.h"
#include "ali/ali_string.h"

namespace ali
{

// ******************************************************************
inline ali::string& format(
    ali::string& str,
    ali::location const& location,
    string_const_ref /*format_string*/ )
// ******************************************************************
{
    return location.format(str);
}

// ******************************************************************
inline ali::wstring& format(
    ali::wstring& str,
    ali::location const& location,
    wstring_const_ref /*format_string*/ )
// ******************************************************************
{
    return location.format(str);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    blob_const_ref value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    blob_const_ref value,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    string_const_ref value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    char const* value,
    string_const_ref format_string ) = delete;
// ******************************************************************
//  Intentionally left undefined.
//  Use the _s suffix for string literals
//  or explicitly construct a c_string_const_ref object.
//{
//    return format(str, c_string_const_ref(value), format_string);
//}

// ******************************************************************
inline ali::string& format(
    ali::string& str,
    char value,
    string_const_ref format_string )
// ******************************************************************
{
    return format(str, string_const_ref{&value, 1}, format_string);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    wstring_const_ref value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    ali::wchar const* value,
    string_const_ref format_string ) = delete;
// ******************************************************************
//  Intentionally left undefined.
//  Use the _s suffix for string literals
//  or explicitly construct a c_string_const_ref object.
//{
//    return format(str, wstring_const_ref(value), format_string);
//}

// ******************************************************************
inline ali::string& format(
    ali::string& str,
    ali::wchar value,
    string_const_ref format_string )
// ******************************************************************
{
    return format(str, wstring_const_ref{&value, 1}, format_string);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    string_const_ref value,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    char const* value,
    wstring_const_ref format_string ) = delete;
// ******************************************************************
//  Intentionally left undefined.
//  Use the _s suffix for string literals
//  or explicitly construct a c_wstring_const_ref object.
//{
//    return format(str, wstring_const_ref(value), format_string);
//}

// ******************************************************************
inline ali::wstring& format(
    ali::wstring& str,
    char value,
    wstring_const_ref format_string )
// ******************************************************************
{
    return format(str, string_const_ref{&value, 1}, format_string);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    wstring_const_ref value,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    ali::wchar const* value,
    wstring_const_ref format_string ) = delete;
// ******************************************************************
//  Intentionally left undefined.
//  Use the _s suffix for string literals
//  or explicitly construct a c_wstring_const_ref object.
//{
//    return format(str, wstring_const_ref(value), format_string);
//}

// ******************************************************************
inline ali::wstring& format(
    ali::wstring& str,
    ali::wchar value,
    wstring_const_ref format_string )
// ******************************************************************
{
    return format(str, wstring_const_ref{&value, 1}, format_string);
}

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
ali::string& format(
    ali::string& str,
    unsigned long long value,
    bool is_negative,
    int original_size,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    unsigned long long value,
    bool is_negative,
    int original_size,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
#define ALI_DEFINE_FORMAT_INTEGER(base_integer_type)                \
inline ali::string& format(                                         \
    ali::string& str,                                               \
    signed base_integer_type value,                                 \
    string_const_ref format_string )                                \
{                                                                   \
    return hidden::format(                                          \
        str, math::unsigned_abs(value),                             \
        value < 0, sizeof(value), format_string);                   \
}                                                                   \
inline ali::string& format(                                         \
    ali::string& str,                                               \
    unsigned base_integer_type value,                               \
    string_const_ref format_string )                                \
{                                                                   \
    return hidden::format(                                          \
        str, value, false,                                          \
        sizeof(value), format_string);                              \
}                                                                   \
inline ali::wstring& format(                                        \
    ali::wstring& str,                                              \
    signed base_integer_type value,                                 \
    wstring_const_ref format_string )                               \
{                                                                   \
    return hidden::format(                                          \
        str, math::unsigned_abs(value),                             \
        value < 0, sizeof(value), format_string);                   \
}                                                                   \
inline ali::wstring& format(                                        \
    ali::wstring& str,                                              \
    unsigned base_integer_type value,                               \
    wstring_const_ref format_string )                               \
{                                                                   \
    return hidden::format(                                          \
        str, value, false,                                          \
        sizeof(value), format_string);                              \
}
// ******************************************************************

ALI_DEFINE_FORMAT_INTEGER(char)
ALI_DEFINE_FORMAT_INTEGER(short)
ALI_DEFINE_FORMAT_INTEGER(int)
ALI_DEFINE_FORMAT_INTEGER(long)
ALI_DEFINE_FORMAT_INTEGER(long long)

#undef  ALI_DEFINE_FORMAT_INTEGER

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    float value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    float value,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    double value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::wstring& format(
    ali::wstring& str,
    double value,
    wstring_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

namespace hidden
{

// ******************************************************************
bool printf_partition(
    string_const_ptr& prefix,
    string_const_ptr& value_format,
    string_const_ptr& suffix,
    string_const_ptr format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
bool printf_partition(
    wstring_const_ptr& prefix,
    wstring_const_ptr& value_format,
    wstring_const_ptr& suffix,
    wstring_const_ptr format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& printf_append_prefix(
    ali::string& str, string_const_ptr prefix );
// ******************************************************************
//  The prefix might contain %%{ which this function maps to %{.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::wstring& printf_append_prefix(
    ali::wstring& str, wstring_const_ptr prefix );
// ******************************************************************
//  The prefix might contain %%{ which this function maps to %{.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& printf(
    ali::string& str, string_const_ptr& format_string );
// ******************************************************************
//  We are short of parameters now, so if we encounter
//  a parameter placeholder we just output it verbatim.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::wstring& printf(
    ali::wstring& str, wstring_const_ptr& format_string );
// ******************************************************************
//  We are short of parameters now, so if we encounter
//  a parameter placeholder we just output it verbatim.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline ali::string& printf(
    ali::string& str,
    string_const_ptr& format_string,
    T const& value )
// ******************************************************************
{
    using ali::format;
    
    string_const_ptr prefix{}, value_format{};
    
    ALI_IF_DEBUG(bool const found =)
        printf_partition(
            prefix, value_format,
            format_string, format_string);

    ali_assert(((void) "Too many parameters to printf", found));
    
    printf_append_prefix(str, prefix);
    
    return format(str, value, *value_format);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline ali::wstring& printf(
    ali::wstring& str,
    wstring_const_ptr& format_string,
    T const& value )
// ******************************************************************
{
    using ali::format;
    
    wstring_const_ptr prefix{}, value_format{};
    
    ALI_IF_DEBUG(bool const found =)
        printf_partition(
            prefix, value_format,
            format_string, format_string);

    ali_assert(((void) "Too many parameters to printf", found));
    
    printf_append_prefix(str, prefix);
    
    return format(str, value, *value_format);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename... Ts>
inline ali::string& printf(
    ali::string& str,
    string_const_ptr& format_string,
    T const& head, Ts const&... tail )
// ******************************************************************
{
    printf(str, format_string, head);
    return printf(str, format_string, tail...);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, typename... Ts>
inline ali::wstring& printf(
    ali::wstring& str,
    wstring_const_ptr& format_string,
    T const& head, Ts const&... tail )
// ******************************************************************
{
    printf(str, format_string, head);
    return printf(str, format_string, tail...);
}

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
template <typename... Ts>
inline ali::string& printf_append(
    ali::string& str,
    string_const_ref format_string,
    Ts const&... params )
// ******************************************************************
{
    string_const_ptr ptr{format_string.pointer()};
    hidden::printf(str, ptr, params...);
    return hidden::printf(str, ptr);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename... Ts>
inline ali::wstring& printf_append(
    ali::wstring& str,
    wstring_const_ref format_string,
    Ts const&... params )
// ******************************************************************
{
    wstring_const_ptr ptr{format_string.pointer()};
    hidden::printf(str, ptr, params...);
    return hidden::printf(str, ptr);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename... Ts>
ali::string printf(
    string_const_ref format_string,
    Ts const&... params )
// ******************************************************************
{
    ali::string str{};
    printf_append(str, format_string, params...);
    return str;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename... Ts>
ali::wstring printf(
    wstring_const_ref format_string,
    Ts const&... params )
// ******************************************************************
{
    ali::wstring str{};
    printf_append(str, format_string, params...);
    return str;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
array<bool> test_printf( void );
// ******************************************************************

}   //  namespace ali
