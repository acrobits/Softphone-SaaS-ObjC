#pragma once

#include "ali/ali_string.h"

namespace Softphone
{
namespace Mergeable
{

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
struct NodeAttributes
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
{
    NodeAttributes(ali::xml::string const& source, int priority)
    :source(source),priority(priority){}
    
    NodeAttributes(){}
    ~NodeAttributes() = default;
    
    ali::xml::string source{ali::string_literal{"default"}};
    int priority{0};
};

}
}

