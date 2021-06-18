/*
 *  AccountFilter.h
 *
 *  Created by Jiri Kral on July 15th 2017
 *  Copyright 2017 Acrobits, s.r.o. All rights reserved.
 *
 */


#pragma once

#include "ali/ali_xml_tree2.h"
#include "Softphone/Mergeable/MergeableXmlFilter.h"

namespace Softphone
{
namespace Account
{
namespace Filter
{
    using namespace Mergeable::Filter;
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class RuleBased : public Mergeable::Filter::Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        RuleBased(ali::xml::tree const& xml);
        virtual ~RuleBased(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone() const override
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ALI_IMOVE(ali::new_auto_ptr<RuleBased>(*this));
        }
        
        virtual bool isNodeAllowed(ali::xml::tree const& node) const override;

    private:
        ali::xml::tree mXml;
    };
}
}
}

