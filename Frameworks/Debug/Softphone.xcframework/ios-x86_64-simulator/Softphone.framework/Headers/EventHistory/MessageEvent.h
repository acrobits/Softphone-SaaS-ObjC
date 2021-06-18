/*
 *  EventHistory/MessageEvent.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2016 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "Softphone/EventHistory/EventHistory.h"
#include "ali/ali_callback.h"

namespace Softphone
{
    namespace EventHistory
    {
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct MessageResult
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {

            enum Type
            {
                Unknown             = 0,
                Initial             = 1 << 0,
                Sending             = 1 << 1,
                SendFailed          = 1 << 2,
                SendSuccessful      = 1 << 3,

                max                 = SendSuccessful,
                all                 = (max << 1) - 1
            };

            static ali::string toString(int result);
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct GenericMessageErrorCode
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            enum Type
            {
                NoError             = 0,
                NoNetwork           = 1,
                NoWifi              = 2,
                NoTransport         = 3,
                Busy                = 4,
                AccountNotFound     = 5,
                TransportFailed     = 6
            };

            static ali::string toString(int result);
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        class MessageEvent
            : public Event
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
        public:
            struct DeliveryNotificationType
            {
                enum Type
                {
                    Delivered,
                    Displayed
                } mValue;
                
                DeliveryNotificationType(Type type) : mValue{type} {};
                
                bool isDelivered() const
                {
                    return mValue == Delivered;
                };
                bool isDisplayed() const
                {
                    return mValue == Displayed;
                };
            };

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Subtype
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                enum Type
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    Unknown     = 0,
                    IM,
                    SMS,
                    MMS
                };
            };

        public:
            typedef EventPointer<MessageEvent> Pointer;
            typedef ali::string ContentDownloadHandle;

            class QueryBuilder;

            static EventHistory::EventType::Type constexpr EventType{EventHistory::EventType::Message};

        protected:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            MessageEvent()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : Event(EventType)
            {}

        public:
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            static Pointer create()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return Pointer(new MessageEvent());
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getSubject() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::subject);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setSubject(ali::string_const_ref subject)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::subject, subject);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getBody() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::body);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setBody(ali::string_const_ref body)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::body, body);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getTransportId() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::transportId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setTransportId(ali::string_const_ref transportId)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::transportId, transportId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getImdnId() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::imdnId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setImdnId(ali::string_const_ref imdnId)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::imdnId, imdnId);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string const& getPreviewText() const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getAttribute(Attributes::previewText);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setPreviewText(ali::string_const_ref text)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::previewText, text);
            }

            ali::mime::content_type getEffectiveContentType() const;

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void setEffectiveContentType(ali::string_const_ref text)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setAttribute(Attributes::effectiveContentType, text);
            }

            ali::optional<ali::mime::content_type> getContentType() const;
            void setContentType(ali::optional<ali::mime::content_type> contentType);

            GenericMessageErrorCode::Type getGenericErrorCode() const;

            Subtype::Type getSubtype() const;
            MessageResult::Type getMessageResult() const;
            void setMessageResult(MessageResult::Type result);
            void clearGenericErrorCode();
            void failWithGenericErrorCode(GenericMessageErrorCode::Type code);


            bool isRemoteUserSuccessful(int i) const;
            bool isRemoteUserSuccessful(ali::generic_peer_address const& addr) const;
            bool isSomeRemoteUserDeliveredOrRead() const;
            bool setRemoteUserSending(int i);
            bool setRemoteUserSent(ali::generic_peer_address const& addr);
            bool setRecipientFailed(ali::generic_peer_address const& addr);
            bool setRemoteUserFailedByExternalId(ali::string_const_ref externalMessageId);
            bool setRemoteUserDeliveredByExternalId(ali::string_const_ref externalMessageId, TimestampType when, ali::generic_peer_address const& sender);
            bool setRemoteUserReadByExternalId(ali::string_const_ref externalMessageId, TimestampType when, ali::generic_peer_address const& sender);
            void updateMessageResult();
            void updateStream(MessageResult::Type result);
            
            void prepareForAutoDeletionAfterSent(TimestampType sentAt);
            void prepareForAutoDeletionAfterDisplayed(TimestampType displayedAt);
            void softDelete();

        private:
            void prepareForAutoDeletion(TimestampType happenedAt, ali::string_const_ref attributeName);

            int getSuccessfulRecipientCount() const;

        public:
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            struct Transients
                : public Event::Transients
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            {
                static const ali::string_literal readElsewhere;
                static const ali::string_literal delivered;
                static const ali::string_literal msgDeleteTrusted;
                static const ali::string_literal notificationThreadId;
            };

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Attributes
                : public Event::Attributes
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                static ali::string_literal const messageResult;
                static ali::string_literal const subject;
                static ali::string_literal const body;
                static ali::string_literal const subtype;
                static ali::string_literal const sipisTimestamp;
                static ali::string_literal const previewText;
                static ali::string_literal const effectiveContentType;
                static ali::string_literal const transportId;
                static ali::string_literal const imdnId;
                static ali::string_literal const localizedBodyKey;
                static ali::string_literal const localizedBodyKeyDefault;
                static ali::string_literal const localizedStringParameterPrefix;
                static ali::string_literal const localizedNumberParameterPrefix;
                static ali::string_literal const genericErrorCode;
                static ali::string_literal const contentType;
                static ali::string_literal const pendingDeliveredDnInfo;
                static ali::string_literal const pendingDisplayedDnInfo;
                static ali::string_literal const displayedDnInfo;
                static ali::string_literal const systemMessage;
                static ali::string_literal const messageProtected;
                static ali::string_literal const deleteAfterSentInSeconds;        // timeout to delete the message after sent
                static ali::string_literal const deleteAfterDisplayedInSeconds;   // timeout to delete the message after it's displayed
                static ali::string_literal const timeToDelete;                    // timestamp to delete the message
                static ali::string_literal const msgDeleteSending;
                static ali::string_literal const msgAttachmentDeleteSending;
            };
        };

        bool Event::isMessage() const { return eventType == EventType::Message; }
        MessageEvent & Event::asMessage() { ali_assert(isMessage()); return static_cast<MessageEvent &>(*this); }
        MessageEvent const& Event::asMessage() const { ali_assert(isMessage()); return static_cast<MessageEvent const&>(*this); }

        struct Query;

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        class MessageEvent::QueryBuilder
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
        public:
            QueryBuilder(Query & query)
            : mQuery(query){}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Query & get()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return mQuery;
            }

            QueryBuilder & setMessageResultMask(int mask);

        private:
            Query & mQuery;
        };

    }
}
