#pragma once

//#define ALI_EXCLUDE_DEPRECATED

#ifdef  ALI_EXCLUDE_DEPRECATED
#   define ALI_DEPRECATED(code)
#else   // !ALI_EXCLUDE_DEPRECATED
#   define ALI_DEPRECATED(code) code
#endif  //  ALI_EXCLUDE_DEPRECATED
