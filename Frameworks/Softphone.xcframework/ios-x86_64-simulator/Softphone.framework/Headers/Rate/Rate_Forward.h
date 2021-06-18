//
//  Rate_Forward.h
//  libsoftphone
//
//  Created by Petr on 09/06/16.
//
//

#pragma once

#include "ali/ali_callback_forward.h"

namespace Rate
{
    struct Record;
    
    typedef ali::callback<void (Record const&)> RateCallback;
    
}   //  namespace Rate
