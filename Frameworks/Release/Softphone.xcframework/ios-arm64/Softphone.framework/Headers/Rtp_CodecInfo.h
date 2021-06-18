#pragma once

#include "ali/ali_array_set_forward.h"
#include "ali/ali_tstring.h"
#include "ali/ali_xml_tree2_forward.h"


namespace Rtp
{

// ******************************************************************
struct CodecInfo
// ******************************************************************
{
    struct Media
    {
        enum Type {Unspecified, Audio, Video};

        Media( Type value = Unspecified )
        :   mValue(value)
        {}

        friend bool operator==( Media a, Media b )
        {
            return a.mValue == b.mValue;
        }

        friend bool operator!=( Media a, Media b )
        {
            return !(a == b);
        }

        bool isUnspecified( void ) const
        {
            return mValue == Unspecified;
        }

        bool isAudio( void ) const
        {
            return mValue == Audio;
        }

        bool isVideo( void ) const
        {
            return mValue == Video;
        }

        ali::c_string_const_ref displayName( void ) const;

        Type    mValue;
    };

    struct AudioInfo
    {
        AudioInfo( void )
        :   mSampleRateInHertz(0)
        {}

        ali::xml::tree& toLoggingXml(
            ali::xml::tree& xml ) const;

        int mSampleRateInHertz;
    };

    struct VideoInfo
    {
        VideoInfo( void ) {}

        ali::xml::tree& toLoggingXml(
            ali::xml::tree& xml ) const;
    };

    CodecInfo( void )
    :   mBitsPerSecond(0)
    {}

    ali::xml::tree& toLoggingXml(
        ali::xml::tree& xml ) const;

    friend bool operator==( CodecInfo const& a, CodecInfo const& b )
    {
        return  a.mMedia == b.mMedia
            &&  a.mDisplayName == b.mDisplayName
            &&  a.mDisplayNameShort == b.mDisplayNameShort
            &&  a.mBitsPerSecond == b.mBitsPerSecond
            &&  a.mPayloadNumber == b.mPayloadNumber;
    }

    friend bool operator!=( CodecInfo const& a, CodecInfo const& b )
    {
        return !(a == b);
    }

    Media           mMedia;
    ali::tstring    mDisplayName;
    ali::tstring    mDisplayNameShort;
    int             mBitsPerSecond;
    ali::string     mPayloadNumber;
    AudioInfo       mAudioInfo;
    VideoInfo       mVideoInfo;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct CodecInfoPayloadNumberComparator
// ******************************************************************
{
    int operator()(
        CodecInfo const& a,
        CodecInfo const& b ) const
    {
        using ali::compare;
        return compare(a.mPayloadNumber, b.mPayloadNumber);
    }

    int operator()(
        CodecInfo const& a,
        ali::string_const_ref b ) const
    {
        using ali::compare;
        return compare(a.mPayloadNumber, b);
    }

    int operator()(
        ali::string_const_ref a,
        CodecInfo const& b ) const
    {
        using ali::compare;
        return compare(a, b.mPayloadNumber);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
using CodecInfoSet = ali::array_set<
    CodecInfo, CodecInfoPayloadNumberComparator>;
// ******************************************************************

}   //  namespace Rtp
