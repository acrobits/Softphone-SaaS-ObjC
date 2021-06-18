#pragma once
#include "ali/ali_debug.h"
#include "ali/ali_initializer_list.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
/// \brief  Models a semi-closed range (a.k.a. interval), using
/// its lower bound, called begin (first value belonging to the range)
/// and its upper bound, called end (first "higher" value NOT belonging
/// to the range).
///
/// \invariant  <tt>this->begin precedes or is equal to this->end</tt>
///
// ******************************************************************
template <typename T>
class range
// ******************************************************************
{
public:
        /// \brief  Default constructor. Constructs empty range.
        ///
        /// \post   <tt>this->is_empty()</tt>
        ///
        /// \throws noexcept <tt>(noexcept(T()))</tt>
        ///
    range( void )
    :   _begin(),
        _end()
    {
        ali_assert(this->is_empty());
    }

#if defined(ALI_FEATURE_INITIALIZER_LIST)
        /// \brief  Initializes this range from an initializer list
        ///
        /// \param[in]  list
        ///             The initializer list
        ///
        /// \post   <tt>this->begin == list.begin() && this->end == list.end()</tt>
        ///
        /// \throws noexcept <tt>(noexcept(T(T)))</tt>
        ///
    template <typename U>
    range( ali::initializer_list<U> list )
    :   _begin(list.begin()),
        _end(list.end())
    {
        ali_assert(this->_begin <= this->_end);
    }
#endif

        /// \brief  Initializes this range with the given bounds.
        ///
        /// \param[in]  begin
        ///             The lower bound of the range.
        /// \param[in]  end
        ///             The upper bound of the range.
        ///
        /// \pre    <tt>begin precedes or is equal to end</tt>
        ///
        /// \post   <tt>this->begin == begin && this->end == end</tt>
        ///
        /// \throws noexcept <tt>(noexcept(T(T)))</tt>
        ///
    range( T begin, T end )
    :   _begin(begin),
        _end(end)
    {
        ali_assert(this->_begin <= this->_end);
    }

        /// \brief  Determines whether the two given ranges have
        /// the same bounds.
        ///
        /// \param[in]  a
        ///             Range to compare.
        /// \param[in]  b
        ///             Range to compare.
        ///
        /// \retval true
        ///         The two ranges have the same bounds.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator==(T,T)))</tt>
        ///
    friend bool operator==( range a, range b )
    {
        return  a._begin == b._begin
            &&  a._end == b._end;
    }

        /// \brief  Determines whether the two given ranges have
        /// different bounds.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two ranges to compare.
        ///
        /// \retval true
        ///         The two ranges have different bounds.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator==(T,T)))</tt>
        ///
    friend bool operator!=( range a, range b )
    {
        return  !(a == b);
    }

        /// \brief  Determines whether this range contains any values.
        ///
        /// \post   <tt>result == (this->begin == this->end)</tt>
        ///
        /// \retval true
        ///         This range contains no values.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator==(T,T)))</tt>
        ///
    bool is_empty( void ) const
    {
        return this->_begin == this->_end;
    }

        /// \brief  Returns the lower bound of this range.
        ///
        /// \return The lower bound this range.
        ///
    T const& begin( void ) const
    {
        return this->_begin;
    }

        /// \brief  Returns the lower bound of this range.
        ///
        /// \return The lower bound this range.
        ///
    T& begin( void )
    {
        return this->_begin;
    }

        /// \brief  Returns the upper bound of this range.
        ///
        /// \return The upper bound of this range.
        ///
    T const& end( void ) const
    {
        return this->_end;
    }

        /// \brief  Returns the upper bound of this range.
        ///
        /// \return The upper bound of this range.
        ///
    T& end( void )
    {
        return this->_end;
    }

    friend T const& begin( range const& r )
    {
        return r._begin;
    }

    friend T& begin( range& r )
    {
        return r._begin;
    }

    friend T const& end( range const& r )
    {
        return r._end;
    }

    friend T& end( range& r )
    {
        return r._end;
    }

        /// \brief  Swaps content of this range with another one.
        ///
        /// \param[in]  b
        ///             Range to swap content with.
        ///
        /// \post
        ///             pre(this)->begin == b.begin
        ///         &&  pre(this)->end == b.end
        ///         &&  this->begin == pre(b).begin
        ///         &&  this->end == pre(b).end
        ///
        /// \throws noexcept
        ///
    void swap( range& b )
    {
        using ali::swap;
        swap(this->_begin, b._begin);
        swap(this->_end, b._end);
    }

        /// \brief  See \ref range::swap.
        ///
    friend void swap( range& a, range& b )
    {
        a.swap(b);
    }

        /// \brief  Determines whether the two given ranges
        /// contain no common value. I.o.w whether their
        /// intersection is an empty range.
        ///
        /// \param[in]  a
        ///             Range to compare.
        /// \param[in]  b
        ///             Range to compare.
        ///
        /// \pre
        ///             T is regular type
        ///         &&  a.begin <= a.end
        ///         &&  b.begin <= b.end
        ///
        /// \retval true
        ///         The two ranges contain no common value.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator<=(T,T)))</tt>
        ///
    friend bool are_disjoint( range a, range b )
    {
        ali_assert(a._begin <= a._end);
        ali_assert(b._begin <= b._end);

        return  a._end <= b._begin
                    //  a is before b
            ||  b._end <= a._begin;
                    //  b is before a
    }

private:    //  Data members
    T   _begin;
        ///<        The lower bound of this range.
        ///         The first value belonging in this range.

    T   _end;
        ///<        The upper bound of this range.
        ///         The first "higher" value NOT belonging in this range.
};


}   //  namespace ali
