/*
 *  ali_array_set.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_algorithm_common_bounds.h"
#include "ali/ali_array_set_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_deprecated.h"
#include "ali/ali_initializer_list.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
/// \brief  Manages a collection of objects ensuring that equivalent
/// ones are stored only once.
///
/// \tparam T
///         Type of contained elements.
/// \tparam comparator_type
///         Type of the comparator used to determine whether
///         two elements are equivalent.
///
/// \pre    Object of the `comparator_type` type is a binary function
///         accepting T as one of its parameters and implementing
///         a strict weak ordering.
///
/// \invariant
///
///             0   <= this->size()
///
///         &&  this->size() <= this->capacity()
///
///         &&  this->capacity() <= this->max_size()
///
///         &&  this->is_empty() == (this->size() == 0)
///
///         &&  forall i, j in set {0, ..., this->size() - 1} &
///                 (i == j) == this->are_equivalent(this->at(i), this->at(j))
///
///         &&  forall i in set {1, ..., this->size() - 1} &
///                 this->is_less(this->at(i - 1), this->at(i))
///
/// \anchor ali_set_element_modification
/// \remark As can be seen from the invariant, the `array_set` class
/// stores its elements in an array ordered using the specified
/// comparator. When working with the set's elements a user must
/// exercise care not to change fields that influence element
/// order in a way that would break the set class' invariant.
/// Deliberate use of `ali_assert(set.invariant())` is encouraged
/// after such manipulations.
///
// ******************************************************************
template <typename T, typename comparator_type>
class array_set : private comparator_type
// ******************************************************************
{
    static_assert(
        meta::comparator_returns_int<comparator_type, T>::result,
        "The comparator parameter has changed from 'bool is_less(T,T)' to 'int compare(T,T)'.\n"
        "See ali::default_comparator for sample implementation.");

public:
    typedef T                                       value_type;
        ///<        Type of contained elements.

    typedef typename ali::array<T>::iterator        iterator;
        ///<\brief  Provides pointer-like read-write
        /// access to individual elements contained
        /// in a `set` object.

    typedef typename ali::array<T>::const_iterator   const_iterator;
        ///<\brief  Provides pointer-like read-only
        /// access to individual elements contained
        /// in a `set` object.

    typedef typename ali::array<T>::range_type       range_type;
    typedef typename ali::array<T>::const_range_type const_range_type;

public:

        /// \brief  Default constructor. Constructs empty set.
        /// \post   <tt>this->is_empty()</tt>
        /// \throws noexcept
        ///
    array_set( void ) {}

        /// \brief  Initializes the comparator and constructs empty
        /// set.
        ///
        /// \param[in]  compare
        ///             The comparator to use for ordering this set's
        ///             elements.
        ///
        /// \post   <tt>this->comparator() == compare && this->is_empty()</tt>
        ///
        /// \throws noexcept <tt>(noexcept(comparator_type(comparator_type)))</tt>
        ///
    explicit array_set( comparator_type const& compare )
    :   comparator_type(compare)
    {}

        /// \brief  Populates this set with the given element.
        ///
        /// \param[in]  value
        ///             Element to insert into this set.
        ///
        /// \post   <tt>post this->insert(value)</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit array_set( T const& value )
    {
        this->insert(value);
    }

        /// \brief  Initializes the comparator and populates this
        /// set with the given element.
        ///
        /// \param[in]  compare
        ///             The comparator to use for ordering this set's
        ///             elements.
        /// \param[in]  value
        ///             Element to insert into this set.
        ///
        /// \post   <tt>this->comparator() == compare && post this->insert(value)</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_set( comparator_type const& compare, T const& value )
    :   comparator_type(compare)
    {
        this->insert(value);
    }

        /// \brief  Populates this set with elements of the given
        /// \ref array.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \param[in]  b
        ///             Array of elements to populate this set with.
        ///
        /// \post   <tt>post this->insert(b)</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit array_set( array_const_ref<T> b )
    {
        this->insert(b);
    }

    array_set( ali::initializer_list<T> b )
    {
        ALI_IF_DEBUG(int const inserted =)
			this->insert(b);
        ALI_IF_DEBUG(int const list_size = static_cast<int>(b.size()));
        ali_assert(inserted == list_size);
    }

        /// \brief  Initializes the comparator and populates this set
        /// with elements of the given \ref array.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \param[in]  compare
        ///             The comparator to use for ordering this set's
        ///             elements.
        /// \param[in]  b
        ///             Array of elements to populate this set with.
        ///
        /// \post   <tt>this->comparator() == compare && post this->insert(b)</tt>
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_set( comparator_type const& compare, array_const_ref<T> b )
    :   comparator_type(compare)
    {
        this->insert(b);
    }

#ifdef  ALI_FEATURE_RVALUE_REF

    array_set( array_set const& b )
    :   comparator_type(b.comparator()),
        _arr(b._arr)
    {}

    array_set( array_set&& b )
    :   comparator_type(ali::move(b.comparator())),
        _arr(ali::move(b._arr))
    {}

    array_set& operator=( array_set const& b )
    {
        static_cast<comparator_type&>(*this) = b.comparator();
        this->_arr = b._arr;
        return *this;
    }

    array_set& operator=( array_set&& b )
    {
        static_cast<comparator_type&>(*this) = ali::move(
            static_cast<comparator_type&>(b));
        this->_arr = ali::move(b._arr);
        return *this;
    }

#endif  //  ALI_FEATURE_RVALUE_REF

        /// \brief Determines whether the two given sets contain
        /// equal elements, as determined by `operator==(T,T)`.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two sets to compare.
        ///
        /// \retval true
        ///         The two sets contain equal elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator==(T,T)))</tt>
        ///
    friend bool operator==( array_set const& a, array_set const& b )
    {
        if ( a.size() != b.size() )
            return false;

        for ( int i = a.size(); i != 0; )
            if ( --i, !(a[i] == b[i]) )
                return false;

        return true;
    }

        /// \brief Determines whether the two given sets contain
        /// non-equal elements, as determined by `operator==(T,T)`.
        ///
        /// \param[in]  a
        /// \param[in]  b
        ///             Two sets to compare.
        ///
        /// \retval true
        ///         The two sets contain non-equal elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(operator==(T,T)))</tt>
        ///
    friend bool operator!=( array_set const& a, array_set const& b )
    {
        return !(a == b);
    }

        /// \brief  Verifies that the set class' invariant holds.
        ///
        /// \retval true
        ///         The set class' invariant holds.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(this->is_less(T,T)))</tt>
        ///
    bool invariant( void ) const
    {
        bool success
            =   0 <= this->size()
            &&  this->size() <= this->capacity()
            &&  this->capacity() <= this->max_size()
            &&  this->is_empty() == (this->size() == 0);

/*      //  Potentially too expensive.
        for ( int i = 0; success && i != this->size(); ++i )
            for ( int j = 0; success && j != this->size(); ++j )
                success = (i == j) == this->are_equivalent(
                    this->at(i), this->at(j));
*/
        if ( this->size() > 1 )
            for ( int i = 1; success && i != this->size(); ++i )
                success = this->is_less(this->at(i - 1), this->at(i));

        return success;
    }

        /// \brief  Determines whether this set contains any
        /// elements.
        ///
        /// \retval true
        ///         This set contains no elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_empty( void ) const
    {
        return this->_arr.is_empty();
    }


        /// \brief Returns the maximum number of
        /// elements this set can potentially hold.
        ///
        /// \returns The maximum number of elements
        /// this set can potentially hold.
        ///
        /// \throws noexcept
        ///
    static int max_size( void )
    {
        return ali::array<T>::max_size();
    }

        /// \brief  Returns the number of elements
        /// currently stored in this set.
        ///
        /// \returns The number of elements currently
        /// stored in this set.
        ///
        /// \throws noexcept
        ///
    int size( void ) const
    {
        return this->_arr.size();
    }

        /// \brief  Returns the number of elements this
        /// set can accommodate without triggering
        /// memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee is
        /// valid only for the memory managed by the set
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// The capacity of a set object can be
        /// increased using the \ref reserve method.
        ///
        /// \returns The number of elements this set
        /// can accommodate without triggering memory
        /// allocation.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref reserve
        ///
    int capacity( void ) const
    {
        return this->_arr.capacity();
    }

        /// \brief  Increases capacity of this set.
        ///
        /// Ensures that this set will be able to
        /// accommodate at least `cap` elements without
        /// triggering memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee
        /// is valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// \note   This method affects only the \ref capacity
        /// of this set, not its \ref size.
        ///
        /// \note   If `cap <= this->capacity()` then this
        /// method has no effect.
        ///
        /// \param[in]  cap
        ///             The desired number of elements this
        ///             set will reserve space for.
        ///
        /// \pre    <tt>0 <= cap</tt>
        ///
        /// \post   <tt>cap <= this->capacity()
        /// && this->capacity() <= this->max_size()</tt>
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_set& reserve( int cap )
    {
        this->_arr.reserve(cap);

        return *this;
    }

        /// \brief  Returns index of element in this set that is
        /// equivalent with the given value.
        ///
        /// \tparam     U
        ///             Type of the given value.
        /// \param[in]  value
        ///             Value to search for.
        ///
        /// \post
        ///
        ///             result == this->size()
        ///         ||  this->are_equivalent(this->at(result), value)
        ///
        /// \returns Index of element in this set that is
        /// equivalent with the given value or `this->size()`
        /// if there is no such element.
        ///
        /// \throws noexcept <tt>(noexcept(this->is_less(T, U)))</tt>
        ///
    template <typename U>
    int index_of( U const& value ) const
    {
        int const idx = this->index_of_lower_bound(value);

        return  idx != this->size()
            &&  this->are_equivalent(this->_arr[idx], value)
                    ? idx : this->size();
    }

    template <typename U>
    int index_of_lower_bound( U const& value ) const
    {
        return generic_index_of_lower_bound(
            this->_arr.size(),
            [this] ( int idx ) -> T const&
            { return this->_arr[idx]; },
            value,
            this->comparator());
    }

        /// \brief  Determines whether this set contains an element
        /// that is equivalent with the given value.
        ///
        /// \tparam     U
        ///             Type of the given value.
        /// \param[in]  value
        ///             Value to search for.
        ///
        /// \post
        ///
        ///         result == exists i in set {0, ..., this->size() - 1} &
        ///             this->are_equivalent(this->at(i), value)
        ///
        /// \retval true
        ///         This set contains an element that is equivalent
        ///         with the given value.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(this->is_less(T, U)))</tt>
        ///
    template <typename U>
    bool contains( U const& value ) const
    {
        return this->index_of(value) != this->size();
    }

        /// \brief  Provides read-only iterator
        /// pointing to the beginning of this set.
        ///
        /// \returns \ref const_iterator object
        /// pointing to the beginning of this set.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator begin( void ) const
    {
        return this->_arr.begin();
    }

    friend const_iterator begin( array_set const& s )
    {
        return s.begin();
    }

        /// \brief  Provides read-write iterator
        /// pointing to the beginning of this set.
        ///
        /// \returns \ref iterator object pointing
        /// to the beginning of this set.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    iterator begin( void )
    {
        return this->_arr.begin();
    }

    friend iterator begin( array_set & s )
    {
        return s.begin();
    }

        /// \brief  Provides read-only iterator pointing to
        /// the one past the last element of this set.
        ///
        /// \returns \ref const_iterator object pointing to
        /// the one past the last element of this set.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator end( void ) const
    {
        return this->_arr.end();
    }

    friend const_iterator end( array_set const& s )
    {
        return s.end();
    }

        /// \brief  Provides read-write iterator pointing to
        /// the one past the last element of this set.
        ///
        /// \returns \ref iterator object pointing to
        /// the one past the last element of this set.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    iterator end( void )
    {
        return this->_arr.end();
    }

    friend iterator end( array_set & s )
    {
        return s.end();
    }

    const_range_type range( void ) const
    {
        return const_range_type(this->begin(), this->end());
    }

    range_type range( void )
    {
        return range_type(this->begin(), this->end());
    }

        /// \brief  Provides read-only access to
        /// individual elements of this set.
        ///
        /// \param[in]  i
        ///             Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        ///
    T const& operator[]( int i ) const
    {
        return this->_arr[i];
    }

        /// \brief  Provides read-write access to
        /// individual elements of this set.
        ///
        /// \param[in]  i
        ///             Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    T& operator[]( int i )
    {
        return this->_arr[i];
    }

        /// \brief  Provides read-only access to
        /// individual elements of this set.
        ///
        /// \param[in]  i
        ///             Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method verifies that the given
        /// index `i` is in valid range and throws an
        /// exception if it isn't.
        ///
        /// \see    \ref operator[]
        ///
    T const& at( int i ) const
    {
        return this->_arr.at(i);
    }

        /// \brief  Provides read-write access to
        /// individual elements of this set.
        ///
        /// \param[in]  i
        ///             Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method verifies that the given
        /// index `i` is in valid range and throws an
        /// exception if it isn't.
        ///
        /// \see    \ref operator[]
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    T& at( int i )
    {
        return this->_arr.at(i);
    }

        /// \brief  Provides read-only access
        /// to the first element of this set.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// first element of this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method throws an
        /// exception if the set is empty.
        ///
        /// \see    \ref unchecked_front
        ///
    T const& front( void ) const
    {
        return this->_arr.front();
    }

        /// \brief  Provides read-write access
        /// to the first element of this set.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the first
        /// element of this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method throws an
        /// exception if the set is empty.
        ///
        /// \see    \ref unchecked_front
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    T& front( void )
    {
        return this->_arr.front();
    }

        /// \brief  Provides read-only access
        /// to the last element of this set.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// last element of this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method throws an
        /// exception if the set is empty.
        ///
        /// \see    \ref unchecked_back
        ///
    T const& back( void ) const
    {
        return this->_arr.back();
    }

        /// \brief  Provides read-write access
        /// to the last element of this set.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the last
        /// element of this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \remark This method throws an
        /// exception if the set is empty.
        ///
        /// \see    \ref unchecked_back
        /// \see    [Set element modification](\ref ali_set_element_modification).
        ///
    T& back( void )
    {
        return this->_arr.back();
    }

        /// \brief  Inserts a new element into this set, provided
        /// it doesn't already contain an equivalent element.
        ///
        /// \param[in]  t
        ///             Element to insert into this set.
        ///
        /// \post
        ///
        ///             (   this->size() == pre(this)->size()
        ///             ||  this->size() == pre(this)->size() + 1)
        ///
        ///         &&  result == (this->size() == pre(this)->size() + 1)
        ///
        ///         &&  exists i in set {0, ..., this->size() - 1} &
        ///                     this->are_equivalent(this->at(i), t)
        ///                 &&  (   result
        ///                     ||  this->at(i) is not changed in any way)
        ///
        /// \retval true
        ///         The new element was inserted into this set.
        /// \retval false
        ///         This set already contains an equivalent element,
        ///         which was not modified in any way by this method.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert_or_update(T const&)
        ///
    bool insert( T const& t, int* index = nullptr )
    {
        int const idx = this->index_of_lower_bound(t);

        bool const not_in_set
            =   idx == this->size()
            ||  !this->are_equivalent(
                    this->_arr[idx], t);

        if ( not_in_set )
            this->_arr.insert(idx, t);

        if ( index != nullptr )
            *index = idx;

        return not_in_set;
    }

    bool insert( T&& t, int* index = nullptr )
    {
        int const idx = this->index_of_lower_bound(t);

        bool const not_in_set
            =   idx == this->size()
            ||  !this->are_equivalent(
                    this->_arr[idx], t);

        if ( not_in_set )
            this->_arr.insert(idx, ali::move(t));

        if ( index != nullptr )
            *index = idx;

        return not_in_set;
    }
    
        /// \brief  Inserts elements of the given \ref array into
        /// this set, provided it doesn't already contain an
        /// equivalent element.
        ///
        /// \note   If the given array contains multiple equivalent
        /// elements, as determined by the \ref are_equivalent method,
        /// then it is unspecified which one is actually inserted
        /// into this set.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \param[in]  b
        ///             Array of elements to insert into this set.
        ///
        /// \post
        ///
        ///             pre(this)->size() <= this->size()
        ///
        ///         &&  this->size() <= pre(this)->size() + b.size()
        ///
        ///         &&  result = this->size() - pre(this)->size()
        ///
        /// \returns Number of newly inserted elements.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert(T const&)
        ///
    template <typename U>
    int insert_others( array_const_ref<U> b )
    {
        int const pre_size = this->size();

        for ( int i = 0; i != b.size(); ++i )
            this->insert(b[i]);

        return this->size() - pre_size;
    }

    int insert( array_const_ref<T> b )
    {
        return this->insert_others(b);
    }

    int insert( ali::initializer_list<T> b )
    {
        int const pre_size = this->size();

        for ( auto const& element : b )
            this->insert(element);

        return this->size() - pre_size;
    }

        /// \brief  Updates an element in this set
        ///
        /// \tparam     updater
        ///             Type of the functor called to update an element.
        /// \param[in]  t
        ///             Element to update an equivalent element in this set
        ///             with.
        /// \param[in]  u
        ///             Functor called to update the element. It has two
        ///             parameters:
        ///               # reference to the original element
        ///               # constant reference to the element to update with
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size()
        ///
        ///         &&  result == (this->contains(t))
        ///
        ///         &&  !result
        ///                 || (  exists i in set {0, ..., this->size() - 1}
        ///                          && this->are_equivalent(this->at(i), t) )
        ///
        /// \retval true
        ///         The element was updated in this set.
        /// \retval false
        ///         There is no equivalent element in this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert(T const&)
        /// \see    \ref insert_or_update(T const&, updater const&)
        ///
    template <typename updater>
    bool update( T const& t, updater const& u )
    {
        int const idx = this->index_of_lower_bound(t);

        bool const not_in_set
            =   idx == this->size()
            ||  !this->are_equivalent(
                    this->_arr[idx], t);

        if ( not_in_set )
            return false;

        u(this->_arr[idx], t);
        
        return true;
    }

        /// \brief  Updates an element in this set
        ///
        /// \param[in]  t
        ///             Element to update an equivalent element in this set
        ///             with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size()
        ///
        ///         &&  result == (this->contains(t))
        ///
        ///         &&  !result
        ///                 || (  exists i in set {0, ..., this->size() - 1}
        ///                          && this->are_equivalent(this->at(i), t)
        ///                          && this->at(i) is a copy of t )
        ///
        /// \retval true
        ///         The element was updated in this set.
        /// \retval false
        ///         There is no equivalent element in this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref update(T const&, updater const& u)
        ///
    bool update( T const& t )
    {
        return this->update(t, [](T & elem, T const& t){ elem = t; });
    }

        /// \brief Updates elements in this set with equal elements specified
        /// in the given \ref array
        ///
        /// \note   If the given array contains multiple equivalent
        /// elements, as determined by the \ref are_equivalent method,
        /// then it is unspecified which one is used to make the
        /// last update of this set's element.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \tparam     updater
        ///             Type of the functor called to update an element.
        /// \param[in]  b
        ///             Array of elements to update equivalent elements with.
        /// \param[in]  u
        ///             Functor called to update an element. It has two
        ///             parameters:
        ///               # reference to the original element
        ///               # constant reference to the element to update with
        ///
        /// \post
        ///
        ///             pre(this)->size() == this->size()
        ///
        ///         &&  result >= 0
        ///
        ///         &&  result <= b.size()
        ///
        /// \returns Number of updated elements.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref update(T const&)
        ///
    template <typename U, typename updater>
    int update_others( array_const_ref<U> b, updater const& u )
    {
        int updated = 0;

        for ( int i = 0; i != b.size(); ++i )
            updated += this->update(b[i], u);

        return updated;
    }

    template <typename updater>
    int update( array_const_ref<T> b, updater const& u )
    {
        return this->update_others(b, u);
    }

        /// \brief Updates elements in this set with equal elements specified
        /// in the given \ref array
        ///
        /// \note   If the given array contains multiple equivalent
        /// elements, as determined by the \ref are_equivalent method,
        /// then it is unspecified which one is used to make the
        /// last update of this set's element.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \param[in]  b
        ///             Array of elements to update equivalent elements with.
        ///
        /// \post
        ///
        ///             pre(this)->size() == this->size()
        ///
        ///         &&  result >= 0
        ///
        ///         &&  result <= b.size()
        ///
        /// \returns Number of updated elements.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref update(ali::array const&, updater const&)
        ///
    template <typename U>
    int update_others( array_const_ref<U> b )
    {
        return this->update_others(
            b, [](T & elem, T const& t){ elem = t; });
    }

    int update( array_const_ref<T> b )
    {
        return this->update_others(b);
    }

        /// \brief  Inserts a new element into this set, or updates
        /// an equivalent element if there is one.
        ///
        /// \tparam     updater
        ///             Type of the functor called to update an element.
        /// \param[in]  t
        ///             Element to insert into this set.
        /// \param[in]  u
        ///             Functor called to update the element. It has two
        ///             parameters:
        ///               # reference to the original element
        ///               # constant reference to the element to update with
        ///
        /// \post
        ///
        ///             (   this->size() == pre(this)->size()
        ///             ||  this->size() == pre(this)->size() + 1)
        ///
        ///         &&  result == (this->size() == pre(this)->size() + 1)
        ///
        ///         &&  exists i in set {0, ..., this->size() - 1}
        ///                  && this->are_equivalent(this->at(i), t)
        ///
        /// \retval true
        ///         The new element was inserted into this set.
        /// \retval false
        ///         The new element was used to update an equivalent
        ///         element already contained in this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert(T const&)
        /// \see    \ref update(T const&)
        ///
    template <typename updater>
    bool insert_or_update( T const& t, updater const& u )
    {
        int const idx = this->index_of_lower_bound(t);

        bool const not_in_set
            =   idx == this->size()
            ||  !this->are_equivalent(
                    this->_arr[idx], t);

        if ( not_in_set )
            this->_arr.insert(idx, t);
        else
            u(this->_arr[idx], t);

        return not_in_set;
    }

        /// \brief  Inserts a new element into this set, or updates
        /// an equivalent element if there is one.
        ///
        /// \param[in]  t
        ///             Element to insert into this set.
        ///
        /// \post
        ///
        ///             (   this->size() == pre(this)->size()
        ///             ||  this->size() == pre(this)->size() + 1)
        ///
        ///         &&  result == (this->size() == pre(this)->size() + 1)
        ///
        ///         &&  exists i in set {0, ..., this->size() - 1}
        ///                   && this->are_equivalent(this->at(i), t)
        ///                   && this->at(i) is a copy of t
        ///
        /// \retval true
        ///         The new element was inserted into this set.
        /// \retval false
        ///         The new element was used to update an equivalent
        ///         element already contained in this set.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert_or_update(T const&, updater const&)
        ///
    bool insert_or_update( T const& t )
    {
        return this->insert_or_update(t, [](T & elem, T const& t){ elem = t; });
    }

        /// \brief  Inserts elements of the given \ref array into
        /// this set or updates equivalent elements that are already
        /// contained in this set.
        ///
        /// \note   If the given array contains multiple equivalent
        /// elements, as determined by the \ref are_equivalent method,
        /// then it is unspecified which one is used to make the
        /// last update of this set's element.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \tparam     updater
        ///             Type of the functor called to update an element.
        /// \param[in]  b
        ///             Array of elements to insert into this set
        ///             or update equivalent elements with.
        /// \param[in]  u
        ///             Functor called to update an element. It has two
        ///             parameters:
        ///               # reference to the original element
        ///               # constant reference to the element to update with
        ///
        /// \post
        ///
        ///             pre(this)->size() <= this->size()
        ///
        ///         &&  this->size() <= pre(this)->size() + b.size()
        ///
        ///         &&  result = this->size() - pre(this)->size()
        ///
        /// \returns Number of newly inserted elements.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert_or_update(T const&)
        ///
    template <typename U, typename updater>
    int insert_or_update_others( array_const_ref<U> b, updater const& u )
    {
        int const pre_size = this->size();

        for ( int i = 0; i != b.size(); ++i )
            this->insert_or_update(b[i], u);

        return this->size() - pre_size;
    }

    template <typename updater>
    int insert_or_update( array_const_ref<T> b, updater const& u )
    {
        return this->insert_or_update_others(b, u);
    }

        /// \brief  Inserts elements of the given \ref array into
        /// this set or updates equivalent elements that are already
        /// contained in this set.
        ///
        /// \note   If the given array contains multiple equivalent
        /// elements, as determined by the \ref are_equivalent method,
        /// then it is unspecified which one is used to make the
        /// last update of this set's element.
        ///
        /// \tparam     U
        ///             Type of elements contained in the given array.
        /// \param[in]  b
        ///             Array of elements to insert into this set
        ///             or update equivalent elements with.
        ///
        /// \post
        ///
        ///             pre(this)->size() <= this->size()
        ///
        ///         &&  this->size() <= pre(this)->size() + b.size()
        ///
        ///         &&  result = this->size() - pre(this)->size()
        ///
        /// \returns Number of newly inserted elements.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref insert_or_update(ali::array const&, updater const&)
        ///
    template <typename U>
    int insert_or_update_others( array_const_ref<U> b )
    {
        return this->insert_or_update_others(
            b, [](T & elem, T const& t){ elem = t; });
    }

    int insert_or_update( array_const_ref<T> b )
    {
        return this->insert_or_update_others(b);
    }

        /// \brief  Erases element that is equivalent with the
        /// given value from this set.
        ///
        /// \tparam     U
        ///             Type of the given value.
        /// \param[in]  value
        ///             Value to search for.
        ///
        /// \post
        ///
        ///             (   this->size() == pre(this)->size()
        ///             ||  this->size() == pre(this)->size() - 1)
        ///
        ///         &&  result == (this->size() == pre(this)->size() - 1)
        ///
        ///         &&  !exists i in set {0, ..., this->size() - 1} &
        ///                 this->are_equivalent(this->at(i), value)
        ///
        /// \retval true
        ///         An equivalent element was erased from this set.
        /// \retval false
        ///         No equivalent element was found in this set.
        ///
        /// \throws noexcept <tt>(noexcept(this->index_of(value))
        /// && noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    template <typename U>
    bool erase( U const& value )
    {
        int const idx = this->index_of(value);

        bool const in_set = idx != this->size();

        if ( in_set )
            this->erase_at(idx);

        return in_set;
    }

        /// \brief  Erases elements at the specified position.
        ///
        /// \param[in]  i
        ///             Index of the first element to erase.
        /// \param[in]  n
        ///             Number of elements to erase.
        ///
        /// \pre
        ///
        ///             0 <= i && i <= this->size()
        ///         &&  0 <= n && n <= this->size() - i
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                 this->at(j) is not changed in any way
        ///
        ///         &&  forall j in set {i, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j + n)
        ///
        ///         &&  forall j in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(j) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
        ///
    array_set& erase_at( int i, int n = 1 )
    {
        this->_arr.erase(i, n);

        return *this;
    }

        /// \brief  Erases elements from the beginning of this set.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is moved from pre(this)->at(i + n)
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    array_set& erase_front( int n = 1 )
    {
        this->_arr.erase_front(n);

        return *this;
    }

        /// \brief  Erases elements from the end of this set.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is not changed in any way
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    array_set& erase_back( int n = 1 )
    {
        this->_arr.erase_back(n);

        return *this;
    }

        /// \brief  Erases elements matching the specified predicate.
        ///
        /// \tparam predicate
        ///         The predicate's type.
        /// \param  p
        ///         The predicate; an element `t` is erased
        ///         if `p(t)` evaluates to `true`.
        ///
        /// \pre    For an object `t` of type `T`, `p(t)` is valid
        ///         expression convertible to `bool`.
        ///
        /// \post
        ///
        ///             result == pre(this)->size() - this->size()
        ///
        ///         &&  let original_indexes_of_retained_elements
        ///                     = [i | 0 <= i && i < pre(this)->size() && !p(pre(this)->at(i))]
        ///                         //  This is a sequence, not a set -- order is important.
        ///
        ///             in      this->size() == len original_indexes_of_retained_elements
        ///
        ///                 &&  forall i in set {0, ..., this->size() - 1} &
        ///                             i == original_indexes_of_retained_elements(i)
        ///                         ||  this->at(i) is moved from pre(this)->at(
        ///                                 original_indexes_of_retained_elements(i))
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns The number of erased elements.
        ///
        /// \throws noexcept <tt>(noexcept(p(T) && noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    template <typename predicate>
    int erase_if( predicate p )
    {
        return this->_arr.erase_if(p);
    }


        /// \brief  Erases all elements contained in this set.
        ///
        /// \post
        ///
        ///             this->is_empty()
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1}
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    array_set& erase( void )
    {
        this->_arr.erase();

        return *this;
    }


        /// \brief  Swaps contents of this set with another one.
        ///
        /// \param  b
        ///         Another set to swap content with.
        ///
        /// \post
        ///
        ///                 this->size() == pre(b).size()
        ///
        ///             &&  forall i in set {0, ..., this->size() - 1} &
        ///                     &this->at(i) == &pre(b).at(i)
        ///
        ///             &&  b.size() == pre(this)->size()
        ///
        ///             &&  forall i in set {0, ..., b.size() - 1} &
        ///                     &b.at(i) == &pre(this)->at(i)
        ///
        /// \throws noexcept
        ///
    void swap( array_set& b )
    {
        using ali::swap;
        swap(
            static_cast<comparator_type&>(*this),
            static_cast<comparator_type&>(b));
        swap(this->_arr, b._arr);
    }

        /// \brief See \ref array_set::swap.
        ///
    friend void swap( array_set& a, array_set& b )
    {
        a.swap(b);
    }

        /// \brief  Provides read-only access to the internal array
        /// holding this set's elements.
        ///
        /// \returns Const reference to the internal array holding
        /// this set's elements.
        ///
        /// \throws noexcept
        ///
    ali::array<T> const& as_array( void ) const
    {
        return this->_arr;
    }

        /// \brief  Provides read-only access to the comparator
        /// object used by this set.
        ///
        /// \returns Const reference to the comparator object used
        /// by this set.
        ///
        /// \throws noexcept
        ///
    comparator_type const& comparator( void ) const
    {
        return *this;
    }

        /// \brief  Determines whether the first object compares
        /// strictly less than the second one according to the
        /// comparator object used by this set.
        ///
        /// \retval true
        ///         The first object compares strictly less than
        ///         the second one.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(this->is_less()(T1,T2)))</tt>
        ///
        ///
    template <typename T1, typename T2>
    bool is_less( T1 const& t1, T2 const& t2 ) const
    {
        return comparator_type::operator()(t1, t2) < 0;
    }

        /// \brief  Determines whether two objects are equivalent
        /// according to the comparator object used by this set.
        ///
        /// \retval true
        ///         The two objects are equivalent.
        /// \retval false
        ///         Otherwise.
        ///
        /// \post   <tt>result == !this->is_less(t1, t2)
        /// && !this->is_less(t2, t1)</tt>
        ///
        /// \throws noexcept <tt>(noexcept(this->is_less(T1,T2))
        /// && noexcept(this->is_less(T2,T1)))</tt>
        ///
        ///
    template <typename T1, typename T2>
    bool are_equivalent( T1 const& t1, T2 const& t2 ) const
    {
        return comparator_type::operator()(t1, t2) == 0;
    }

    void assert_invariant( void ) const
    {
        this->_arr.assert_invariant();
    }

private:    //  Data members
    ali::array<T>   _arr;
        ///<        Array used for storing the elements.
};

}   //  namespace ali

