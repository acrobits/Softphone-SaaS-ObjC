/*
 *  EventHistory/EventHistoryTypes.h
 *  libsoftphone
 *
 *  Copyright (c) 2013, 2014 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_string.h"
#include "ali/ali_time.h"

namespace Softphone {
namespace EventHistory {

    typedef unsigned long                   EventIdType;
    typedef unsigned long                   EventAttachmentIdType;
    typedef ali::time::mac_absolute_time    TimestampType;
    
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct ContactId
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        ali::string id;
        ali::string source;
        
        ContactId() = default;
        
        ContactId(ali::string const& source,ali::string const& id)
        :id(id),source(source){}
        
        ContactId(ContactId const& other)
        :id(other.id),source(other.source){}
        
        ContactId(ContactId && other)
        :id(ali::move(other.id)),source(ali::move(other.source)){}
        
        inline ContactId & operator =(ContactId const& other)
        {
            id = other.id;
            source = other.source;
            return *this;
        }
        
        inline bool operator ==(ContactId const& other) const
        {
            return id==other.id && source == other.source;
        }
        
        inline bool operator !=(ContactId const& other) const
        {
            return !operator==(other);
        }
        
        inline bool operator < (ContactId const& other) const
        {
            const int d = id.compare(other.id);
            
            if(d < 0)
                return true;
            
            if(d > 0)
                return false;
            
            return source.compare(other.source) < 0;
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        friend int compare(ContactId const& cId1, ContactId const& cId2)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            using ali::compare;
            return compare(cId1.id, cId2.id);
        }
        
        bool isEmpty() const
        {
            return id.is_empty();
        }
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct EventType
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unknown,
            Call,
            Message,
            FileTransfer,
            Location,
            ParticipantAction
        };

        static ali::string_literal toString(EventType::Type evt);
        
        friend ali::string& format(
            ali::string& str,
            EventType::Type value,
            ali::string_const_ref /*formatString*/ )
        {
            return str.append(EventType::toString(value));
        }
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Direction
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unspecified = 0,
            Incoming = 1 << 0,
            Outgoing = 1 << 1,

            max = Outgoing,
            all = (max << 1) - 1
        };

        static ali::string_literal toString(Direction::Type dir);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct DeliveryStatus
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unspecified = 0,
            Pending = 1,
            Sent = 2,
            Delivered = 3,
            Read = 4,
            Error = 5
        };

        static ali::string_literal toString(DeliveryStatus::Type status);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct StorageStatus
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            New = 0,
            Stored = 1,
            Dirty = 2,
            Removed = 3,
            BeingRemoved = 4,
        };

        static ali::string_literal toString(StorageStatus::Type status);
    };
}
}
