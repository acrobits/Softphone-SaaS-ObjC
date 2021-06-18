/*
 *  ali_json.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_json_forward.h"
#include "ali/ali_array.h"
#include "ali/ali_array_map.h"
#include "ali/ali_noncopyable.h"
#include "ali/ali_string.h"

namespace ali
{
    namespace json
    {
        // ******************************************************************
        enum type
        // ******************************************************************
        {
            Null,
            String,
            Array,
            Dict,
            Bool,
            Int,
            Float
        };

        // ******************************************************************
        class array
            : public ali::array<object>
        // ******************************************************************
        {
        public:
            using super = ali::array<object>;

            // ******************************************************************
            array( void )
            // ******************************************************************
                : super()
            {}

            // ******************************************************************
            array( ali::initializer_list<value_type> const& values )
            // ******************************************************************
                : super(values)
            {}

            // ******************************************************************
            array( array const& b )
            // ******************************************************************
            : super(b)
            {}


            // ******************************************************************
            friend void swap( array& a, array& b )
            // ******************************************************************
            {
                a.swap(b);
            }

            // ******************************************************************
            array& operator=( array const& b )
            // ******************************************************************
            {
                super::operator =(b);
                return *this;
            };

#ifdef  ALI_FEATURE_RVALUE_REF

            array( array && b ) = default;

            array& operator=( array && b ) = default;
#endif
        };

        // ******************************************************************
        class dict : public array_map<string, object>
        // ******************************************************************
        {
        public:
            using super = array_map<string, object>;

            // ******************************************************************
            dict( void )
            // ******************************************************************
                : super()
            {}

            // ******************************************************************
            dict( dict const& b )
            // ******************************************************************
            : super(b)
            {}


            // ******************************************************************
            dict( ali::initializer_list<value_type> const& pairs )
            // ******************************************************************
                : super(pairs)
            {}

            // ******************************************************************
            friend void swap( dict& a, dict& b )
            // ******************************************************************
            {
                a.swap(b);
            }

            // ******************************************************************
            dict& operator=( dict const& b )
            // ******************************************************************
            {
                super::operator =(b);
                return *this;
            }

#ifdef  ALI_FEATURE_RVALUE_REF

            dict( dict && b ) = default;

            dict& operator=( dict && b ) = default;
#endif

            template <typename K>
            object const& get( K const& key ) const;

            template <typename K>
            object& get( K const& key );

            template <typename K>
            string const* find_string( K const& key ) const;

            template <typename K>
            array const* find_array( K const& key ) const;

            template <typename K>
            dict const* find_dict( K const& key ) const;

            template <typename K>
            bool const* find_bool( K const& key ) const;

            template <typename K>
            ali::int64 const* find_int( K const& key ) const;

            template <typename K>
            double const* find_float( K const& key ) const;

            template <typename K>
            string* find_string( K const& key );

            template <typename K>
            array* find_array( K const& key );

            template <typename K>
            dict* find_dict( K const& key );

            template <typename K>
            bool* find_bool( K const& key );

            template <typename K>
            ali::int64* find_int( K const& key );

            template <typename K>
            double* find_float( K const& key );
        };

        // ******************************************************************
        class object
        // ******************************************************************
        {
        friend class parser;

        public:
            // ******************************************************************
            object( nullptr_type = nullptr )
            // ******************************************************************
            {}

            // ******************************************************************
            object( type t )
            // ******************************************************************
            {
                set_type(t);
            }

            // ******************************************************************
            object( string_const_ref str )
            // ******************************************************************
            {
                operator =(string(str));
            }

            // ******************************************************************
            object( string const& str )
            // ******************************************************************
            {
                operator =(str);
            }

#if defined(ALI_FEATURE_RVALUE_REF)
            // ******************************************************************
            object( string && str )
            // ******************************************************************
            {
                operator =(ali::move(str));
            }
#endif

            // ******************************************************************
            object( array const& arr )
            // ******************************************************************
            {
                operator =(arr);
            }

            // ******************************************************************
            object( dict const& d )
            // ******************************************************************
            {
                operator =(d);
            }

            // ******************************************************************
            object( bool b )
            // ******************************************************************
            {
                operator =(b);
            }

            // ******************************************************************
            object( int i )
            // ******************************************************************
            {
                operator =(i);
            }

            // ******************************************************************
            object( ali::int64 i )
            // ******************************************************************
            {
                operator =(i);
            }

            // ******************************************************************
            object( double d )
            // ******************************************************************
            {
                operator =(d);
            }

            object( object const& other );
            object& operator =( object const& other );

#if defined(ALI_FEATURE_RVALUE_REF)
            // ******************************************************************
            object( object && other )
            // ******************************************************************
            {
                swap(other);
            }

            // ******************************************************************
            object& operator =( object && other )
            // ******************************************************************
            {
                swap(other);
                return *this;
            }
#endif

            // ******************************************************************
            ~object( void )
            // ******************************************************************
            {
                clear();
            }

            // ******************************************************************
            type get_type( void ) const
            // ******************************************************************
            {
                return _type;
            }

            // ******************************************************************
            void set_type( type t )
            // ******************************************************************
            {
                if(_type == t)
                    return;

                switch(_type)
                {
                case String:
                case Array:
                case Dict:
                    clear();
                    break;
                case Bool:
                case Int:
                case Float:
                case Null:
                    break;
                }
                _type = t;

                switch(_type)
                {
                case String:
                    new(_string) string();
                    break;

                case Array:
                    new(_array) array();
                    break;

                case Dict:
                    new(_dict) dict();
                    break;

                case Bool:
                    _bool = false;
                    break;

                case Int:
                    _int = 0;
                    break;

                case Float:
                    _float = 0.0;
                    break;

                case Null:
                    break;
                }
            }

            bool is_null( void ) const                  {return _type == Null;}

            string const* is_string( void ) const       {return (_type == String)   ? reinterpret_cast<string const*>(_string)  : nullptr;}
            array const* is_array( void ) const         {return (_type == Array)    ? reinterpret_cast<array const*>(_array)    : nullptr;}
            dict const* is_dict( void ) const           {return (_type == Dict)     ? reinterpret_cast<dict const*>(_dict)      : nullptr;}
            bool const* is_bool( void ) const           {return (_type == Bool)     ? reinterpret_cast<bool const*>(&_bool)     : nullptr;}
            ali::int64 const* is_int( void ) const      {return (_type == Int)      ? reinterpret_cast<ali::int64 const*>(&_int): nullptr;}
            double const* is_float( void ) const        {return (_type == Float)    ? reinterpret_cast<double const*>(&_float)  : nullptr;}

            string* is_string( void )        {return (_type == String)   ? reinterpret_cast<string*>(_string)  : nullptr;}
            array* is_array( void )          {return (_type == Array)    ? reinterpret_cast<array*>(_array)    : nullptr;}
            dict* is_dict( void )            {return (_type == Dict)     ? reinterpret_cast<dict*>(_dict)      : nullptr;}
            bool* is_bool( void )            {return (_type == Bool)     ? reinterpret_cast<bool*>(&_bool)     : nullptr;}
            ali::int64* is_int( void )       {return (_type == Int)      ? reinterpret_cast<ali::int64*>(&_int): nullptr;}
            double* is_float( void )         {return (_type == Float)    ? reinterpret_cast<double*>(&_float)  : nullptr;}

            // ******************************************************************
            array& as_array( void )
            // ******************************************************************
            {
                set_type(Array);

                return *reinterpret_cast<array *>(_array);
            }

            // ******************************************************************
            array const& as_array( void ) const
            // ******************************************************************
            {
                if (array const* a = is_array())
                    return *a;

                return _emptyArray;
            }

            // ******************************************************************
            dict& as_dict( void )
            // ******************************************************************
            {
                set_type(Dict);

                return *reinterpret_cast<dict *>(_dict);
            }

            // ******************************************************************
            dict const& as_dict( void ) const
            // ******************************************************************
            {
                if (dict const* d = is_dict())
                    return *d;

                return _emptyDict;
            }

            // ******************************************************************
            string& as_string( void )
            // ******************************************************************
            {
                set_type(String);

                return *reinterpret_cast<string *>(_string);
            }

            // ******************************************************************
            string const& as_string( void ) const
            // ******************************************************************
            {
                if (string const* s = is_string())
                    return *s;

                return _emptyString;
            }

            // ******************************************************************
            ali::int64 as_int( void ) const
            // ******************************************************************
            {
                if(!is_int())
                    return 0;

                return _int;
            }

            // ******************************************************************
            ali::int64& as_int( void )
            // ******************************************************************
            {
                set_type(Int);

                return _int;
            }

            // ******************************************************************
            double as_float( void ) const
            // ******************************************************************
            {
                if(!is_float())
                    return 0.0;

                return _float;
            }

            // ******************************************************************
            double& as_float( void )
            // ******************************************************************
            {
                set_type(Float);

                return _float;
            }

            // ******************************************************************
            bool as_bool( void ) const
            // ******************************************************************
            {
                if(!is_bool())
                    return false;

                return _bool;
            }

            // ******************************************************************
            bool& as_bool( void )
            // ******************************************************************
            {
                set_type(Bool);

                return _bool;
            }

            // ******************************************************************
            object & operator [](int idx)
            // ******************************************************************
            {
                return as_array()[idx];
            }

            // ******************************************************************
            object const& operator [](int idx) const
            // ******************************************************************
            {
                return as_array()[idx];
            }

            // ******************************************************************
            object & operator [](ali::string_const_ref key)
            // ******************************************************************
            {
                return as_dict().get(key);
            }

            // ******************************************************************
            object const& operator [](ali::string_const_ref key) const
            // ******************************************************************
            {
                return as_dict().get(key);
            }

            // ******************************************************************
            bool operator ==( object const& other ) const
            // ******************************************************************
            {
                if (get_type() != other.get_type())
                    return false;

                switch (get_type())
                {
                case String:
                    return as_string() == other.as_string();
                case Array:
                    return as_array() == other.as_array();
                case Dict:
                    return as_dict() == other.as_dict();
                case Bool:
                    return as_bool() == other.as_bool();
                case Int:
                    return as_int() == other.as_int();
                case Float:
                    return as_float() == other.as_float();
                case Null:
                    return true;
                }
                ali_assert(false);
                return false;
            }

            // ******************************************************************
            bool operator !=( object const& other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            bool operator ==( nullptr_type ) const
            // ******************************************************************
            {
                return is_null();
            }

            // ******************************************************************
            bool operator !=( nullptr_type ) const
            // ******************************************************************
            {
                return !is_null();
            }

            // ******************************************************************
            friend bool operator ==( nullptr_type,
                                     object const& second )
            // ******************************************************************
            {
                return second.is_null();
            }

            // ******************************************************************
            friend bool operator !=( nullptr_type,
                                     object const& second )
            // ******************************************************************
            {
                return !second.is_null();
            }

            // ******************************************************************
            bool operator ==( string const& other ) const
            // ******************************************************************
            {
                return is_string() && as_string() == other;
            }

            // ******************************************************************
            bool operator !=( string const& other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( string const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( string const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( string_const_ref other ) const
            // ******************************************************************
            {
                return is_string() && as_string() == other;
            }

            // ******************************************************************
            bool operator !=( string_const_ref other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( string_const_ref first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( string_const_ref first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( array const& other ) const
            // ******************************************************************
            {
                return is_array() && as_array() == other;
            }

            // ******************************************************************
            bool operator !=( array const& other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( array const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( array const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( dict const& other ) const
            // ******************************************************************
            {
                return is_dict() && as_dict() == other;
            }

            // ******************************************************************
            bool operator !=( dict const& other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( dict const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( dict const& first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( bool other ) const
            // ******************************************************************
            {
                return is_bool() && as_bool() == other;
            }

            // ******************************************************************
            bool operator !=( bool other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( bool first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( bool first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( int other ) const
            // ******************************************************************
            {
                return operator==(static_cast<ali::int64>(other));
            }

            // ******************************************************************
            bool operator !=( int other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( int first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( int first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( ali::int64 other ) const
            // ******************************************************************
            {
                return is_int() && as_int() == other;
            }

            // ******************************************************************
            bool operator !=( ali::int64 other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( ali::int64 first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( ali::int64 first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            bool operator ==( double other ) const
            // ******************************************************************
            {
                return is_float() && as_float() == other;
            }

            // ******************************************************************
            bool operator !=( double other ) const
            // ******************************************************************
            {
                return !operator==(other);
            }

            // ******************************************************************
            friend bool operator ==( double first,
                                     object const& second )
            // ******************************************************************
            {
                return second == first;
            }

            // ******************************************************************
            friend bool operator !=( double first,
                                     object const& second )
            // ******************************************************************
            {
                return second != first;
            }

            // ******************************************************************
            friend void swap( object& a, object& b )
            // ******************************************************************
            {
                a.swap(b);
            }

            // ******************************************************************
            object& operator =( nullptr_type )
            // ******************************************************************
            {
                set_null();
                return *this;
            }

            // ******************************************************************
            object& operator =( string_const_ref str )
            // ******************************************************************
            {
                as_string() = str;
                return *this;
            }

            // ******************************************************************
            object& operator =( string const& str )
            // ******************************************************************
            {
                as_string() = str;
                return *this;
            }

#if defined(ALI_FEATURE_RVALUE_REF)
            // ******************************************************************
            object& operator =( string && str )
            // ******************************************************************
            {
                as_string() = str;
                return *this;
            }
#endif

            // ******************************************************************
            object& operator =( bool b )
            // ******************************************************************
            {
                as_bool() = b;
                return *this;
            }

            // ******************************************************************
            object& operator =( int i )
            // ******************************************************************
            {
                return operator=(static_cast<ali::int64>(i));
            }

            // ******************************************************************
            object& operator =( ali::int64 i )
            // ******************************************************************
            {
                as_int() = i;
                return *this;
            }

            // ******************************************************************
            object& operator =( double d )
            // ******************************************************************
            {
                as_float() = d;
                return *this;
            }

            // ******************************************************************
            object& operator =( array const& arr )
            // ******************************************************************
            {
                as_array() = arr;
                return *this;
            }

            // ******************************************************************
            object& operator =( dict const& d )
            // ******************************************************************
            {
                as_dict() = d;
                return *this;
            }

            // ******************************************************************
            void set_null( void )
            // ******************************************************************
            {
                set_type(Null);
            }

            // ******************************************************************
            void swap( object& other )
            // ******************************************************************
            {
                using ali::swap;
                if (_type == other._type)
                    switch (_type)
                    {
                    case String:
                        swap(as_string(), other.as_string());
                        return;
                    case Array:
                        swap(as_array(), other.as_array());
                        return;
                    case Dict:
                        swap(as_dict(), other.as_dict());
                        return;
                    case Bool:
                        swap(as_bool(), other.as_bool());
                        return;
                    case Int:
                        swap(as_int(), other.as_int());
                        return;
                    case Float:
                        swap(as_float(), other.as_float());
                        return;
                    case Null:
                        return;
                    default:
                        ali_assert(false);
                        return;
                    }

                if (_type == Null)
                {
                    set_type(other._type);
                    this->swap(other);
                    other.set_type(Null);
                }
                else if (other._type == Null)
                {
                    other.set_type(_type);
                    this->swap(other);
                    set_type(Null);
                }
                else
                {
                    object d(_type);
                    this->swap(d); // nothing in this, new in other, old in d
                    set_type(other._type);
                    this->swap(other); // new in this, nothing in other, old in d
                    other.set_type(d._type);
                    other.swap(d); // new in this, old in other, nothing in d
                }
            }

            ali::int64 to_int( void ) const;
            double to_float( void ) const;
            bool to_bool( void ) const;
            string to_string( void ) const;

        private:
            void copy( object const& other );
            void clear( void );

        private:
            // Cannot use pointers
            template <typename T>
            object( T * ) ALI_ATTRIBUTE_DELETED;
            template <typename T>
            object& operator =( T * ) ALI_ATTRIBUTE_DELETED;
            template <typename T>
            bool operator ==( T * ) const ALI_ATTRIBUTE_DELETED;
            template <typename T>
            bool operator !=( T * ) const ALI_ATTRIBUTE_DELETED;

            template <typename T>
            friend bool operator ==( T *, object& );
            template <typename T>
            friend bool operator !=( T *, object& );

        private:
            type                        _type{Null};

            union
            {
                alignas(string) char    _string[sizeof(string)];
                alignas(array)  char    _array[sizeof(array)];
                alignas(dict)   char    _dict[sizeof(dict)];
                bool                    _bool;
                ali::int64              _int;
                double                  _float;
            };

            static const array                _emptyArray;
            static const object               _emptyObject;
            static const dict                 _emptyDict;
            static const ali::string          _emptyString;

        friend class dict;
        };

        // ******************************************************************
        template <typename K>
        inline object const& dict::get( K const& key ) const
        // ******************************************************************
        {
            object const* value = find(key);
            if (value == nullptr)
                return object::_emptyObject;
            return *value;
        }

        // ******************************************************************
        template <typename K>
        inline object& dict::get( K const& key )
        // ******************************************************************
        {
            object* value = find(key);

            if (value == nullptr)
            {
                set(key,object());
                value = find(key);
            }

            return *value;
        }

        template <typename K>
        inline string const* dict::find_string( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_string();
            else
                return nullptr;
        }

        template <typename K>
        inline array const* dict::find_array( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_array();
            else
                return nullptr;
        }

        template <typename K>
        inline dict const* dict::find_dict( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_dict();
            else
                return nullptr;
        }

        template <typename K>
        inline bool const* dict::find_bool( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_bool();
            else
                return nullptr;
        }

        template <typename K>
        inline ali::int64 const* dict::find_int( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_int();
            else
                return nullptr;
        }

        template <typename K>
        inline double const* dict::find_float( K const& key ) const
        {
            if ( object const* value = find(key) )
                return value->is_float();
            else
                return nullptr;
        }

        template <typename K>
        inline string* dict::find_string( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_string();
            else
                return nullptr;
        }

        template <typename K>
        inline array* dict::find_array( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_array();
            else
                return nullptr;
        }

        template <typename K>
        inline dict* dict::find_dict( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_dict();
            else
                return nullptr;
        }

        template <typename K>
        inline bool* dict::find_bool( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_bool();
            else
                return nullptr;
        }

        template <typename K>
        inline ali::int64* dict::find_int( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_int();
            else
                return nullptr;
        }

        template <typename K>
        inline double* dict::find_float( K const& key )
        {
            if ( object* value = find(key) )
                return value->is_float();
            else
                return nullptr;
        }

        bool parse( object& root, ali::string_const_ref str, int * processed = nullptr );
        bool parse( array& root, ali::string_const_ref str, int * processed = nullptr );
        bool parse( dict& root, ali::string_const_ref str, int * processed = nullptr );

        ali::string& escape_string_in_place( ali::string& str );
        
        ali::string& escape_string( ali::string& out, string_const_ref str );
        
        inline ali::string escape_string( string_const_ref str )
        {
            ali::string out{};
            escape_string(out, str);
            return out;
        }

        // ******************************************************************
        class escaped
                : public ali::string
        // ******************************************************************
        {
        public:
            escaped( ali::string_const_ref str );
        };

        // ******************************************************************
        class to_string
                : public string
        // ******************************************************************
        {
        public:
            // when used with empty indend, a compact string will be created
            to_string( object const& doc, ali::string_const_ref indent=""_s );
            to_string( array const& doc, ali::string_const_ref indent=""_s );
            to_string( dict const& doc, ali::string_const_ref indent=""_s );

        private:
            void process( object const& doc, int depth );
            void process( array const& arr, int depth );
            void process( dict const& d, int depth );
            void append_indent( int depth );
            void append_newline( void );

        private:
            ali::string _indent;
        };

        // ******************************************************************
        class to_pretty_string
                : public to_string
        // ******************************************************************
        {
        public:
        // ******************************************************************
            to_pretty_string( object const& doc, int indent = 3 )
        // ******************************************************************
                :to_string(doc,ali::string().append_(' ',indent))
            {}

            // ******************************************************************
            to_pretty_string( array const& arr, int indent = 3 )
            // ******************************************************************
                :to_string(arr,ali::string().append_(' ',indent))
            {}

            // ******************************************************************
            to_pretty_string( dict const& d, int indent = 3 )
            // ******************************************************************
                :to_string(d,ali::string().append_(' ',indent))
            {}
        };

        // ******************************************************************
        class from_string
            : public object
        // ******************************************************************
        {
        public:
            // ******************************************************************
            from_string( ali::string_const_ref str )
            // ******************************************************************
            {
                _valid = parse(*this,str);

                if(!_valid)
                    set_null();
            }

            // ******************************************************************
            bool is_valid( void ) const
            // ******************************************************************
            {
                return _valid;
            }

        private:
            bool _valid;
        };

        // ******************************************************************
        class walker
        // ******************************************************************
        {
        public:
            // ******************************************************************
            walker( ali::json::object const& object,
                    ali::string_const_ref path )
            // ******************************************************************
                : result(walk(object, path))
            {}

            // ******************************************************************
            walker( ali::json::array const& array,
                    ali::string_const_ref path )
            // ******************************************************************
                : result(walk(array, path))
            {}

            // ******************************************************************
            walker( ali::json::dict const& dict,
                    ali::string_const_ref path )
            // ******************************************************************
                : result(walk(dict, path))
            {}

            // ******************************************************************
            bool is_null( void )
            // ******************************************************************
            {
                return result == nullptr;
            }

            // ******************************************************************
            ali::json::object const* operator ->( void )
            // ******************************************************************
            {
                ali_assert(result != nullptr);
                return result;
            }

            // ******************************************************************
            ali::json::object const& operator *( void )
            // ******************************************************************
            {
                ali_assert(result != nullptr);
                return *result;
            }

        protected:
            ali::json::object const* walk( ali::json::object const& object,
                                           ali::string_const_ref path );

            ali::json::object const* walk( ali::json::array const& array,
                                           ali::string_const_ref path );

            ali::json::object const* walk( ali::json::dict const& dict,
                                           ali::string_const_ref path );

            ali::json::object const* result;
        };

        ali::array<bool> test( void );
    }
}
