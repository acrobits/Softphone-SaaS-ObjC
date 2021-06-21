/*
 *  ali_time_common.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_time_forward.h"
#include "ali/ali_array_utils_forward.h"
#include "ali/ali_array_forward.h"
#include "ali/ali_typed_number.h"
#include "ali/ali_string_forward.h"

namespace ali
{

namespace time
{

// ******************************************************************
inline time::milliseconds64& convert(
    time::milliseconds64& a,
    time::seconds b )
// ******************************************************************
{
    return a = time::milliseconds64{
        1000 * static_cast<ali::int64>(b.value)};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::seconds& operator+=(
        time::seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator+=(
        time::seconds64& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60 * static_cast<ali::int64>(b.value);
    return a;
}

// ******************************************************************
inline time::float_seconds& operator+=(
        time::float_seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value += 60.0 * static_cast<double>(b.value);
    return a;
}

// ******************************************************************
inline time::seconds& operator-=(
        time::seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator-=(
        time::seconds64& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60 * static_cast<ali::int64>(b.value);
    return a;
}

// ******************************************************************
inline time::float_seconds& operator-=(
        time::float_seconds& a,
        time::minutes b )
// ******************************************************************
{
    a.value -= 60.0 * static_cast<double>(b.value);
    return a;
}

// ******************************************************************
inline time::seconds& operator+=(
        time::seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator+=(
        time::seconds64& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60 * 60 * static_cast<ali::int64>(b.value);
    return a;
}

// ******************************************************************
inline time::float_seconds& operator+=(
        time::float_seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value += 60.0 * 60.0 * static_cast<double>(b.value);
    return a;
}

// ******************************************************************
inline time::seconds& operator-=(
        time::seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60 * 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds64& operator-=(
        time::seconds64& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60 * 60 * static_cast<ali::int64>(b.value);
    return a;
}

// ******************************************************************
inline time::float_seconds& operator-=(
        time::float_seconds& a,
        time::hours b )
// ******************************************************************
{
    a.value -= 60.0 * 60.0 * static_cast<double>(b.value);
    return a;
}

// ******************************************************************
inline time::minutes& operator+=(
    time::minutes& a,
    time::hours b )
// ******************************************************************
{
    a.value += 60 * b.value;
    return a;
}

// ******************************************************************
inline time::minutes& operator-=(
    time::minutes& a,
    time::hours b )
// ******************************************************************
{
    a.value -= 60 * b.value;
    return a;
}

// ******************************************************************
inline time::seconds operator+(
        time::seconds a,
        time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds64 operator+(
        time::seconds64 a,
        time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds operator-(
        time::seconds a,
        time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
        time::seconds64 a,
        time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds operator+(
        time::seconds a,
        time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds64 operator+(
        time::seconds64 a,
        time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::seconds operator-(
        time::seconds a,
        time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
        time::seconds64 a,
        time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::minutes operator+(
    time::minutes a,
    time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::minutes operator-(
    time::minutes a,
    time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::hours whole_hours( time::minutes m )
// ******************************************************************
{
    return time::hours{m.value / 60};
}

// ******************************************************************
inline time::minutes strip_hours( time::minutes m )
// ******************************************************************
{
    return time::minutes{m.value % 60};
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_timestamp
// ******************************************************************
{
    unix_timestamp( void )
    {}
    
    explicit unix_timestamp( time::seconds value )
    :   value{value}
    {}

    unix_timestamp& operator+=( time::seconds delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::seconds delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp& operator+=( time::minutes delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::minutes delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp& operator+=( time::hours delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp& operator-=( time::hours delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_timestamp b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_timestamp b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_timestamp b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_timestamp b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_timestamp b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_timestamp b ) const
    {
        return value >= b.value;
    }

    void swap( time::unix_timestamp& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    friend void swap(
        time::unix_timestamp& a,
        time::unix_timestamp& b )
    {
        a.swap(b);
    }
    
    time::seconds value;
};

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp operator+(
    time::unix_timestamp a, time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp operator-(
    time::unix_timestamp a, time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds operator-(
    time::unix_timestamp a,
    time::unix_timestamp b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_timestamp64
// ******************************************************************
{
    unix_timestamp64( void )
    {}
    
    explicit unix_timestamp64( time::seconds64 value )
    :   value{value}
    {}

    unix_timestamp64& operator+=( time::seconds64 delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::seconds64 delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp64& operator+=( time::minutes delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::minutes delta )
    {
        value -= delta;
        return *this;
    }
    
    unix_timestamp64& operator+=( time::hours delta )
    {
        value += delta;
        return *this;
    }
    
    unix_timestamp64& operator-=( time::hours delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_timestamp64 b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_timestamp64 b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_timestamp64 b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_timestamp64 b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_timestamp64 b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_timestamp64 b ) const
    {
        return value >= b.value;
    }
    
    time::seconds64 value;
};

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::seconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::seconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::minutes b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::minutes b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_timestamp64 operator+(
    time::unix_timestamp64 a, time::hours b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_timestamp64 operator-(
    time::unix_timestamp64 a, time::hours b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::seconds64 operator-(
    time::unix_timestamp64 a,
    time::unix_timestamp64 b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct unix_micro_timestamp
// ******************************************************************
{
    unix_micro_timestamp( void )
    {}
    
    explicit unix_micro_timestamp( time::seconds value )
    :   value{static_cast<ali::int64>(value.value) * 1000000}
    {}

    explicit unix_micro_timestamp( time::float_seconds value )
    :   value{static_cast<ali::int64>(value.value * 1000000.0)}
    {}

    explicit unix_micro_timestamp( time::microseconds64 value )
    :   value{value}
    {}

    unix_micro_timestamp& operator+=( time::seconds delta )
    {
        return operator+=(unix_micro_timestamp{delta}.value);
    }

    unix_micro_timestamp& operator+=( time::float_seconds delta )
    {
        return operator+=(unix_micro_timestamp{delta}.value);
    }

    unix_micro_timestamp& operator+=( time::microseconds64 delta )
    {
        value += delta;
        return *this;
    }
    
    unix_micro_timestamp& operator-=( time::seconds delta )
    {
        return operator-=(unix_micro_timestamp{delta}.value);
    }

    unix_micro_timestamp& operator-=( time::float_seconds delta )
    {
        return operator-=(unix_micro_timestamp{delta}.value);
    }

    unix_micro_timestamp& operator-=( time::microseconds64 delta )
    {
        value -= delta;
        return *this;
    }
    
    bool operator==( time::unix_micro_timestamp b ) const
    {
        return value == b.value;
    }

    bool operator!=( time::unix_micro_timestamp b ) const
    {
        return value != b.value;
    }

    bool operator<( time::unix_micro_timestamp b ) const
    {
        return value < b.value;
    }

    bool operator<=( time::unix_micro_timestamp b ) const
    {
        return value <= b.value;
    }

    bool operator>( time::unix_micro_timestamp b ) const
    {
        return value > b.value;
    }

    bool operator>=( time::unix_micro_timestamp b ) const
    {
        return value >= b.value;
    }
    
    time::microseconds64    value;
};

// ******************************************************************
inline time::unix_micro_timestamp operator+(
    time::unix_micro_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator+(
    time::unix_micro_timestamp a, time::microseconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator-(
    time::unix_micro_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::unix_micro_timestamp operator-(
    time::unix_micro_timestamp a, time::microseconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::microseconds64 operator-(
    time::unix_micro_timestamp a,
    time::unix_micro_timestamp b )
// ******************************************************************
{
    return a.value - b.value;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct ntp_timestamp
//  NTP timestamp is a 64-bit fixed point number of seconds
//  from 0h on Jan 1, 1900 with 32-bit fractional part.
// ******************************************************************
{
    ntp_timestamp( void ) = default;

    ntp_timestamp& operator+=( time::seconds delta );
    
    ntp_timestamp& operator+=( time::milliseconds delta );
    
    ntp_timestamp& operator-=( time::seconds delta );
    
    ntp_timestamp& operator-=( time::milliseconds delta );
    
    bool operator==( ntp_timestamp b ) const
    {
        return value == b.value;
    }
    
    bool operator!=( ntp_timestamp b ) const
    {
        return value != b.value;
    }
    
    bool operator<( ntp_timestamp b ) const
    {
        return value < b.value;
    }
    
    bool operator<=( ntp_timestamp b ) const
    {
        return value <= b.value;
    }
    
    bool operator>( ntp_timestamp b ) const
    {
        return value > b.value;
    }
    
    bool operator>=( ntp_timestamp b ) const
    {
        return value >= b.value;
    }

    friend int compare( ntp_timestamp a, ntp_timestamp b )
    {
        using ali::compare;
        return compare(a.value, b.value);
    }
    
    ali::uint32 seconds( void ) const
    {
        return static_cast<ali::uint32>((value >> 32) & 0xffffffffU);
    }
    
    ali::uint32 fraction( void ) const
    {
        return static_cast<ali::uint32>(value & 0xffffffffU);
    }
    
    ali::uint64 value{};
};

// ******************************************************************
inline time::ntp_timestamp operator+(
    time::ntp_timestamp a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::ntp_timestamp operator+(
    time::ntp_timestamp a, time::milliseconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::ntp_timestamp operator-(
    time::ntp_timestamp a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::ntp_timestamp operator-(
    time::ntp_timestamp a, time::milliseconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct julian_day
// ******************************************************************
{
    explicit julian_day( double value = 0.0 )
    :   value{value}
    {}

    void swap( julian_day& b )
    {
        using ali::swap;
        swap(value, b.value);
    }

    julian_day& operator+=( time::seconds b )
    {
        value += b.value / 86400.0;
        return *this;
    }

    friend julian_day operator+( julian_day a, time::seconds b )
    {
        a += b; return a;
    }

    julian_day& operator-=( time::seconds delta )
    {
        value -= delta.value / 86400.0;
        return *this;
    }

    friend julian_day operator-( julian_day a, time::seconds b )
    {
        a -= b; return a;
    }

    friend void swap( julian_day& a, julian_day& b )
    {
        a.swap(b);
    }

    friend bool operator==( julian_day a, julian_day b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( julian_day a, julian_day b )
    {
        return !(a == b);
    }

    friend bool operator<( julian_day a, julian_day b )
    {
        return a.value < b.value;
    }

    friend bool operator<=( julian_day a, julian_day b )
    {
        return a.value <= b.value;
    }

    friend bool operator>( julian_day a, julian_day b )
    {
        return a.value > b.value;
    }

    friend bool operator>=( julian_day a, julian_day b )
    {
        return a.value >= b.value;
    }

    friend double operator-( julian_day a, julian_day b )
    {
        return a.value - b.value;
    }

    friend int compare( julian_day a, julian_day b )
    {
        using ali::compare;
        return compare(a.value, b.value);
    }
    
    double  value{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct mac_absolute_time : time::float_seconds
// ******************************************************************
{
    mac_absolute_time( time::float_seconds value = time::float_seconds{} )
    :   float_seconds{value}
    {}

    /* TODO: make this ctor explicit eventually.
    explicit*/ mac_absolute_time( double value )
    :   time::float_seconds{value}
    {}

    mac_absolute_time& operator+=( time::float_seconds delta )
    {
        value += delta.value;
        return *this;
    }

    mac_absolute_time& operator+=( time::seconds delta )
    {
        value += delta.value;
        return *this;
    }

    mac_absolute_time& operator+=( time::milliseconds delta )
    {
        value += static_cast<double>(delta.value) / 1000.0;
        return *this;
    }

    mac_absolute_time& operator+=( time::microseconds64 delta )
    {
        value += static_cast<double>(delta.value) / 1000000.0;
        return *this;
    }

    mac_absolute_time& operator-=( time::float_seconds delta )
    {
        value -= delta.value;
        return *this;
    }

    mac_absolute_time& operator-=( time::seconds delta )
    {
        value -= delta.value;
        return *this;
    }

    mac_absolute_time& operator-=( time::milliseconds delta )
    {
        value -= static_cast<double>(delta.value) / 1000.0;
        return *this;
    }

    mac_absolute_time& operator-=( time::microseconds64 delta )
    {
        value -= static_cast<double>(delta.value) / 1000000.0;
        return *this;
    }
};

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::float_seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::seconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::milliseconds b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator+(
    time::mac_absolute_time a, time::microseconds64 b )
// ******************************************************************
{
    return a += b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::float_seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::seconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::milliseconds b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
inline time::mac_absolute_time operator-(
    time::mac_absolute_time a, time::microseconds64 b )
// ******************************************************************
{
    return a -= b;
}

// ******************************************************************
// ******************************************************************
    
// ******************************************************************
struct parts
// ******************************************************************
{
    static bool is_leap_year( short year );
    
    bool is_leap_year( void ) const
    {
        return is_leap_year(year);
    }
    
    static short days_in_month(
        short year, short month );

    short days_in_month( void ) const
    {
        return days_in_month(year, month);
    }
    
    bool is_valid( void ) const;

    void swap( parts& b )
    {
        ali::swap_by_copy(*this, b);
    }

    friend void swap( parts& a, parts& b )
    {
        a.swap(b);
    }
    
    short year{};
    short month{};          //  1=January..12=December
    short day{};            //  1..days_in_month()
    short hour{};           //  0..23
    short minute{};         //  0..59
    short second{};         //  0..59
    short millisecond{};    //  0..999
};

// ******************************************************************
inline bool operator==( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return a.millisecond == b.millisecond
        && a.second == b.second
        && a.minute == b.minute
        && a.hour == b.hour
        && a.day == b.day
        && a.month == b.month
        && a.year == b.year;
}

// ******************************************************************
inline bool operator!=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return !(a == b);
}

// ******************************************************************
bool operator<( time::parts const& a, time::parts const& b );
// ******************************************************************

// ******************************************************************
inline bool operator<=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    //  a <= b --> !(a > b) --> !(b < a)

    return !(b < a);
}

// ******************************************************************
inline bool operator>( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    return b < a;
}

// ******************************************************************
inline bool operator>=( time::parts const& a, time::parts const& b )
// ******************************************************************
{
    //  a >= b --> !(a < b)
    
    return !(a < b);
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::unix_timestamp64 t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::unix_micro_timestamp t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp& convert(
    time::unix_timestamp& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::unix_timestamp t )
// ******************************************************************
{
    timestamp.value.value = t.value.value;
    return timestamp;
}

// ******************************************************************
time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::unix_micro_timestamp t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
time::unix_timestamp64& convert(
    time::unix_timestamp64& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
inline time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    time::unix_timestamp t )
// ******************************************************************
{
    timestamp.value.value = t.value.value;
    timestamp.value.value *= 1000000;
    return timestamp;
}

// ******************************************************************
inline time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    time::unix_timestamp64 t )
// ******************************************************************
{
    timestamp.value.value = t.value.value;
    timestamp.value.value *= 1000000;
    return timestamp;
}

// ******************************************************************
time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    time::mac_absolute_time t );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
    time::mac_absolute_time& timestamp,
    time::unix_timestamp t );
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
    time::mac_absolute_time& timestamp,
    time::unix_timestamp64 t );
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
     time::mac_absolute_time& timestamp,
     time::unix_micro_timestamp t );
// ******************************************************************

// ******************************************************************
time::mac_absolute_time& convert(
    time::mac_absolute_time& timestamp,
    time::parts const& parts );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::julian_day jd );
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_timestamp64 timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
    time::parts& parts,
    time::unix_micro_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts& convert(
     time::parts& parts,
     time::mac_absolute_time mat );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::ntp_timestamp& convert(
    time::ntp_timestamp& ntp,
    time::unix_micro_timestamp timestamp );
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& micro_timestamp,
    time::ntp_timestamp ntp );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
time::julian_day& convert(
    time::julian_day& jd,
    time::parts parts );
// ******************************************************************

// ******************************************************************
// ******************************************************************
   
// ******************************************************************
template <typename T>
class timer
// ******************************************************************
{
public:
    void start( void );
    T read( void ) const;
    T stop( void );
};

const double MacAbsoluteTimeIntervalSince1970 = 978307200.0;

// ******************************************************************
namespace day_of_week
// ******************************************************************
{

// ******************************************************************
enum type {mon,tue,wed,thu,fri,sat,sun};
// ******************************************************************

// ******************************************************************
day_of_week::type get( time::julian_day jd );
// ******************************************************************

// ******************************************************************
inline day_of_week::type get( time::parts const& parts )
// ******************************************************************
{
    time::julian_day jd;
    return day_of_week::get(time::convert(jd, parts));
}

// ******************************************************************
inline day_of_week::type get( time::unix_timestamp timestamp )
// ******************************************************************
{
    time::parts parts;
    return day_of_week::get(time::convert(parts, timestamp));
}


}   //  namespace day_of_week

namespace current
{

namespace gmt
{

// ******************************************************************
time::unix_timestamp as_unix_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp64 as_unix_timestamp64( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp as_unix_micro_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts as_parts( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::mac_absolute_time as_mac_absolute_time( void );
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
inline time::ntp_timestamp as_ntp_timestamp( void )
// ******************************************************************
{
    time::ntp_timestamp timestamp;
    return time::convert(timestamp, as_unix_micro_timestamp());
}

// ******************************************************************
inline time::julian_day as_julian_day( void )
// ******************************************************************
{
    time::julian_day jd;
    return time::convert(jd, as_parts());
}

}   //  namespace gmt

namespace local
{

// ******************************************************************
time::unix_timestamp as_unix_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp64 as_unix_timestamp64( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp as_unix_micro_timestamp( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::parts as_parts( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::mac_absolute_time as_mac_absolute_time( void );
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
inline time::ntp_timestamp as_ntp_timestamp( void )
// ******************************************************************
{
    time::ntp_timestamp timestamp;
    return time::convert(timestamp, as_unix_micro_timestamp());
}

// ******************************************************************
inline time::julian_day as_julian_day( void )
// ******************************************************************
{
    time::julian_day jd;
    return time::convert(jd, as_parts());
}

}   //  namespace local

}   //  namespace current

namespace local
{

// ******************************************************************
time::minutes gmt_offset( void );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

// ******************************************************************
time::unix_timestamp from_gmt( time::unix_timestamp timestamp );
// ******************************************************************
//  Implemented by platform.
// ******************************************************************

}   //  namespace local

namespace rfc1123
{

ali::string& format( ali::string& str, time::parts const& parts );

ali::string format( time::parts const& parts );
    
bool parse(
    time::parts& parts,
    time::minutes& shift,
    ali::string_const_ref str );

}

namespace iso8601w3c // See: http://www.w3.org/TR/NOTE-datetime
{

namespace format_option
{

enum type
{
    year, year_month, date,
    date_hours_minutes,
    date_hours_minutes_seconds,
    date_hours_minutes_seconds_milliseconds,
};

}   //  namespace format

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::parts const& parts,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::parts const& parts,
    time::minutes shift,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

// ******************************************************************
ali::string format(
    time::parts const& parts,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

// ******************************************************************
ali::string format(
    time::parts const& parts,
    time::minutes shift,
    format_option::type fmt = format_option::date_hours_minutes_seconds_milliseconds );
// ******************************************************************

}   //  namespace iso8601w3c

namespace rfc3339
{

// ******************************************************************
ali::string& format_date(
    ali::string& str, time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string format_date( time::parts const& parts );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format_time(
    ali::string& str, time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string& format_time(
    ali::string& str, time::parts const& parts, time::minutes shift );
// ******************************************************************
//  When shift != 0, then the parts structure shall contain
//  LOCAL time and the shift shall be "LOCAL time" - "UTC time".
// ******************************************************************

// ******************************************************************
ali::string format_time( time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string format_time(
    time::parts const& parts, time::minutes shift );
// ******************************************************************
//  When shift != 0, then the parts structure shall contain
//  LOCAL time and the shift shall be "LOCAL time" - "UTC time".
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format_date_time(
    ali::string& str, time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string& format_date_time(
    ali::string& str, time::parts const& parts, time::minutes shift );
// ******************************************************************
//  When shift != 0, then the parts structure shall contain
//  LOCAL time and the shift shall be "LOCAL time" - "UTC time".
// ******************************************************************

// ******************************************************************
ali::string format_date_time( time::parts const& parts );
// ******************************************************************

// ******************************************************************
ali::string format_date_time(
    time::parts const& parts, time::minutes shift );
// ******************************************************************
//  When shift != 0, then the parts structure shall contain
//  LOCAL time and the shift shall be "LOCAL time" - "UTC time".
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format( ali::string& str, time::parts const& parts );
// ******************************************************************
//  Deprecated in favor of format_date_time.
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str, time::parts const& parts, time::minutes shift );
// ******************************************************************
//  Deprecated in favor of format_date_time.
// ******************************************************************

// ******************************************************************
ali::string format( time::parts const& parts );
// ******************************************************************
//  Deprecated in favor of format_date_time.
// ******************************************************************

// ******************************************************************
ali::string format( time::parts const& parts, time::minutes shift );
// ******************************************************************
//  Deprecated in favor of format_date_time.
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
bool parse_date(
    time::parts& parts,
    string_const_ref str );
// ******************************************************************
//  date: "YYYY-MM-DD"
// ******************************************************************

// ******************************************************************
bool parse_time(
    time::parts& parts,
    time::minutes& shift,
    string_const_ref str );
// ******************************************************************
//           time: "HH:MM:SS" [time-secfrac] time-offset
//     time-shift: "Z" / time-numshift
//  time-numshift: ("+" / "-") "HH:MM"
//  
//  When shift != 0, then the parts structure contains LOCAL time.
//  To obtain UTC time, one needs to SUBTRACT the shift minutes.
// ******************************************************************

// ******************************************************************
bool parse_date_time(
    time::parts& parts,
    time::minutes& shift,
    string_const_ref str );
// ******************************************************************
//  date-time: date "T" time
//  
//  When shift != 0, then the parts structure contains LOCAL time.
//  To obtain UTC time, one needs to SUBTRACT the shift minutes.
// ******************************************************************

// ******************************************************************
bool parse(
    time::parts& parts,
    time::minutes& shift,
    string_const_ref str );
// ******************************************************************
//  Deprecated in favor of parse_date_time.
// ******************************************************************

}   //  namespace rfc3339

namespace rfc6350
{

//  https://tools.ietf.org/html/rfc6350#section-4.3
//
//  --  Based on iso8601:2004 and iso8601:2000
//  --  Only the basic format is supported.

// ******************************************************************
bool parse_date( time::parts& parts, string_const_ref str );
// ******************************************************************
//  Complete representations:
//  Basic format: YYYYMMDD
//
//  Representations with reduced accuracy:
//  -   A specific month
//      Basic format: YYYY-MM
//
//  -   A specific year
//      Basic format: YYYY
// ******************************************************************

// ******************************************************************
bool parse_time( time::parts& parts, string_const_ref str );
// ******************************************************************
//  Complete representations:
//  Basic format: hhmmss
//
//  Representations with reduced accuracy:
//  -   A specific hour and minute
//      Basic format: hhmm
//
//  -   A specific hour
//      Basic format: hh
// ******************************************************************

// ******************************************************************
bool parse_date_time( time::parts& parts, string_const_ref str );
// ******************************************************************
//  <date>T<time>
// ******************************************************************

// ******************************************************************
bool parse_date_and_or_time( time::parts& parts, string_const_ref str );
// ******************************************************************
//  Either a DATE-TIME, a DATE, or a TIME value. To allow unambiguous
//  interpretation, a stand-alone TIME value is always preceded by a "T".
//
//  <date>
//  T<time>
//  <date>T<time>
// ******************************************************************

}   //  namespace rfc6350

// ******************************************************************
namespace vcard = rfc6350;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::parts const& value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::unix_timestamp value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::unix_timestamp64 value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::unix_micro_timestamp value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    time::mac_absolute_time value,
    string_const_ref format_string );
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
array<bool> test( void );
// ******************************************************************

}   //  namespace time

}   //  namespace ali
