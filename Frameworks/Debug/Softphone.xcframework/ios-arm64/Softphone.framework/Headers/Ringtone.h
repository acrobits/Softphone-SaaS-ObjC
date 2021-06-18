/*
 *  Ringtone.h
 *  softphone
 *
 *  Copyright (c) 2009 - 2016 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include <ali/ali_auto_ptr.h>
#include <ali/ali_string.h>

namespace ToneGenerator
{
    class Base;
}

namespace Ringtone
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        ali::string_literal type;

    public:
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Base(ali::string_literal type)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        : type{type}
        {}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ~Base()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {}

        // Returns nullptr if the ringtone is played externally
        virtual ali::auto_ptr<ToneGenerator::Base> play(int samplingRate) = 0;
        virtual void stop() = 0;
        virtual bool isPlaying() const = 0;
        virtual Base* copy() const = 0;
    };

};
