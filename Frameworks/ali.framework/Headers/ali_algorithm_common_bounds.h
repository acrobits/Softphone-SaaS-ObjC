/*
 *  ali_algorithm_common_bounds.h
 *  ali Library
 *
 *  Copyright 2008-2017 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_utility.h"
#include "ali/ali_meta.h"
#include "ali/ali_debug.h"

namespace ali
{

// ******************************************************************
// ******************************************************************

// ******************************************************************
//  post:   result == count
//      ||  b <= at(result)     //  compare(b, at(result)) <= 0
// ******************************************************************
template <typename At, typename B, typename Comparator>
int generic_index_of_lower_bound(
    int count,
    At at,
    B const& b,
    Comparator compare )
// ******************************************************************
{
    static_assert(
        meta::comparator_returns_int<Comparator, decltype(at(0)), B>::result,
        "The comparator template parameter has changed from 'bool is_less(...)' to 'int compare(...)'.");
    
    ali_assert(0 <= count);
    
    int first = 0;

    while ( count > 0 )
    {
        int const count2 = count / 2;
        
        int mid = first + count2;

        bool const try_right_half
            = compare(at(mid), b) < 0;
        
        if ( try_right_half )
        {
            first = ++mid;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
    }

    return first;
}

// ******************************************************************
//  post:   result == count
//      ||  b <= at(result)     //  compare(b, at(result)) <= 0
// ******************************************************************
template <typename At, typename B>
int generic_index_of_lower_bound(
    int count,
    At at,
    B const& b )
// ******************************************************************
{
    return generic_index_of_lower_bound(count, at, b, default_comparator{});
}

// ******************************************************************
//  post:   result == size
//      ||  b <= at(result)     //  compare(b, at(result)) <= 0
// ******************************************************************
template <int size, typename A, typename B, typename Comparator>
inline int index_of_lower_bound(
    A const (&arr)[size],
    B const& b,
    Comparator compare )
// ******************************************************************
{
    return generic_index_of_lower_bound(
        size, [&arr](int i) -> A const& { return arr[i]; }, b, compare);
}

// ******************************************************************
//  post:   result == size
//      ||  b <= at(result)     //  compare(b, at(result)) <= 0
// ******************************************************************
template <int size, typename A, typename B>
inline int index_of_lower_bound(
    A const (&arr)[size],
    B const& b )
// ******************************************************************
{
    return generic_index_of_lower_bound(
        size, [&arr](int i) -> A const& { return arr[i]; }, b);
}

// ******************************************************************
//  post:   result == count
//      ||  b < at(result)      //  compare(b, at(result)) < 0
// ******************************************************************
template <typename At, typename B, typename Comparator>
int generic_index_of_upper_bound(
    int count,
    At at,
    B const& b,
    Comparator compare )
// ******************************************************************
{
    static_assert(
        meta::comparator_returns_int<Comparator, B, decltype(at(0))>::result,
        "The comparator template parameter has changed from 'bool is_less(...)' to 'int compare(...)'.");
    
    ali_assert(0 <= count);

    int first = 0;

    while ( count > 0 )
    {
        int const count2 = count / 2;
        
        int mid = first + count2;

        bool const try_right_half
            = compare(b, at(mid)) >= 0;

        if ( try_right_half )
        {
            first = ++mid;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
    }

    return first;
}

// ******************************************************************
//  post:   result == count
//      ||  b < at(result)      //  compare(b, at(result)) < 0
// ******************************************************************
template <typename At, typename B>
int generic_index_of_upper_bound(
    int count,
    At at,
    B const& b )
// ******************************************************************
{
    return generic_index_of_upper_bound(count, at, b, default_comparator{});
}

// ******************************************************************
//  post:   result == size
//      ||  b < at(result)      //  compare(b, at(result)) < 0
// ******************************************************************
template <int size, typename A, typename B, typename Comparator>
int index_of_upper_bound(
    A const (&arr)[size],
    B const& b,
    Comparator compare )
// ******************************************************************
{
    return generic_index_of_upper_bound(
        size, [&arr](int i) -> A const& { return arr[i]; }, b, compare);
}

// ******************************************************************
//  post:   result == size
//      ||  b < at(result)      //  compare(b, at(result)) < 0
// ******************************************************************
template <int size, typename A, typename B>
int index_of_upper_bound(
    A const (&arr)[size],
    B const& b )
// ******************************************************************
{
    return generic_index_of_upper_bound(
        size, [&arr](int i) -> A const& { return arr[i]; }, b);
}

}
