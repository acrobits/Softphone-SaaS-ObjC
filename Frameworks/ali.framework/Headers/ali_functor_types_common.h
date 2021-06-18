#pragma once
#include "ali/ali_meta.h"

namespace ali
{

namespace functor_types
{
/*
// ******************************************************************
// Base classes allowing compile-time polymorphism.
// (Poor man's concepts.)
// ******************************************************************

// ******************************************************************
template <typename final_predicate>
struct unary_predicate
// ******************************************************************
//  The final_predicate class must:
//  derive from:
//      unary_predicate
//  and define:
//      bool operator()( T ) const;
//      OR bool operator()( T const& ) const;
//      //  for some T
// ******************************************************************
{
    final_predicate const& downcast( void ) const
    {
        return static_cast<final_predicate const&>(*this);
    }
};

// ******************************************************************
// Composition of unary predicates.
// ******************************************************************

namespace detail
{

// ******************************************************************
template <typename predicate>
struct not_unary_predicate
    : unary_predicate<not_unary_predicate<predicate> >,
        private predicate
// ******************************************************************
{
    not_unary_predicate( predicate p )
    :   predicate(p)
    {}

    predicate const& base_predicate( void ) const
    {
        return *this;
    }

    template <typename T>
    typename meta::enable_if<
        !meta::is_fundamental<T>::result,
        bool>::type operator()( T const& t ) const
    {
        return !predicate::operator()(t);
    }

    template <typename T>
    typename meta::enable_if<
        meta::is_fundamental<T>::result,
        bool>::type operator()( T t ) const
    {
        return !predicate::operator()(t);
    }
};

// ******************************************************************
template <typename predicate1, typename predicate2>
struct and_unary_predicate
    : unary_predicate<and_unary_predicate<predicate1, predicate2> >,
        private predicate1, private predicate2
// ******************************************************************
{
    and_unary_predicate( predicate1 p1, predicate2 p2 )
    :   predicate1(p1),
        predicate2(p2)
    {}

    template <typename T>
    typename meta::enable_if<
        !meta::is_fundamental<T>::result,
        bool>::type operator()( T const& t ) const
    {
        return  predicate1::operator()(t)
            &&  predicate2::operator()(t);
    }

    template <typename T>
    typename meta::enable_if<
        meta::is_fundamental<T>::result,
        bool>::type operator()( T t ) const
    {
        return  predicate1::operator()(t)
            &&  predicate2::operator()(t);
    }
};

// ******************************************************************
template <typename predicate1, typename predicate2>
struct or_unary_predicate
    : unary_predicate<or_unary_predicate<predicate1, predicate2> >,
        private predicate1, private predicate2
// ******************************************************************
{
    or_unary_predicate( predicate1 p1, predicate2 p2 )
    :   predicate1(p1),
        predicate2(p2)
    {}

    template <typename T>
    typename meta::enable_if<
        !meta::is_fundamental<T>::result,
        bool>::type operator()( T const& t ) const
    {
        return  predicate1::operator()(t)
            ||  predicate2::operator()(t);
    }

    template <typename T>
    typename meta::enable_if<
        meta::is_fundamental<T>::result,
        bool>::type operator()( T t ) const
    {
        return  predicate1::operator()(t)
            ||  predicate2::operator()(t);
    }
};

}   //  namespace detail

// ******************************************************************
template <typename final_predicate>
inline detail::not_unary_predicate<final_predicate> operator!(
    unary_predicate<final_predicate> const& p )
// ******************************************************************
//  Add not<...>
// ******************************************************************
{
    return detail::not_unary_predicate<
        final_predicate>(p.downcast());
}

// ******************************************************************
template <typename final_predicate>
inline final_predicate operator!(
    unary_predicate<detail::not_unary_predicate<final_predicate> > const& p )
// ******************************************************************
//  Remove not<...>
// ******************************************************************
{
    return p.downcast().base_predicate();
}

// ******************************************************************
template <typename final_predicate1, typename final_predicate2>
inline detail::and_unary_predicate<
    final_predicate1, final_predicate2> operator&&(
        unary_predicate<final_predicate1> const& p1,
        unary_predicate<final_predicate2> const& p2 )
// ******************************************************************
{
    return detail::and_unary_predicate<
        final_predicate1,
        final_predicate2>(
            p1.downcast(),
            p2.downcast());
}

// ******************************************************************
template <typename final_predicate1, typename final_predicate2>
inline detail::or_unary_predicate<
    final_predicate1, final_predicate2> operator||(
        unary_predicate<final_predicate1> const& p1,
        unary_predicate<final_predicate2> const& p2 )
// ******************************************************************
{
    return detail::or_unary_predicate<
        final_predicate1,
        final_predicate2>(
            p1.downcast(),
            p2.downcast());
}
*/
// ******************************************************************
// Common functor types
// ******************************************************************

// ******************************************************************
struct identity
// ******************************************************************
{
    template <typename T>
    T const& operator()( T const& t ) const noexcept
    {
        return t;
    }
};

}   //  namespace functor_types

// ******************************************************************
functor_types::identity const identity = {};
// ******************************************************************

}   //  namespace ali
