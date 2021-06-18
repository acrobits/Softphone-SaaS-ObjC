#pragma once

#include "ali/ali_array_utils_forward.h"
#include "ali/ali_string_forward.h"
#include "ali/ali_tchar.h"

namespace ali
{

#ifdef UNICODE

using tstring_ref = wstring_ref;
using tstring_ptr = wstring_ptr;
using tstring_const_ref = wstring_const_ref;
using tstring_const_ptr = wstring_const_ptr;
using c_tstring_const_ref = c_wstring_const_ref;
using c_tstring_const_ptr = c_wstring_const_ptr;
using tstring_literal = wstring_literal;
using tstring = wstring;

namespace tstr
{

using namespace wstr;

}   //  namespace tstr

#else

using tstring_ref = string_ref;
using tstring_ptr = string_ptr;
using tstring_const_ref = string_const_ref;
using tstring_const_ptr = string_const_ptr;
using c_tstring_const_ref = c_string_const_ref;
using c_tstring_const_ptr = c_string_const_ptr;
using tstring_literal = string_literal;
using tstring = string;

namespace tstr
{

using namespace str;

}   //  namespace tstr

#endif  //  UNICODE

}   //  namespace ali

