#pragma once

#include "ali/ali_time_common.h"
#include <mach/clock.h>
#include <CoreFoundation/CoreFoundation.h>

// ******************************************************************
struct timespec;
// ******************************************************************

namespace ali
{

namespace time
{

// ******************************************************************
class platform_stopwatch
// ******************************************************************
{
public:
    static ali::int64 tick_frequency( void )
        //  Returns number of ticks per second.
    {
        return 1000000000;
            //  The ticks are in nanoseconds.
    }

protected:  //  Typedefs
    typedef ali::int64 monotonic_timestamp;

protected:  //  Methods
    platform_stopwatch( void );

    void split( void )
    {
        _split = this->now();
    }

    ali::int64 elapsed_tick_count( monotonic_timestamp split ) const
        //  return split - _start;
    {
        return split - _start;
    }

    void swap( platform_stopwatch& b )
    {
        using ali::swap;
        swap(_start, b._start);
        swap(_split, b._split);
        swap_by_copy(_clock_service, b._clock_service);
    }

    monotonic_timestamp now( void ) const
    {
        mach_timespec_t t = {};
    
        clock_get_time(_clock_service, &t);

        return  ali::int64(t.tv_sec) * tick_frequency()
                    + ali::int64(t.tv_nsec);
    }

    static monotonic_timestamp subtract(
        monotonic_timestamp a,
        monotonic_timestamp b )
            //  return a - b;
    {
        return a - b;
    }

protected:  //  Data members
    monotonic_timestamp _start;
    monotonic_timestamp _split;
    clock_serv_t        _clock_service;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <>
class timer<milliseconds>
// ******************************************************************
{
public:
    timer();
    
    void start( void );

    bool is_started( void ) const
    {
        return _is_started;
    }

    milliseconds read( void ) const;

    milliseconds stop( void );
    
    void swap( timer& b );

    friend void swap( timer& a, timer& b )
    {
        a.swap(b);
    }

private:    // Data members
    mach_timespec_t     _start;
    clock_serv_t        _clock_service;
    bool                _is_started;
};
    
// ******************************************************************
template <>
class timer<milliseconds64>
// ******************************************************************
{
public:
    timer();

    void start( void );
    
    bool is_started( void ) const
    {
        return _is_started;
    }

    milliseconds64 read( void ) const;
    
    milliseconds64 stop( void );
    
    void swap( timer& b );
    
    friend void swap( timer& a, timer& b )
    {
        a.swap(b);
    }
    
private:    // Data members
    mach_timespec_t     _start;
    clock_serv_t        _clock_service;
    bool                _is_started;
};

// ******************************************************************
template <>
class timer<microseconds64>
// ******************************************************************
{
public:
    timer();

    void start( void );

    bool is_started( void ) const
    {
        return _is_started;
    }

    microseconds64 read( void ) const;

    microseconds64 stop( void );

    void swap( timer& b );
    
    friend void swap( timer& a, timer& b )
    {
        a.swap(b);
    }

private:    // Data members
    mach_timespec_t     _start;
    clock_serv_t        _clock_service;
    bool                _is_started;
};
    
// ******************************************************************
// ******************************************************************

// ******************************************************************
time::unix_micro_timestamp& convert(
    time::unix_micro_timestamp& timestamp,
    timespec const& t );
// ******************************************************************

}   //  namespace time

}   //  namespace ali
