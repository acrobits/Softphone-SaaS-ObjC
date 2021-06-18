//
//  Call/DesiredMedia.h
//  libsoftphone
//
//  Copyright (c) 2012 - 2014 Acrobits s.r.o. All rights reserved.
//

#pragma once

#include "ali/ali_json.h"

namespace Call
{
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct DesiredMedia
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        DesiredMedia() :
            mAudioEnabled(false),mIncomingVideoEnabled(false),mOutgoingVideoEnabled(false),mTextMessageEnabled(false){}
        
        DesiredMedia(bool incomingVideoEnabled, bool outgoingVideoEnabled) :
            mAudioEnabled(true),
            mIncomingVideoEnabled(incomingVideoEnabled),
            mOutgoingVideoEnabled(outgoingVideoEnabled),
            mTextMessageEnabled(false){}

        DesiredMedia & enableAudio(bool e = true) { mAudioEnabled = e; return *this;}
        DesiredMedia & enableIncomingVideo(bool e = true) { mIncomingVideoEnabled = e; return *this;}
        DesiredMedia & enableOutgoingVideo(bool e = true) { mOutgoingVideoEnabled = e; return *this;}
        DesiredMedia & enableTextMessage(bool e = true) { mTextMessageEnabled = e; return *this;}

        bool isAudioEnabled() const {return mAudioEnabled;}
        bool isIncomingVideoEnabled() const {return mIncomingVideoEnabled;}
        bool isOutgoingVideoEnabled() const {return mOutgoingVideoEnabled;}
        bool isTextMessageEnabled() const {return mTextMessageEnabled;}

        static DesiredMedia voiceOnly() { return DesiredMedia().enableAudio();}
        static DesiredMedia videoBothWays() { return DesiredMedia(true,true);}

        static DesiredMedia fromTransients(ali::json::dict const& transients);
        void toTransients(ali::json::dict & transients) const;

    private:
        bool mAudioEnabled;
        bool mIncomingVideoEnabled;
        bool mOutgoingVideoEnabled;
        bool mTextMessageEnabled;
    };
}
