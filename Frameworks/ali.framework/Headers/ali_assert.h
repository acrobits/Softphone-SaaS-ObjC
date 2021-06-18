/*
 *  ali_assert.h
 *  ali Library
 *
 *  Copyright 2013 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_debug.h"



/*
 !!!!! NO MACROS  !!!!!!
 */




/*
#if defined(DEBUG)
#   define ali_assert_run(expr, cmd) \
        ali_assert(expr)
#else
#   define ali_assert_run(expr, cmd) \
    if ( expr ) ; else cmd
#endif
*/

/** @brief Assert in debug or return in release
  * @param expr Expression that is checked
  * @param ... Possible return value */
/*
#define ali_assert_return(expr, ...) \
    ali_assert_run(expr, return __VA_ARGS__)
*/
/** @brief Assert in debug or break in release
  * @param expr Expression that is checked */
/*
#define ali_assert_break(expr) \
    ali_assert_run(expr, break)
*/
/** @brief Assert in debug or continue in release
  * @param expr Expression that is checked */
/*
#define ali_assert_continue(expr) \
    ali_assert_run(expr, continue)
*/
