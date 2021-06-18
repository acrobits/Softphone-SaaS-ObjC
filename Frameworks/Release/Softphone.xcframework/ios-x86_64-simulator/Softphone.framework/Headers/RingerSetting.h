/*
 *  RingerSetting.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 01/15/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_string.h"
#include "Softphone/PreferenceKeys/KeyValueConvertor.h"

namespace Softphone
{
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct RingerSetting
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Value
        {
            Default,
            Yes,
            No
        };

        Value vibrate{Default};
        Value sound{Default};

        RingerSetting()
        {}

        RingerSetting(Value vibrate, Value sound)
            :vibrate{vibrate}
            ,sound{sound}
        {}

        RingerSetting(bool vibrate, bool sound)
            :vibrate{vibrate ? Yes : No}
            ,sound{sound ? Yes : No}
        {}

        ali::string toString() const;

        static RingerSetting fromString(ali::string_const_ref s);
    };
}



namespace PreferenceKeys
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    template<>
    class KeyValueConvertor<Softphone::RingerSetting>
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static ali::string_literal const typeName;

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        static ali::string toString(Softphone::RingerSetting const& v)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return v.toString();
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        static Softphone::RingerSetting fromString(ali::string_const_ref str)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return Softphone::RingerSetting::fromString(str);
        }
    };
}
