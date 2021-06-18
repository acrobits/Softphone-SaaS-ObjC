#pragma once

#include "ali/ali_string_map_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_string.h"
#include "ali/ali_utility.h"

namespace ali
{

namespace hidden
{

// ******************************************************************
struct common_string_map_entry
// ******************************************************************
{
        /// \brief  Default constructor.
        /// Constructs empty string map entry.
        ///
        /// \post   <tt>this->name.is_empty() && this->value.is_empty()</tt>
        ///
        /// \throws noexcept
        ///
    common_string_map_entry( void ) {}

        /// \brief  Initializes this string map entry with the
        /// specified name.
        ///
        /// \param[in]  name
        ///         String to initialize the `name` data member with.
        ///
        /// \post   <tt>this->name == name && this->value.is_empty()</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit common_string_map_entry(
        xml::string const& name )
    :   name{name}
    {}

        /// \brief  Initializes this string map entry with the
        /// specified name and value.
        ///
        /// \param[in]  name
        ///         String to initialize the `name` data member with.
        /// \param[in]  value
        ///         String to initialize the `value` data member with.
        ///
        /// \post   <tt>this->name == name && this->value == value</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    common_string_map_entry(
        xml::string const& name,
        xml::string const& value )
    :   name{name},
        value{value}
    {}

        /// \brief  Determines whether the two given names are
        /// equal (m.p. equivalent) using the specified comparator.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///         Two name strings to compare.
        /// \param[in]  compare
        ///         Comparator to be used for the comparison.
        ///
        /// \pre    `compare` is a binary function over strings
        ///         implementing a strict weak ordering.
        ///         compare(a, b) < 0     iff a < b
        ///         compare(a, b) == 0    iff a == b
        ///         compare(a, b) > 0     iff a > b
        ///
        /// \post   <tt>result == (compare(a, b) == 0)</tt>
        ///
        /// \retval true
        ///         According to the specified comparator, the two
        ///         given names are equivalent.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(compare(a,b)))</tt>
        ///
    template <typename comparator_type>
    static bool are_names_equal(
        string_const_ref a,
        string_const_ref b,
        comparator_type const& compare )
    {
        static_assert(
            meta::comparator_returns_int<comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        return compare(a, b) == 0;
    }

        /// \brief  Determines whether the `name` data member of this
        /// string map entry is equal (m.p. equivalent) to the
        /// given string using the specified comparator.
        ///
        /// \param[in]  b
        ///         String to compare the `name` data member with.
        /// \param[in]  compare
        ///         Comparator to be used for the comparison.
        ///
        /// \pre    `compare` is a binary function over strings
        ///         implementing a strict weak ordering.
        ///         compare(a, b) < 0     iff a < b
        ///         compare(a, b) == 0    iff a == b
        ///         compare(a, b) > 0     iff a > b
        ///
        /// \post   <tt>result == this->are_names_equal(this->name, b, comparator)</tt>
        ///
        /// \retval true
        ///         According to the specified comparator, the `name`
        ///         data member is equivalent with the given string.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(comparator(a,b)))</tt>
        ///
    template <typename comparator_type>
    bool is_name_equal_to(
        string_const_ref b,
        comparator_type const& compare ) const
    {
        static_assert(
            meta::comparator_returns_int<comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        return are_names_equal(this->name, b, compare);
    }

    common_string_map_entry& set_value( xml::string const& another_value )
    {
        this->value = another_value;
        return *this;
    }

    common_string_map_entry& set_value( string_const_ref another_value )
    {
        this->value = another_value;
        return *this;
    }

    common_string_map_entry& set_value( short another_value );
    common_string_map_entry& set_value( unsigned short another_value );
    common_string_map_entry& set_value( int another_value );
    common_string_map_entry& set_value( unsigned int another_value );
    common_string_map_entry& set_value( long another_value );
    common_string_map_entry& set_value( unsigned long another_value );
    common_string_map_entry& set_value( long long another_value );
    common_string_map_entry& set_value( unsigned long long another_value );
    common_string_map_entry& set_value( float another_value );
    common_string_map_entry& set_value( double another_value );

        /// \brief  Updates this string map entry with a name
        /// and value extracted from the given string.
        ///
        /// The accepted format is:
        ///
        ///     str = wsp* name wsp* separator wsp* value wsp*
        ///     name = not_separator_or_wsp [not_separator* not_separator_or_wsp]
        ///     value = not_wsp [any* not_wsp]
        ///     any = {c | true}    //  any character
        ///     wsp = {c | ali::is_space(c)}
        ///     not_wsp = {c | !ali::is_space(c)}
        ///     not_separator = {c | c != separator}
        ///     not_separator_or_wsp = {c | c != separator && !ali::is_space(c)}
        ///
        /// All leading and trailing whitespace (wsp) characters
        /// are stripped from `name` and `value`.
        ///
        /// \tparam separator
        ///     The separator character.
        ///
        /// \param[in]  str
        ///     String to split into name value pair.
        ///
        /// \post
        ///
        ///             (   result
        ///
        ///             &&  !this->name.is_empty()
        ///             &&  this->name == name
        ///             &&  !ali::is_space(this->name.front())
        ///             &&  !ali::is_space(this->name.back())
        ///             &&  forall i in set {0, ..., this->name.size() - 1} &
        ///                     this->name[i] != separator
        ///
        ///             &&  !this->value.is_empty()
        ///             &&  this->value == value
        ///             &&  !ali::is_space(this->value.front())
        ///             &&  !ali::is_space(this->value.back()))
        ///
        ///         ||  (   !result
        ///             &&  *this == *pre(this))
        ///
        /// \retval true
        ///     This string map entry was updated with name and value
        ///     pair extracted from the given string.
        /// \retval false
        ///     The given string could not be split into name and
        ///     value pair. This string map entry was not modified
        ///     in any way.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <char separator>
    bool parse( string_const_ref str )
    {
        return this->private_parse(str, separator, false);
    }

        /// \brief  Updates this string map entry with a name
        /// and value extracted from the given string.
        ///
        /// The accepted format is:
        ///
        ///     str = wsp* name wsp* [separator wsp* [value wsp*]]
        ///     name = not_separator_or_wsp [not_separator* not_separator_or_wsp]
        ///     value = not_wsp [any* not_wsp]
        ///     any = {c | true}    //  any character
        ///     wsp = {c | ali::is_space(c)}
        ///     not_wsp = {c | !ali::is_space(c)}
        ///     not_separator = {c | c != separator}
        ///     not_separator_or_wsp = {c | c != separator && !ali::is_space(c)}
        ///
        /// All leading and trailing whitespace (wsp) characters
        /// are stripped from `name` and `value`.
        ///
        /// \tparam separator
        ///     The separator character.
        ///
        /// \param[in]  str
        ///     String to split into name value pair.
        ///
        /// \post
        ///
        ///             (   result
        ///
        ///             &&  !this->name.is_empty()
        ///             &&  this->name == name
        ///             &&  !ali::is_space(this->name.front())
        ///             &&  !ali::is_space(this->name.back())
        ///             &&  forall i in set {0, ..., this->name.size() - 1} &
        ///                     this->name[i] != separator
        ///
        ///             &&  (   (   str doesn't contain value
        ///                     &&  this->value.is_empty())
        ///
        ///                 ||  (   str contains value
        ///                     &&  this->value == value
        ///                     &&  !ali::is_space(this->value.front())
        ///                     &&  !ali::is_space(this->value.back()))))
        ///
        ///         ||  (   !result
        ///             &&  *this == *pre(this))
        ///
        /// \retval true
        ///     This string map entry was updated with name and value
        ///     pair extracted from the given string.
        /// \retval false
        ///     The given string could not be split into name and
        ///     value pair. This string map entry was not modified
        ///     in any way.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <char separator>
    bool parse_accepting_empty_value( string_const_ref str )
    {
        return this->private_parse(str, separator, true);
    }

        /// \brief  Calls this->parse<'='>(str).
        ///
    bool parse( string_const_ref str )
    {
        return this->parse<'='>(str);
    }

        /// \brief  Calls this->parse_accepting_empty_value<'='>(str).
        ///
    bool parse_accepting_empty_value( string_const_ref str )
    {
        return this->parse_accepting_empty_value<'='>(str);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the `parse` method of the output object.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        ///
        /// \pre    `T` defines `parse` method accepting
        ///         \ref ali::string and returning `bool`.
        ///
        /// \post   <tt>result == t.parse(this->value) && post(t.parse(this->value))</tt>
        ///
        /// \returns Value returned by the `parse` method of `t`.
        ///
        /// \throws noexcept <tt>(noexcept(t.parse(this->value)))</tt>
        ///
    template <typename T>
    bool parse_value( T& t ) const
    {
        return t.parse(this->value);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified function with the output object
        /// and the `value` data member as arguments.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     U
        ///         Type of the string object the function accepts.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        /// \param[in]  parse
        ///         Parsing function to be invoked.
        ///
        /// \pre    `ali::string` object is implicitly convertible
        ///         to an object of type `U`
        ///
        /// \post   <tt>result == parse(t, this->value) && post(parse(t, this->value))</tt>
        ///
        /// \returns Value returned by the parse function.
        ///
        /// \throws noexcept <tt>(noexcept(parse(t, this->value)))</tt>
        ///
    template <typename T, typename U>
    bool parse_value( T& t, bool (*parse)( T&, U ) ) const
    {
        return parse(t, this->value);
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified method of the output object.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     U
        ///         Type that defines the specified method. (Might
        ///         be a base class of `T`.)
        /// \tparam     V
        ///         Type of the string object the specified method
        ///         accepts.
        /// \param[out] t
        ///         Object of type T to be initialized from a string
        ///         value stored in the `value` data member of this
        ///         string map entry.
        /// \param[in]  parse
        ///         Parsing method to be invoked.
        ///
        /// \pre    `ali::string` object is implicitly convertible
        ///         to an object of type `V`
        ///
        /// \post   <tt>result == (t.*parse)(this->value) && post((t.*parse)(this->value))</tt>
        ///
        /// \returns Value returned by the specified method of `t`.
        ///
        /// \throws noexcept <tt>(noexcept((t.*parse)(this->value)))</tt>
        ///
    template <typename T, typename U, typename V>
    bool parse_value( T& t, bool (U::*parse)( V ) ) const
    {
        return (t.*parse)(this->value);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     short int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `short int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `short int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( short& t ) const
    {
        return this->private_parse_value_as_signed_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned short int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned short int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned short
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned short& t ) const
    {
        return this->private_parse_value_as_unsigned_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `int` data
        ///         type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( int& t ) const
    {
        return this->private_parse_value_as_signed_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned int& t ) const
    {
        return this->private_parse_value_as_unsigned_integer(t);
    }

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `long int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned long
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// a signed decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     a signed integer that fits into the
        ///                     long long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of a signed integer that fits into
        ///         the `long long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of a signed integer or
        ///         that integer doesn't fit into the `long long int`
        ///         data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( long long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// an unsigned decimal integer.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal textual representation of
        ///                     an unsigned integer that fits into
        ///                     the unsigned long int data type
        ///                 &&  t is equal to the value of that integer)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal textual
        ///         representation of an unsigned integer that fits
        ///         into the `unsigned long int` data type.
        ///         The value of that integer was stored into the `t`
        ///         parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         textual representation of an unsigned integer or
        ///         that integer doesn't fit into the `unsigned long
        ///         int` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( unsigned long long& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// a decimal real number in the fixed point notation.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal fixed point textual
        ///                     representation of a real number that
        ///                     fits into the float data type
        ///                 &&  t is equal to "a reasonable
        ///                     approximation" of that real number)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal fixed
        ///         point textual representation of a real number
        ///         that fits into the `float` data type.
        ///         "A reasonable approximation" of that real number
        ///         was stored into the `t` parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         fixed point textual representation of a real
        ///         number or that number doesn't fit into the
        ///         `float` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( float& t ) const;

        /// \brief  Tries to parse the `value` data member as
        /// a decimal real number in the fixed point notation.
        ///
        /// \param[out]  t
        ///         Variable to store the parsed value into.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  the value data member contains
        ///                     decimal fixed point textual
        ///                     representation of a real number that
        ///                     fits into the double data type
        ///                 &&  t is equal to "a reasonable
        ///                     approximation" of that real number)
        ///             ||  (   !result
        ///                 &&  t is not changed in any way)
        ///
        /// \retval true
        ///         The `value` data member contains decimal fixed
        ///         point textual representation of a real number
        ///         that fits into the `double` data type.
        ///         "A reasonable approximation" of that real number
        ///         was stored into the `t` parameter.
        /// \retval false
        ///         The `value` data member doesn't contain decimal
        ///         fixed point textual representation of a real
        ///         number or that number doesn't fit into the
        ///         `double` data type.
        ///         The `t` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse_value( double& t ) const;

        /// \brief  Tries to parse the `value` data member by
        /// invoking the `parse` method of the output object,
        /// returning either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \param[in]  default_value
        ///         Object of type T to be returned by this method
        ///         in case the parsing fails.
        ///
        /// \pre
        ///
        ///                 T has default constructor
        ///             &&  T has copy constructor
        ///             &&  T has assignment operator
        ///             &&  let t is object type T
        ///                 in  pre(this->parse_value(t))
        ///
        /// \post
        ///
        ///             let t is object type T
        ///             in  post(this->parse_value(t))
        ///                 &&  (   (this->parse_value(t) && result == t)
        ///                     ||  (!this->parse_value(t) && result == default_value))
        ///
        /// \returns Either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \throws noexcept <tt>(noexcept(T()) && noexcept(T(T))
        /// && noexcept(this->parse_value(T)) && noexcept(T = T))</tt>
        ///
    template <typename T>
    T get_value( T const& default_value = T() ) const
    {
        T temp;
        if ( !this->parse_value(temp) )
            temp = default_value;
        return temp;
    }

        /// \brief  Tries to parse the `value` data member by
        /// invoking the specified parsing function or method,
        /// returning either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \tparam     T
        ///         Type of the output object.
        /// \tparam     parse_function
        ///         Type of the parsing function or method.
        /// \param[in]  parse
        ///         Parsing function or method to be invoked.
        /// \param[in]  default_value
        ///         Object of type T to be returned by this method
        ///         in case the parsing fails.
        ///
        /// \pre
        ///
        ///                 T has default constructor
        ///             &&  T has copy constructor
        ///             &&  T has assignment operator
        ///             &&  let t is object of type T
        ///                 in  pre(this->parse_value(t, parse))
        ///
        /// \post
        ///
        ///             let t is object of type T
        ///             in  post(this->parse_value(t, parse))
        ///                 &&  (   (this->parse_value(t, parse) && result == t)
        ///                     ||  (!this->parse_value(t, parse) && result == default_value))
        ///
        /// \returns Either the `value` initialized object or the
        /// specified default one if the parsing fails.
        ///
        /// \throws noexcept <tt>(noexcept(T()) && noexcept(T(T))
        /// && noexcept(this->parse_value(T, parse)) && noexcept(T = T))</tt>
        ///
    template <typename T, typename parse_function>
    T get_value(
        parse_function parse,
        T const& default_value = T() ) const
    {
        T temp;
        if ( !this->parse_value(temp, parse) )
            temp = default_value;
        return temp;
    }

        /// \brief  Applies the given in-place transform to the
        /// `value` data member.
        ///
        /// \param[in]  t
        ///         In-place string tranform to apply.
        ///
        /// \pre    `t` is in-place transform accepting
        ///         \ref ali::string objects.
        ///
        /// \post   <tt>post(t(this->value))</tt>
        ///
        /// \returns `*this`;
        ///
        /// \throws noexcept <tt>(noexcept(t(ali::string)))</tt>
        ///
    template <typename string_in_place_transform>
    common_string_map_entry& transform_value_in_place(
        string_in_place_transform t )
    {
        t(this->value);
        return *this;
    }

        /// \brief  Swaps contents of this string map entry with
        /// another one.
        ///
        /// \param[in,out]  b
        ///         Another string map entry to swap content with.
        ///
        /// \post
        ///
        ///                 this->name == pre(b).name
        ///             &&  this->value == pre(b).value
        ///             &&  this->is_less() == pre(b).is_less()
        ///             &&  pre(this)->name == b.name
        ///             &&  pre(this)->value == b.value
        ///             &&  pre(this)->is_less() == b.is_less()
        ///
        /// \throws noexcept
        ///
    void swap( common_string_map_entry& b )
    {
        using ali::swap;
        swap(this->name, b.name);
        swap(this->value, b.value);
    }

    void assert_invariant( void ) const
    {
        this->name.assert_invariant();
        this->value.assert_invariant();
    }


    xml::string name;
        ///<        Holds the name of this string map entry.

    xml::string value;
        ///<        Holds the value of this string map entry.

private:    //  Methods
    bool private_parse(
        string_const_ref str,
        char separator,
        bool accept_empty_value )
    {
        int const separator_pos = str.index_of_first(separator);

        if ( !accept_empty_value && separator_pos == str.size() )
            return false;

        string_const_ref name_ref
            = str.ref_left(separator_pos).trim();

        string_const_ref value_ref
            = str.ref_right(
                ali::mini(
                    str.size(),
                    separator_pos + 1)).trim();

        if ( name_ref.is_empty() )
            return false;

        if ( !accept_empty_value && value_ref.is_empty() )
            return false;

        this->name = name_ref;

        this->value = value_ref;

        return true;
    }

    template <typename target_type>
    bool private_parse_value_as_signed_integer( target_type& t ) const
    {
        long const min_value = meta::integer::min_value<target_type>::result;
        long const max_value = meta::integer::max_value<target_type>::result;

        long temp =  0;

        bool const is_valid
            =   this->parse_value(temp)
            &&  min_value <= temp
            &&  temp <= max_value;

        if ( is_valid )
            t = static_cast<target_type>(temp);

        return is_valid;
    }

    template <typename target_type>
    bool private_parse_value_as_unsigned_integer( target_type& t ) const
    {
        unsigned long const max_value
            = meta::integer::max_value<target_type>::result;

        unsigned long temp =  0;

        bool const is_valid
            =   this->parse_value(temp)
            &&  temp <= max_value;

        if ( is_valid )
            t = static_cast<target_type>(temp);

        return is_valid;
    }
};

// ******************************************************************
// ******************************************************************

}   //  namespace hidden

// ******************************************************************
/// \brief  Aggregates two \ref string data members `name` and
/// `value`. Also stores default comparator for comparing the
/// `name` data members.
///
/// \tparam comparator_type
///         Type of the default comparator to be used for comparing
///         the `name` data members.
///
/// \pre    Object of the `comparator_type` type is a binary function
///         over strings implementing a strict weak ordering.
///         comparator_type{}(a, b) < 0     iff a < b
///         comparator_type{}(a, b) == 0    iff a == b
///         comparator_type{}(a, b) > 0     iff a > b
///
// ******************************************************************
template <typename comparator_type>
struct ALI_ATTRIBUTE_EMPTY_BASES string_map_entry
// ******************************************************************
    :   hidden::common_string_map_entry,
        private comparator_type
{
    static_assert(
        meta::comparator_returns_int<comparator_type, xml::string>::result,
        "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
        "See ali::default_comparator for sample implementation.");

    using hidden::common_string_map_entry::common_string_map_entry;

    using hidden::common_string_map_entry::are_names_equal;

        /// \brief  Determines whether the two given names are
        /// equal (m.p. equivalent) using the default comparator.
        ///
        /// See \ref are_names_equal(ali::string_const_ref,ali::string_const_ref,other_comparator_type const&).
        ///
        /// \post   <tt>result == this->are_names_equal(a, b, this->comparator())</tt>
        ///
    bool are_names_equal(
        string_const_ref a,
        string_const_ref b ) const
    {
        return are_names_equal(a, b, this->comparator());
    }

    using hidden::common_string_map_entry::is_name_equal_to;

        /// \brief  Determines whether the `name` data member of this
        /// string map entry is equal (m.p. equivalent) to the
        /// given string using the default comparator.
        ///
        /// \param[in]  b
        ///         String to compare the `name` data member with.
        ///
        /// \post   <tt>result == this->are_names_equal(this->name, b, this->comparator())</tt>
        ///
        /// \retval true
        ///         According to the default comparator, the `name`
        ///         data member is equivalent with the given string.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(comparator_type()(a,b)))</tt>
        ///
    bool is_name_equal_to(
        string_const_ref b ) const
    {
        return are_names_equal(this->name, b, this->comparator());
    }

        /// \brief  Determines whether the two given string map
        /// entries are equal.
        ///
        /// Uses the default comparator for the `name` data members
        /// and `operator==` for the default comparator and `value`
        /// data members.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two string map entries to compare.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  a.comparator() == b.comparator()
        ///                 &&  a.is_name_equal_to(b.name)
        ///                 &&  a.value == b.value
        ///
        /// \retval true
        ///         The given two string map entries are equal.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(comparator_type == comparator_type)
        /// && noexcept(comparator_type()(a,b))) && noexcept(ali::string == ali::string)</tt>
        ///
    friend bool operator==(
        string_map_entry const& a,
        string_map_entry const& b )
    {
        return  a.comparator() == b.comparator()
            &&  a.is_name_equal_to(b.name)
            &&  a.value == b.value;
    }

        /// \brief  Determines whether the two given string map
        /// entries differ.
        ///
        /// Uses the default comparator for the `name` data members
        /// and `operator==` for the default comparator and `value`
        /// data members.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two string map entries to compare.
        ///
        /// \post   <tt>result == !(a == b)</tt>
        ///
        /// \retval true
        ///         The given two string map entries differ.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(a == b))</tt>
        ///
    friend bool operator!=(
        string_map_entry const& a,
        string_map_entry const& b )
    {
        return !(a == b);
    }

        /// \brief  Swaps contents of this string map entry with
        /// another one.
        ///
        /// \param[in,out]  b
        ///         Another string map entry to swap content with.
        ///
        /// \post
        ///
        ///                 this->name == pre(b).name
        ///             &&  this->value == pre(b).value
        ///             &&  this->is_less() == pre(b).is_less()
        ///             &&  pre(this)->name == b.name
        ///             &&  pre(this)->value == b.value
        ///             &&  pre(this)->is_less() == b.is_less()
        ///
        /// \throws noexcept
        ///
    void swap( string_map_entry& b )
    {
        using ali::swap;
        hidden::common_string_map_entry::swap(b);
        swap(
            static_cast<comparator_type&>(*this),
            static_cast<comparator_type&>(b));
    }

        /// \brief  See \ref string_map_entry::swap.
        ///
    friend void swap(
        string_map_entry& a,
        string_map_entry& b )
    {
        a.swap(b);
    }

        /// \brief  Returns the default comparator used for
        /// comparing the `name` data members.
        ///
        /// \returns The default comparator used for comparing the
        /// `name` data members.
        ///
        /// \throws noexcept
        ///
    comparator_type const& comparator( void ) const
    {
        return *this;
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename comparator_type>
class small_string_map : private comparator_type
// ******************************************************************
{
    static_assert(
        meta::comparator_returns_int<comparator_type, xml::string>::result,
        "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
        "See ali::default_comparator for sample implementation.");

public:     //  Typedefs
    typedef string_map_entry<comparator_type>       entry;

public:     //  Class
    class const_iterator : public iterator_base<
        random_access_iterator_tag, entry>
    {
	public:
		const_iterator( void ) {}

        const_iterator(
            array_const_ref<ali::auto_ptr<entry>> entries,
            int index = 0 )
        :   _index{index},
            _entries{entries.pointer()}
        {
            ali_assert(0 <= _index);
            ali_assert(_index <= _entries->size());
        }

        entry const& operator*( void ) const
        {
            return *_entries->at(_index);
        }

        const_iterator& operator++( void )
		{
            ali_assert(_index < _entries->size());
            ++_index;
            return *this;
        }

        const_iterator operator++( int )
		{
            const_iterator const t{*this};
            ++*this;
            return t;
        }

		const_iterator& operator+=( int d )
		{
            //  0 <= _index + d <= _entries->size()
            ali_assert(-_index <= d);
            ali_assert(d <= _entries->size() - _index);
            _index += d;
            return *this;
        }

        friend const_iterator operator+( const_iterator it, int d )
		{
            it += d;
            return it;
        }

        const_iterator& operator--( void )
		{
            ali_assert(_index > 0);
            --_index;
		    return *this;
        }

        const_iterator operator--( int )
		{
            const_iterator const t{*this};
			--*this;
			return t;
        }

        const_iterator& operator-=( int d )
		{
            //  0 <= _index - d <= _entries->size()
            ali_assert(-_index <= -d);
            ali_assert(-d <= _entries->size() - _index);
            _index -= d;
            return *this;
        }

        friend const_iterator operator-( const_iterator it, int d )
		{
            it -= d;
            return it;
        }

        friend int operator-( const_iterator a, const_iterator b )
        {
            ali_assert(a._entries->data() == b._entries->data());
            return a._index - b._index;
        }

        friend bool operator==( const_iterator a, const_iterator b )
        {
            ali_assert(a._entries->data() == b._entries->data());
            return a._index == b._index;
        }

		friend bool operator!=( const_iterator a, const_iterator b )
        {
            return !(a == b);
        }

    protected:  //  Data members
        int                                     _index{};
		array_const_ptr<ali::auto_ptr<entry>>   _entries{};
    };

public:     //  Methods
    small_string_map( void ) {}
    
    small_string_map( small_string_map const& b )
    {
        this->_entries.reserve(b._entries.size());
        
        for ( int i{}; i != b._entries.size(); ++i )
            this->_entries.push_back(new_auto_ptr<
                entry>(*b._entries[i]));
    }
    
    small_string_map( small_string_map&& b )
    {
        this->swap(b);
    }

    ~small_string_map( void )
    {
        ALI_IF_DEBUG(this->assert_invariant());
    }

    small_string_map& operator=( small_string_map b )
    {
        this->swap(b);
        return *this;
    }

    bool operator==( small_string_map const& b ) const
    {
        if ( this->_entries.size() != b._entries.size() )
            return false;
        else for ( int i = this->_entries.size(); i != 0; )
            if ( --i, *this->_entries[i] != *b._entries[i] )
                return false;

        return true;
    }

    bool operator!=( small_string_map const& b ) const
    {
        return !this->operator==(b);
    }

    bool is_empty( void ) const
    {
        return this->_entries.is_empty();
    }

    int size( void ) const
    {
        return this->_entries.size();
    }

    entry& operator[]( string_const_ref name )
    {
        int const idx = this->index_of(name);

        if ( idx == this->_entries.size() )
            this->_entries.push_back(
                new_auto_ptr<entry>(name));

        return *this->_entries[idx];
    }

    entry const& operator[]( string_const_ref name ) const
    {
        if ( entry const* const e = this->find(name) )
            return *e;

        return this->empty_entry();
    }

    entry& operator[]( int pos )
    {
        return *this->_entries[pos];
    }

    entry const& operator[]( int pos ) const
    {
        return *this->_entries[pos];
    }

    entry& at( int pos )
    {
        return *this->_entries.at(pos);
    }

    entry const& at( int pos ) const
    {
        return *this->_entries.at(pos);
    }

    small_string_map& set( entry const& b )
    {
        if ( entry* const e = this->find(b.name) )
            e->value = b.value;
        else
            this->_entries.push_back(
                new_auto_ptr<entry>(b));

        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        xml::string const& value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        string_const_ref value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        short value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned short value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        int value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned int value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        long long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        unsigned long long value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        float value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& set(
        string_const_ref name,
        double value )
    {
        this->operator[](name).set_value(value);
        return *this;
    }

    small_string_map& erase( string_const_ref name )
    {
        int const idx = this->index_of(name);

        if ( idx != this->_entries.size() )
            this->_entries.erase(idx);

        return *this;
    }

    small_string_map& erase( int pos, int n = 1 )
    {
        this->_entries.erase(pos, n);

        return *this;
    }

    small_string_map& erase( void )
    {
        this->_entries.erase();

        return *this;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return this->_entries.erase_if(
            [p] ( ali::auto_ptr<entry> const& element )
            { return p(static_cast<entry const&>(*element)); });
    }

    struct this_method_returns_size_if_the_name_was_not_found
    {
        this_method_returns_size_if_the_name_was_not_found( int value )
        :   value(value)
        {}

        int value;
    };

    template <typename other_comparator_type>
    int index_of(
        string_const_ref name,
        other_comparator_type const& compare ) const
    {
        static_assert(
            meta::comparator_returns_int<other_comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        int idx = 0;

        for ( ; idx != this->_entries.size(); ++idx )
            if ( this->_entries[idx]->is_name_equal_to(name, compare) )
                break;

        return idx;
    }

    int index_of( string_const_ref name ) const
    {
        return this->index_of(name, this->comparator());
    }

    template <typename other_comparator_type>
    bool contains(
        string_const_ref name,
        other_comparator_type const& compare ) const
    {
        static_assert(
            meta::comparator_returns_int<other_comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        return this->index_of(name, compare) != this->size();
    }

    bool contains( string_const_ref name ) const
    {
        return this->contains(name, this->comparator());
    }

    template <typename other_comparator_type>
    entry const* find(
        string_const_ref name,
        other_comparator_type const& compare ) const
    {
        static_assert(
            meta::comparator_returns_int<other_comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        int const idx = this->index_of(name, compare);

        if ( idx != this->_entries.size() )
            return this->_entries[idx].get();

        return nullptr;
    }

    template <typename other_comparator_type>
    entry* find(
        string_const_ref name,
        other_comparator_type const& compare )
    {
        static_assert(
            meta::comparator_returns_int<other_comparator_type, xml::string>::result,
            "The comparator parameter has changed from 'bool is_less(string,string)' to 'int compare(string,string)'.\n"
            "See ali::default_comparator for sample implementation.");

        int const idx = this->index_of(name, compare);

        if ( idx != this->_entries.size() )
            return this->_entries[idx].get();

        return nullptr;
    }

    entry const* find( string_const_ref name ) const
    {
        return this->find(name, this->comparator());
    }

    entry* find( string_const_ref name )
    {
        return this->find(name, this->comparator());
    }

    xml::string const* find_value( string_const_ref name ) const
    {
        if ( entry const* const e = this->find(name) )
            return &e->value;
        else
            return nullptr;
    }

    xml::string* find_value( string_const_ref name )
    {
        if ( entry* const e = this->find(name) )
            return &e->value;
        else
            return nullptr;
    }

    template <
        char name_value_separator,
        char entry_separator>
    bool parse( string_const_ref str )
    {
        small_string_map temp;

        string_const_ptr ptr = str.pointer();

        while ( !ptr->is_empty() )
        {
            int const end = ptr->index_of_first_unquoted(entry_separator);

            temp._entries.push_back(new_auto_ptr<entry>());

            bool const success
                = temp._entries.back()->template parse<
                    name_value_separator>(
                        *ptr->pointer_left(end));

            if ( !success )
                return false;

            ptr += ali::mini(ptr->size(), end + 1);

        }

        this->swap(temp);

        return true;
    }

    template <
        char name_value_separator,
        char entry_separator>
    bool parse_accepting_empty_values( string_const_ref str )
    {
        small_string_map temp;

        string_const_ptr ptr = str.pointer();

        while ( !ptr->is_empty() )
        {
            int const end = ptr->index_of_first_unquoted(entry_separator);

            temp._entries.push_back(new_auto_ptr<entry>());

            bool const success
                = temp._entries.back()
                    ->template parse_accepting_empty_value<
                        name_value_separator>(
                            *ptr->pointer_left(end));

            if ( !success )
                return false;

            ptr += ali::mini(ptr->size(), end + 1);

        }

        this->swap(temp);

        return true;
    }

    bool parse( string_const_ref str )
    {
        return this->parse<'=', ';'>(str);
    }

    bool parse_accepting_empty_values( string_const_ref str )
    {
        return this->parse_accepting_empty_values<'=', ';'>(str);
    }

    template <char name_value_separator>
    bool parse_lines( string_const_ref str )
    {
        small_string_map temp;

        auto const crlf = "\r\n"_s;
            //  This is used as a set.

        string_const_ptr ptr = str.pointer();

        while ( ptr = ptr->pointer_right(
                    ptr->index_of_first_not_of(crlf)),
                        !ptr->is_empty() )
        {
            string_const_ptr line
                = ptr->pointer_left(
                    ptr->index_of_first_of(crlf));

            temp._entries.push_back(new_auto_ptr<entry>());

            bool const success
                = temp._entries.back()->template parse<
                    name_value_separator>(*line);

            if ( !success )
                return false;

            ptr += line->size();
        }

        this->swap(temp);

        return true;
    }

    template <char name_value_separator>
    bool parse_lines_accepting_empty_values( string_const_ref str )
    {
        small_string_map temp;

        auto const crlf = "\r\n"_s;
            //  This is used as a set.

        string_const_ptr ptr = str.pointer();

        while ( ptr = ptr->pointer_right(
                    ptr->index_of_first_not_of(crlf)),
                        !ptr->is_empty() )
        {
            string_const_ptr line
                = ptr->pointer_left(
                    ptr->index_of_first_of(crlf));

            temp._entries.push_back(new_auto_ptr<entry>());

            bool const success
                = temp._entries.back()
                    ->template parse_accepting_empty_value<
                        name_value_separator>(*line);

            if ( !success )
                return false;

            ptr += line->size();
        }

        this->swap(temp);

        return true;
    }

    bool parse_lines( string_const_ref str )
    {
        return this->parse_lines<'='>(str);
    }

    bool parse_lines_accepting_empty_values( string_const_ref str )
    {
        return this->parse_lines_accepting_empty_values<'='>(str);
    }

    template <typename T>
    bool parse_value( int i, T& t ) const
    {
        return  0 <= i && i < this->_entries.size()
            &&  this->_entries[i]->parse_value(t);
    }

    template <typename T, typename parse_function>
    bool parse_value( int i, T& t, parse_function parse ) const
    {
        return  0 <= i && i < this->_entries.size()
            &&  this->_entries[i]->parse_value(t, parse);
    }

    template <typename T>
    bool parse_value(
        string_const_ref name,
        T& t ) const
    {
        return this->parse_value(this->index_of(name), t);
    }

    template <typename T, typename parse_function>
    bool parse_value(
        string_const_ref name,
        T& t, parse_function parse ) const
    {
        return this->parse_value(this->index_of(name), t, parse);
    }

    template <typename T>
    bool parse_value_accepting_missing( int i, T& t ) const
    {
        return  i < 0 || this->_entries.size() <= i
            ||  this->_entries[i]->parse_value(t);
    }

    template <typename T, typename parse_function>
    bool parse_value_accepting_missing(
        int i, T& t, parse_function parse ) const
    {
        return  i < 0 || this->_entries.size() <= i
            ||  this->_entries[i]->parse_value(t, parse);
    }

    template <typename T>
    bool parse_value_accepting_missing(
        string_const_ref name,
        T& t ) const
    {
        return this->parse_value_accepting_missing(
            this->index_of(name), t);
    }

    template <typename T, typename parse_function>
    bool parse_value_accepting_missing(
        string_const_ref name,
        T& t, parse_function parse ) const
    {
        return this->parse_value_accepting_missing(
            this->index_of(name), t, parse);
    }

    xml::string get_value(
        int i,
        xml::string const& default_value ) const
    {
        return 0 <= i && i < this->_entries.size()
            ? this->_entries[i]->value
            : default_value;
    }

    xml::string get_value(
        int i,
        string_const_ref default_value = string_const_ref{} ) const
    {
        return 0 <= i && i < this->_entries.size()
            ? this->_entries[i]->value
            : xml::string{default_value};
    }

    template <typename T>
    T get_non_string_value( int i, T const& default_value ) const
    {
        return 0 <= i && i < this->_entries.size()
            ? this->_entries[i]->get_value(default_value)
            : default_value;
    }

    template <typename T, typename parse_function>
    T get_non_string_value(
        int i,
        parse_function parse,
        T const& default_value ) const
    {
        return 0 <= i && i < this->_entries.size()
            ? this->_entries[i]->get_value(parse, default_value)
            : default_value;
    }

    xml::string get_value(
        string_const_ref name,
        xml::string const& default_value ) const
    {
        return this->get_value(this->index_of(name), default_value);
    }

    xml::string get_value(
        string_const_ref name,
        string_const_ref default_value = string_const_ref{} ) const
    {
        return this->get_value(this->index_of(name), default_value);
    }

    template <typename T>
    T get_non_string_value(
        string_const_ref name,
        T const& default_value ) const
    {
        return this->get_non_string_value(
            this->index_of(name), default_value);
    }

    template <typename T, typename parse_function>
    T get_non_string_value(
        string_const_ref name,
        parse_function parse,
        T const& default_value ) const
    {
        return this->get_non_string_value(
            this->index_of(name), parse, default_value);
    }

    template <typename string_in_place_transform>
    small_string_map& transform_values_in_place(
        string_in_place_transform t )
    {
        for ( int i = this->_entries.size(); i != 0; )
            this->_entries[--i]->transform_value_in_place(t);
        return *this;
    }

    bool are_names_equal(
        string_const_ref a,
        string_const_ref b ) const
    {
        return entry::are_names_equal(
            a, b, this->comparator());
    }

    void swap( small_string_map& b )
    {
        using ali::swap;
        swap(
            static_cast<comparator_type&>(*this),
            static_cast<comparator_type&>(b));
        swap(this->_entries, b._entries);
    }

    friend void swap(
        small_string_map& a,
        small_string_map& b )
    {
        a.swap(b);
    }

    comparator_type const& comparator( void ) const
    {
        return *this;
    }

    const_iterator begin( void ) const
    {
        return const_iterator{this->_entries.ref()};
    }
    
    friend const_iterator begin( small_string_map const& a )
    {
        return a.begin();
    }
    
    const_iterator end( void ) const
    {
        return const_iterator{
            this->_entries.ref(),
            this->_entries.size()};
    }
    
    friend const_iterator end( small_string_map const& a )
    {
        return a.end();
    }

    template <typename entry_comparator>
    small_string_map& sort( entry_comparator&& comparator )
    {
        _entries.mutable_ref().sort(
            [c = ali::forward<entry_comparator>(comparator)]
                ( ali::auto_ptr<entry> const& a,
                  ali::auto_ptr<entry> const& b )
            { return c(*a, *b); });
        
        return *this;
    }

    void assert_invariant( void ) const
    {
        this->_entries.assert_invariant();
        this->_entries.ref().for_each(
            [] ( ali::auto_ptr<entry> const& element )
        {
            element.assert_invariant();
            element->assert_invariant();
        });
    }

private:    //  Methods
    static entry const& empty_entry( void )
    {
        static entry const empty{};
        return empty;
    }

private:    //  Data members
    ali::array<ali::auto_ptr<entry>>    _entries;
};

}   //  namespace ali
