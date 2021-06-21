/*
 *  MicrophoneType.h
 *  softphone
 *
 *  Created by Jiri Kral on 10/18/19.
 *  Copyright 2019 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_utils.h"
#include "ali/ali_string.h"

namespace Softphone
{
    // ******************************************************************
    struct Microphone
    // ******************************************************************
    {
        // ******************************************************************
        enum Type
        // ******************************************************************
        {
            Unknown     = 0,
            Physical    = 1 << 1,
            Simulated   = 1 << 2        // values should be 2^n
        };

        static ali::c_string_const_ref toString( Type );
    };
}
