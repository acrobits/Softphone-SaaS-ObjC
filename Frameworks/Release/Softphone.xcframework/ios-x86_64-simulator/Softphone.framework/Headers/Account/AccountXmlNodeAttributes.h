#pragma once

#include "ali/ali_string.h"
#include "Softphone/Mergeable/MergeableXmlNodeAttributes.h"

namespace Softphone
{
namespace Account
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct NodeAttributes : public Mergeable::NodeAttributes
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        NodeAttributes(ali::xml::string const& source, int priority)
        :Mergeable::NodeAttributes(source,priority){}
        
        NodeAttributes():Mergeable::NodeAttributes(){}
        
        static NodeAttributes defaults();
        static NodeAttributes legacy();
        static NodeAttributes provisioning();
        static NodeAttributes externalProvisioning();
        static NodeAttributes gui();
        static NodeAttributes system();
        static NodeAttributes addonInit();
    };

}
}

