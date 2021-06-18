/*
 * PushTestScheduleResult.h
 * libsoftphone
 *
 * Copyright 2013 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_string.h"


namespace Softphone
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct PushTestScheduleResult
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            Success,
            PushNotEnabled,
            NoPushToken,
            Timeout,
            Failure
        };
        
        static ali::c_string_const_ref toString( Type value );
        
        friend ali::string& format(
            ali::string& str,
            Type value,
            ali::string_const_ref /*formatString*/ )
        {
            return str.append(toString(value));
        }
    };
}
