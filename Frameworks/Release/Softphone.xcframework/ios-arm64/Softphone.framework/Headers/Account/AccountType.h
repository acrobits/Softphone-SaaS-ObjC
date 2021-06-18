#pragma once

#include "ali/ali_string.h"

namespace Softphone
{
namespace Account
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Type
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Value
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Unknown,
            Sip,
            Xmpp,
            Gsm,
            GoogleVoice
        };

        explicit Type(ali::string_const_ref str)
            :mValue{fromString(str)}{}

        bool isSip()         const {return mValue == Sip;}
        bool isXmpp()        const {return mValue == Xmpp;}
        bool isGsm()         const {return mValue == Gsm;}
        bool isGoogleVoice() const {return mValue == GoogleVoice;}

        ali::string_literal toString() const;

        Value get() const {return mValue;}

        static Type unknown() {return Type(Unknown);}
        static Type sip() {return Type(Sip);}
        static Type xmpp() {return Type(Xmpp);}
        static Type gsm() {return Type(Gsm);}
        static Type googleVoice() {return Type(GoogleVoice);}

        inline bool operator == (Type const& other) const
        {
            return mValue == other.mValue;
        }

        inline bool operator != (Type const& other) const
        {
            return !operator == (other);
        }

    private:
        explicit Type(Value v):mValue(v){}

        static Value fromString(ali::string_const_ref str);

    private:
        Value  mValue;
    };
}
}
