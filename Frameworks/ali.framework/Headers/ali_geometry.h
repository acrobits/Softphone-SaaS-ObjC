//
//  ali_geometry.h
//
//  Copyright (c) 2011 - 2017 Acrobits, s.r.o. All rights reserved.
//

#pragma once

#include "ali/ali_utility.h"

namespace ali
{
    // ******************************************************************
    template<class T>
    struct point
    // ******************************************************************
    {
        point( void )
        :_x{0},_y{0}{}

        point( T x, T y )
        :_x{x},_y{y}{}

        template <class TT>
        explicit point( point<TT> const& other )
            :_x{static_cast<T>(other.x())}
            ,_y{static_cast<T>(other.y())}
        {}

        point( point const& other ) = default;

        T x( void ) const {return _x;}
        T y( void ) const {return _y;}

        bool operator ==( point const& p ) const
        {
            return _x == p._x && _y == p._y;
        }

        bool operator !=( point const& p ) const
        {
            return !operator ==(p);
        }

        [[nodiscard]]
        point transpose( void ) const
        {
            return {_y, _x};
        }

    private:
        T _x;
        T _y;
    };

    // ******************************************************************
    template<class T>
    struct size
    // ******************************************************************
    {
        size( void )
        :_w{0},_h{0}{}

        size( T w, T h )
        :_w{w},_h{h}{}

        template <class TT>
        explicit size( size<TT> const& other )
            :_w{static_cast<T>(other.w())}
            ,_h{static_cast<T>(other.h())}
        {}

        size( size const& other ) = default;

        T w( void ) const {return _w;}
        T h( void ) const {return _h;}

        [[nodiscard]]
        size transpose( void ) const
        {
            return {_h, _w};
        }

        void swap( size & b )
        {
            using ali::swap;

            swap(_w, b._w);
            swap(_h, b._h);
        }

        friend void swap( size & a,
                          size & b )
        {
            a.swap(b);
        }

        bool operator ==( size const& s ) const
        {
            return _w == s._w && _h == s._h;
        }

        bool operator !=( size const& s ) const
        {
            return !operator ==(s);
        }

        bool is_zero( void )
        {
            return _w == 0 && _h == 0;
        }

    private:
        T _w;
        T _h;
    };

    // ******************************************************************
    template<class T>
    struct rect
    // ******************************************************************
    {
    public:
        rect( void ){}

        rect( T x, T y, T w, T h )
        :_origin{x,y},_size{w,h}{}

        rect( point<T> const& origin, ali::size<T> const& size )
        :_origin{origin},_size{size}{}

        rect( point<T> const& from, point<T> const& to )
        :_origin{from},_size{to.x() - from.x(), to.y() - from.y()}{}

        template <class TT>
        explicit rect( rect<TT> const& other )
            :_origin{point<T>{other.origin()}}
            ,_size{ali::size<T>{other.size()}}
        {}

        rect( rect const& other ) = default;

        point<T> const& origin( void ) const {return _origin;}
        ali::size<T> const& size( void ) const {return _size;}

        bool operator ==( rect const& r ) const
        {
            return _origin == r._origin && _size == r._size;
        }

        bool operator !=( rect const& r ) const
        {
            return !operator ==(r);
        }

        [[nodiscard]]
        rect transpose( void ) const
        {
            return {origin().transpose(), size().transpose()};
        }

    private:
        point<T> _origin;
        ali::size<T> _size;
    };
}

