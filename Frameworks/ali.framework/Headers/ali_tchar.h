/*
 *  ali_tchar.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_wchar.h"

#ifdef UNICODE

namespace ali { using tchar = wchar; }

#define ALI_HIDDEN_T(x)   L ## x

#else

namespace ali { using tchar = char; }

#define ALI_HIDDEN_T(x)   x

#endif // UNICODE

#define ALI_T(x)                    ALI_HIDDEN_T(x)


#define ALI_HIDDEN_STRINGIZE(x)     #x
#define ALI_STRINGIZE(x)            ALI_HIDDEN_STRINGIZE(x)

#define ALI_HIDDEN_WSTRINGIZE(x)    L ## #x
#define ALI_WSTRINGIZE(x)           ALI_HIDDEN_WSTRINGIZE(x)

#define ALI_HIDDEN_TSTRINGIZE(x)    ALI_T(#x)
#define ALI_TSTRINGIZE(x)           ALI_HIDDEN_TSTRINGIZE(x)
