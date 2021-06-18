/*
 *  CountryCode/DetectionResult.h
 *  softphone
 *
 *  Created by Jiri Kral on 1/26/09.
 *  Copyright 2016 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_string.h"

namespace Softphone
{
namespace CountryCode
{
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct DetectionResult
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        ali::string title;
        ali::string regionalPrefix;
        ali::string iso2Code;
        ali::string olympicCode;
        ali::string formattedNumber;
        ali::string e164Number;
    };
}
}

