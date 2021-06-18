/*
 *  AudioRoute.h
 *  softphone
 *
 *  Created by Jiri Kral on 1/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_utils.h"
#include "ali/ali_string.h"

namespace Softphone
{
    // ******************************************************************
    struct AudioRoute
    // ******************************************************************
    {
        // ******************************************************************
        enum Type
        // ******************************************************************
        {
            Unselected = 0,
            Receiver = 1,
            Speaker = 2,
            BluetoothSCO = 3,
            BluetoothA2DP = 4,
            Headset = 5, /// may be Receiver if detection is not supported
        };

        static ali::c_string_const_ref toString( Type );
        static AudioRoute::Type fromString(ali::string_const_ref route);
    };



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
