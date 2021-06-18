//
//  MediaUploaderAgent.hpp
//  libsoftphone
//
//  Created by Stanislav Kutil on 10/04/2017.
//
//

#pragma once

#include "ali/ali_handle.h"

namespace Softphone::Messaging
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class TransferHandle : public ali::handle
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        virtual ~TransferHandle() {};
    public:
        bool                                            mAutoCancel{false};
    };
}
