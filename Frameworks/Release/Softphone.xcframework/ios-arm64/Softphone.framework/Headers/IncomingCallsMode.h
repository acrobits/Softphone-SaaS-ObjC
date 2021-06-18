/*
 *  IncomingCallsMode.h
 *  softphone
 *
 *  Created by Jiri Kral on 10/27/16.
 *  Copyright 2009-2016 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_string.h"

namespace Softphone
{
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct IncomingCallsMode
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Strings
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            static const ali::string_literal Auto;
            static const ali::string_literal Push;
            static const ali::string_literal KeepAwake;
            static const ali::string_literal Off;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        enum Type
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            Auto,
            Push,
            KeepAwake,
            Off
        };

        IncomingCallsMode() = default;

        IncomingCallsMode(Type v)
        :value(v){}

        static IncomingCallsMode fromString(ali::string_const_ref str);
        static ali::string_const_ref toString(IncomingCallsMode const& t);

        inline bool isAuto() const      { return value == Auto; }
        inline bool isPush() const      { return value == Push; }
        inline bool isKeepAwake() const { return value == KeepAwake; }
        inline bool isOff() const       { return value == Off;  }

        inline bool operator ==(IncomingCallsMode const& other) const
        {
            return value == other.value;
        }

        inline bool operator !=(IncomingCallsMode const& other) const
        {
            return !operator == (other);
        }

        Type value { Off };
    };
}

