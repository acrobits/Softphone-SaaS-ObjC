/*
 *  EventHistory/CallEvent.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2018 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "Softphone/EventHistory/EventHistory.h"
#include "Softphone/NumberRewriter.h"

namespace Softphone
{
namespace EventHistory
{
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct CallResult
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        enum Type
        {
            Unknown     = 0,
            Accepted    = 1 << 0,       // incoming call, accepted
            Missed      = 1 << 1,       // incoming call, unanswered without callee intervention
            Rejected    = 1 << 2,       // incoming call, actively rejected

            Busy        = 1 << 3,       // outgoing call, replied with busy status
            Answered    = 1 << 4,       // outgoing call, answered
            Unanswered  = 1 << 5,       // outgoing call, ended without connecting
            Canceled    = 1 << 6,       // outgoing call, canceled by the caller
            Error       = 1 << 7,       // outgoing call, failed
            Forwarded   = 1 << 8,       // incoming call, forwarded
            Initiated   = 1 << 9,       // incoming call, initiated Google Voice call

            AnsweredElsewhere = 1 << 10,    // incoming call, answered on different device
            // Calls in this state should not be written to persistent storage

            max         = AnsweredElsewhere,
            all         = (max << 1) - 1,

            incoming = (Accepted|Missed|Rejected|Forwarded),
            outgoing = (Busy|Answered|Unanswered|Canceled|Error|Initiated),
            successful = (Accepted|Answered|Initiated|AnsweredElsewhere)
        };

        static ali::string toString(int result);
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class CallEvent
        : public Event
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        typedef EventPointer<CallEvent> Pointer;

        class QueryBuilder;

        static EventType::Type constexpr EventType{EventType::Call};

    protected:
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        CallEvent()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            : Event(EventType)
        {}

    public:
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        static Pointer create()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return Pointer(new CallEvent());
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        static Pointer create(RemoteUser && user)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            Pointer ret = create();
            ret->addRemoteUser(ali::move(user));
            return ret;
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        static Pointer create(ali::string_const_ref accountId,
                              ali::string const& uri)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            Pointer ret = create(RemoteUser(StreamParty(uri).match(accountId)));
            ret->setAccount(accountId);
            return ret;
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::generic_peer_address getRemote() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getRemoteUser().getTransportUri();
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        TimestampType getTimeEstablished() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getRemoteUser().getTimeDelivered();
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setTimeEstablished(TimestampType time)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            getRemoteUser().setTimeDelivered(time);
        }

        CallResult::Type getCallResult() const;
        void setCallResult(CallResult::Type result);

        ali::time::float_seconds getDuration() const;
        void setDuration(ali::time::float_seconds duration);

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        RemoteUser const& getRemoteUser() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali_assert(getRemoteUserCount() == 1);

            return Event::getRemoteUser(0);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        RemoteUser & getRemoteUser()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali_assert(getRemoteUserCount() == 1);

            return Event::getRemoteUser(0);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::filesystem2::path getRecording() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getFileAttribute(Attributes::recording);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void removeRecording()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            removeAttribute(Attributes::recording);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setVideoEnabled(bool incoming, bool outgoing)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            setAttribute(Attributes::incomingVideo, incoming ? "1"_s : "0"_s);
            setAttribute(Attributes::outgoingVideo, outgoing ? "1"_s : "0"_s);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setTransferredTo(ali::string_const_ref destination)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            setAttribute(Attributes::transferredTo, destination);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setForwardedTo(ali::string_const_ref destination)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            setAttribute(Attributes::forwardedTo, destination);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setAutoCallRecording(bool autoCallRecord)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            setAttribute(Attributes::autoCallRecording, autoCallRecord ? "1"_s : "0"_s);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool getAutoCallRecording() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            return getAttribute(Attributes::autoCallRecording) == "1"_s;
        }
        
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setLocationPolicy(ali::string const& locationPolicy)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            setAttribute(Attributes::locationPolicy, locationPolicy);
        }
        
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::string const& getLocationPolicy() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getAttribute(Attributes::locationPolicy);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::string getTransportUriWithAppliedRewriting() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali::string const& rewrittenTransportUri = getAttribute(Attributes::rewrittenTransportUri);
            
            return rewrittenTransportUri.is_empty() ? getRemote().get() : rewrittenTransportUri;
        }
        
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setIncomingCallAction(NumberRewriting::IncomingCallAction const& incomingCallAction)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;

            if (incomingCallAction.actionType == NumberRewriting::IncomingCallAction::None)
            {
                if (hasAttribute(Attributes::incomingCallAction))
                    removeAttribute(Attributes::incomingCallAction);

                if (hasAttribute(Attributes::incomingCallActionParam))
                    removeAttribute(Attributes::incomingCallActionParam);

                return;
            }

            setAttribute(Attributes::incomingCallAction, NumberRewriting::IncomingCallAction::toString(incomingCallAction.actionType));

            if (!incomingCallAction.actionParam.is_empty())
                setAttribute(Attributes::incomingCallActionParam, incomingCallAction.actionParam);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        NumberRewriting::IncomingCallAction getIncomingCallAction()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;

            NumberRewriting::IncomingCallAction incomingCallAction;

            if (!hasAttribute(Attributes::incomingCallAction))
                return incomingCallAction;

            ali::string const& icAction = getAttribute(Attributes::incomingCallAction);
            incomingCallAction.actionType = NumberRewriting::IncomingCallAction::fromString(icAction);

            if (hasAttribute(Attributes::incomingCallActionParam))
            {
                ali::string const& icActionParam = getAttribute(Attributes::incomingCallActionParam);
                incomingCallAction.actionParam = icActionParam;
            }

            return incomingCallAction;
        }


        void updateCallDuration(TimestampType const& now);

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::json::dict const& customHeaders() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            return transients.get("customHeaders"_s).as_dict();
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setCustomHeader(ali::string_const_ref name, ali::string_const_ref value)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            transients["customHeaders"_s].as_dict()[name].as_string() = value;
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::string const* findCustomHeader(ali::string_const_ref name) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return customHeaders().find_string(name);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void mergeHeaders(ali::json::dict const& headers)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::operator ""_s;
            auto & currentHeaders = transients["customHeaders"_s].as_dict();

            for(auto const& h : headers)
                currentHeaders[h.first].as_string() = h.second.as_string();
        }

    protected:
        virtual bool isUnreadMarkable() const {return (getCallResult() & CallResult::Missed) != 0;}

    public:
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Attributes
            : public Event::Attributes
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            static ali::string_literal const callResult;
            static ali::string_literal const callDuration;
            static ali::string_literal const cellular;
            static ali::string_literal const recording;
            static ali::string_literal const recordingProtected;
            static ali::string_literal const recordingUploaded;
            static ali::string_literal const incomingVideo;
            static ali::string_literal const outgoingVideo;
            static ali::string_literal const transferredTo;
            static ali::string_literal const forwardedTo;
            static ali::string_literal const sipisTimestamp;
            static ali::string_literal const autoCallRecording;
            static ali::string_literal const dialAction;
            static ali::string_literal const smart;
            static ali::string_literal const pushCallId;
            static ali::string_literal const rejectReason;
            static ali::string_literal const callId;
            static ali::string_literal const incomingCallAction;
            static ali::string_literal const incomingCallActionParam;
            static ali::string_literal const diversion;
            static ali::string_literal const locationPolicy;
            static ali::string_literal const rewrittenTransportUri;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Transients
            : public Event::Transients
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            static const ali::string_literal callGroupId;
        };
    };

    bool Event::isCall() const { return eventType == EventType::Call; }
    CallEvent & Event::asCall() { ali_assert(isCall()); return static_cast<CallEvent &>(*this); }
    CallEvent const& Event::asCall() const { ali_assert(isCall()); return static_cast<CallEvent const&>(*this); }

    struct Query;

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class CallEvent::QueryBuilder
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        QueryBuilder(Query & query)
        :mQuery(query){}

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Query & get()
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return mQuery;
        }

        CallEvent::QueryBuilder & setCallResultMask(int mask);

        CallEvent::QueryBuilder & setWithRecording();
        CallEvent::QueryBuilder & setWithoutRecording();
        CallEvent::QueryBuilder & clearRecording();

    private:
        Query & mQuery;
    };
}
}

