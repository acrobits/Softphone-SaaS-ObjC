//
//  ComposingInfo.hpp
//  libsoftphone
//
//  Created by Stanislav Kutil on 29.01.18.
//

#pragma once
#include "Softphone/EventHistory/EventHistory.h"

namespace Softphone::Messaging
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct ComposingInfo
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        ComposingInfo(EventHistory::EventStream::Pointer stream) : mStream(stream) {};
        
        EventHistory::EventStream::Pointer      mStream;
        ali::optional<ali::mime::content_type>  mContentType;
        ali::string                             mAccountId;
        bool                                    mActive{true};
    };
    
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct InboundComposingInfo : public ComposingInfo
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        InboundComposingInfo(EventHistory::EventStream::Pointer stream, ali::generic_peer_address const& sender);
        
        ali::string getPartyId() const;
        
        Softphone::EventHistory::StreamParty        mParty;
    };

}
