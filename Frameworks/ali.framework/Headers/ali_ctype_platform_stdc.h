/*
 *  ali_ctype.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_debug.h"
#include "ali/ali_functor_types_common.h"
#include "ali/ali_wchar.h"
#include <ctype.h>
#include <wctype.h>

namespace ali
{

namespace functor_types
{

// ******************************************************************
struct to_lower_in_place;
// ******************************************************************

// ******************************************************************
struct to_upper_in_place;
// ******************************************************************

}   //  namespace functor_types

// ******************************************************************
extern functor_types::to_lower_in_place const to_lower_in_place;
// ******************************************************************

// ******************************************************************
extern functor_types::to_upper_in_place const to_upper_in_place;
// ******************************************************************

namespace functor_types
{

// ******************************************************************
#define ALI_DEFINE_CTYPE_FUNCTOR(what)                              \
struct is_##what /*: unary_predicate<is_##what>*/                   \
{                                                                   \
    bool operator()( unsigned char c ) const noexcept               \
    { return is##what(c) != 0; }                                    \
                                                                    \
    bool operator()( char c ) const noexcept                        \
    { return is##what(static_cast<unsigned char>(c)) != 0; }        \
                                                                    \
    bool operator()( ali::wchar c ) const noexcept                  \
    { return isw##what(c) != 0; }                                   \
}
// ******************************************************************

ALI_DEFINE_CTYPE_FUNCTOR(alpha);
ALI_DEFINE_CTYPE_FUNCTOR(upper);
ALI_DEFINE_CTYPE_FUNCTOR(lower);
ALI_DEFINE_CTYPE_FUNCTOR(digit);
ALI_DEFINE_CTYPE_FUNCTOR(xdigit);
ALI_DEFINE_CTYPE_FUNCTOR(space);
ALI_DEFINE_CTYPE_FUNCTOR(punct);
ALI_DEFINE_CTYPE_FUNCTOR(alnum);
ALI_DEFINE_CTYPE_FUNCTOR(print);
ALI_DEFINE_CTYPE_FUNCTOR(graph);
ALI_DEFINE_CTYPE_FUNCTOR(cntrl);

#undef  ALI_DEFINE_CTYPE_FUNCTOR

// ******************************************************************
/// \brief  Returns lower-case variant of the given character
/// or object.
///
/// \see
///         - \ref ali::to_lower
///         - \ref ali::to_lower_in_place
///
// ******************************************************************
struct to_lower
// ******************************************************************
{
        /// \brief  Returns lower-case variant of the given character.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             (   c is upper-case letter
        ///             &&  result is its lower-case variant)
        ///         ||  (   c is not upper-case letter
        ///             &&  result == c)
        ///
        /// \returns Lower-case variant of the given character `c`,
        /// provided it is an upper-case letter; `c` otherwise.
        ///
        /// \throws noexcept
        ///
    char operator()( char c ) const noexcept
    {
        return static_cast<char>(tolower(c));
    }

        /// \brief  Returns lower-case variant of the given character.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             (   c is upper-case letter
        ///             &&  result is its lower-case variant)
        ///         ||  (   c is not upper-case letter
        ///             &&  result == c)
        ///
        /// \returns Lower-case variant of the given character `c`,
        /// provided it is an upper-case letter; `c` otherwise.
        ///
        /// \throws noexcept
        ///
    wchar operator()( wchar c ) const noexcept
    {
        return static_cast<wchar>(towlower(c));
    }

        /// \brief  Returns lower-case variant of the given object.
        ///
        /// \tparam     T
        ///         Type of the object to transform.
        /// \param[in]  value
        ///         Object to transform.
        ///
        /// \pre
        ///
        ///             T is implicitly convertible to string_ref
        ///         ||  T is implicitly convertible to wstring_ref
        ///
        /// \post
        ///
        ///         foreach character in value &
        ///                 (   character is upper-case letter
        ///                 &&  corresponding character in result is its lower-case variant)
        ///             ||  (   character is not upper-case letter
        ///                 &&  corresponding character in result is its copy)
        ///
        /// \returns Lower-case variant of the given object.
        ///
        /// \throws noexcept <tt>(noexcept(T(T)))</tt>
        ///
    template <typename T>
    T operator()( T value ) const
    {
        return ali::to_lower_in_place(value);
    }
};

// ******************************************************************
/// \brief  Returns upper-case variant of the given character
/// or object.
///
/// \see
///         - \ref ali::to_upper
///         - \ref ali::to_upper_in_place
///
// ******************************************************************
struct to_upper
// ******************************************************************
{
        /// \brief  Returns upper-case variant of the given character.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             (   c is lower-case letter
        ///             &&  result is its upper-case variant)
        ///         ||  (   c is not lower-case letter
        ///             &&  result == c)
        ///
        /// \returns Upper-case variant of the given character `c`,
        /// provided it is a lower-case letter; `c` otherwise.
        ///
        /// \throws noexcept
        ///
    char operator()( char c ) const noexcept
    {
        return static_cast<char>(toupper(c));
    }

        /// \brief  Returns upper-case variant of the given character.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             (   c is lower-case letter
        ///             &&  result is its upper-case variant)
        ///         ||  (   c is not lower-case letter
        ///             &&  result == c)
        ///
        /// \returns Upper-case variant of the given character `c`,
        /// provided it is a lower-case letter; `c` otherwise.
        ///
        /// \throws noexcept
        ///
    wchar operator()( wchar c ) const noexcept
    {
        return static_cast<wchar>(towupper(c));
    }

        /// \brief  Returns upper-case variant of the given object.
        ///
        /// \tparam     T
        ///         Type of the object to transform.
        /// \param[in]  value
        ///         Object to transform.
        ///
        /// \pre
        ///
        ///             T is implicitly convertible to string_ref
        ///         ||  T is implicitly convertible to wstring_ref
        ///
        /// \post
        ///
        ///         foreach character in value &
        ///                 (   character is lower-case letter
        ///                 &&  corresponding character in result is its upper-case variant)
        ///             ||  (   character is not lower-case letter
        ///                 &&  corresponding character in result is its copy)
        ///
        /// \returns Upper-case variant of the given object.
        ///
        /// \throws noexcept <tt>(noexcept(T(T)))</tt>
        ///
    template <typename T>
    T operator()( T value ) const
    {
        return ali::to_upper_in_place(value);
    }
};

}   //  namespace functor_types

/*
// ******************************************************************
inline bool is_alpha( unsigned char c )
// ******************************************************************
{
    return isalpha(c) != 0;
}

// ******************************************************************
inline bool is_upper( unsigned char c )
// ******************************************************************
{
    return isupper(c) != 0;
}

// ******************************************************************
inline bool is_lower( unsigned char c )
// ******************************************************************
{
    return islower(c) != 0;
}

// ******************************************************************
inline bool is_digit( unsigned char c )
// ******************************************************************
{
    return isdigit(c) != 0;
}

// ******************************************************************
inline bool is_xdigit( unsigned char c )
// ******************************************************************
{
    return isxdigit(c) != 0;
}

// ******************************************************************
inline bool is_space( unsigned char c )
// ******************************************************************
{
    return isspace(c) != 0;
}

// ******************************************************************
inline bool is_punct( unsigned char c )
// ******************************************************************
{
    return ispunct(c) != 0;
}

// ******************************************************************
inline bool is_alnum( unsigned char c )
// ******************************************************************
{
    return isalnum(c) != 0;
}

// ******************************************************************
inline bool is_print( unsigned char c )
// ******************************************************************
{
    return isprint(c) != 0;
}

// ******************************************************************
inline bool is_graph( unsigned char c )
// ******************************************************************
{
    return isgraph(c) != 0;
}

// ******************************************************************
inline bool is_cntrl( unsigned char c )
// ******************************************************************
{
    return iscntrl(c) != 0;
}

// ******************************************************************
inline bool is_alpha( char c )
// ******************************************************************
{
    return is_alpha(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_upper( char c )
// ******************************************************************
{
    return is_upper(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_lower( char c )
// ******************************************************************
{
    return is_lower(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_digit( char c )
// ******************************************************************
{
    return is_digit(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_xdigit( char c )
// ******************************************************************
{
    return is_xdigit(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_space( char c )
// ******************************************************************
{
    return is_space(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_punct( char c )
// ******************************************************************
{
    return is_punct(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_alnum( char c )
// ******************************************************************
{
    return is_alnum(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_print( char c )
// ******************************************************************
{
    return is_print(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_graph( char c )
// ******************************************************************
{
    return is_graph(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_cntrl( char c )
// ******************************************************************
{
    return is_cntrl(static_cast<unsigned char>(c));
}

// ******************************************************************
inline bool is_alpha( ali_wchar c )
// ******************************************************************
{
    return iswalpha(c) != 0;
}

// ******************************************************************
inline bool is_upper( ali_wchar c )
// ******************************************************************
{
    return iswupper(c) != 0;
}

// ******************************************************************
inline bool is_lower( ali_wchar c )
// ******************************************************************
{
    return iswlower(c) != 0;
}

// ******************************************************************
inline bool is_digit( ali_wchar c )
// ******************************************************************
{
    return iswdigit(c) != 0;
}

// ******************************************************************
inline bool is_xdigit( ali_wchar c )
// ******************************************************************
{
    return iswxdigit(c) != 0;
}

// ******************************************************************
inline bool is_space( ali_wchar c )
// ******************************************************************
{
    return iswspace(c) != 0;
}

// ******************************************************************
inline bool is_punct( ali_wchar c )
// ******************************************************************
{
    return iswpunct(c) != 0;
}

// ******************************************************************
inline bool is_alnum( ali_wchar c )
// ******************************************************************
{
    return iswalnum(c) != 0;
}

// ******************************************************************
inline bool is_print( ali_wchar c )
// ******************************************************************
{
    return iswprint(c) != 0;
}

// ******************************************************************
inline bool is_graph( ali_wchar c )
// ******************************************************************
{
    return iswgraph(c) != 0;
}

// ******************************************************************
inline bool is_cntrl( ali_wchar c )
// ******************************************************************
{
    return iswcntrl(c) != 0;
}
*/

// ******************************************************************
extern functor_types::is_alpha const is_alpha;
extern functor_types::is_upper const is_upper;
extern functor_types::is_lower const is_lower;
extern functor_types::is_digit const is_digit;
extern functor_types::is_xdigit const is_xdigit;
extern functor_types::is_space const is_space;
extern functor_types::is_punct const is_punct;
extern functor_types::is_alnum const is_alnum;
extern functor_types::is_print const is_print;
extern functor_types::is_graph const is_graph;
extern functor_types::is_cntrl const is_cntrl;
// ******************************************************************

// ******************************************************************
/// \brief  Default instance of the \ref functor_types::to_lower
/// functor.
///
/// \copydetails functor_types::to_lower
///
/// \hideinitializer
///
// ******************************************************************
extern functor_types::to_lower const to_lower;
// ******************************************************************

// ******************************************************************
/// \brief  Default instance of the \ref functor_types::to_upper
/// functor.
///
/// \copydetails functor_types::to_upper
///
/// \hideinitializer
///
// ******************************************************************
extern functor_types::to_upper const to_upper;
// ******************************************************************

}   //  namespace ali
