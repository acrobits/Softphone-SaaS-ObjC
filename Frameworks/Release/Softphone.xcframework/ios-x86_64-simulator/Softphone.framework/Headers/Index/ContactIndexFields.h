//
//  ContactIndexFields.h
//  libsoftphone
//
//  Created by Jiri Kral on 17/12/2019.
//
#pragma once

#include "ali/ali_string.h"


namespace Softphone::Index
{
    struct Fields
    {
        static const ali::string_literal     SourceId;
        static const ali::string_literal     DisplayName;
        static const ali::string_literal     DisplayNameWithoutDiacritics;
        static const ali::string_literal     Company;
        static const ali::string_literal     ContactOriginalEntries;
        static const ali::string_literal     ContactEntries;
        static const ali::string_literal     ContactEntriesReversed;
        static const ali::string_literal     T9DisplayName;
        static const ali::string_literal     ContactEntryPositions;
        static const ali::string_literal     HasPhoneNumber;
    };
}

