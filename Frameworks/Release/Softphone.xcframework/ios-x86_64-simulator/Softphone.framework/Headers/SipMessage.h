#pragma once
#include "ali/ali_net_address.h"
#include "ali/ali_time.h"

namespace Softphone
{

// *******************************************************************
struct SipMessage
// *******************************************************************
{
    struct Direction
    {
        enum Enum {Unspecified, Outgoing, Incoming};
        
        Direction( Enum value = Unspecified )
        :   mValue{value}
        {}
        
        friend bool operator==( Direction a, Direction b )
        {
            return a.mValue == b.mValue;
        }
        
        friend bool operator!=( Direction a, Direction b )
        {
            return !(a == b);
        }
        
        bool isUnspecified( void ) const
        {
            return mValue == Unspecified;
        }
        
        bool isOutgoing( void ) const
        {
            return mValue == Outgoing;
        }
        
        bool isIncoming( void ) const
        {
            return mValue == Incoming;
        }
        
        ali::c_string_const_ref identifier( void ) const;
        
        bool fromIdentifier( ali::string_const_ref identifier );
        
        Enum    mValue{Unspecified};
    };
    
    ali::time::unix_micro_timestamp mTimestamp{};
    
    Direction                       mDirection{};
    
    ali::network::address           mLocalAddress{};
    
    ali::network::address           mRemoteAddress{};
    
    ali::string                     mMessage{};
};
    
}   //  namespace Softphone
