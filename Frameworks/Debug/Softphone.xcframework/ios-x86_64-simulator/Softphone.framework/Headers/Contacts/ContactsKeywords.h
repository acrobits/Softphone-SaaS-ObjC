/*
 *  Contacts/Keywords.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 7/30/15.
 *  Copyright 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_string.h"

namespace Softphone
{
namespace Contacts
{
    struct Keyword
    {
        static const ali::string_literal SOURCE_ID;
        static const ali::string_literal CONTACT_ID;
        static const ali::string_literal DEVICE_ID;
        static const ali::string_literal DISPLAY_NAME;
        static const ali::string_literal DISPLAY_NAME_WITHOUT_DIACRITICS;
        static const ali::string_literal CONTACT_ENTRIES;
        static const ali::string_literal CONTACT_ADDRESSES;
        static const ali::string_literal BLOCK_ID;
        static const ali::string_literal CHECKSUM;
        static const ali::string_literal VERSION;
        static const ali::string_literal ENTRY_ID;
        static const ali::string_literal TYPE;
        static const ali::string_literal URI;
        static const ali::string_literal CURI;
        static const ali::string_literal CONTACTS;
        static const ali::string_literal LABEL;
        static const ali::string_literal AVATAR;
        static const ali::string_literal LARGE_AVATAR;
        static const ali::string_literal ENTRY_TYPE_TEL;
        static const ali::string_literal ENTRY_TYPE_EMAIL;
        static const ali::string_literal ENTRY_TYPE_URL;
        static const ali::string_literal HAS_PHONE_NUMBER;

        static const ali::string_literal FIRST_NAME;
        static const ali::string_literal MIDDLE_NAME;
        static const ali::string_literal LAST_NAME;
        static const ali::string_literal COMPANY;
        static const ali::string_literal JOB_TITLE;
        static const ali::string_literal DEPARTMENT_NAME;
        static const ali::string_literal BIRTHDAY;
        static const ali::string_literal ADDR_STREET;
        static const ali::string_literal ADDR_CITY;
        static const ali::string_literal ADDR_STATE;
        static const ali::string_literal ADDR_ZIP;
        static const ali::string_literal ADDR_COUNTRY;
        static const ali::string_literal ADDR_COUNTRY_CODE;
        static const ali::string_literal NOTES;
        static const ali::string_literal RINGTONE;

        static const ali::string_literal PHONETIC_FIRST_NAME;
        static const ali::string_literal PHONETIC_MIDDLE_NAME;
        static const ali::string_literal PHONETIC_LAST_NAME;
        static const ali::string_literal NICK;
        static const ali::string_literal NAME_PREFIX;
        static const ali::string_literal NAME_SUFFIX;

        static const ali::string_literal SORT_KEY;
        static const ali::string_literal AUXILIARY;
    };
}
}

