//
//  LocalizationHelper.h
//  softphone
//
//  Created by Adeel on 30/03/2017.
//
//
#pragma once

#include "ali/ali_string.h"
#include "ali/ali_xml_tree2.h"

namespace Localization
{

struct LocalizableString : ali::xml::string
{
    LocalizableString() {};
    
    LocalizableString(
        ali::string_const_ref string,
        bool localizable = false,
        ali::string_const_ref context = {} )
    :   ali::xml::string{ali::move(string)},
        mLocalizable{localizable},
        mContext{ali::move(context)}
    {};
    
    bool mLocalizable{false};
    ali::xml::string mContext{};
};

namespace Helpers
{
    LocalizableString getLocalizedNodeText(ali::string_const_ref language,
                                           ali::string_const_ref locale,
                                           ali::xml::tree const& xml,
                                           ali::string_const_ref nodeName,
                                           LocalizableString const& defaultValue = {} );

    LocalizableString getLocalizedNodeText(ali::xml::tree const& xml,
                                           ali::string_const_ref nodeName,
                                           LocalizableString const& defaultValue = {} );
}
}
