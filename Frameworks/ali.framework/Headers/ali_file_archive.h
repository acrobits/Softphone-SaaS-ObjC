#pragma once
#include "ali/ali_background_work.h"
#include "ali/ali_error.h"
#include "ali/ali_filesystem2.h"
#include "ali/ali_message.h"
#include "ali/ali_serializer.h"
#include "ali/ali_shared_ptr.h"
#include "ali/ali_variant.h"

namespace ali::file_archive
{

// ******************************************************************
struct data_source
// ******************************************************************
//  Move-only struct holding information about file data source.
// ******************************************************************
{
    data_source( void ) {}

    data_source( data_source const& b ) = delete;
    data_source( data_source&& b ) = default;

    data_source& operator=( data_source const& ) = delete;
    data_source& operator=( data_source&& ) = default;

    static data_source external_file( filesystem2::path path )
    {
        data_source data{};
        data.source = ali::move(path);
        return data;
    }

    static data_source external_file( tstring_const_ref path )
    {
        return external_file(filesystem2::path{path});
    }

    static data_source deserializer( deserializer2 d )
    {
        data_source data{};
        data.source = ali::move(d);
        return data;
    }

    static data_source deserializer(
        ali::auto_ptr<ali::deserializer> d )
    {
        return deserializer(deserializer2::from(ali::move(d)));
    }

    static data_source memory( ali::blob&& m )
    {
        data_source data{};
        data.source = ali::move(m);
        return data;
    }

    static data_source memory( ali::string&& m )
    {
        data_source data{};
        data.source = ali::move(m);
        return data;
    }
    
    ali::variant<
        filesystem2::path,
        deserializer2,
        ali::blob,
        ali::string>    source;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct data_sink
// ******************************************************************
//  Move-only struct holding information
//  about the archive file data sink.
//  NOTE: This should NOT be extended with a simple
//  serializer support--we need to be able to communicate
//  to the sink the fact that the archive has been actually
//  properly finished and that it is OK to keep it.
//  By default, the sink should destroy the data if it is
//  being destroyed without the above confirmation.
// ******************************************************************
{
    data_sink( void ) {}

    data_sink( data_source const& b ) = delete;
    data_sink( data_sink&& b ) = default;

    data_sink& operator=( data_sink const& ) = delete;
    data_sink& operator=( data_sink&& ) = default;

    static data_sink external_file( filesystem2::path path )
    {
        data_sink data{};
        data.sink = ali::move(path);
        return data;
    }

    static data_sink external_file( tstring_const_ref path )
    {
        return external_file(filesystem2::path{path});
    }

    ali::variant<
        filesystem2::path>  sink;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct writer_state
// ******************************************************************
{
    enum type
    {
        accepting_entries,
        writing_archive,
        finished,
        error
    };

    writer_state( type value )
    :   value{value}
    {}

    friend bool operator==( writer_state a, writer_state b )
    {
        return a.value == b.value;
    }

    friend bool operator!=( writer_state a, writer_state b )
    {
        return !(a == b);
    }

    bool is_accepting_entries( void ) const
    {
        return value == accepting_entries;
    }

    bool is_writing_archive( void ) const
    {
        return value == writing_archive;
    }

    bool is_finished( void ) const
    {
        return value == finished;
    }

    bool is_error( void ) const
    {
        return value == error;
    }

    type    value;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct writer_progress_info
// ******************************************************************
{
    filesystem2::path   path_in_archive{};
        //  In-archive-path of input data source currently
        //  being processed.
    
    ali::int64          uncompressed_size{};
        //  Sum of sizes of all input data sources.
    
    ali::int64          uncompressed_position{};
        //  How many input bytes the writer has processed so far.
        //  0 <= uncompressed_position <= uncompressed_size

    ali::int64          compressed_size{};
        //  How many output bytes the writer has written so far.
};

// ******************************************************************
// ******************************************************************

namespace tar
{

// ******************************************************************
class writer
// ******************************************************************
{
public:
    writer( void );
    writer( writer const& ) = delete;
    ~writer( void );

    writer& operator=( writer const& ) = delete;

    writer_state state( void ) const
    {
        return _state;
    }

    void notify_state_changed( message::auto_ptr msg )
    {
        _state_changed.arm(ali::move(msg));
    }

    void cancel_state_changed( void )
    {
        _state_changed.reset();
    }

    error_stack& error( error_stack& error ) const;

    //   pre:   this->state().is_accepting_entries()
    //  post:   this->state().is_accepting_entries()
    //      ||  this->state().is_error()
    writer& add_file(
        filesystem2::path path_in_archive,
        data_source&& data );

    //   pre:   this->state().is_accepting_entries()
    //  post:   this->state().is_accepting_entries()
    //      ||  this->state().is_error()
    writer& add_file(
        tstring_const_ref path_in_archive,
        data_source&& data )
    {
        return add_file(
            filesystem2::path{path_in_archive},
            ali::move(data));
    }

    //   pre:   this->state().is_accepting_entries()
    //  post:   this->state().is_writing_archive()
    //      ||  this->state().is_error()
    void write_archive_async(
        data_sink&& sink,
        filesystem2::file::overwrite overwrite_if_exists
            = filesystem2::file::overwrite::no );

    //   pre:   this->state().is_accepting_entries()
    //  post:   this->state().is_finished()
    //      ||  this->state().is_error()
    void write_archive(
        data_sink&& sink,
        filesystem2::file::overwrite overwrite_if_exists
            = filesystem2::file::overwrite::no );

    writer_progress_info& progress(
        writer_progress_info& info ) const;

    //  post:   this->state().is_accepting_entries()
    void reset( void );

private:    //  Struct
    struct shared_info;
    class writing_state;

private:    //  Methods
    void set_state( writer_state state );
    
private:    //  Data members
    writer_state                    _state{writer_state::accepting_entries};
    message::holder                 _state_changed{};
    ali::shared_ptr<shared_info>    _shared{};
    ali::auto_ptr<
        background_work_result<
            void>>                  _result{};
};

}   //  namespace tar

}   //  namespace ali::file_archive
