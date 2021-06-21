/*
 *  CallHandlerAppInterface.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2018 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "Softphone/DialAction/DialActionType.h"
#include "ali/ali_array.h"
#include "Softphone/EventHistory/MessageEvent.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
struct CallActionSheetConfig
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Entry
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Entry(Softphone::DialAction::Id const& action,
              ali::string_const_ref displayName = {})
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            : action(action)
            , displayName(displayName)
        {}

        Softphone::DialAction::Id   action;
        ali::string                 displayName;
    };

    ali::string                                         title;
    Softphone::EventHistory::Event::Pointer             event;
    ali::string                                         accountId;
    Softphone::DialAction::Id                           defaultAction;

    bool                                                haveCustomDialActions;
    
    ali::array<Entry>                                   entries;
};
