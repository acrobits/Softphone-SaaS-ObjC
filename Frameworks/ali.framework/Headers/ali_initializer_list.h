#pragma once

#include "ali/ali_features.h"

#if defined(ALI_FEATURE_INITIALIZER_LIST)

#include <initializer_list>


namespace ali
{

using std::initializer_list;

}

#else

namespace ali
{

// Dummy implemenation
template <typename T>
struct initializer_list
{};

}

#endif
