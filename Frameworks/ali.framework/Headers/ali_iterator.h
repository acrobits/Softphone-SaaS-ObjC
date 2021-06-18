/*
 *  ali_iterator.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2018 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

namespace ali
{

// ******************************************************************
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// ******************************************************************


// ******************************************************************
template <class C, class T, class D = long>
struct iterator_base
// ******************************************************************
{
    typedef C iterator_category;
    typedef T value_type;
    typedef D distance_type;
};


// ******************************************************************
template <class It>
struct iterator_traits
// ******************************************************************
{
  typedef typename It::iterator_category iterator_category;
  typedef typename It::value_type value_type;
  typedef typename It::distance_type distance_type;
};


// ******************************************************************
template <class C, class T, class D>
inline C get_iterator_category( iterator_base<C, T, D> const& )
// ******************************************************************
{
  return C();
}

// ******************************************************************
template <class T>
inline random_access_iterator_tag get_iterator_category( T const* )
// ******************************************************************
{
  return random_access_iterator_tag();
}


// ******************************************************************
template <class C, class T, class D>
inline T* get_iterator_value_ptr( iterator_base<C, T, D> const& )
// ******************************************************************
{
  return static_cast<T*>(nullptr);
}

// ******************************************************************
template <class T>
inline T* get_iterator_value_ptr( T const* )
// ******************************************************************
{
  return static_cast<T*>(nullptr);
}


// ******************************************************************
template <class RI, class T, class R = T&, class P = T*, class D = long>
class reverse_iterator : public iterator_base<random_access_iterator_tag, T, D>
// ******************************************************************
{
  public: // Typedefs
    typedef reverse_iterator<RI, T, R, P, D> mytype;
    typedef RI iter_type;
    typedef R reference_type;
    typedef P pointer_type;

  public: // Methods
    reverse_iterator( void ) {}
    explicit reverse_iterator( RI x ) : _current(x) {}

    RI base( void ) const { return _current; }
    R operator*( void ) const {return *(_current - 1); }
    P operator->( void ) const { return &**this; }

    mytype& operator++( void )
    {
        --_current;
        return *this;
    }

    mytype operator++( int )
    {
        mytype t(*this);
        --_current;
        return t;
    }

    mytype& operator--( void )
    {
        ++_current;
        return *this;
    }

    mytype operator--( int )
    {
        mytype t(*this);
        ++_current;
        return t;
    }

    mytype& operator+=( D n )
    {
        _current -= n;
        return *this;
    }

    mytype operator+( D n ) const
    {
        return mytype(_current - n);
    }

    mytype& operator-=( D n )
    {
        _current += n;
        return *this;
    }

    mytype operator-( D n ) const
    {
        return mytype(_current + n);
    }

    R operator[]( D n ) const
    {
        return *(*this + n);
    }

  protected:
      RI _current;
};

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator==( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return x.base() == y.base();
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator!=( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return !(x == y);
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator<( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return y.base() < x.base();
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator>( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return y < x;
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator<=( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return !(y < x);
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline bool operator>=( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return !(x < y);
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline D operator-( reverse_iterator<RI, T, R, P, D> const& x,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return y.base() - x.base();
}

// ******************************************************************
template <class RI, class T, class R, class P, class D>
inline reverse_iterator<RI, T, R, P, D> operator+( D n,
    reverse_iterator<RI, T, R, P, D> const& y )
// ******************************************************************
{
    return reverse_iterator<RI, T, R, P, D>(y.base() - n);
}


} // namespace ali



