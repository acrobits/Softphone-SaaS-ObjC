/*
 *  ali_array_map.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_map_forward.h"
#include "ali/ali_algorithm_common_bounds.h"
#include "ali/ali_array.h"
#include "ali/ali_attribute.h"
#include "ali/ali_deprecated.h"
#include "ali/ali_exception_if.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
template <
    typename K, typename T,
    typename comparator_type>
class ALI_ATTRIBUTE_EMPTY_BASES array_map
// ******************************************************************
    :   private comparator_type,
        public exposes_array_const_ref<
            ali::pair<K, T>,
            array_map<K, T, comparator_type>>
{
    static_assert(
        meta::comparator_returns_int<comparator_type, K>::result,
        "The comparator parameter has changed from 'bool is_less(K,K)' to 'int compare(K,K)'.\n"
        "See ali::default_comparator for sample implementation.");

public:     // Typedefs
    typedef ali::pair<K, T>         value_type;
    typedef typename ali::array<
        value_type>::const_iterator const_iterator;

public:
    array_map( void ) {}

    array_map( ali::initializer_list<value_type> const& pairs )
    {
        this->set(pairs);
    }

    template <typename actual_key_type>
    array_map& set( actual_key_type const& key, T const& value )
    {
        return this->private_set(key, value);
    }

    template <typename actual_key_type>
    array_map& set( actual_key_type const& key, T&& value )
    {
        return this->private_set(key, ali::move(value));
    }

    array_map& set( ali::initializer_list<value_type> const& pairs )
    {
        for ( auto const& value : pairs )
            this->set(value.first, value.second);

        return *this;
    }

    template <typename actual_key_type>
    int index_of_lower_bound( actual_key_type const& key ) const
        //  post:   0 <= result <= this->size()
        //      &&  this->at(result - 1).first < key <= this->at(result).first
        //              //  Where the index is not out of range.
    {
        return generic_index_of_lower_bound(
            this->_arr.size(),
            [this] ( int idx ) -> K const&
            { return this->_arr[idx].first; },
            key,
            this->comparator());
    }

    template <typename actual_key_type>
    int index_of( actual_key_type const& key ) const
        //  post:   result == this->size()
        //      ||  are_keys_equal(this->at(result).first, key)
    {
        int const idx{this->index_of_lower_bound(key)};

        return idx != this->_arr.size() && this->are_keys_equal(
            this->_arr[idx].first, key) ? idx : this->_arr.size();
    }

    template <typename actual_key_type>
    T const* find( actual_key_type const& key ) const
    {
        int const idx{this->index_of(key)};

        return idx != this->_arr.size()
            ? &this->_arr[idx].second : nullptr;
    }

    template <typename actual_key_type>
    T* find( actual_key_type const& key )
    {
        int const idx{this->index_of(key)};

        return idx != this->_arr.size()
            ? &this->_arr[idx].second : nullptr;
    }

    template <typename actual_key_type>
    T const& get( actual_key_type const& key ) const
    {
        ali_assert(this->contains(key));
        return *ALI_X_GENERAL_IF_NULL(this->find(key));
    }

    template <typename actual_key_type>
    T& get( actual_key_type const& key )
    {
        ali_assert(this->contains(key));
        return *ALI_X_GENERAL_IF_NULL(this->find(key));
    }

    template <typename actual_key_type>
    bool contains( actual_key_type const& key ) const
    {
        return this->index_of(key) != this->_arr.size();
    }

    template <typename actual_key_type>
    bool erase( actual_key_type const& key )
    {
        int const idx{this->index_of(key)};

        if ( idx == this->_arr.size() )
            return false;

        this->_arr.erase(idx);

        return true;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return this->_arr.erase_if(p);
    }

    array_map& erase_at( int i )
    {
        this->_arr.erase(i);

        return *this;
    }

    array_map& erase( void )
    {
        this->_arr.erase();

        return *this;
    }

    template <typename actual_key_type>
    T& operator[]( actual_key_type const& key )
    {
        int const idx{this->index_of_lower_bound(key)};

        bool const is_new_key{
                idx == this->_arr.size()
            ||  !are_keys_equal(this->_arr[idx].first, key)};

        if ( is_new_key )
            this->_arr.insert(idx, value_type{key, T()});

        return this->_arr[idx].second;
    }

    int size( void ) const
    {
        return this->_arr.size();
    }

    int capacity( void ) const
    {
        return this->_arr.capacity();
    }

    bool is_empty( void ) const
    {
        return this->_arr.is_empty();
    }

    value_type const& at( int i ) const
    {
        return this->_arr[i];
    }

    T const& value_at( int i ) const
    {
        return this->_arr[i].second;
    }

    T& value_at( int i )
    {
        return this->_arr[i].second;
    }

    value_type const& front( void ) const
    {
        return this->_arr.front();
    }

    T const& front_value( void ) const
    {
        return this->_arr.front().second;
    }

    T& front_value( void )
    {
        return this->_arr.front().second;
    }

    value_type const& back( void ) const
    {
        return this->_arr.back();
    }

    T const& back_value( void ) const
    {
        return this->_arr.back().second;
    }

    T& back_value( void )
    {
        return this->_arr.back().second;
    }

    const_iterator begin( void ) const
    {
        return this->_arr.begin();
    }

    friend const_iterator begin( array_map const& arr )
    {
        return arr.begin();
    }

    const_iterator end( void ) const
    {
        return this->_arr.end();
    }

    friend const_iterator end( array_map const& arr )
    {
        return arr.end();
    }

    friend bool operator==(
        array_map const& a,
        array_map const& b )
    {
        if ( a._arr.size() != b._arr.size() )
            return false;

        for ( int i = a._arr.size(); i != 0; )
        {
            --i;

            bool const are_pairs_equal
                =   a.are_keys_equal(
                        a._arr[i].first,
                        b._arr[i].first)
                            //  Same keys.
                &&  a._arr[i].second == b._arr[i].second;
                            //  Same values.

            if ( !are_pairs_equal )
                return false;
        }

        return true;
    }

    friend bool operator!=(
        array_map const& a,
        array_map const& b )
    {
        return !(a == b);
    }

    void swap( array_map& b )
    {
        using ali::swap;
        swap(
            static_cast<comparator_type&>(*this),
            static_cast<comparator_type&>(b));
        swap(this->_arr, b._arr);
    }

    friend void swap( array_map& a, array_map& b )
    {
        a.swap(b);
    }

    template <typename K1, typename K2>
    bool are_keys_equal( K1 const& k1, K2 const& k2 ) const
    {
        return comparator_type::operator()(k1, k2) == 0;
    }

    array_const_ref<value_type> ref( void ) const
    {
        return this->_arr.ref();
    }

    array_const_ref<value_type> ref( int pos, int n ) const
    {
        return this->_arr.ref(pos, n);
    }

    comparator_type const& comparator( void ) const
    {
        return *this;
    }

private:    //  Methods
    template <
        typename actual_key_type,
        typename actual_value_type>
    array_map& private_set(
        actual_key_type const& key,
        actual_value_type&& value )
    {
        int const idx{this->index_of_lower_bound(key)};

        bool const is_existing_key{
                idx != this->_arr.size()
            &&  are_keys_equal(this->_arr[idx].first, key)};

        if ( is_existing_key )
            this->_arr[idx].second = ali::forward<
                actual_value_type>(value);
        else
            this->_arr.insert(idx, value_type{
                key, ali::forward<actual_value_type>(value)});

        return *this;
    }


private:    //  Data members
    ali::array<value_type>  _arr{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <
    typename K, typename T,
    typename comparator_type>
class ALI_ATTRIBUTE_EMPTY_BASES auto_ptr_array_map
// ******************************************************************
    :   private comparator_type,
        public exposes_array_const_ref<
            ali::pair<K, T*>,
            auto_ptr_array_map<K, T, comparator_type>>
{
    static_assert(
        meta::comparator_returns_int<comparator_type, K>::result,
        "The comparator parameter has changed from 'bool is_less(K,K)' to 'int compare(K,K)'.\n"
        "See ali::default_comparator for sample implementation.");

public:     // Typedefs
    typedef ali::pair<K, T*>        value_type;

public:
    auto_ptr_array_map( void ) {}

    auto_ptr_array_map(
        auto_ptr_array_map const& b )
    {
        auto_ptr_array_map temp{};

        temp._arr.reserve(b._arr.size());

        for ( int i{}; i != b._arr.size(); ++i )
        {
            ali::auto_ptr<T> value{};

            if ( b._arr[i].second != nullptr )
                value = new_auto_ptr<T>(*b._arr[i].second);

            temp._arr.push_back(
                value_type{
                    b._arr[i].first,
                    value.get()});

            value.release();
        }

        this->swap(temp);
    }

    ~auto_ptr_array_map( void )
    {
        this->erase();
    }

    auto_ptr_array_map& operator=(
        auto_ptr_array_map const& b )
    {
        if ( this != &b )
            auto_ptr_array_map{b}.swap(*this);
        return *this;
    }

    template <typename actual_key_type>
    auto_ptr_array_map& set(
        actual_key_type const& key,
        ali::auto_ptr<T> value )
    {
        int const idx{this->index_of_lower_bound(key)};

        bool const is_existing_key{
                idx != this->_arr.size()
            &&  this->are_keys_equal(
                    this->_arr[idx].first, key)};

        if ( is_existing_key )
        {
            ali::auto_ptr<T> temp{this->_arr[idx].second};

            this->_arr[idx].second = value.release();
        }
        else
        {
            this->_arr.insert(idx, value_type{key, value.get()});

            value.release();
        }

        return *this;
    }

    template <typename actual_key_type>
    int index_of_lower_bound( actual_key_type const& key ) const
        //  post:   0 <= result <= this->size()
        //      &&  this->at(result - 1).first < key <= this->at(result).first
        //              //  Where the index is not out of range.
    {
        return generic_index_of_lower_bound(
            this->_arr.size(),
            [this] ( int idx ) -> K const&
            { return this->_arr[idx].first; },
            key,
            this->comparator());
    }

    template <typename actual_key_type>
    int index_of( actual_key_type const& key ) const
        //  post:   result == this->size()
        //      ||  are_keys_equal(this->at(result).first, key)
    {
        int const idx{this->index_of_lower_bound(key)};

        return idx != this->_arr.size() && this->are_keys_equal(
            this->_arr[idx].first, key) ? idx : this->_arr.size();
    }

    template <typename actual_key_type>
    T const* find( actual_key_type const& key ) const
    {
        int const idx{this->index_of(key)};

        return idx != this->_arr.size()
            ? this->_arr[idx].second : nullptr;
    }

    template <typename actual_key_type>
    T* find( actual_key_type const& key )
    {
        int const idx{this->index_of(key)};

        return idx != this->_arr.size()
            ? this->_arr[idx].second : nullptr;
    }

    template <typename actual_key_type>
    T const& get( actual_key_type const& key ) const
    {
        ali_assert(this->contains(key));
        return *ALI_X_GENERAL_IF_NULL(this->find(key));
    }

    template <typename actual_key_type>
    T& get( actual_key_type const& key )
    {
        ali_assert(this->contains(key));
        return *ALI_X_GENERAL_IF_NULL(this->find(key));
    }

    template <typename actual_key_type>
    bool contains( actual_key_type const& key ) const
    {
        return this->index_of(key) != this->_arr.size();
    }

    template <typename actual_key_type>
    ali::auto_ptr<T> erase( actual_key_type const& key )
    {
        ali::auto_ptr<T> t{};

        int const idx{this->index_of(key)};

        if ( idx != this->_arr.size() )
        {
            t.reset(this->_arr[idx].second);

            this->_arr.erase(idx);
        }

        return t;
    }

    template <typename actual_key_type>
    ali::auto_ptr<T> release_no_erase( actual_key_type const& key )
    {
        ali::auto_ptr<T> t{};

        int const idx{this->index_of(key)};

        if ( idx != this->_arr.size() )
        {
            t.reset(this->_arr[idx].second);

            this->_arr[idx].second = nullptr;
        }

        return t;
    }

    bool is_empty( void ) const
    {
        return this->_arr.is_empty();
    }

    int size( void ) const
    {
        return this->_arr.size();
    }

    int capacity( void ) const
    {
        return this->_arr.capacity();
    }

    value_type const& at( int i ) const
    {
        return this->_arr[i];
    }

    T* value_at( int i )
    {
        return this->_arr[i].second;
    }

    ali::auto_ptr<T> erase_at( int i )
    {
        ali::auto_ptr<T> t{this->_arr[i].second};

        this->_arr.erase(i);

        return t;
    }

    auto_ptr_array_map& erase( void )
    {
        for ( ; !_arr.is_empty(); _arr.erase_back() )
        {
            ali::auto_ptr<T>{this->_arr.back().second};

            this->_arr.back().second = nullptr;
        }

        return *this;
    }

    template <typename predicate>
    int erase_if( predicate p )
    {
        return this->_arr.erase_if(
            [p = ali::move(p)]( value_type& element )
            {
                bool const erase{p(element)};
                if ( erase )
                {
                    ali::auto_ptr<T>{element.second};

                    element.second = nullptr;
                }
                return erase;
            });
    }

    friend bool operator==(
        auto_ptr_array_map const& a,
        auto_ptr_array_map const& b )
    {
        if ( a._arr.size() != b._arr.size() )
            return false;

        for ( int i = a._arr.size(); i != 0; )
        {
            --i;

            bool const are_pairs_equal
                =   a.are_keys_equal(
                        a._arr[i].first,
                        b._arr[i].first)
                            //  Same keys.
                &&  (a._arr[i].second == nullptr)
                        == (b._arr[i].second == nullptr)
                                //  Both null or both non-null.
                &&  (   a._arr[i].second == nullptr
                    ||  *a._arr[i].second == *b._arr[i].second);
                            //  Same values.

            if ( !are_pairs_equal )
                return false;
        }

        return true;
    }

    friend bool operator!=(
        auto_ptr_array_map const& a,
        auto_ptr_array_map const& b )
    {
        return !(a == b);
    }

    void swap( auto_ptr_array_map& b )
    {
        using ali::swap;
        swap(
            static_cast<comparator_type&>(*this),
            static_cast<comparator_type&>(b));
        swap(this->_arr, b._arr);
    }

    friend void swap(
        auto_ptr_array_map& a,
        auto_ptr_array_map& b )
    {
        a.swap(b);
    }

    template <typename K1, typename K2>
    bool are_keys_equal( K1 const& k1, K2 const& k2 ) const
    {
        return comparator_type::operator()(k1, k2) == 0;
    }

    array_const_ref<value_type> ref( void ) const
    {
        return this->_arr.ref();
    }

    array_const_ref<value_type> ref( int pos, int n ) const
    {
        return this->_arr.ref(pos, n);
    }

    comparator_type const& comparator( void ) const
    {
        return *this;
    }

    void assert_invariant( void ) const
    {
        _arr.assert_invariant();
    }

private:    // Data members
    ali::array<value_type>  _arr{};
};

// ******************************************************************
// ******************************************************************

}   //  namespace ali

