#pragma once
#include "ali/ali_time_platform.h"

namespace ali
{

namespace time
{

// ******************************************************************
class stopwatch : public platform_stopwatch
// ******************************************************************
//  The stopwatch is always running.
// ******************************************************************
// 
//  class platform_stopwatch
//  {
//  public:
//      static ali::int64 tick_frequency( void );
//      //  or ali::int64 tick_frequency( void ) const;
//          //  Returns number of ticks per second.
//  
//  protected:  //  Typedefs
//      typedef ... monotonic_timestamp;
//  
//  protected:  //  Methods
//      platform_stopwatch( void ) {}
//  
//      void split( void );
//          //  Makes _split == now()
//
//      ali::int64 elapsed_tick_count( monotonic_timestamp split ) const;
//          //  Returns split - _start;
//
//      void swap( platform_stopwatch& b );
//  
//      static monotonic_timestamp now( void )
//      //  or monotonic_timestamp now( void ) const
//      
//      static monotonic_timestamp subtract(
//          monotonic_timestamp a,
//          monotonic_timestamp b );
//              //  Returns a - b
//  
//  protected:  //  Data members
//      monotonic_timestamp _start;
//      monotonic_timestamp _split;
//  };
//
// ******************************************************************
{
public:
    stopwatch( void )
    {
        reset();
    }

    stopwatch& split( void )
        //  Takes note of the elapsed time.
        //
        //  _split = now
    {
        platform_stopwatch::split();
        return *this;
    }
    
    stopwatch& resume_from_split( void )
        //  Erases time between split and now.
        //  (Essentially pause/resume.)
        //
        //  _start = _start' + (now - _split')
        //  _split = now
        //
        //  _split - _start = now - (_start' + now - _split')
        //  _split - _start = _split' - _start'
    {
        _start = this->subtract(_split, _start);
            //  _start'' = _split' - _start'
        platform_stopwatch::split();
            //  _split = now
        _start = this->subtract(_split, _start);
            //  _start = now - _start''
            //      = now - (_split' - _start')
            //      = _start' + now - _split'
        return *this;
    }
        
    stopwatch& reset_from_split( void )
        //  Makes the split the new start.
        //
        //  _start = _split = _split'
    {
        _start = _split;
        return *this;
    }

    stopwatch& reset( void )
        //  Makes now the new start.
        //
        //  _start = _split = now
    {
        platform_stopwatch::split();
        _start = _split;
        return *this;
    }

    time::hours total_hours( void ) const
        //  return now - _start;
    {
        return elapsed_hours(this->now());
    }
    
    time::minutes total_minutes( void ) const
        //  return now - _start;
    {
        return elapsed_minutes(this->now());
    }
    
    time::seconds total_seconds( void ) const
        //  return now - _start;
    {
        return elapsed_seconds(this->now());
    }
    
    time::seconds64 total_seconds64( void ) const
        //  return now - _start;
    {
        return elapsed_seconds64(this->now());
    }
    
    time::milliseconds total_milliseconds( void ) const
        //  return now - _start;
    {
        return elapsed_milliseconds(this->now());
    }
    
    time::milliseconds64 total_milliseconds64( void ) const
        //  return now - _start;
    {
        return elapsed_milliseconds64(this->now());
    }
    
    time::microseconds64 total_microseconds64( void ) const
        //  return now - _start;
    {
        return elapsed_microseconds64(this->now());
    }

    ali::int64 total_units(
        ali::int64 units,
        ali::int64 seconds ) const
            //  return now - _start;
            //
            //          units "units" == seconds "seconds"
            //  E.g.:       1000 "ms" ==       1 "seconds"
            //            1 "minutes" ==      60 "seconds"
    {
        return elapsed_units(this->now(), units, seconds);
    }
    
    ali::int64 total_tick_count( void ) const
        //  return now - _start;
    {
        return elapsed_tick_count(this->now());
    }
        

    time::hours split_hours( void ) const
        //  return _split - _start;
    {
        return elapsed_hours(_split);
    }
    
    time::minutes split_minutes( void ) const
        //  return _split - _start;
    {
        return elapsed_minutes(_split);
    }
    
    time::seconds split_seconds( void ) const
        //  return _split - _start;
    {
        return elapsed_seconds(_split);
    }
    
    time::seconds64 split_seconds64( void ) const
        //  return _split - _start;
    {
        return elapsed_seconds64(_split);
    }
    
    time::milliseconds split_milliseconds( void ) const
        //  return _split - _start;
    {
        return elapsed_milliseconds(_split);
    }
    
    time::milliseconds64 split_milliseconds64( void ) const
        //  return _split - _start;
    {
        return elapsed_milliseconds64(_split);
    }
    
    time::microseconds64 split_microseconds64( void ) const
        //  return _split - _start;
    {
        return elapsed_microseconds64(_split);
    }

    ali::int64 split_units(
        ali::int64 units,
        ali::int64 seconds ) const
            //  return _split - _start;
            //
            //          units "units" == seconds "seconds"
            //  E.g.:       1000 "ms" ==       1 "seconds"
            //            1 "minutes" ==      60 "seconds"
    {
        return elapsed_units(_split, units, seconds);
    }
    
    ali::int64 split_tick_count( void ) const
        //  return _split - _start;
    {
        return elapsed_tick_count(_split);
    }

    void swap( stopwatch& b )
    {
        platform_stopwatch::swap(static_cast<platform_stopwatch&>(b));
    }

    friend void swap( stopwatch& a, stopwatch& b )
    {
        a.swap(b);
    }

private:    //  Methods
    time::hours elapsed_hours( monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::hours(to_int(
            elapsed_units(split, 1, 60 * 60)));
    }
    
    time::minutes elapsed_minutes( monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::minutes(to_int(
            elapsed_units(split, 1, 60)));
    }

    time::seconds elapsed_seconds( monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::seconds(to_int(
            elapsed_tick_count(split) / tick_frequency()));
    }

    time::seconds64 elapsed_seconds64( monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::seconds64(
            elapsed_tick_count(split) / tick_frequency());
    }

    time::milliseconds elapsed_milliseconds(
        monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::milliseconds(to_int(
            elapsed_units(split, 1000, 1)));
    }

    time::milliseconds64 elapsed_milliseconds64(
        monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::milliseconds64(
            elapsed_units(split, 1000, 1));
    }

    time::microseconds64 elapsed_microseconds64(
        monotonic_timestamp split ) const
        //  return split - _start;
    {
        return time::microseconds64(
            elapsed_units(split, 1000 * 1000, 1));
    }

    ali::int64 elapsed_units(
        monotonic_timestamp split,
        ali::int64 units,
        ali::int64 seconds ) const
    {
        return to_units(
            elapsed_tick_count(split),
            tick_frequency(),
            units, seconds);
    }

    static ali::int64 to_units(
        ali::int64 tick_count,
        ali::int64 tick_frequency,
        ali::int64 units,
        ali::int64 seconds )
    {
        //  (y units) .. (x seconds)
        //      units .. seconds
        //  _______________________________
        //  (y units) = (units * (x seconds)) / seconds
        //
        //  E.g.
        //
        //  (y milliseconds) .. (x seconds)
        //              1000 .. 1
        //  _______________________________
        //  (y milliseconds) / 1000 = (x seconds) / 1
        //  (y milliseconds) = (1000 * (x seconds)) / 1
        //
        //
        //  (y minutes) .. (x seconds)
        //            1 .. 60
        //  _______________________________
        //  (y minutes) / 1 = (x seconds) / 60
        //  (y minutes) = (1 * (x seconds)) / 60
        //
        //  _______________________________
        //
        //  (x seconds) = tick_count(split) / tick_frequency()
        //  (y units) = (units * tick_count(split)) / (tick_frequency() * seconds)
        //            = ((units * tick_count(split)) / seconds) / tick_frequency()
        //
        //  (a * b) / c
        //  b = (b / c) * c + (b % c)
        //
        //  (a * b) / c = (a * ((b / c) * c + (b % c))) / c
        //      = (a * ((b / c) * c) + a * (b % c)) / c
        //      = (a * ((b / c) * c)) / c + (a * (b % c)) / c
        //      = a * (b / c) + (a * (b % c)) / c
        //
    
        return  (units * (tick_count / tick_frequency)
                    + (units * (tick_count % tick_frequency))
                        / tick_frequency) / seconds;
            //  Here we are calculating
            //      ((units * tick_count) / tick_frequency) / seconds
            //  without the whole intermediate result of
            //      units * tick_count
    }

    static int to_int( ali::int64 value );
};

// ******************************************************************
// ******************************************************************

namespace current
{

// ******************************************************************
extern stopwatch const uptime;
// ******************************************************************

}   //  namespace current

}   //  namespace time

}   //  namespace ali
