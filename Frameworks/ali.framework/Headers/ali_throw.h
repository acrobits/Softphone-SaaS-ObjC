/*
 *  ali_throw.h
 *  ali Library
 *
 *  Copyright 2010 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#define ALI_THROW_LOC(e, l) \
    throw (e ^ l)

#define ALI_THROW(e) \
    ALI_THROW_LOC(e, ALI_HERE)

#define ALI_TRY \
    try

#define ALI_CATCH(t, v) \
    catch (const t &v)

#define ALI_CATCH_ALL \
    catch (...)

#define ALI_RETHROW \
    throw
