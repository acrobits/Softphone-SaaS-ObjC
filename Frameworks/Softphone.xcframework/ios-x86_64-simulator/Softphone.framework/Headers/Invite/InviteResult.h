#pragma once

#include "ali/ali_string.h"
#include "ali/ali_json.h"

namespace Softphone
{
namespace Invite
{
    using ResultCallback = ali::callback<void(ali::string const& message, ali::json::array const&)>;
}
}

