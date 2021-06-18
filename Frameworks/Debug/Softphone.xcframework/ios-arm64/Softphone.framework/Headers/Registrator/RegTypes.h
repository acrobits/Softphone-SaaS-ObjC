//
//  RegTypes.h
//  libsoftphone
//
//  Copyright (c) 2012 - 2017 Acrobits, s.r.o. All rights reserved.
//

#pragma once

#include "ali/ali_string.h"

namespace Registrator
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct State
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            None,
            Discovering,
            NotRegistered,
            PushHandshake,
            Registering,
            Registered,
            Unregistering,
            Unauthorized,
            Error
        };

        static ali::string_literal toString(Type state);
        
        friend ali::string& format(
            ali::string& str,
            Type value,
            ali::string_const_ref /*formatString*/ )
        {
            return str.append(toString(value));
        }
    };
}
