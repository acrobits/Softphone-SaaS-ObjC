//
//  CallTypes.h
//  libsoftphone
//
//  Created by Jiri Kral on 4/27/12.
//  Copyright (c) 2012 Acrobits. All rights reserved.
//

#pragma once

#include "ali/ali_printf.h"
#include "ali/ali_time.h"

namespace Call
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct State
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Unknown,
            Trying,
            Ringing,
            Busy,
            IncomingTrying,
            IncomingRinging,
            IncomingIgnored,
            IncomingRejected,
            IncomingMissed,
            Established,
            Error,
            Unauthorized,
            Terminated,
            IncomingForwarded,
            IncomingAnsweredElsewhere,
            RedirectedToAlternativeService
        };

        static ali::string_literal toString(Type state);
        static bool isTerminal(Type state);
        static bool isFailed(Type state);

        friend ali::string& format(
            ali::string& str,
            Type value,
            ali::string_const_ref /*formatString*/ )
        {
            return str.append(Call::State::toString(value));
        }
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct HoldState
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            None,
            Active,
            Pending,
            Held
        };
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct HoldStates
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        HoldStates(ali::nullptr_type = nullptr)
        {}

        HoldStates(HoldState::Type l, HoldState::Type r)
        :local{l},remote{r}{}

        HoldState::Type local{HoldState::None};
        HoldState::Type remote{HoldState::None};
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Contact
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        ali::string scheme;
        ali::string userDisplayName;
        ali::string userName;
        ali::string domain;
        ali::string domainPort;

        ali::string phonebookRecordId;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct RecordingStatus
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum State
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Initial,
            Recording,
            Paused
        };

        ali::time::float_seconds duration;
        State state;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct MediaStatus
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct StreamStatus
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            bool operator==( StreamStatus const& b ) const
            {
                return  incomingActive == b.incomingActive
                &&  outgoingActive == b.outgoingActive
                &&  incomingPayloadNumber == b.incomingPayloadNumber
                &&  outgoingPayloadNumber == b.outgoingPayloadNumber;
            }

            bool operator!=( StreamStatus const& b ) const
            {
                return !operator==(b);
            }

            friend ali::string& format(
                ali::string& str,
                StreamStatus const& value,
                ali::string_const_ref /*formatString*/ )
            {
                using ali::operator""_s;
                return ali::printf_append(str,
                    "incoming %{} %{}, outgoing %{} %{}"_s,
                    value.incomingActive ? "active"_s : "inactive"_s,
                    value.incomingPayloadNumber,
                    value.outgoingActive ? "active"_s : "inactive"_s,
                    value.outgoingPayloadNumber);
            }

            bool        incomingActive{false};
            bool        outgoingActive{false};
            ali::string incomingPayloadNumber;
            ali::string outgoingPayloadNumber;
        };

        bool operator==( MediaStatus const& b ) const
        {
            return  audio == b.audio
            &&  video == b.video;
        }

        bool operator!=( MediaStatus const& b ) const
        {
            return !operator==(b);
        }

        friend ali::string& format(
            ali::string& str,
            MediaStatus const& value,
            ali::string_const_ref /*formatString*/ )
        {
            using ali::operator""_s;
            return ali::printf_append(str,
                "audio {%{}}, video {%{}}"_s,
                value.audio, value.video);
        }

        StreamStatus         audio;
        StreamStatus         video;
    };
}
