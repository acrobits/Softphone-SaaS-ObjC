/*
 *  ali_str_split.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array.h"
#include "ali/ali_meta.h"
#include "ali/ali_tstring.h"

namespace ali
{

namespace str
{

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<string_const_ptr>& split(
    array<string_const_ptr>& entries,
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<string>& split(
    array<string>& entries,
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   result.size() <= max_count
inline array<string> split(
    string_const_ref str,
    string_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true )
// ******************************************************************
{
    array<string> entries{};
    split(entries, str, separators, max_count, trim);
    return entries;
}

// ******************************************************************
int split(
    array_ref<string_const_ptr> entries,
    string_const_ref str,
    string_const_ref separators,
    bool trim = true );
// ******************************************************************

// ******************************************************************
template <int max_count>
inline int split(
    string (&entries)[max_count],
    string_const_ref str,
    string_const_ref separators,
    bool trim = true )
// ******************************************************************
{
    string_const_ptr ptr_entries[max_count]{};
    
    int const result{split(ptr_entries, str, separators, trim)};

    for ( int i{result}; i != 0; )
        --i, entries[i] = *ptr_entries[i];

    return result;
}

// ******************************************************************
int split_lines(
    array_ref<string_const_ptr> lines,
    string_const_ref str,
    string_const_ref eol = "\r\n"_s );
// ******************************************************************

// ******************************************************************
array<string> split_lines(
    string_const_ref str,
    string_const_ref eol = "\r\n"_s );
// ******************************************************************

// ******************************************************************
bool get_line(
    string_const_ptr& line,
    string_const_ptr& lines,
    string_const_ref eol = "\r\n"_s );
// ******************************************************************

// ******************************************************************
bool get_line_robust(
    string_const_ptr& line,
    string_const_ptr& lines );
// ******************************************************************

}   //  namespace str

namespace wstr
{

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<wstring_const_ptr>& split(
    array<wstring_const_ptr>& entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   entries.size() - pre(entries).size() <= max_count
array<wstring>& split(
    array<wstring>& entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true );
// ******************************************************************

// ******************************************************************
//  post:   result.size() <= max_count
inline array<wstring> split(
    wstring_const_ref str,
    wstring_const_ref separators,
    int max_count = meta::integer::max_value<int>::result,
    bool trim = true )
// ******************************************************************
{
    array<wstring> entries{};
    split(entries, str, separators, max_count, trim);
    return entries;
}

// ******************************************************************
int split(
    array_ref<wstring_const_ptr> entries,
    wstring_const_ref str,
    wstring_const_ref separators,
    bool trim = true );
// ******************************************************************

// ******************************************************************
template <int max_count>
inline int split(
    wstring (&entries)[max_count],
    wstring_const_ref str,
    wstring_const_ref separators,
    bool trim = true )
// ******************************************************************
{
    wstring_const_ptr ptr_entries[max_count]{};
    
    int const result{split(ptr_entries, str, separators, trim)};

    for ( int i{result}; i != 0; )
        --i, entries[i] = *ptr_entries[i];

    return result;
}

// ******************************************************************
int split_lines(
    array_ref<wstring_const_ptr> lines,
    wstring_const_ref str,
    wstring_const_ref eol = L"\r\n"_s );
// ******************************************************************

// ******************************************************************
array<wstring> split_lines(
    wstring_const_ref str,
    wstring_const_ref eol = L"\r\n"_s );
// ******************************************************************

// ******************************************************************
bool get_line(
    wstring_const_ptr& line,
    wstring_const_ptr& lines,
    wstring_const_ref eol = L"\r\n"_s );
// ******************************************************************

// ******************************************************************
bool get_line_robust(
    wstring_const_ptr& line,
    wstring_const_ptr& lines );
// ******************************************************************

}   //  namespace wstr

}   //  namespace ali

