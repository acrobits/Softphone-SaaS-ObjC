#pragma once
#include "ali/ali_array_utils.h"
#include "ali/ali_ctype.h"

namespace ali
{

//// ******************************************************************
//struct nocase_less
//// ******************************************************************
//{
//    bool operator()(
//        string_const_ref a,
//        string_const_ref b ) const
//    {
//        return a.nocase_compare(b) < 0;
//    }
//
//    bool operator()(
//        wstring_const_ref a,
//        wstring_const_ref b ) const
//    {
//        return a.nocase_compare(b) < 0;
//    }
//
//    friend bool operator==( nocase_less, nocase_less ) { return true; }
//    friend bool operator!=( nocase_less, nocase_less ) { return false; }
//
//    void swap( nocase_less& ) {}
//    friend void swap( nocase_less&, nocase_less& ) {}
//};

// ******************************************************************
struct nocase_comparator
// ******************************************************************
{
    int operator()(
        string_const_ref a,
        string_const_ref b ) const noexcept
    {
        return ali_sanoex(a.nocase_compare(b));
    }

    int operator()(
        wstring_const_ref a,
        wstring_const_ref b ) const noexcept
    {
        return ali_sanoex(a.nocase_compare(b));
    }

    friend bool operator==(
        nocase_comparator,
        nocase_comparator ) noexcept
    {
        return true;
    }

    friend bool operator!=(
        nocase_comparator,
        nocase_comparator ) noexcept
    {
        return false;
    }

    void swap( nocase_comparator& ) noexcept {}
    
    friend void swap(
        nocase_comparator&,
        nocase_comparator& ) noexcept {}
};


// ******************************************************************
// ******************************************************************

namespace functor_types
{

// ******************************************************************
/// \brief  Determines whether the given string begins and ends
/// with double or single quotes.
///
/// \param[in]  str
///         String to test.
///
/// \post
///
///             result
///                 ==  str.size() > 1
///                 &&  str.front() == str.back()
///                 &&  (   str.front() == '"'
///                     ||  str.front() == '\'')
///
/// \retval true
///         The given string has at least two characters, the first
///         and the last being the same quote character.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
/// \see    \ref ali::is_quoted
///
// ******************************************************************
struct is_quoted
// ******************************************************************
{
        /// \brief  Overload accepting \ref string_const_ref.
        ///
    bool operator()( string_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  (   str.front() == '"'
                ||  str.front() == '\''));
    }

        /// \brief  Overload accepting \ref wstring_const_ref.
        ///
    bool operator()( wstring_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  (   str.front() == L'"'
                ||  str.front() == L'\''));
    }
};

// ******************************************************************
/// \brief  Determines whether the given string begins and ends
/// with the specified quote character.
///
/// \param[in]  str
///         String to test.
///
/// \post
///
///             result
///                 ==  str.size() > 1
///                 &&  str.front() == str.back()
///                 &&  str.front() == this->quote
///
/// \retval true
///         The given string has at least two characters, the first
///         and the last being the specified quote character.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
/// \see
///         - \ref ali::make_is_quoted(char)
///         - \ref ali::make_is_quoted(wchar)
///
// ******************************************************************
struct is_quoted_1
// ******************************************************************
{
        /// \brief  Initializes this functor with the given quote
        /// character.
        ///
        /// \param[in]  quote
        ///         Quote character to test for.
        ///
        /// \post   <tt>this->quote == static_cast<wchar>(quote)</tt>
        ///
        /// \throws noexcept
        ///
    is_quoted_1( char quote ) noexcept
    :   quote{static_cast<wchar>(quote)}
    {}

        /// \brief  Initializes this functor with the given quote
        /// character.
        ///
        /// \param[in]  quote
        ///         Quote character to test for.
        ///
        /// \post   <tt>this->quote == quote</tt>
        ///
        /// \throws noexcept
        ///
    is_quoted_1( wchar quote ) noexcept
    :   quote{quote}
    {}

        /// \brief  Overload accepting \ref string_const_ref.
        ///
    bool operator()( string_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  static_cast<wchar>(str.front()) == quote);
    }

        /// \brief  Overload accepting \ref wstring_const_ref.
        ///
    bool operator()( wstring_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  str.front() == quote);
    }

    wchar   quote;
        ///<        Quote character to test for.
};

// ******************************************************************
/// \brief  Determines whether the given string begins and ends
/// with the specified quote characters.
///
/// \param[in]  str
///         String to test.
///
/// \post
///
///             result
///                 ==  str.size() > 1
///                 &&  str.front() == this->lquote
///                 &&  str.back() == this->rquote
///
/// \retval true
///         The given string has at least two characters, the first
///         and the last being the specified quote characters.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
/// \see
///         - \ref ali::make_is_quoted(char, char)
///         - \ref ali::make_is_quoted(wchar, wchar)
///
// ******************************************************************
struct is_quoted_2
// ******************************************************************
{
        /// \brief  Initializes this functor with the given quote
        /// characters.
        ///
        /// \param[in]  lquote
        ///         Left quote character to test for.
        /// \param[in]  rquote
        ///         Right quote character to test for.
        ///
        ///
        /// \post   
        ///
        ///             this->lquote == static_cast<wchar>(lquote)
        ///         &&  this->rquote == static_cast<wchar>(rquote)
        ///
        /// \throws noexcept
        ///
    is_quoted_2( char lquote, char rquote ) noexcept
    :   lquote{static_cast<wchar>(lquote)},
        rquote{static_cast<wchar>(rquote)}
    {}

        /// \brief  Initializes this functor with the given quote
        /// character.
        ///
        /// \param[in]  quote
        ///         Quote character to test for.
        ///
        /// \post   <tt>this->quote == quote</tt>
        ///
        /// \throws noexcept
        ///
    is_quoted_2( wchar lquote, wchar rquote ) noexcept
    :   lquote{lquote},
        rquote{rquote}
    {}

        /// \brief  Overload accepting \ref string_const_ref.
        ///
    bool operator()( string_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  static_cast<wchar>(str.front()) == lquote
            &&  static_cast<wchar>(str.back()) == rquote);
    }

        /// \brief  Overload accepting \ref wstring_const_ref.
        ///
    bool operator()( wstring_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == lquote
            &&  str.back() == rquote);
    }

    wchar   lquote;
        ///<        Left quote character to test for.
    wchar   rquote;
        ///<        Right quote character to test for.
};

// ******************************************************************
/// \brief  Determines whether the given string begins and ends
/// with one of the specified quote characters.
///
/// \param[in]  str
///         String to test.
///
/// \post
///
///             result
///                 ==  str.size() > 1
///                 &&  str.front() == str.back()
///                 &&  this->quotes->contains(str.front())
///
/// \retval true
///         The given string has at least two characters, the first
///         and the last being one of the specified quote characters.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
/// \see    \ref ali::make_is_quoted(string_const_ref)
///
// ******************************************************************
struct is_quoted_n
// ******************************************************************
{
        /// \brief  Initializes this functor with the given set of
        /// quote characters.
        ///
        /// \param[in]  quotes
        ///         Set of quote characters to test for.
        ///         The set must remain valid for the entire lifetime
        ///         of this instance.
        ///
        /// \post   <tt>this->quotes == quotes.pointer()</tt>
        ///
        /// \throws noexcept
        ///
    is_quoted_n( string_const_ref quotes ) noexcept
    :   quotes{quotes.pointer()}
    {}

        /// \brief  Overload accepting \ref string_const_ref.
        ///
    bool operator()( string_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  quotes->contains(str.front()));
    }

        /// \brief  Overload accepting \ref wstring_const_ref.
        ///
    bool operator()( wstring_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  str.front() <= static_cast<wchar>(
                    meta::integer::max_value<unsigned char>::result)
            &&  quotes->contains(static_cast<unsigned char>(str.front())));
    }

    string_const_ptr    quotes;
        ///<        Set of quote characters to test for.
};

// ******************************************************************
/// \brief  Determines whether the given string begins and ends
/// with one of the specified quote characters.
///
/// \param[in]  str
///         String to test.
///
/// \post
///
///             result
///                 ==  str.size() > 1
///                 &&  str.front() == str.back()
///                 &&  this->quotes->contains(str.front())
///
/// \retval true
///         The given string has at least two characters, the first
///         and the last being one of the specified quote characters.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
/// \see    \ref ali::make_is_quoted(wstring_const_ref)
///
// ******************************************************************
struct is_quoted_nw
// ******************************************************************
{
        /// \copydoc is_quoted_n::is_quoted_n(string_const_ref)
        ///
    is_quoted_nw( wstring_const_ref quotes ) noexcept
    :   quotes{quotes.pointer()}
    {}

        /// \brief  Overload accepting \ref string_const_ref.
        ///
    bool operator()( string_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  quotes->contains(str.front()));
    }

        /// \brief  Overload accepting \ref wstring_const_ref.
        ///
    bool operator()( wstring_const_ref str ) const noexcept
    {
        return  ali_sanoex(
                str.size() > 1
            &&  str.front() == str.back()
            &&  quotes->contains(str.front()));
    }

    wstring_const_ptr    quotes;
        ///<        Set of quote characters to test for.
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief  Transforms the given character or object to its
/// lower-case variant.
///
/// \see
///         - \ref ali::to_lower
///         - \ref ali::to_lower_in_place
///
// ******************************************************************
struct to_lower_in_place
// ******************************************************************
{
        /// \brief  Transforms the given character to its lower-case
        /// variant.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             &result == &c
        ///         &&  (   (   pre(c) is upper-case letter
        ///                 &&  result is its lower-case variant)
        ///             ||  (   pre(c) is not upper-case letter
        ///                 &&  result == pre(c)))
        ///
        /// \returns `c`
        ///
        /// \throws noexcept
        ///
    char& operator()( char& c ) const noexcept
    {
        return ali_sanoex(c = ali::to_lower(c));
    }

        /// \brief  Transforms the given character to its lower-case
        /// variant.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             &result == &c
        ///         &&  (   (   pre(c) is upper-case letter
        ///                 &&  result is its lower-case variant)
        ///             ||  (   pre(c) is not upper-case letter
        ///                 &&  result == pre(c)))
        ///
        /// \returns `c`
        ///
        /// \throws noexcept
        ///
    wchar& operator()( wchar& c ) const noexcept
    {
        return ali_sanoex(c = ali::to_lower(c));
    }

        /// \brief  Transforms the given object to its lower-case
        /// variant.
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
        ///             &result == &value
        ///         &&  foreach character in pre(value) &
        ///                     (   character is upper-case letter
        ///                     &&  corresponding character in result is its lower-case variant)
        ///                 ||  (   character is not upper-case letter
        ///                     &&  corresponding character in result is not changed in any way)
        ///
        /// \returns `value`
        ///
        /// \throws noexcept
        ///
    template <typename T>
    T& operator()( T& value ) const noexcept(
        noexcept(this->transform(value)))
    {
        this->transform(value);
        return value;
    }

    string_ref operator()( string_ref value ) const noexcept
    {
        ali_sanoex(this->transform(value));
        return value;
    }

    wstring_ref operator()( wstring_ref value ) const noexcept
    {
        ali_sanoex(this->transform(value));
        return value;
    }

private:    //  Methods
    void transform( string_ref str ) const noexcept
    {
        ali_sanoex(str.for_each(*this));
    }

    void transform( wstring_ref str ) const noexcept
    {
        ali_sanoex(str.for_each(*this));
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief  Transforms the given character or object to its
/// upper-case variant.
///
/// \see
///         - \ref ali::to_upper
///         - \ref ali::to_upper_in_place
///
// ******************************************************************
struct to_upper_in_place
// ******************************************************************
{
        /// \brief  Transforms the given character to its upper-case
        /// variant.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             &result == &c
        ///         &&  (   (   pre(c) is lower-case letter
        ///                 &&  result is its upper-case variant)
        ///             ||  (   pre(c) is not lower-case letter
        ///                 &&  result == pre(c)))
        ///
        /// \returns `c`
        ///
        /// \throws noexcept
        ///
    char& operator()( char& c ) const noexcept
    {
        return ali_sanoex(c = ali::to_upper(c));
    }

        /// \brief  Transforms the given character to its upper-case
        /// variant.
        ///
        /// \param[in]  c
        ///         Character to transform.
        ///
        /// \post
        ///
        ///             &result == &c
        ///         &&  (   (   pre(c) is lower-case letter
        ///                 &&  result is its upper-case variant)
        ///             ||  (   pre(c) is not lower-case letter
        ///                 &&  result == pre(c)))
        ///
        /// \returns `c`
        ///
        /// \throws noexcept
        ///
    wchar& operator()( wchar& c ) const noexcept
    {
        return ali_sanoex(c = ali::to_upper(c));
    }

        /// \brief  Transforms the given object to its upper-case
        /// variant.
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
        ///             &result == &value
        ///         &&  foreach character in pre(value) &
        ///                     (   character is lower-case letter
        ///                     &&  corresponding character in result is its upper-case variant)
        ///                 ||  (   character is not lower-case letter
        ///                     &&  corresponding character in result is not changed in any way)
        ///
        /// \returns `value`
        ///
        /// \throws noexcept
        ///
    template <typename T>
    T& operator()( T& value ) const noexcept(
        noexcept(this->transform(value)))
    {
        this->transform(value);
        return value;
    }

    string_ref operator()( string_ref value ) const noexcept
    {
        ali_sanoex(this->transform(value));
        return value;
    }

    wstring_ref operator()( wstring_ref value ) const noexcept
    {
        ali_sanoex(this->transform(value));
        return value;
    }

private:    //  Methods
    void transform( string_ref str ) const noexcept
    {
        ali_sanoex(str.for_each(*this));
    }

    void transform( wstring_ref str ) const noexcept
    {
        ali_sanoex(str.for_each(*this));
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct nocase_compare
// ******************************************************************
{
    int operator()(
        string_const_ref a,
        string_const_ref b ) const noexcept
    {
        return ali_sanoex(a.nocase_compare(b));
    }

    int operator()(
        wstring_const_ref a,
        wstring_const_ref b ) const noexcept
    {
        return ali_sanoex(a.nocase_compare(b));
    }

    friend bool operator==(
        nocase_compare, nocase_compare ) noexcept
    {
        return true;
    }

    friend bool operator!=(
        nocase_compare, nocase_compare ) noexcept
    {
        return false;
    }

    void swap( nocase_compare& ) noexcept {}
    
    friend void swap(
        nocase_compare&,
        nocase_compare& ) noexcept {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct compare_using_user_locale
// ******************************************************************
{
    int operator()(
        string_const_ref a,
        string_const_ref b ) const;
            //  Implemented in ali_str_utils_platform.cpp.

    int operator()(
        wstring_const_ref a,
        wstring_const_ref b ) const;
            //  Implemented in ali_str_utils_platform.cpp.

    friend bool operator==(
        compare_using_user_locale,
        compare_using_user_locale ) noexcept
    {
        return true;
    }

    friend bool operator!=(
        compare_using_user_locale,
        compare_using_user_locale ) noexcept
    {
        return false;
    }

    void swap( compare_using_user_locale& ) noexcept {}

    friend void swap(
        compare_using_user_locale&,
        compare_using_user_locale& ) noexcept {}
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct nocase_compare_using_user_locale
// ******************************************************************
{
    int operator()(
        string_const_ref a,
        string_const_ref b ) const;
            //  Implemented in ali_str_utils_platform.cpp.

    int operator()(
        wstring_const_ref a,
        wstring_const_ref b ) const;
            //  Implemented in ali_str_utils_platform.cpp.

    friend bool operator==(
        nocase_compare_using_user_locale,
        nocase_compare_using_user_locale ) noexcept
    {
        return true;
    }

    friend bool operator!=(
        nocase_compare_using_user_locale,
        nocase_compare_using_user_locale ) noexcept
    {
        return false;
    }

    void swap( nocase_compare_using_user_locale& ) noexcept {}
    
    friend void swap(
        nocase_compare_using_user_locale&,
        nocase_compare_using_user_locale& ) noexcept {}
};

// ******************************************************************
// ******************************************************************

}   //  namespace functor_types

// ******************************************************************
/// \brief  Default instance of the \ref functor_types::is_quoted
/// functor.
///
/// \copydetails functor_types::is_quoted
///
/// \hideinitializer
///
// ******************************************************************
extern functor_types::is_quoted const is_quoted;
// ******************************************************************

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_1
/// functor initialized with the specified quote character.
///
/// \see    \ref functor_types::is_quoted_1::is_quoted_1(char)
///
// ******************************************************************
inline functor_types::is_quoted_1 make_is_quoted( char quote ) noexcept
// ******************************************************************
{
    return ali_sanoex(functor_types::is_quoted_1{quote});
}

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_1
/// functor initialized with the specified quote character.
///
/// \see    \ref functor_types::is_quoted_1::is_quoted_1(wchar)
///
// ******************************************************************
inline functor_types::is_quoted_1 make_is_quoted( wchar quote ) noexcept
// ******************************************************************
{
    return ali_sanoex(functor_types::is_quoted_1{quote});
}

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_n
/// functor initialized with the specified set of quote characters.
///
/// \see    \ref functor_types::is_quoted_n::is_quoted_n(string_const_ref)
///
// ******************************************************************
inline functor_types::is_quoted_n make_is_quoted(
    string_const_ref quotes ) noexcept
// ******************************************************************
{
    return ali_sanoex(functor_types::is_quoted_n{quotes});
}

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_nw
/// functor initialized with the specified set of quote characters.
///
/// \see    \ref functor_types::is_quoted_nw::is_quoted_nw(wstring_const_ref)
///
// ******************************************************************
inline functor_types::is_quoted_nw make_is_quoted(
    wstring_const_ref quotes ) noexcept
// ******************************************************************
{
    return ali_sanoex(functor_types::is_quoted_nw{quotes});
}

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_2
/// functor initialized with the specified quote characters.
///
/// \see    \ref functor_types::is_quoted_2::is_quoted_2(char, char)
///
// ******************************************************************
inline functor_types::is_quoted_2 make_is_quoted(
    char lquote, char rquote ) noexcept
// ******************************************************************
{
    return ali_sanoex((functor_types::is_quoted_2{lquote, rquote}));
}

// ******************************************************************
/// \brief  Returns instance of the \ref functor_types::is_quoted_2
/// functor initialized with the specified quote characters.
///
/// \see    \ref functor_types::is_quoted_2::is_quoted_2(wchar, wchar)
///
// ******************************************************************
inline functor_types::is_quoted_2 make_is_quoted(
    wchar lquote, wchar rquote ) noexcept
// ******************************************************************
{
    return ali_sanoex((functor_types::is_quoted_2{lquote, rquote}));
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief  Default instance of the \ref functor_types::to_lower_in_place
/// functor.
///
/// \copydetails functor_types::to_lower_in_place
///
/// \hideinitializer
///
// ******************************************************************
extern functor_types::to_lower_in_place const to_lower_in_place;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief  Default instance of the \ref functor_types::to_upper_in_place
/// functor.
///
/// \copydetails functor_types::to_upper_in_place
///
/// \hideinitializer
///
// ******************************************************************
extern functor_types::to_upper_in_place const to_upper_in_place;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
extern functor_types::nocase_compare const nocase_compare;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
extern functor_types::compare_using_user_locale const
    compare_using_user_locale;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
extern functor_types::nocase_compare_using_user_locale const
    nocase_compare_using_user_locale;
// ******************************************************************

}   //  namespace ali

