/*
 *  ali_xml_tree2.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_xml_tree2_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_deprecated.h"
#include "ali/ali_exception_memory.h"
#include "ali/ali_string.h"
#include "ali/ali_string_map.h"

namespace ali
{

namespace xml
{

// ******************************************************************
class trees
// ******************************************************************
{
public:
    class iterator;
    friend iterator;

	class const_iterator;
	friend const_iterator;

    using assert_invariant_now_and_on_exit
        = debug::assert_invariant_now_and_on_exit<trees>;

public:     //  Class
    class const_iterator : public iterator_base<
        random_access_iterator_tag, tree>
    {
	public:
		const_iterator( void ) {}

        const_iterator(
            array_const_ref<ali::auto_ptr<tree>> trees,
            int index = 0 )
        :   _index{index},
            _trees{trees.pointer()}
        {
            ali_assert(0 <= _index);
            ali_assert(_index <= _trees->size());
        }

        tree const& operator*( void ) const
        {
            return *_trees->at(_index);
        }

        const_iterator& operator++( void )
		{
            ali_assert(_index < _trees->size());
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
            //  0 <= _index + d <= _trees->size()
            ali_assert(-_index <= d);
            ali_assert(d <= _trees->size() - _index);
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
            //  0 <= _index - d <= _trees->size()
            ali_assert(-_index <= -d);
            ali_assert(-d <= _trees->size() - _index);
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
            ali_assert(a._trees->data() == b._trees->data());
            return a._index - b._index;
        }

        friend bool operator==( const_iterator a, const_iterator b )
        {
            ali_assert(a._trees->data() == b._trees->data());
            return a._index == b._index;
        }

		friend bool operator!=( const_iterator a, const_iterator b )
        {
            return !(a == b);
        }

    protected:  //  Data members
        int                                     _index{};
		array_const_ptr<ali::auto_ptr<tree>>    _trees{};
    };

public:     //  Class
    class iterator : public const_iterator
    {
    public:
		iterator( void ) {}

        iterator(
            array_const_ref<ali::auto_ptr<tree>> trees,
            int index = 0 )
        :   const_iterator{trees, index}
        {}

		tree& operator*( void ) const
        {
            return *_trees->at(_index);
        }

        iterator& operator++( void )
		{
            const_iterator::operator++();
			return *this;
        }

        iterator operator++( int )
		{
            iterator const t{*this};
		    ++*this;
			return t;
        }

        iterator& operator+=( int d )
		{
            const_iterator::operator+=(d);
            return *this;
        }

        friend iterator operator+( iterator it, int d )
		{
            it += d;
            return it;
        }

        iterator& operator--( void )
		{
            const_iterator::operator--();
			return *this;
        }

		iterator operator--( int )
		{
            iterator const t{*this};
			--*this;
			return t;
        }

        iterator& operator-=( int d )
		{
            const_iterator::operator-=(d);
            return *this;
        }

        friend iterator operator-( iterator it, int d )
		{
            it -= d;
            return it;
        }

        friend int operator-( iterator a, iterator b )
        {
            return  static_cast<const_iterator&>(a)
                -   static_cast<const_iterator&>(b);
        }

        friend bool operator==( iterator a, iterator b )
        {
            return  static_cast<const_iterator&>(a)
                ==  static_cast<const_iterator&>(b);
        }

		friend bool operator!=( iterator a, iterator b )
        {
            return !(a == b);
        }
    };

public:     //  Methods
    trees( void );

    trees( trees const& b );

    trees( trees&& b );

    ~trees( void );

    trees& operator=( trees b )
    {
        swap(b);
        return *this;
    }

    int size( void ) const
    {
        return _trees.size();
    }

    bool is_empty( void ) const
    {
        return _trees.is_empty();
    }

    tree& add( void );

#ifdef  ALI_XML_STRING_IS_STABLE_STRING

    tree& add( stable_string const& name );
    tree& add( string_const_ref name )
    {
        return add(stable_string{name});
    }
    
    tree& add( stable_string const& name, stable_string const& data );
    tree& add( string_const_ref name, stable_string const& data )
    {
        return add(stable_string{name}, data);
    }
    tree& add( stable_string const& name, string_const_ref data )
    {
        return add(name, stable_string{data});
    }
    tree& add( string_const_ref name, string_const_ref data )
    {
        return add(stable_string{name}, stable_string{data});
    }

#else   //  !ALI_XML_STRING_IS_STABLE_STRING

    tree& add( string_const_ref name );
    
    tree& add( string_const_ref name, string_const_ref data );

#endif  //  ALI_XML_TREE_WITH_STABLE_STR

    tree& add( tree const& t );
    tree& add( ali::auto_ptr<tree> t );

    tree& add_swap( tree& t );

#ifdef  ALI_XML_STRING_IS_STABLE_STRING

    tree& insert( int idx, stable_string const& name );
    tree& insert( int idx, string_const_ref name )
    {
        return insert(idx, stable_string{name});
    }

    tree& insert( int idx, stable_string const& name, stable_string const& data );
    tree& insert( int idx, string_const_ref name, stable_string const& data )
    {
        return insert(idx, stable_string{name}, data);
    }
    tree& insert( int idx, stable_string const& name, string_const_ref data )
    {
        return insert(idx, name, stable_string{data});
    }
    tree& insert( int idx, string_const_ref name, string_const_ref data )
    {
        return insert(idx, stable_string{name}, stable_string{data});
    }

#else   //  !ALI_XML_STRING_IS_STABLE_STRING

    tree& insert( int idx, ali::string const& name );
    tree& insert( int idx, ali::string const& name, ali::string const& data );

#endif  //  ALI_XML_TREE_WITH_STABLE_STR

    tree& insert( int idx, tree const& t );
    tree& insert( int idx, ali::auto_ptr<tree> t );

    tree& insert_swap( int idx, tree& t );

    int erase_all( string_const_ref name );

    template <typename predicate>
    int erase_if( predicate&& p )
    //  bool predicate( ali::xml::tree& )
    {
        return _trees.erase_if(
            [&p]( ali::auto_ptr<tree>& element ) -> bool
            { return p(*element); });
    }

    ali::auto_ptr<tree> erase( int i );

    trees& move( int dst_idx, int src_idx );

    trees& erase( void );

    tree* find_with_attr(
        string_const_ref name,
        string_const_ref attr_name,
        string_const_ref attr_value );

    tree const* find_with_attr(
        string_const_ref name,
        string_const_ref attr_name,
        string_const_ref attr_value ) const;

    tree* find_first( string_const_ref name )
    {
        int const idx = find_first_index(name);
        if ( idx < 0 ) return nullptr;
        else return _trees[idx].get();
    }

    tree const* find_first( string_const_ref name ) const
    {
        int const idx = find_first_index(name);
        if ( idx < 0 ) return nullptr;
        else return _trees[idx].get();
    }

    tree* find_first(
        string_const_ref name1, string_const_ref name2 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4 ) const;

    tree* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4,
        string_const_ref name5 );

    tree const* find_first(
        string_const_ref name1, string_const_ref name2,
        string_const_ref name3, string_const_ref name4,
        string_const_ref name5 ) const;

    int find_first_index( string_const_ref name ) const
    {
        return find_next_index(name);
    }

    int find_next_index(
        string_const_ref name,
        int excluded_start_index = -1 ) const;

    tree& operator[]( string_const_ref name )
    {
        if ( tree* tree = find_first(name) )
            return *tree;
        else
            return add(name);
    }

#ifdef  ALI_XML_STRING_IS_STABLE_STRING

    tree& operator[]( stable_string const& name )
    
#else   //  !ALI_XML_STRING_IS_STABLE_STRING

    tree& operator[]( ali::string const& name )
    
#endif  //  ALI_XML_TREE_WITH_STABLE_STR
    {
        if ( tree* tree = find_first(name) )
            return *tree;
        else
            return add(name);
    }

    tree const& operator[]( string_const_ref name ) const
    {
        if ( tree const* const tree = find_first(name) )
            return *tree;
        else
            return empty_tree();
    }

    tree& operator[]( int i )
    {
        return *_trees[i];
    }

    tree const& operator[]( int i ) const
    {
        return *_trees[i];
    }

    iterator begin( void )
    {
        return iterator{_trees.ref()};
    }

    friend iterator begin( trees& t )
    {
        return t.begin();
    }

    const_iterator begin( void ) const
    {
        return const_iterator{_trees.ref()};
    }

    friend const_iterator begin( trees const& t )
    {
        return t.begin();
    }

    iterator end( void )
    {
        return iterator{_trees.ref(), _trees.size()};
    }

    friend iterator end( trees& t )
    {
        return t.end();
    }

    const_iterator end( void ) const
    {
        return const_iterator{_trees.ref(), _trees.size()};
    }

    friend const_iterator end( trees const& t )
    {
        return t.end();
    }

    void swap( trees& b )
    {
        using ali::swap;
        swap(_trees, b._trees);
    }

    friend void swap( trees& a, trees& b )
    {
        a.swap(b);
    }

    bool operator==( trees const& b ) const;

    bool operator!=( trees const& b ) const
    {
        return !operator==(b);
    }

    void assert_invariant( void ) const;

private:    //  Methods
    static tree const& empty_tree( void );

private:    // Data members
    ali::array<ali::auto_ptr<tree>> _trees;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct tree
// ******************************************************************
{
    using assert_invariant_now_and_on_exit
        = debug::assert_invariant_now_and_on_exit<tree>;

    tree( void );

    tree( tree const& b );

    tree( tree&& b );

#ifdef  ALI_XML_STRING_IS_STABLE_STRING

    explicit tree( stable_string const& name );
    explicit tree( string_const_ref name )
    :   tree{stable_string{name}}
    {}

    tree( stable_string const& name, stable_string const& data );
    tree( string_const_ref name, stable_string const& data )
    :   tree{stable_string{name}, data}
    {}
    
    tree( stable_string const& name, string_const_ref data )
    :   tree{name, stable_string{data}}
    {}
    
    tree( string_const_ref name, string_const_ref data )
    :   tree{stable_string{name}, stable_string{data}}
    {}

#else   //  !ALI_XML_STRING_IS_STABLE_STRING

    explicit tree( string_const_ref name );

    tree( ali::string_const_ref name, ali::string_const_ref data );

#endif  //  ALI_XML_STRING_IS_STABLE_STRING

    ~tree( void );

    tree& operator=( tree b )
    {
        swap(b);
        return *this;
    }

    bool is_empty( void ) const
    {
        return  name.is_empty()
            &&  data.is_empty()
            &&  nodes.is_empty()
            &&  attrs.is_empty();
    }

    void swap( tree& b )
    {
        using ali::swap;
        swap(name, b.name);
        swap(data, b.data);
        swap(nodes, b.nodes);
        swap(attrs, b.attrs);
    }

    friend void swap( tree& a, tree& b )
    {
        a.swap(b);
    }

    tree& merge_with( tree const& b );

    bool operator==( tree const& b ) const
    {
        return name == b.name && data == b.data
            && attrs == b.attrs && nodes == b.nodes;
    }

    bool operator!=( tree const& b ) const
    {
        return !operator==(b);
    }

    void assert_invariant( void ) const
    {
        name.assert_invariant();
        data.assert_invariant();
        nodes.assert_invariant();
        attrs.assert_invariant();
    }

    xml::string     name{};
    xml::string     data{};
    
    trees           nodes{};
    attributes      attrs{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class string_from_tree : public ali::string
// ******************************************************************
{
public:
    string_from_tree( tree const& t );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class pretty_string_from_tree : public ali::string
// ******************************************************************
{
public:
    pretty_string_from_tree(
        tree const& t,
        int indent_step = 2,
        int indent = 0 );

    pretty_string_from_tree(
        trees const& t,
        int indent_step = 2,
        int indent = 0 );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class openxcap_string_from_tree : public ali::string
// ******************************************************************
{
public:
    openxcap_string_from_tree( tree const& t );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class openxcap_pretty_string_from_tree : public ali::string
// ******************************************************************
//  OpenXCAP doesn't understand short empty tags, e.g. <Empty />
// ******************************************************************
{
public:
    openxcap_pretty_string_from_tree(
        tree const& t,
        int indent_step = 2,
        int indent = 0 );

    openxcap_pretty_string_from_tree(
        trees const& t,
        int indent_step = 2,
        int indent = 0 );
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
ali::string& format(
    ali::string& str,
    tree const& t,
    string_const_ref format_string );
// ******************************************************************
//  format_string := epsilon | "pretty" | "openxcap"
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
class encoded : public ali::string
// ******************************************************************
{
public:
    encoded( string_const_ref str );
};

// ******************************************************************
class decoded : public ali::string
// ******************************************************************
{
public:
    decoded( string_const_ref str );
};

}   //  namespace xml

}   //  namespace ali

