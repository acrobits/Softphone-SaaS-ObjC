/*
 *  ali_serializer.h
 *  ali Library
 *
 *  Copyright (c) 2010 - 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_array_utils.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_blob_forward.h"
#include "ali/ali_shared_ptr_forward.h"
#include "ali/ali_utility.h"

namespace ali
{

// ******************************************************************
class seekable
// ******************************************************************
{
public:
    virtual ~seekable( void ) {}
    virtual ali::int64 beg( ali::int64 off ) = 0;
    virtual ali::int64 cur( ali::int64 off ) = 0;
    virtual ali::int64 end( ali::int64 off ) = 0;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class commonlizer
// ******************************************************************
{
public:
    virtual ~commonlizer( void ) {}

    virtual seekable* seek( void ) = 0;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class serializer : public commonlizer
// ******************************************************************
{
public:
    virtual ~serializer( void ) {}

    virtual int write( blob_const_ref buf ) = 0;

    bool write_exact( blob_const_ref buf )
    {
        return write(buf) == buf.size();
    }

    virtual ali::int64 avail_to_write( void ) const = 0;
        //  Returns meta::integer::max_value<ali::int64>::result
        //  when the size is not known in advance.
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
class deserializer : public commonlizer
// ******************************************************************
{
public:
    virtual ~deserializer( void ) {}

    virtual int read( blob_ref buf ) = 0;

    bool read_all( blob& buf );
        //  This methods *appends* the data to the buffer.

    blob read_all( void );
        //  Returns empty blob on failure.

    bool read_exact( blob_ref buf )
    {
        return read(buf) == buf.size();
    }

    virtual ali::int64 avail_to_read( void ) const = 0;
        //  Returns meta::integer::max_value<ali::int64>::result
        //  when the size is not known in advance.
};

// ******************************************************************
// ******************************************************************

namespace serializer_concept
{

// ******************************************************************
template <typename instance>
inline seekable* do_seek( instance& s )
// ******************************************************************
{
    return s.seek();
}

// ******************************************************************
template <typename instance>
inline int do_write( instance& s, blob_const_ref buf )
// ******************************************************************
{
    return s.write(buf);
}

// ******************************************************************
template <typename instance>
inline ali::int64 do_avail_to_write( instance const& s )
// ******************************************************************
{
    return s.avail_to_write();
}

// ******************************************************************
// ******************************************************************

}   //  namespace serializer_concept

// ******************************************************************
class serializer2
// ******************************************************************
{
public:
    serializer2( void ) {}

    serializer2( serializer2&& b )
    :   _concept{ali::move(b._concept)}
    {}

    template <typename instance, typename... params>
    static serializer2 from( params&&... p )
    {
        return serializer2{new_auto_ptr<
            model<instance>>(ali::forward<params>(p)...)};
    }

    static serializer2 from( ali::auto_ptr<serializer> s )
    {
        return serializer2{ali::move(s)};
    }

    static serializer2 from(
        ali::shared_ptr<serializer> const& s );

    template <typename instance>
    static serializer2 from_ref( instance& s )
    {
        return serializer2{new_auto_ptr<
            model_ref<instance>>(s)};
    }

    serializer2& operator=( serializer2&& b )
    {
        _concept = ali::move(b._concept);
        
        return *this;
    }

    bool is_null( void ) const
    {
        return _concept.is_null();
    }

    friend bool is_null( serializer2 const& a )
    {
        return a.is_null();
    }

    seekable* seek( void )
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? nullptr : _concept->seek();
    }

    int write( blob_const_ref buf )
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? 0 : _concept->write(buf);
    }

    bool write_exact( blob_const_ref buf )
    {
        ali_assert(!_concept.is_null());
        
        return  !_concept.is_null()
            &&  _concept->write_exact(buf);
    }

    ali::int64 avail_to_write( void ) const
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? 0 : _concept->avail_to_write();
    }

    void reset( void )
    {
        _concept.reset();
    }

private:    //  Class
    template <typename instance>
    class model : public serializer
    {
    public:
        template <typename... params>
        explicit model( params&&... p )
        :   _instance{ali::forward<params>(p)...}
        {}

        virtual seekable* seek( void ) override
        {
            using namespace serializer_concept;

            return do_seek(deref(this->_instance));
        }

        virtual int write( blob_const_ref buf ) override
        {
            using namespace serializer_concept;

            return do_write(deref(this->_instance), buf);
        }

        virtual ali::int64 avail_to_write( void ) const override
        {
            using namespace serializer_concept;

            return do_avail_to_write(deref(this->_instance));
        }

    private:    //  Methods
        template <typename inner>
        static inner& deref( ali::shared_ptr<inner> const& s )
        {
            return *s;
        }

        template <typename inner>
        static inner& deref( ali::shared_ptr<inner>& s )
        {
            return *s;
        }

        template <typename T>
        static T& deref( T& s )
        {
            return s;
        }

    private:    //  Data members
        instance    _instance;
    };

private:    //  Class
    template <typename instance>
    class model_ref : public serializer
    {
    public:
        explicit model_ref( instance& s )
        :   _instance{s}
        {}

        virtual seekable* seek( void ) override
        {
            using namespace serializer_concept;

            return do_seek(this->_instance);
        }

        virtual int write( blob_const_ref buf ) override
        {
            using namespace serializer_concept;

            return do_write(this->_instance, buf);
        }

        virtual ali::int64 avail_to_write( void ) const override
        {
            using namespace serializer_concept;

            return do_avail_to_write(this->_instance);
        }

    private:    //  Data members
        instance&   _instance;
    };

private:    //  Methods
    explicit serializer2( ali::auto_ptr<serializer> c )
    :   _concept{ali::move(c)}
    {}

    serializer2( serializer2 const& ) = delete;
    serializer2& operator=( serializer2 const& ) = delete;

private:    //  Data members
    ali::auto_ptr<serializer>   _concept{};
};

// ******************************************************************
// ******************************************************************

namespace deserializer_concept
{

// ******************************************************************
template <typename instance>
inline seekable* do_seek( instance& d )
// ******************************************************************
{
    return d.seek();
}

// ******************************************************************
template <typename instance>
inline int do_read( instance& d, blob_ref buf )
// ******************************************************************
{
    return d.read(buf);
}

// ******************************************************************
template <typename instance>
inline ali::int64 do_avail_to_read( instance const& d )
// ******************************************************************
{
    return d.avail_to_read();
}

// ******************************************************************
// ******************************************************************

}   //  namespace deserializer_concept

// ******************************************************************
class deserializer2
// ******************************************************************
{
public:
    deserializer2( void ) {}

    deserializer2( deserializer2&& b )
    :   _concept{ali::move(b._concept)}
    {}

    template <typename instance, typename... params>
    static deserializer2 from( params&&... p )
    {
        return deserializer2{new_auto_ptr<
            model<instance>>(ali::forward<params>(p)...)};
    }

    static deserializer2 from( ali::auto_ptr<deserializer> d )
    {
        return deserializer2{ali::move(d)};
    }

    static deserializer2 from(
        ali::shared_ptr<deserializer> const& d );

    template <typename instance>
    static deserializer2 from_ref( instance& d )
    {
        return deserializer2{new_auto_ptr<
            model_ref<instance>>(d)};
    }

    deserializer2& operator=( deserializer2&& b )
    {
        _concept = ali::move(b._concept);
        
        return *this;
    }

    bool is_null( void ) const
    {
        return _concept.is_null();
    }

    friend bool is_null( deserializer2 const& a )
    {
        return a.is_null();
    }

    seekable* seek( void )
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? nullptr : _concept->seek();
    }

    int read( blob_ref buf )
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? 0 : _concept->read(buf);
    }

    bool read_all( blob& buf )
        //  This methods *appends* the data to the buffer.
    {
        ali_assert(!_concept.is_null());
        
        return  !_concept.is_null()
            &&  _concept->read_all(buf);
    }

    blob read_all( void );
        //  Returns empty blob on failure.

    bool read_exact( blob_ref buf )
    {
        ali_assert(!_concept.is_null());
        
        return  !_concept.is_null()
            &&  _concept->read_exact(buf);
    }

    ali::int64 avail_to_read( void ) const
    {
        ali_assert(!_concept.is_null());
        
        return _concept.is_null() ? 0 : _concept->avail_to_read();
    }

    void reset( void )
    {
        _concept.reset();
    }

private:    //  Class
    template <typename instance>
    class model : public deserializer
    {
    public:
        template <typename... params>
        explicit model( params&&... p )
        :   _instance{ali::forward<params>(p)...}
        {}

        virtual seekable* seek( void ) override
        {
            using namespace deserializer_concept;

            return do_seek(deref(this->_instance));
        }

        virtual int read( blob_ref buf ) override
        {
            using namespace deserializer_concept;

            return do_read(deref(this->_instance), buf);
        }

        virtual ali::int64 avail_to_read( void ) const override
        {
            using namespace deserializer_concept;

            return do_avail_to_read(deref(this->_instance));
        }

    private:    //  Methods
        template <typename inner>
        static inner& deref( ali::shared_ptr<inner> const& d )
        {
            return *d;
        }

        template <typename inner>
        static inner& deref( ali::shared_ptr<inner>& d )
        {
            return *d;
        }

        template <typename T>
        static T& deref( T& d )
        {
            return d;
        }

    private:    //  Data members
        instance    _instance;
    };

private:    //  Class
    template <typename instance>
    class model_ref : public deserializer
    {
    public:
        explicit model_ref( instance& s )
        :   _instance{s}
        {}

        virtual seekable* seek( void ) override
        {
            using namespace deserializer_concept;

            return do_seek(this->_instance);
        }

        virtual int read( blob_ref buf ) override
        {
            using namespace deserializer_concept;

            return do_read(this->_instance, buf);
        }

        virtual ali::int64 avail_to_read( void ) const override
        {
            using namespace deserializer_concept;

            return do_avail_to_read(this->_instance);
        }

    private:    //  Data members
        instance&   _instance;
    };

private:    //  Methods
    explicit deserializer2( ali::auto_ptr<deserializer> c )
    :   _concept{ali::move(c)}
    {}

    deserializer2( deserializer2 const& ) = delete;
    deserializer2& operator=( deserializer2 const& ) = delete;

private:    //  Data members
    ali::auto_ptr<deserializer> _concept{};
};

}   //  namespace ali
