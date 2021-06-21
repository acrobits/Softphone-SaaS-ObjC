//
//  DialogInfo.h
//  libsoftphone
//
//  Created by Jiri Kral on 2018-02-07
//
//

#pragma once

#include "ali/ali_array.h"
#include "ali/ali_array_map.h"
#include "ali/ali_stable_string.h"
#include "ali/ali_string.h"
#include "ali/ali_time.h"

namespace Softphone
{

// ******************************************************************
struct BusyLampFieldUri
// ******************************************************************
{
    ali::string uri;
    ali::string accountId;
    
    bool operator==(BusyLampFieldUri const&  b) const
    {
        return uri == b.uri && accountId == b.accountId;
    }

    bool operator<(BusyLampFieldUri const&  b) const
    {
        if (uri == b.uri)
            return accountId < b.accountId;
        return uri < b.uri;
    }

    friend int compare(BusyLampFieldUri const& a, BusyLampFieldUri const& b)
    {
        using ali::compare;

        int result = compare(a.uri, b.uri);

        if (result == 0)
            result = compare(a.accountId, b.accountId);

        return result;
    }
};

// ******************************************************************
struct BusyLampField
// ******************************************************************
{
public:     //  Enum
    struct State
    {
        enum Enum
        {
            Unknown,
            WaitingForFirstStatusUpdate,
            Idle,
            Ringing,
            Established,
            Error
        };
        
        State( Enum value = Unknown )
        :   mValue{value}
        {}

        bool isUnknown( void ) const
        {
            return mValue == Unknown;
        }

        bool isWaitingForFirstStatusUpdate( void ) const
        {
            return mValue == WaitingForFirstStatusUpdate;
        }

        bool isIdle( void ) const
        {
            return mValue == Idle;
        }

        bool isRinging( void ) const
        {
            return mValue == Ringing;
        }

        bool isOnCall( void ) const
        {
            return mValue == Established;
        }

        bool isError( void ) const
        {
            return mValue == Error;
        }

        State& set( Enum value )
        {
            mValue = value;
            return *this;
        }

        State& setUnknown( void )
        {
            return set(Unknown);
        }

        State& setWaitingForFirstStatusUpdate( void )
        {
            return set(WaitingForFirstStatusUpdate);
        }

        State& setIdle( void )
        {
            return set(Idle);
        }

        State& setRinging( void )
        {
            return set(Ringing);
        }

        State& setEstablished( void )
        {
            return set(Established);
        }

        State& setError( void )
        {
            return set(Error);
        }
        
        ali::c_string_const_ref displayName( void ) const;

        friend ali::string& format(
            ali::string& str,
            State value,
            ali::string_const_ref )
        {
            return str.append(value.displayName());
        }

        Enum    mValue{Unknown};
    };

public:     //  Methods
    bool canPickupCall( void ) const
    {
        return  mState.isRinging()
            &&  !mCallPickupUri.is_empty();
    }

public:     //  Data members
    State               mState{};
    ali::stable_string  mCallPickupUri{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct Dialog
// ******************************************************************
{
public:     //  Enum
    struct State
    {
        enum Enum
        {
            Unknown,
            Trying,
            Proceeding,
            Early,
            Confirmed,
            Terminated
        };

        State( Enum value = Unknown )
        :   mValue{value}
        {}

        bool isUnknown( void ) const
        {
            return mValue == Unknown;
        }

        bool isTrying( void ) const
        {
            return mValue == Trying;
        }

        bool isProceeding( void ) const
        {
            return mValue == Proceeding;
        }

        bool isEarly( void ) const
        {
            return mValue == Early;
        }

        bool isConfirmed( void ) const
        {
            return mValue == Confirmed;
        }

        bool isTerminated( void ) const
        {
            return mValue == Terminated;
        }

        ali::c_string_const_ref identifier( void ) const;
        
        bool fromIdentifier( ali::string_const_ref str );

        ali::c_string_const_ref displayName( void ) const
        {
            return identifier();
        }

        friend ali::string& format(
            ali::string& str,
            State value,
            ali::string_const_ref )
        {
            return str.append(value.displayName());
        }

        Enum    mValue{Unknown};
    };

public:     //  Enum
    struct Direction
    {
        enum Enum
        {
            Unknown,
            Initiator,
            Recipient
        };

        Direction( Enum value = Unknown )
        :   mValue{value}
        {}

        bool isUnknown( void ) const
        {
            return mValue == Unknown;
        }

        bool isInitiator( void ) const
        {
            return mValue == Initiator;
        }

        bool isRecipient( void ) const
        {
            return mValue == Recipient;
        }

        ali::c_string_const_ref identifier( void ) const;
        
        bool fromIdentifier( ali::string_const_ref str );

        ali::c_string_const_ref displayName( void ) const
        {
            return identifier();
        }

        friend ali::string& format(
            ali::string& str,
            Direction value,
            ali::string_const_ref )
        {
            return str.append(value.displayName());
        }

        Enum    mValue{Unknown};
    };

public:     //  Data members
    ali::stable_string  mLocalIdentity{};
    ali::stable_string  mRemoteIdentity{};

    State               mState{};
    Direction           mDirection{};
    ali::time::seconds  mDuration{};
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
struct DialogInfo
// ******************************************************************
{
public:     //  Enum
    struct UpdateType
    {
        enum Enum
        {
            Error,
            WaitingForFirstStatusUpdate,
                //  Only the mAccountId and mEntity fields
                //  are valid when this update type is set.

            Full,
            Partial
        };

        UpdateType( Enum value )
        :   mValue{value}
        {}

        bool isError( void ) const
        {
            return mValue == Error;
        }

        bool isWaitingForFirstStatusUpdate( void ) const
        {
            return mValue == WaitingForFirstStatusUpdate;
        }

        bool isFull( void ) const
        {
            return mValue == Full;
        }

        bool isPartial( void ) const
        {
            return mValue == Partial;
        }

        UpdateType& set( Enum value )
        {
            mValue = value;
            return *this;
        }

        UpdateType& setError( void )
        {
            return set(Error);
        }

        UpdateType& setWaitingForFirstStatusUpdate( void )
        {
            return set(WaitingForFirstStatusUpdate);
        }

        UpdateType& setFull( void )
        {
            return set(Full);
        }

        UpdateType& setPartial( void )
        {
            return set(Partial);
        }

        ali::c_string_const_ref displayName( void ) const;

        friend ali::string& format(
            ali::string& str,
            UpdateType value,
            ali::string_const_ref )
        {
            return str.append(value.displayName());
        }

        Enum    mValue;
    };
    
    DialogInfo( UpdateType updateType = UpdateType::Full )
    :   mUpdateType{updateType}
    {}
        
    UpdateType                              mUpdateType;
    int                                     mVersion{};
    ali::stable_string                      mAccountId{};
    ali::stable_string                      mEntity{};
                                                //  Who is this dialog info about.
    ali::array_map<
        ali::stable_string,
        Dialog>                             mDialogs{};
};

}   //  namespace Softphone
