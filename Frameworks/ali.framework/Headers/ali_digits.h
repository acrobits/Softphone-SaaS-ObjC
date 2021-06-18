/*
 *  ali_digits.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_assert.h"
#include "ali/ali_wchar.h"
#include "ali/ali_array_utils.h"

namespace ali
{

extern char const* const XDIGITS_UPPER;
extern char const* const XDIGITS_LOWER;
extern wchar const* const XDIGITSW_UPPER;
extern wchar const* const XDIGITSW_LOWER;

// ******************************************************************
//  pre:    2 <= radix <= 16
template <class Int>
bool parse_digit( Int& digit, char c, int radix )
// ******************************************************************
{
    ali_assert(2 <= radix);
    ali_assert(radix <= 16);
    
    switch ( c )
    {
    case '0':   digit = 0; return true;
    case '1':   digit = 1; return true;
    case '2':   return radix > 2 ? (digit = 2, true) : false;
    case '3':   return radix > 3 ? (digit = 3, true) : false;
    case '4':   return radix > 4 ? (digit = 4, true) : false;
    case '5':   return radix > 5 ? (digit = 5, true) : false;
    case '6':   return radix > 6 ? (digit = 6, true) : false;
    case '7':   return radix > 7 ? (digit = 7, true) : false;
    case '8':   return radix > 8 ? (digit = 8, true) : false;
    case '9':   return radix > 9 ? (digit = 9, true) : false;
    case 'a':
    case 'A':   return radix > 10 ? (digit = 10, true) : false;
    case 'b':
    case 'B':   return radix > 11 ? (digit = 11, true) : false;
    case 'c':
    case 'C':   return radix > 12 ? (digit = 12, true) : false;
    case 'd':
    case 'D':   return radix > 13 ? (digit = 13, true) : false;
    case 'e':
    case 'E':   return radix > 14 ? (digit = 14, true) : false;
    case 'f':
    case 'F':   return radix > 15 ? (digit = 15, true) : false;
    }
    
    return false;
}

// ******************************************************************
//  pre:    2 <= radix <= 16
template <class Int>
bool parse_digit( Int& digit, wchar c, int radix )
// ******************************************************************
{
    ali_assert(2 <= radix);
    ali_assert(radix <= 16);
    
    switch ( c )
    {
    case L'0':   digit = 0; return true;
    case L'1':   digit = 1; return true;
    case L'2':   return radix > 2 ? (digit = 2, true) : false;
    case L'3':   return radix > 3 ? (digit = 3, true) : false;
    case L'4':   return radix > 4 ? (digit = 4, true) : false;
    case L'5':   return radix > 5 ? (digit = 5, true) : false;
    case L'6':   return radix > 6 ? (digit = 6, true) : false;
    case L'7':   return radix > 7 ? (digit = 7, true) : false;
    case L'8':   return radix > 8 ? (digit = 8, true) : false;
    case L'9':   return radix > 9 ? (digit = 9, true) : false;
    case L'a':
    case L'A':   return radix > 10 ? (digit = 10, true) : false;
    case L'b':
    case L'B':   return radix > 11 ? (digit = 11, true) : false;
    case L'c':
    case L'C':   return radix > 12 ? (digit = 12, true) : false;
    case L'd':
    case L'D':   return radix > 13 ? (digit = 13, true) : false;
    case L'e':
    case L'E':   return radix > 14 ? (digit = 14, true) : false;
    case L'f':
    case L'F':   return radix > 15 ? (digit = 15, true) : false;
    }
    
    return false;
}

// ******************************************************************
template <typename Int, int n>
bool parse_n_dec_digits(
    Int (&digits)[n],
    string_const_ref str,
    int& pos )
// ******************************************************************
{
    int idx{};
    
    for ( ; idx != n && pos != str.size(); ++idx, ++pos )
        if ( !parse_digit(digits[idx], str[pos], 10) )
            return false;
    
    return idx == n;
}

// ******************************************************************
template <typename Int>
bool parse_two_dec_digits(
    Int& value,
    string_const_ref str,
    int& pos )
// ******************************************************************
{
    Int digits[2]{};
    
    bool const success{
        parse_n_dec_digits(digits, str, pos)};
    
    if ( success )
        value = digits[0] * 10 + digits[1];
    
    return success;
}

// ******************************************************************
template <typename Int>
bool parse_four_dec_digits(
    Int& value,
    string_const_ref str,
    int& pos )
// ******************************************************************
{
    Int digits[4]{};
    
    bool const success{
        parse_n_dec_digits(digits, str, pos)};
    
    if ( success )
        value = ((digits[0] * 10
                    + digits[1]) * 10
                        + digits[2]) * 10
                            + digits[3];

    return success;
}

}   //  namespace ali

