/*
 *  ali_net_address.h
 *  ali Library
 *
 *  Copyright 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_net_address_forward.h"
#include "ali/ali_integer.h"
#include "ali/ali_string.h"
#include "ali/ali_utility.h"

namespace ali
{

/// \brief  Provides facilities for communication over a network.
///
namespace network
{

// ******************************************************************
/// \brief Encapsulates a network port value.
///
// ******************************************************************
struct port
// ******************************************************************
{
        /// \brief  Default constructor.
        /// Initializes this instance with value representing
        /// unspecified port number.
        ///
        /// \post   <tt>this->is_unspecified()</tt>
        ///
        /// \throws noexcept
        ///
    port( void ) {}

        /// \brief  Initializes this instance with the given
        /// port number.
        ///
        /// \post   <tt>this->value == value</tt>
        ///
        /// \throws noexcept
        ///
    port( ali::uint16 value )
    :   value{value}
    {}

        /// \brief  Updates this instance with the given
        /// port number.
        ///
        /// \post   <tt>this->value == another_value</tt>
        ///
        /// \throws noexcept
        ///
    port& operator=( ali::uint16 another_value )
    {
        value = another_value;
        return *this;
    }

        /// \brief  Updates this instance with a value extracted
        /// from the given string representation.
        ///
        /// The supported format is:
        ///
        ///     port = 16-bit unsigned decimal number
        ///
        /// \param[in]  str
        ///         String to parse.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  0 <= this->value
        ///                 &&  this->value < 65636)
        ///             ||  (   !result
        ///                     && *this == *pre(this))
        ///
        /// \retval true
        ///         The given string has acceptable format and this
        ///         instance was updated with the value represented
        ///         by it.
        /// \retval false
        ///         The given string has unacceptable format and this
        ///         instance was not modified in any way.
        ///
        /// \throws noexcept
        ///
        /// \remark This method accepts `0` as a valid port value.
        ///
        /// \see    \ref parse_rejecting_unspecified
        ///
    bool parse( string_const_ref str );
    bool parse( wstring_const_ref str );

        /// \brief  Updates this instance with a value extracted
        /// from the given string representation.
        ///
        /// The supported format is:
        ///
        ///     port = 16-bit unsigned decimal number
        ///
        /// \param[in]  str
        ///         String to parse.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  0 < this->value
        ///                 &&  this->value < 65636)
        ///             ||  (   !result
        ///                     && *this == *pre(this))
        ///
        /// \retval true
        ///         The given string has acceptable format and this
        ///         instance was updated with the value represented
        ///         by it.
        /// \retval false
        ///         The given string has unacceptable format and this
        ///         instance was not modified in any way.
        ///
        /// \throws noexcept
        ///
        /// \remark This method doesn't accept `0` as a valid port
        /// value.
        ///
        /// \see    \ref parse
        ///
    bool parse_rejecting_unspecified( string_const_ref str );
    bool parse_rejecting_unspecified( wstring_const_ref str );

        /// \brief  Determines whether this instance contains the
        /// default value.
        ///
        /// \retval true
        ///         This instance contains the default value.
        /// \retval false
        ///         This instance contains other than the default
        ///         value.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified( void ) const
    {
        return value == 0;
    }

        /// \brief  Swaps value of this instance with another one.
        ///
        /// \param[in,out]  b
        ///         Another instance to swap value with.
        ///
        /// \post
        ///
        ///                 *this == pre(b)
        ///             &&  *pre(this) == b
        ///
        /// \throws noexcept
        ///
    void swap( port& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

        /// \brief  See `port::swap`.
        ///
    friend void swap( port& a, port& b )
    {
        a.swap(b);
    }

        /// \brief  Determines whether the two given instances of
        /// the \ref port class represent the same port number.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///         Two instances of the \ref port class.
        ///
        /// \retval true
        ///         `a` and `b` represent the same port number.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator==( port a, port b )
    {
        return a.value == b.value;
    }

        /// \brief  Determines whether the two given instances of
        /// the \ref port class represent different port numbers.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///         Two instances of the \ref port class.
        ///
        /// \post   <tt>result == !(a == b)</tt>
        ///
        /// \retval true
        ///         `a` and `b` represent different port numbers.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator!=( port a, port b )
    {
        return !(a == b);
    }

    friend int compare( port a, port b )
    {
        using ali::compare;

        return compare(a.value, b.value);
    }

    ali::uint16 value{};
        ///<        Holds the actual port number.
};

// ******************************************************************
ali::string& format(
    ali::string& str,
    network::port value,
    string_const_ref /*format_string*/ );
// ******************************************************************

// ******************************************************************
/// \brief  Encapsulates an IPv4 address.
///
/// The interface of this class supports four types of IPv4 address
/// representations:
///
/// -   4x8-bit; four 8-bit numbers: `a8, b8, c8, d8`
/// -   2x8-bit + 1x16-bit; two 8-bit numbers and one 16-bit number:
///     `a8, b8, c16`
/// -   1x8-bit + 1x24-bit; one 8-bit number and one 24-bit number:
///     `a8, b24`
/// -   1x32-bit; one 32-bit number: `a32`
///
/// The following equalities hold:
///
///     c16 == (c8 << 8) | d8
///     b24 == (b8 << 16) | c16 == (b8 << 16) | (c8 << 8) | d8
///     a32 == (a8 << 24) | b24 == (a8 << 24) | (b8 << 16) | c16 == (a8 << 24) | (b8 << 16) | (c8 << 8) | d8
///
// ******************************************************************
struct address_ipv4
// ******************************************************************
{
        /// \brief  Returns address_ipv4 instance representing the
        /// loopback address.
        ///
        /// \post   <tt>result.is_loopback()</tt>
        ///
        /// \returns `address_ipv4` instance representing the
        /// loopback address.
        ///
        /// \throws noexcept
        ///
    static address_ipv4 loopback( void )
    {
        return address_ipv4{0x7f000001U};
    }

        /// \brief  Default constructor.
        /// Initializes this instance to value representing
        /// unspecified IPv4 address.
        ///
        /// \post   <tt>this->is_unspecified()</tt>
        ///
        /// \throws noexcept
        ///
    address_ipv4( void ) {}

        /// \brief  Constructor.
        /// Initializes this address from the given 4x8-bit
        /// representation.
        ///
        /// \param[in]  parts8
        ///         Array of four 8-bit numbers to initialize this
        ///         address with.
        ///
        /// \pre
        ///
        ///             forall i in set {0, ..., 3} &
        ///                 0 <= parts8[i] && parts8[i] < 256
        ///
        /// \throws noexcept
        ///
    explicit address_ipv4(
        ali::uint32 const (&parts8)[4] )
    {
        set(parts8);
    }

        /// \brief  Constructor.
        /// Initializes this address from the given 4x8-bit
        /// representation.
        ///
        /// \param[in]  a8
        /// \param[in]  b8
        /// \param[in]  c8
        /// \param[in]  d8
        ///         Four 8-bit numbers to initialize this address
        ///         with.
        ///
        /// \pre
        ///
        ///             forall param in set {a8, b8, c8, d8} &
        ///                 0 <= param && param < 256
        ///
        /// \throws noexcept
        ///
    address_ipv4(
        ali::uint32 a8,
        ali::uint32 b8,
        ali::uint32 c8,
        ali::uint32 d8 )
    {
        set(a8,b8,c8,d8);
    }

        /// \brief  Constructor.
        /// Initializes this address from the given 2x8-bit
        /// + 1x16-bit representation.
        ///
        /// \param[in]  a8
        /// \param[in]  b8
        ///         Two 8-bit numbers to initialize this address
        ///         with.
        /// \param[in]  c16
        ///         16-bit number to initialize this address with.
        ///
        /// \pre
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b8 && b8 < 256
        ///             &&  0 <= c16 && c16 < 65536
        ///
        /// \throws noexcept
        ///
    address_ipv4( ali::uint32 a8, ali::uint32 b8, ali::uint32 c16 )
    {
        set(a8,b8,c16);
    }

        /// \brief  Constructor.
        /// Initializes this address from the given 1x8-bit
        /// + 1x24-bit representation.
        ///
        /// \param[in]  a8
        ///         8-bit number to initialize this address with.
        /// \param[in]  b24
        ///         24-bit number to initialize this address with.
        ///
        /// \pre
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b24 && b24 < 16777216
        ///
        /// \throws noexcept
        ///
    address_ipv4( ali::uint32 a8, ali::uint32 b24 )
    {
        set(a8,b24);
    }

        /// \brief  Constructor.
        /// Initializes this address from the given 1x32-bit
        /// representation.
        ///
        /// \param[in]  a32
        ///         32-bit number to initialize this address with.
        ///
        /// \throws noexcept
        ///
    explicit address_ipv4( ali::uint32 a32 )
    :   _value{a32}
    {}

        /// \brief  Updates this address with a value extracted from
        /// the given string representation.
        ///
        /// The supported format is:
        ///
        ///     ipv4 = bare_ipv4
        ///     ipv4 = "[" bare_ipv4 "]"
        ///     bare_ipv4 = n(8) "." n(8) "." n(8) "." n(8)
        ///     bare_ipv4 = n(8) "." n(8) "." n(16)
        ///     bare_ipv4 = n(8) "." n(24)
        ///     bare_ipv4 = n(32)
        ///     n(m) = m-bit C unsigned integer literal
        ///
        /// \param[in]  str
        ///         String to parse.
        ///
        /// \post   result || *this == *pre(this)
        ///
        /// \retval true
        ///         The given string has acceptable format and this
        ///         address was set to the value represented by it.
        /// \retval false
        ///         The given string has unacceptable format and this
        ///         address was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse( string_const_ref str )
    {
        return str.begins_with('[') && str.ends_with(']')
            ? parse_bare(*str.pointer(1, str.size() - 2))
            : parse_bare(str);
    }

    bool parse( wstring_const_ref str )
    {
        return str.begins_with(L'[') && str.ends_with(L']')
            ? parse_bare(*str.pointer(1, str.size() - 2))
            : parse_bare(str);
    }

        /// \brief  Returns the 4x8-bit representation of the IPv4
        /// address stored in this instance.
        ///
        /// \param[out] parts8
        ///         Array of four numbers to store the result to.
        ///
        /// \post
        ///
        ///             forall i in set {0, ..., 3} &
        ///                 0 <= parts8[i] && parts8[i] < 256
        ///
        /// \throws noexcept
        ///
    void get( ali::uint32 (&parts8)[4] ) const
    {
        parts8[0] = (_value >> 24) & 0x000000ffU;
        parts8[1] = (_value >> 16) & 0x000000ffU;
        parts8[2] = (_value >>  8) & 0x000000ffU;
        parts8[3] =  _value        & 0x000000ffU;
    }

        /// \brief  Returns the 4x8-bit representation of the IPv4
        /// address stored in this instance.
        ///
        /// \param[out] a8
        /// \param[out] b8
        /// \param[out] c8
        /// \param[out] d8
        ///         Variables to store the result to.
        ///
        /// \post
        ///
        ///             forall param in set {a8, b8, c8, d8} &
        ///                 0 <= param && param < 256
        ///
        /// \throws noexcept
        ///
    void get(
        ali::uint32& a8,
        ali::uint32& b8,
        ali::uint32& c8,
        ali::uint32& d8 ) const
    {
        a8 = (_value >> 24) & 0x000000ffU;
        b8 = (_value >> 16) & 0x000000ffU;
        c8 = (_value >>  8) & 0x000000ffU;
        d8 =  _value        & 0x000000ffU;
    }

        /// \brief  Returns the 2x8-bit + 1x16-bit representation of
        /// the IPv4 address stored in this instance.
        ///
        /// \param[out] a8
        /// \param[out] b8
        /// \param[out] c16
        ///         Variables to store the result to.
        ///
        /// \post
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b8 && b8 < 256
        ///             &&  0 <= c16 && c16 < 65536
        ///
        /// \throws noexcept
        ///
    void get(
        ali::uint32& a8,
        ali::uint32& b8,
        ali::uint32& c16 ) const
    {
        a8  = (_value >> 24) & 0x000000ffU;
        b8  = (_value >> 16) & 0x000000ffU;
        c16 =  _value        & 0x0000ffffU;
    }

        /// \brief  Returns the 1x8-bit + 1x24-bit representation of
        /// the IPv4 address stored in this instance.
        ///
        /// \param[out] a8
        /// \param[out] b24
        ///         Variables to store the result to.
        ///
        /// \post
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b24 && b24 < 16777216
        ///
        /// \throws noexcept
        ///
    void get(
        ali::uint32& a8,
        ali::uint32& b24 ) const
    {
        a8  = (_value >> 24) & 0x000000ffU;
        b24 =  _value        & 0x00ffffffU;
    }

        /// \brief  Returns the 1x32-bit representation of the IPv4
        /// address stored in this instance.
        ///
        /// \param[out] a32
        ///         Variable to store the result to.
        ///
        /// \throws noexcept
        ///
    void get(
        ali::uint32& a32 ) const
    {
        a32 = _value;
    }

        /// \brief  Returns the 1x32-bit representation of the IPv4
        /// address stored in this instance.
        ///
        /// \returns The 1x32-bit representation of the IPv4 address
        /// stored in this instance.
        ///
        /// \throws noexcept
        ///
    ali::uint32 get( void ) const
    {
        return _value;
    }

        /// \brief  Updates this address from the given 4x8-bit
        /// representation.
        ///
        /// \param[in]  parts8
        ///         Array of four 8-bit numbers to update this
        ///         address with.
        ///
        /// \pre
        ///
        ///             forall i in set {0, ..., 3} &
        ///                 0 <= parts8[i] && parts8[i] < 256
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv4& set( ali::uint32 const (&parts8)[4] )
    {
        ali_assert(parts8[0] < 0x00000100U);
        ali_assert(parts8[1] < 0x00000100U);
        ali_assert(parts8[2] < 0x00000100U);
        ali_assert(parts8[3] < 0x00000100U);

        _value
            = (parts8[0] << 24)
            | (parts8[1] << 16)
            | (parts8[2] <<  8)
            | (parts8[3]      );

        return *this;
    }

        /// \brief  Updates this address from the given 4x8-bit
        /// representation.
        ///
        /// \param[in]  a8
        /// \param[in]  b8
        /// \param[in]  c8
        /// \param[in]  d8
        ///         Four 8-bit numbers to update this address with.
        ///
        /// \pre
        ///
        ///             forall param in set {a8, b8, c8, d8} &
        ///                 0 <= param && param < 256
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv4& set(
        ali::uint32 a8,
        ali::uint32 b8,
        ali::uint32 c8,
        ali::uint32 d8 )
    {
        ali_assert(a8 < 0x00000100U);
        ali_assert(b8 < 0x00000100U);
        ali_assert(c8 < 0x00000100U);
        ali_assert(d8 < 0x00000100U);

        _value
            = (a8 << 24)
            | (b8 << 16)
            | (c8 <<  8)
            | (d8      );

        return *this;
    }

        /// \brief  Updates this address from the given 2x8-bit
        /// + 1x16-bit representation.
        ///
        /// \param[in]  a8
        /// \param[in]  b8
        ///         Two 8-bit numbers to update this address with.
        /// \param[in]  c16
        ///         16-bit number to update this address with.
        ///
        /// \pre
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b8 && b8 < 256
        ///             &&  0 <= c16 && c16 < 65536
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv4& set(
        ali::uint32 a8,
        ali::uint32 b8,
        ali::uint32 c16 )
    {
        ali_assert( a8 < 0x00000100U);
        ali_assert( b8 < 0x00000100U);
        ali_assert(c16 < 0x00010000U);

        _value
            = (a8 << 24)
            | (b8 << 16)
            | (c16     );

        return *this;
    }

        /// \brief  Updates this address from the given 1x8-bit
        /// + 1x24-bit representation.
        ///
        /// \param[in]  a8
        ///         8-bit number to update this address with.
        /// \param[in]  b24
        ///         24-bit number to update this address with.
        ///
        /// \pre
        ///
        ///                 0 <= a8 && a8 < 256
        ///             &&  0 <= b24 && b24 < 16777216
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv4& set(
        ali::uint32 a8,
        ali::uint32 b24 )
    {
        ali_assert( a8 < 0x00000100U);
        ali_assert(b24 < 0x01000000U);

        _value = (a8 << 24) | b24;

        return *this;
    }

        /// \brief  Updates this address from the given 1x32-bit
        /// representation.
        ///
        /// \param[in]  a32
        ///         32-bit number to update this address with.
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv4& set( ali::uint32 a32 )
    {
        _value = a32;

        return *this;
    }

        /// \brief  Determines whether this address is equal to the
        /// other given address.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         `this` and `b` represent the same IPv4 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator==( address_ipv4 a, address_ipv4 b )
    {
        return a._value == b._value;
    }

        /// \brief  Determines whether this address is different
        /// from the other given address.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         `this` and `b` represent two different IPv4
        ///         addresses.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator!=( address_ipv4 a, address_ipv4 b )
    {
        return !(a == b);
    }

    friend bool operator<( address_ipv4 a, address_ipv4 b )
    {
        return a._value < b._value;
    }

    friend bool operator<=( address_ipv4 a, address_ipv4 b )
    {
        //  a <= b  <==>  !(a > b)  <==>  !(b < a)
        return !(b < a);
    }

    friend bool operator>( address_ipv4 a, address_ipv4 b )
    {
        //  a > b  <==>  b < a
        return b < a;
    }

    friend bool operator>=( address_ipv4 a, address_ipv4 b )
    {
        //  a >= b  <==>  !(a < b)
        return !(a < b);
    }

    friend int compare( address_ipv4 a, address_ipv4 b )
    {
        using ali::compare;
        return compare(a._value, b._value);
    }

        /// \brief  Determines whether this address contains the
        /// default value.
        ///
        /// \retval true
        ///         This address contains the default value.
        /// \retval false
        ///         This address contains other than the default
        ///         value.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified( void ) const
    {
        return _value == 0U;
    }

        /// \brief  Determines whether this address represents
        /// a loopback address.
        ///
        /// According to [RFC 6890](http://tools.ietf.org/html/rfc6890),
        /// a loopback address resides in the 127.0.0.0/8 IPv4
        /// address block.
        ///
        /// \retval true
        ///         This address represents a loopback address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_loopback( void ) const
    {
        return (_value & 0xff000000U) == 0x7f000000U;   //  127.0.0.0/8
    }

        /// \brief  Determines whether this address represents an
        /// IPv4 address that was reserved for private use.
        ///
        /// According to [RFC 6890](http://tools.ietf.org/html/rfc6890),
        /// a private address resides in one of the following IPv4
        /// address blocks:
        /// -   10.0.0.0/8
        /// -   172.16.0.0/12
        /// -   192.168.0.0/16
        /// -   100.64.0.0/10 (Shared Address Space for Carrier-Grade NAT)
        ///
        /// \retval true
        ///         This address represents a private IPv4 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_private( void ) const
    {
        return  (_value & 0xff000000U) == 0x0a000000U   //  10.0.0.0/8
            ||  (_value & 0xfff00000U) == 0xac100000U   //  172.16.0.0/12
            ||  (_value & 0xffff0000U) == 0xc0a80000U   //  192.168.0.0/16
            ||  (_value & 0xffc00000U) == 0x64400000U;  //  100.64.0.0/10
    }

        /// \brief  Determines whether this address represents an
        /// IPv4 multicast address.
        ///
        /// According to [RFC 1112](http://tools.ietf.org/html/rfc1112),
        /// a multicast address resides in the 224.0.0.0/4 IPv4
        /// address block.
        ///
        /// \retval true
        ///         This address represents a multicast IPv4 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_multicast( void ) const
    {
        return (_value & 0xf0000000U) == 0xe0000000U;   //  224.0.0.0/4
    }

        /// \brief  Determines whether this address represents an
        /// IPv4 link local address.
        ///
        /// According to [RFC 6890](http://tools.ietf.org/html/rfc6890),
        /// a link local address resides in the 169.254.0.0/16 IPv4
        /// address block.
        ///
        /// \retval true
        ///         This address represents a link local IPv4
        ///         address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_link_local( void ) const
    {
        return (_value & 0xffff0000U) == 0xa9fe0000U;   //  169.254.0.0/16
    }

    address_ipv4& operator++( void )
    {
        ++_value;
        return *this;
    }

    address_ipv4 operator++( int )
    {
        address_ipv4 const t{*this};
        ++(*this);
        return t;
    }

    address_ipv4& operator--( void )
    {
        --_value;
        return *this;
    }

    address_ipv4 operator--( int )
    {
        address_ipv4 const t{*this};
        --(*this);
        return t;
    }

        /// \brief  Swaps value of this address with another one.
        ///
        /// \param[in,out]  b
        ///         Another address to swap value with.
        ///
        /// \post
        ///
        ///                 *this == pre(b)
        ///             &&  *pre(this) == b
        ///
        /// \throws noexcept
        ///
    void swap( address_ipv4& b )
    {
        using ali::swap;
        swap(_value, b._value);
    }

        /// \brief  See `address_ipv4::swap`.
        ///
    friend void swap( address_ipv4& a, address_ipv4& b )
    {
        a.swap(b);
    }

private:    //  Methods
    template <typename t_string_const_ref, typename t>
    bool parse_bare( t_string_const_ref str, t dot );
    bool parse_bare( string_const_ref str );
    bool parse_bare( wstring_const_ref str );

private:    //  Data members
    ali::uint32 _value{};
        ///<        Internal representation of an IPv4 address.
};

// ******************************************************************
/// \brief  Encapsulates an IPv6 address.
///
/// The interface of this class supports the 8x16-bit representation.
///
// ******************************************************************
struct address_ipv6
// ******************************************************************
{
    class embedded_ipv4_translator;

        /// \brief  Returns address_ipv6 instance representing the
        /// loopback address.
        ///
        /// \post   <tt>result.is_loopback()</tt>
        ///
        /// \returns `address_ipv6` instance representing the
        /// loopback address.
        ///
        /// \throws noexcept
        ///
    static address_ipv6 loopback( void )
    {
        return address_ipv6{_loopback};
    }

        /// \brief  Constructs the <em>[IPv4-mapped IPv6 address]
        /// (https://tools.ietf.org/html/rfc4291#section-2.5.5)</em>
        /// from the given IPv4 address.
        ///
        /// \param[in]  ipv4
        ///         IPv4 address to map to IPv6 address.
        ///
        /// \returns IPv4-mapped IPv6 address of the given IPv4
        /// address.
        ///
        /// \throws noexcept
        ///
    static address_ipv6 mapped_ipv4(
        address_ipv4 ipv4 );

        /// \brief  Constructs the <em>[IPv4-embedded IPv6 address]
        /// (https://tools.ietf.org/html/rfc6052#section-2)</em>
        /// from the given IPv4 address.
        ///
        /// This overload uses the well-known 64:ff9b::/96 prefix
        /// to construct IPv4-embedded IPv6 address.
        ///
        /// \param[in]  ipv4
        ///         IPv4 address to embed in IPv6 address.
        ///
        /// \returns IPv4-embedded IPv6 address of the given IPv4
        /// address.
        ///
        /// \throws noexcept
        ///
    static address_ipv6 embedded_ipv4(
        address_ipv4 ipv4 );

        /// \brief  Default constructor.
        /// Initializes this instance to value representing
        /// unspecified IPv6 address.
        ///
        /// \post   <tt>this->is_unspecified()</tt>
        ///
        /// \throws noexcept
        ///
    address_ipv6( void ) {}

        /// \brief  Constructor.
        /// Initializes this address from the given 8x16-bit
        /// representation.
        ///
        /// \param[in]  parts16
        ///         Array of eight 16-bit numbers to initialize this
        ///         address with.
        ///
        /// \pre
        ///
        ///             forall i in set {0, ..., 7} &
        ///                 0 <= parts16[i] && parts16[i] < 65536
        ///
        /// \throws noexcept
        ///
    explicit address_ipv6(
        ali::uint32 const (&parts16)[8] )
    {
        set(parts16);
    }

        /// \brief  Constructor.
        /// Initializes this address from the given 8x16-bit
        /// representation.
        ///
        /// \param[in]  a16
        /// \param[in]  b16
        /// \param[in]  c16
        /// \param[in]  d16
        /// \param[in]  e16
        /// \param[in]  f16
        /// \param[in]  g16
        /// \param[in]  h16
        ///         Eight 16-bit numbers to initialize this address
        ///         with.
        ///
        /// \pre
        ///
        ///             forall param in set {a16, b16, c16, d16, e16, f16, g16, h16} &
        ///                 0 <= param && param < 65536
        ///
        /// \throws noexcept
        ///
    address_ipv6(
        ali::uint32 a16,
        ali::uint32 b16,
        ali::uint32 c16,
        ali::uint32 d16,
        ali::uint32 e16,
        ali::uint32 f16,
        ali::uint32 g16,
        ali::uint32 h16 )
    {
        set(a16,b16,c16,d16,e16,f16,g16,h16);
    }

        /// \brief  Updates this address with a value extracted from
        /// the given string representation.
        ///
        /// The supported format is as specified in [RFC 4291]
        /// (https://tools.ietf.org/html/rfc4291#section-2.2)
        /// (including the compressed form).
        /// Subset of the supported grammar is:
        ///
        ///     ipv6 = bare_ipv6
        ///     ipv6 = "[" bare_ipv6 "]"
        ///     bare_ipv6 = x:x:x:x:x:x:x:x
        ///     bare_ipv6 = x:x:x:x:x:x:d.d.d.d
        ///     x = 16-bit hexadecimal number
        ///     d = 8-bit decimal number
        ///     : = ":"
        ///     . = "."
        ///
        /// \param[in]  str
        ///         String to parse.
        ///
        /// \post   result || *this == *pre(this)
        ///
        /// \retval true
        ///         The given string has acceptable format and this
        ///         address was set to the value represented by it.
        /// \retval false
        ///         The given string has unacceptable format and this
        ///         address was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool parse( string_const_ref str )
    {
        return str.begins_with('[') && str.ends_with(']')
            ? parse_bare(*str.pointer(1, str.size() - 2))
            : parse_bare(str);
    }

    bool parse( wstring_const_ref str )
    {
        return str.begins_with(L'[') && str.ends_with(L']')
            ? parse_bare(*str.pointer(1, str.size() - 2))
            : parse_bare(str);
    }

        /// \brief  Returns the 8x16-bit representation of the IPv6
        /// address stored in this instance.
        ///
        /// \param[out] parts16
        ///         Array of eight numbers to store the result to.
        ///
        /// \post
        ///
        ///             forall i in set {0, ..., 7} &
        ///                 0 <= parts16[i] && parts16[i] < 65536
        ///
        /// \throws noexcept
        ///
    void get( ali::uint32 (&parts16)[8] ) const
    {
        parts16[0] = _value.words[0];
        parts16[1] = _value.words[1];
        parts16[2] = _value.words[2];
        parts16[3] = _value.words[3];
        parts16[4] = _value.words[4];
        parts16[5] = _value.words[5];
        parts16[6] = _value.words[6];
        parts16[7] = _value.words[7];
    }

        /// \brief  Returns the 8x16-bit representation of the IPv6
        /// address stored in this instance.
        ///
        /// \param[out] a16
        /// \param[out] b16
        /// \param[out] c16
        /// \param[out] d16
        /// \param[out] e16
        /// \param[out] f16
        /// \param[out] g16
        /// \param[out] h16
        ///         Variables to store the result to.
        ///
        /// \post
        ///
        ///             forall param in set {a16, b16, c16, d16, e16, f16, g16, h16} &
        ///                 0 <= param && param < 65536
        ///
        /// \throws noexcept
        ///
    void get(
        ali::uint32& a16,
        ali::uint32& b16,
        ali::uint32& c16,
        ali::uint32& d16,
        ali::uint32& e16,
        ali::uint32& f16,
        ali::uint32& g16,
        ali::uint32& h16 ) const
    {
        a16 = _value.words[0];
        b16 = _value.words[1];
        c16 = _value.words[2];
        d16 = _value.words[3];
        e16 = _value.words[4];
        f16 = _value.words[5];
        g16 = _value.words[6];
        h16 = _value.words[7];
    }

        /// \brief  Updates this address from the given 8x16-bit
        /// representation.
        ///
        /// \param[in]  parts16
        ///         Array of eight 16-bit numbers to update this
        ///         address with.
        ///
        /// \pre
        ///
        ///             forall i in set {0, ..., 7} &
        ///                 0 <= parts16[i] && parts16[i] < 65536
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv6& set( ali::uint32 const (&parts16)[8] )
    {
        ali_assert(parts16[0] < 0x00010000U);
        ali_assert(parts16[1] < 0x00010000U);
        ali_assert(parts16[2] < 0x00010000U);
        ali_assert(parts16[3] < 0x00010000U);
        ali_assert(parts16[4] < 0x00010000U);
        ali_assert(parts16[5] < 0x00010000U);
        ali_assert(parts16[6] < 0x00010000U);
        ali_assert(parts16[7] < 0x00010000U);
        _value.words[0] = static_cast<ali::uint16>(parts16[0]);
        _value.words[1] = static_cast<ali::uint16>(parts16[1]);
        _value.words[2] = static_cast<ali::uint16>(parts16[2]);
        _value.words[3] = static_cast<ali::uint16>(parts16[3]);
        _value.words[4] = static_cast<ali::uint16>(parts16[4]);
        _value.words[5] = static_cast<ali::uint16>(parts16[5]);
        _value.words[6] = static_cast<ali::uint16>(parts16[6]);
        _value.words[7] = static_cast<ali::uint16>(parts16[7]);
        return *this;
    }

        /// \brief  Updates this address from the given 8x16-bit
        /// representation.
        ///
        /// \param[in]  a16
        /// \param[in]  b16
        /// \param[in]  c16
        /// \param[in]  d16
        /// \param[in]  e16
        /// \param[in]  f16
        /// \param[in]  g16
        /// \param[in]  h16
        ///         Eight 16-bit numbers to update this address with.
        ///
        /// \pre
        ///
        ///             forall param in set {a16, b16, c16, d16, e16, f16, g16, h16} &
        ///                 0 <= param && param < 65536
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    address_ipv6& set(
        ali::uint32 a16,
        ali::uint32 b16,
        ali::uint32 c16,
        ali::uint32 d16,
        ali::uint32 e16,
        ali::uint32 f16,
        ali::uint32 g16,
        ali::uint32 h16 )
    {
        ali_assert(a16 < 0x00010000U);
        ali_assert(b16 < 0x00010000U);
        ali_assert(c16 < 0x00010000U);
        ali_assert(d16 < 0x00010000U);
        ali_assert(e16 < 0x00010000U);
        ali_assert(f16 < 0x00010000U);
        ali_assert(g16 < 0x00010000U);
        ali_assert(h16 < 0x00010000U);
        _value.words[0] = static_cast<ali::uint16>(a16);
        _value.words[1] = static_cast<ali::uint16>(b16);
        _value.words[2] = static_cast<ali::uint16>(c16);
        _value.words[3] = static_cast<ali::uint16>(d16);
        _value.words[4] = static_cast<ali::uint16>(e16);
        _value.words[5] = static_cast<ali::uint16>(f16);
        _value.words[6] = static_cast<ali::uint16>(g16);
        _value.words[7] = static_cast<ali::uint16>(h16);
        return *this;
    }

        /// \brief  Determines whether this address is equal to the
        /// other given address.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         `this` and `b` represent the same IPv6 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator==( address_ipv6 a, address_ipv6 b )
    {
        return a._value == b._value;
    }

        /// \brief  Determines whether this address is different
        /// from the other given address.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         `this` and `b` represent two different IPv6
        ///         addresses.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator!=( address_ipv6 a, address_ipv6 b )
    {
        return !(a == b);
    }

    friend bool operator<( address_ipv6 a, address_ipv6 b )
    {
        return a._value < b._value;
    }

    friend bool operator<=( address_ipv6 a, address_ipv6 b )
    {
        //  a <= b  <==>  !(a > b)  <==>  !(b < a)
        return !(b < a);
    }

    friend bool operator>( address_ipv6 a, address_ipv6 b )
    {
        //  a > b  <==>  b < a
        return b < a;
    }

    friend bool operator>=( address_ipv6 a, address_ipv6 b )
    {
        //  a >= b  <==>  !(a < b)
        return !(a < b);
    }

    friend int compare( address_ipv6 a, address_ipv6 b )
    {
        return a._value.compare(b._value);
    }

        /// \brief  Determines whether this address contains the
        /// default value.
        ///
        /// \retval true
        ///         This address contains the default value.
        /// \retval false
        ///         This address contains other than the default
        ///         value.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified( void ) const
    {
        return _value == _zero;
    }

        /// \brief  Determines whether this address represents
        /// the IPv6 loopback address.
        ///
        /// According to [RFC 4291](https://tools.ietf.org/html/rfc4291#section-2.5.3),
        /// the loopback address is 0:0:0:0:0:0:0:1 (or ::1 in
        /// compressed form).
        ///
        /// \retval true
        ///         This address represents the loopback address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_loopback( void ) const
    {
        return _value == _loopback;
    }

        /// \brief  Determines whether this address represents
        /// a local IPv6 unicast address.
        ///
        /// According to [RFC 4193](https://tools.ietf.org/html/rfc4193#section-3.1),
        /// a local IPv6 unicast address uses FC00::/7 prefix.
        ///
        /// \retval true
        ///         This address represents a local IPv6 unicast address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_local( void ) const
    {
        return (_value.words[0] & 0xFE00) == 0xFC00;
    }

        /// \brief  Determines whether this address represents
        /// a link local IPv6 address.
        ///
        /// According to [RFC 4291](http://tools.ietf.org/html/rfc4291#section-2.5.6),
        /// a link local IPv6 address begins with FE80:0:0:0 prefix.
        ///
        /// \retval true
        ///         This address represents a link local IPv6 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_link_local( void ) const
    {
        return  _value.words[0] == 0xFE80
            &&  _value.words[1] == 0x0000
            &&  _value.words[2] == 0x0000
            &&  _value.words[3] == 0x0000;
    }

        /// \brief  Determines whether this address represents
        /// a multicast IPv6 address.
        ///
        /// According to [RFC 4291](https://tools.ietf.org/html/rfc4291#section-2.7),
        /// a multicast IPv6 address begins with FF00::/8 prefix.
        ///
        /// \retval true
        ///         This address represents a multicast IPv6 address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_multicast( void ) const
    {
        return (_value.words[0] & 0xFF00) == 0xFF00;
    }

        /// \brief  Extracts the embedded IPv4 address if this
        /// address represents an <em>[IPv4-mapped IPv6 address]
        /// (https://tools.ietf.org/html/rfc4291#section-2.5.5).</em>
        ///
        /// \param[out] ipv4
        ///         IPv4 address to store the result to.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  mapped_ipv4(ipv4) == *this)
        ///
        ///             ||  (   !result
        ///                 &&  ipv4 == pre(ipv4))
        ///
        /// \retval true
        ///         This address represents IPv4-mapped IPv6 address
        ///         and the embedded IPv4 address was written to the
        ///         `ipv4` parameter.
        /// \retval false
        ///         This address doesn't represent IPv4-mapped IPv6
        ///         address and the `ipv4` parameter was not modified
        ///         in any way.
        ///
        /// \throws noexcept
        ///
    bool extract_mapped_ipv4(
        address_ipv4& ipv4 ) const;

        /// \brief  Extracts the embedded IPv4 address if this
        /// address represents an <em>[IPv4-embedded IPv6 address]
        /// (https://tools.ietf.org/html/rfc6052#section-2)</em>
        /// and the used prefix is the well-known 64:ff9b::/96.
        ///
        /// \param[out] ipv4
        ///         IPv4 address to store the result to.
        ///
        /// \post
        ///
        ///                 (   result
        ///                 &&  embedded_ipv4(ipv4) == *this)
        ///
        ///             ||  (   !result
        ///                 &&  ipv4 == pre(ipv4))
        ///
        /// \retval true
        ///         This address represents IPv4-embedded IPv6 address
        ///         with the well-known prefix 64:ff9b::/96 and
        //          the embedded IPv4 address was written to the
        ///         `ipv4` parameter.
        /// \retval false
        ///         This address doesn't represent IPv4-embeedded IPv6
        ///         address with the well-known prefix 64:ff9b::/96
        //          and the `ipv4` parameter was not modified in any way.
        ///
        /// \throws noexcept
        ///
    bool extract_embedded_ipv4(
        address_ipv4& ipv4 ) const;

        /// \brief  Swaps value of this address with another one.
        ///
        /// \param[in,out]  b
        ///         Another address to swap value with.
        ///
        /// \post
        ///
        ///                 *this == pre(b)
        ///             &&  *pre(this) == b
        ///
        /// \throws noexcept
        ///
    void swap( address_ipv6& b )
    {
        using ali::swap;
        swap(_value, b._value);
    }

        /// \brief  See `address_ipv6::swap`.
        ///
    friend void swap( address_ipv6& a, address_ipv6& b )
    {
        a.swap(b);
    }

private:    //  Struct
    struct ip_type
    {
        bool operator==( ip_type const& b ) const;

        bool operator!=( ip_type const& b ) const
        {
            return !operator==(b);
        }

        bool operator<( ip_type const& b ) const;

        int compare( ip_type const& b ) const;

        void swap( ip_type& b )
        {
            using ali::swap;
            swap(words, b.words);
        }

        friend void swap( ip_type& a, ip_type& b )
        {
            a.swap(b);
        }

        ali::uint16 words[8];
    };

    ali_static_assert(meta::is_trivially_copyable<ip_type>::result);

private:    //  Methods
    explicit address_ipv6( ip_type const& value )
    {
        _value = value; //  C++ Defect 1467
    }

    template <typename t_string_const_ref, typename t>
    bool parse_bare( t_string_const_ref str, t dot, t colon );
    bool parse_bare( string_const_ref str );
    bool parse_bare( wstring_const_ref str );

private:    //  Data members
    ip_type                 _value = _zero; //  C++ Defect 1467
        ///<        Internal representation of an IPv6 address.
    static ip_type const    _zero;
        ///<        Constant representing the unspecified IPv6
        ///         address.
    static ip_type const    _loopback;
        ///<        Constant representing the IPv6 loopback address.
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class address_ipv6::embedded_ipv4_translator
// ******************************************************************
{
public:
    embedded_ipv4_translator( void ) {}

    address_ipv6 translate( address_ipv4 ipv4 ) const
    {
        return address_ipv6{parts(_parts)
            .embed_ipv4(_prefix_size_in_bits, ipv4)
            .value};
    }

    bool learn_from_rfc7050( address_ipv6 ipv6 );
        //  https://tools.ietf.org/html/rfc7050
        //  ipv6 should be the result of a DNS *AAAA*
        //  query for the "ipv4only.arpa" domain name.
        //  This domain name is *defined* to always
        //  resolve to either 192.0.0.170 or 192.0.0.171.
        //  and so if we get any AAAA response back
        //  that response is going to be a result
        //  of network specific DNS64 mapping from which
        //  we might learn the rules for constructing
        //  IPv6 embedded-IPv4 addresses.

    ali::string& format_for_logging( ali::string& str ) const;

    friend ali::string& format(
        ali::string& str,
        embedded_ipv4_translator const& value,
        string_const_ref )
    {
        return value.format_for_logging(str);
    }

private:    //  Data members
    struct parts
    {
        parts& embed_ipv4(
            int prefix_size_in_bits,
            address_ipv4 ipv4 );

        ali::uint32 value[8];

    }               _parts{{0x0064U, 0xff9bU}};
    int             _prefix_size_in_bits{96};
};

// ******************************************************************
/// \brief  Provides one place where to store all information
/// about a network endpoint address.
///
/// The mutual relation of all data members of this class is left
/// unspecified and shall be defined on a per use basis.
///
// ******************************************************************
struct address
// ******************************************************************
{
        /// \brief  Constructs address object from a textual
        /// representation.
        ///
        /// The supported format is:
        ///
        ///     name = fqdn optional_port
        ///     name = bare_ipv4 optional_port
        ///     name = bare_ipv6
        ///     name = "[" fqdn "]" optional_port
        ///     name = "[" bare_ipv4 "]" optional_port
        ///     name = "[" bare_ipv6 "]" optional_port
        ///     optional_port = [ ":" [ port ] ]
        ///
        /// \param[in]  name
        ///         String containing textual representation
        ///         of a network endpoint address.
        /// \param[in]  default_port
        ///         Default port value to use if the textual
        ///         representation doesn't include the port part.
        ///
        /// \post
        ///
        ///             (   (   name contains port
        ///                 &&  result.port == port)
        ///
        ///             ||  (   name doesn't contain port
        ///                 &&  result.port == default_port))
        ///
        ///         &&  (   (   name contains fqdn
        ///                 &&  result.name == fqdn
        ///                 &&  result.ipv4.is_unspecified()
        ///                 &&  result.ipv6.is_unspecified())
        ///
        ///             ||  (   name contains ipv4
        ///                 &&  result.name.is_empty()
        ///                 &&  result.ipv4 == ipv4
        ///                 &&  result.ipv6.is_unspecified())
        ///
        ///             ||  (   name contains ipv6
        ///                 &&  result.name.is_empty()
        ///                 &&  result.ipv4.is_unspecified()
        ///                 &&  result.ipv6 == ipv6))
        ///
        /// \remark If `name` contains an fqdn enclosed in square
        /// brackets then those brackets _are not included_ in
        /// `result.name`.
        ///
        /// \returns \ref address object initialized from the given
        /// textual representation.
        ///
        /// \throws noexcept
        ///
    static address from_name_and_port(
        string_const_ref name,
        network::port default_port );
    static address from_name_and_port(
        wstring_const_ref name,
        network::port default_port );

        /// \brief  Constructs address object from an IPv4 address
        /// and port.
        ///
        /// \param[in]  ipv4
        /// \param[in]  port
        ///         IPv4 address and port to initialize the resulting
        ///         address with.
        ///
        /// \post
        ///
        ///                 result.name.is_empty()
        ///             &&  result.ipv4 == ipv4
        ///             &&  result.ipv6.is_unspecified()
        ///             &&  result.port == port
        ///
        /// \returns \ref address object initialized with the given
        /// IPv4 address and port.
        ///
        /// \throws noexcept
        ///
    static address from_ipv4(
        address_ipv4 ipv4,
        network::port port )
    {
        address t{};
        t.ipv4 = ipv4;
        t.port = port;
        return t;
    }

        /// \brief  Constructs address object from an IPv6 address
        /// and port.
        ///
        /// \param[in]  ipv6
        /// \param[in]  port
        ///         IPv6 address and port to initialize the resulting
        ///         address with.
        ///
        /// \post
        ///
        ///                 result.name.is_empty()
        ///             &&  result.ipv4.is_unspecified()
        ///             &&  result.ipv6 == ipv6
        ///             &&  result.port == port
        ///
        /// \returns \ref address object initialized with the given
        /// IPv6 address and port.
        ///
        /// \throws noexcept
        ///
    static address from_ipv6(
        address_ipv6 const& ipv6,
        network::port port )
    {
        address t{};
        t.ipv6 = ipv6;
        t.port = port;
        return t;
    }

        /// \brief  Initializes this address object with an empty address
        ///
        /// \post
        ///
        ///                 this->name.is_empty()
        ///             &&  this->ipv4.is_unspecified()
        ///             &&  this->ipv6.is_unspecified()
        ///             &&  this->port.is_unspecified()
        ///
        /// \throws noexcept
        ///
    address( void ) {}

        /// \brief  Initializes this address object with the given
        /// port number.
        ///
        /// \param[in]  port
        ///         Port to initialize this address with.
        ///
        /// \post
        ///
        ///                 this->name.is_empty()
        ///             &&  this->ipv4.is_unspecified()
        ///             &&  this->ipv6.is_unspecified()
        ///             &&  this->port == port
        ///
        /// \throws noexcept
        ///
    explicit address( network::port port )
    :   port{port}
    {}

        /// \brief  Determines whether all data members of this and
        /// the other given address contain equal values.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         All pairs of corresponding data members of
        ///         `this` and `b` contain equal values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator==( address const& a, address const& b )
    {
        return  a.name == b.name
            &&  a.ipv4 == b.ipv4
            &&  a.ipv6 == b.ipv6
            &&  a.port == b.port
            &&  a.optional == b.optional;
    }

        /// \brief  Determines whether at least one data member of
        /// this address is not equal to the corresponding data
        /// member in the other given address.
        ///
        /// \param[in]  b
        ///         Another instance of this class.
        ///
        /// \retval true
        ///         There is a pair of corresponding data members
        ///         of `this` and `b` containing unequal values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    friend bool operator!=( address const& a, address const& b )
    {
        return !(a == b);
    }

    friend int compare( address const& a, address const& b )
    {
        using ali::compare;

        int result{compare(a.name, b.name)};
        
        if ( result != 0 ) return result;

        result = compare(a.ipv4, b.ipv4);

        if ( result != 0 ) return result;

        result = compare(a.ipv6, b.ipv6);

        if ( result != 0 ) return result;

        result = compare(a.port, b.port);

        if ( result != 0 ) return result;

        return result = compare(a.optional, b.optional);
    }

        /// \brief  Swaps content of this address with the specfied
        /// one.
        ///
        /// \param[in,out]  b
        ///         Another address to swap content with.
        ///
        /// \post
        ///
        ///                 *this == pre(b)
        ///             &&  *pre(this) == b
        ///
        /// \throws noexcept
        ///
    void swap( address& b )
    {
        using ali::swap;
        swap(name, b.name);
        swap(ipv4, b.ipv4);
        swap(ipv6, b.ipv6);
        swap(port, b.port);
        swap(optional, b.optional);
    }

        /// \brief  See `address::swap`.
        ///
    friend void swap( address& a, address& b )
    {
        a.swap(b);
    }

        /// \brief  Determines whether `name`, `ipv4`, `ipv6`
        /// and `port` data members of this address contain
        /// default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->name.is_empty()
        ///                 &&  this->ipv4.is_unspecified()
        ///                 &&  this->ipv6.is_unspecified()
        ///                 &&  this->port.is_unspecified()
        ///
        /// \retval true
        ///         All the four data members `name`, `ipv4`, `ipv6`
        ///         and `port` contain default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified_all( void ) const
    {
        //  Note: This method will be renamed
        //  to just "is_unspecified" eventually.

        return  name.is_empty()
            &&  is_unspecified_ip_address_and_port();
    }

        /// \brief  Determines whether `name`, `ipv4` and `ipv6`
        /// data members of this address contain default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->name.is_empty()
        ///                 &&  this->ipv4.is_unspecified()
        ///                 &&  this->ipv6.is_unspecified()
        ///
        /// \retval true
        ///         All the three data members `name`, `ipv4` and
        ///         `ipv6` contain default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified_name_and_ip_address( void ) const
    {
        return  name.is_empty()
            &&  is_unspecified_ip_address();
    }

    bool is_unspecified( void ) const
        //  Retained for historical reasons: doesn't check port value!
        //  Consider using is_unspecified_all() instead.
    {
        return is_unspecified_name_and_ip_address();
    }

        /// \brief  Determines whether `ipv4` and `ipv6`
        /// data members of this address contain default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->ipv4.is_unspecified()
        ///                 &&  this->ipv6.is_unspecified()
        ///
        /// \retval true
        ///         Both data members `ipv4` and
        ///         `ipv6` contain default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified_ip_address( void ) const
    {
        return  ipv4.is_unspecified()
            &&  ipv6.is_unspecified();
    }

        /// \brief  Determines whether `ipv4` or `ipv6`
        /// data members of this address contain non-default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  !this->ipv4.is_unspecified()
        ///                 ||  !this->ipv6.is_unspecified()
        ///
        /// \retval true
        ///         Either `ipv4` or `ipv6` (or both) data
        ///         members contain non-default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_specified_ip_address( void ) const
    {
        return !is_unspecified_ip_address();
    }

        /// \brief  Determines whether `ipv4`, `ipv6` and `port`
        /// data members of this address contain default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->ipv4.is_unspecified()
        ///                 &&  this->ipv6.is_unspecified()
        ///                 &&  this->port.is_unspecified()
        ///
        /// \retval true
        ///         All the three data members `ipv4`, `ipv6`
        ///         and `port` contain default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_unspecified_ip_address_and_port( void ) const
    {
        return  port.is_unspecified()
            &&  is_unspecified_ip_address();
    }

        /// \brief  Determines whether `port` and `ipv4` or `ipv6`
        /// data members of this address contain non-default values.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  (   !this->ipv4.is_unspecified()
        ///                 ||      !this->ipv6.is_unspecified())
        ///                 &&  !this->port.is_unspecified()
        ///
        /// \retval true
        ///         The `port` and `ipv4` or `ipv6` data members
        ///         contain non-default values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_specified_ip_address_and_port( void ) const
    {
        return  !port.is_unspecified()
            &&  is_specified_ip_address();
    }

        /// \brief  Determines whether there is at least one
        /// specified IP address and _every_ specified IP address
        /// represents a loopback address.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  !(this->ipv4.is_unspecified() && this->ipv6.is_unspecified())
        ///                         //  At least one IP address is specified.
        ///                 &&  (this->ipv4.is_unspecified() || this->ipv4.is_loopback())
        ///                 &&  (this->ipv6.is_unspecified() || this->ipv6.is_loopback())
        ///
        /// \retval true
        ///         There is at least one specified IP address and
        ///         every specified IP address represents a loopback
        ///         address.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_loopback( void ) const
    {
        return  !is_unspecified_ip_address()
                    //  At least one IP address is specified.
            &&  (ipv4.is_unspecified() || ipv4.is_loopback())
            &&  (ipv6.is_unspecified() || ipv6.is_loopback());
    }

    /// \brief  Determines whether there is at least one
    /// specified IP address and _every_ specified IP address
    /// represents a link-local address.
    ///
    /// \post
    ///
    ///             result
    ///                 ==  !(this->ipv4.is_unspecified() && this->ipv6.is_unspecified())
    ///                         //  At least one IP address is specified.
    ///                 &&  (this->ipv4.is_unspecified() || this->ipv4.is_link_local())
    ///                 &&  (this->ipv6.is_unspecified() || this->ipv6.is_link_local())
    ///
    /// \retval true
    ///         There is at least one specified IP address and
    ///         every specified IP address represents a link-local
    ///         address.
    /// \retval false
    ///         Otherwise.
    ///
    /// \throws noexcept
    ///
    bool is_link_local( void ) const
    {
        return  !is_unspecified_ip_address()
                    //  At least one IP address is specified.
            &&  (ipv4.is_unspecified() || ipv4.is_link_local())
            &&  (ipv6.is_unspecified() || ipv6.is_link_local());
    }

        /// \brief  Formats all information contained in this
        /// address and appends it to the given string.
        ///
        /// \param[out] str
        ///         String to append the formatted content of this
        ///         address to.
        ///
        /// \post   <tt>&result == &str && str.size() >= pre(str).size()</tt>
        ///
        /// \returns `str`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    ali::string& format_for_logging( ali::string& str ) const;

        /// \brief  Formats all information contained in this
        /// address and returns the resulting string.
        ///
        /// \returns String containing formatted content of this
        /// address.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    ali::string format_for_logging( void ) const
    {
        ali::string str{};
        format_for_logging(str);
        return str;
    }

        /// \brief  Formats this address as hostname.
        ///
        /// \param[out] str
        ///         String to append the formatted content of this
        ///         address to.
        ///
        /// \post   <tt>&result == &str && str.size() >= pre(str).size()</tt>
        ///
        /// \returns `str`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark If this->is_unspecified() then 0.0.0.0 is used for hostname.
        ///
        /// \remark This method always encloses IPv6 address in square brackets.
        ///
    ali::string& format_name_bracketed_if_ipv6( ali::string& str ) const;

        /// \brief  Formats this address as hostname.
        ///
        /// \returns String containing formatted content of this
        /// address.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark If this->is_unspecified() then 0.0.0.0 is used for hostname.
        ///
        /// \remark This method always encloses IPv6 address in square brackets.
        ///
    ali::string format_name_bracketed_if_ipv6( void ) const
    {
        ali::string str;
        return format_name_bracketed_if_ipv6(str);
    }

        /// \brief  Formats this address as hostname optionally
        /// followed by colon and port number.
        ///
        /// \param[out] str
        ///         String to append the formatted content of this
        ///         address to.
        ///
        /// \post   <tt>&result == &str && str.size() >= pre(str).size()</tt>
        ///
        /// \returns `str`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark If this->is_unspecified() then 0.0.0.0 is used for hostname.
        ///
        /// \remark This method always encloses IPv6 address in square brackets.
        ///
        /// \remark If this->port.is_unspecified() then no port information is included.
        ///
    ali::string& format_name_and_port( ali::string& str ) const;

        /// \brief  Formats this address as hostname optionally
        /// followed by colon and port number.
        ///
        /// \returns String containing formatted content of this
        /// address.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark If this->is_unspecified() then 0.0.0.0 is used for hostname.
        ///
        /// \remark This method always encloses IPv6 address in square brackets.
        ///
        /// \remark If this->port.is_unspecified() then no port information is included.
        ///
    ali::string format_name_and_port( void ) const
    {
        ali::string str;
        return format_name_and_port(str);
    }

    friend ali::string& format(
        ali::string& str,
        address const& value,
        string_const_ref )
    {
        return value.format_for_logging(str);
    }

    ali::string         name{};
        ///<\brief  Holds an unresolved host name; most often fully
        ///         qualified domain name (FQDN).

    address_ipv4        ipv4{};
        ///<        Holds an IPv4 address.

    address_ipv6        ipv6{};
        ///<        Holds an IPv6 address.

    network::port       port{};
        ///<        Holds a port value.

        /// \brief  Contains additional inessential information
        /// about a network endpoint address.
        ///
    struct optional_info
    {
            /// \brief  Determines whether all corresponding data
            /// members of the two given instances contain equal
            /// values.
            ///
            /// \param[in]  a
            /// \param[in]  b
            ///         Two instances of the \ref optional_info class.
            ///
            /// \retval true
            ///         All pairs of corresponding data members of
            ///         `a` and `b` contain equal values.
            /// \retval false
            ///         Otherwise.
            ///
            /// \throws noexcept
            ///
        friend bool operator==(
            optional_info const& a,
            optional_info const& b )
        {
            return a.original_name == b.original_name;
        }

            /// \brief  Determines whether at least one pair of
            /// corresponding data members of the two given instances
            /// contain unequal values.
            ///
            /// \param[in]  a
            /// \param[in]  b
            ///         Two instances of the \ref optional_info class.
            ///
            /// \retval true
            ///         There is a pair of corresponding data members
            ///         of `a` and `b` containing unequal values.
            /// \retval false
            ///         Otherwise.
            ///
            /// \throws noexcept
            ///
        friend bool operator!=(
            optional_info const& a,
            optional_info const& b )
        {
            return !(a == b);
        }

        friend int compare(
            optional_info const& a,
            optional_info const& b )
        {
            using ali::compare;
            return compare(a.original_name, b.original_name);
        }

            /// \brief  Swaps content of this instance with the
            /// specified one.
            ///
            /// \param[in,out]  b
            ///         Another instance to swap content with.
            ///
            /// \post
            ///
            ///                 *this == pre(b)
            ///             &&  *pre(this) == b
            ///
            /// \throws noexcept
            ///
        void swap( optional_info& b )
        {
            using ali::swap;
            swap(original_name, b.original_name);
        }

            /// \brief  See `optional_info::swap`.
        friend void swap(
            optional_info& a,
            optional_info& b )
        {
            a.swap(b);
        }

        ali::string     original_name{};
            ///<    \brief  This is typically a name that a _USER_
            ///     has specified in preferences. From this name,
            ///     another names can be derived, using, for
            ///     example, DNS SRV -- these are then placed in the
            ///     `address::name` data member.
            ///
            ///     In particular, a server provided certificate
            ///     should be verified against this member and not
            ///     the `address::name` member which might be
            ///     populated using insecure DNS SRV query.
            ///

    }                   optional{};
        ///<        Holds additional inessential information.
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief  Determines whether the given two addresses specify
/// the same IP address.
///
/// \param[in]  a
/// \param[in]  b
///             Two instances of the \ref address class.
///
/// \post
///
///             result
///                 ==  (!a.ipv4.is_unspecified() && a.ipv4 == b.ipv4)
///                 ||  (!a.ipv6.is_unspecified() && a.ipv6 == b.ipv6);
///
/// \retval true
///         Addresses `a` and `b` specify the same IP address.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
// ******************************************************************
inline bool same_specified_ip(
    address const& a,
    address const& b )
// ******************************************************************
{
    return  (!a.ipv4.is_unspecified() && a.ipv4 == b.ipv4)
        ||  (!a.ipv6.is_unspecified() && a.ipv6 == b.ipv6);
}


// ******************************************************************
/// \brief  Determines whether the given two addresses specify
/// the same port and at least one IP address.
///
/// \param[in]  a
/// \param[in]  b
///             Two instances of the \ref address class.
///
/// \post
///
///             result
///                 ==  a.port == b.port
///                 &&  (   (!a.ipv4.is_unspecified() && a.ipv4 == b.ipv4)
///                     ||  (!a.ipv6.is_unspecified() && a.ipv6 == b.ipv6));
///
/// \retval true
///         Addresses `a` and `b` specify the same port and at least
///         one IP address.
/// \retval false
///         Otherwise.
///
/// \throws noexcept
///
// ******************************************************************
inline bool same_specified_ip_and_port(
    address const& a,
    address const& b )
// ******************************************************************
{
    return a.port == b.port && same_specified_ip(a, b);
}

}   //  namespace network

}   //  namespace ali
