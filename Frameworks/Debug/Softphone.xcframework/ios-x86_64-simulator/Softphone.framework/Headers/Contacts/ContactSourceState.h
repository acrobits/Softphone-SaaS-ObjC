/*
 *  ContactSourceState.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 7/29/15.
 *  Copyright 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

namespace Softphone::Contacts::Source
{
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    enum class State
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        Invalid,
        Initial,
        QueryPending,
        Idle,
        Error
    };

}
