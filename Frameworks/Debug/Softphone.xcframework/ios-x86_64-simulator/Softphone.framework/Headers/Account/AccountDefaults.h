/*
 *  AccountDefaults.h
 *
 *  Created by Jiri Kral on July 14th 2017
 *  Copyright 2017 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_xml_tree2.h"
#include "Softphone/Account/AccountXmlNodeAttributes.h"
#include "Softphone/Account/AccountType.h"

#include "Softphone/Account/AccountValidator.h"

namespace Softphone
{
namespace Account
{
    class Xml;
    
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class Defaults
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
{
public:
    Defaults(Type type)
    :mType(type){}
    
    Defaults(Type type, ali::xml::tree const& initialDefaults)
    :mDefaults{initialDefaults},mType{type}{}
    
    Defaults(Defaults const& other)
    :mDefaults{other.mDefaults},mType{other.mType}{}
    
    ~Defaults() = default;
    
    /** @brief Merge-in mode account defaults
     * @param xml with defaults to merge
     * Old defaults will be overwritten by the ones from the passed xml.
     * Returns false in case the passed xml is in wrong format.
     */
    bool mergeDefaults(ali::xml::tree const& defaults);

    void mergeDefaults(Defaults const& defaults)
    {
        const bool success [[maybe_unused]] = mergeDefaults(defaults.mDefaults);
        ali_assert(success);
    }
    
    /** @brief Generate Account XML with default values filled in from this object
     */
    Xml getAccountXml(NodeAttributes const& defaultAttributes = {}) const;

    Validator::Array getValidators() const;
    
    Type getType() const {return mType;}
    
private:
    ali::xml::tree mDefaults;
    Type mType;
};

}
}
