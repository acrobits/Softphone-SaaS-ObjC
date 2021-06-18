/*
 *  RateRecord.h
 *  libsoftphone
 *
 *  Copyright (c) 2011 - 2014 Acrobits s.r.o. All rights reserved.
 */

#pragma once

#include "Softphone/Rate/Rate_Forward.h"
#include "ali/ali_callback.h"
#include "ali/ali_json.h"
#include "ali/ali_optional.h"
#include "ali/ali_string.h"

namespace Rate
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Record
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        /** @brief JSON items */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct JSON
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            /** @brief Call
              * @type{dict}
              * @see @ref Call::JSON */
            static ali::string_literal const call;

            /** @brief Message
              * @type{dict}
              * @see @ref Message::JSON */
            static ali::string_literal const message;

            /** @brief SmartCall
             * @type{dict}
             * @see @ref Call::JSON */
            static ali::string_literal const smartCall;
            
            /** @brief SmartMessage
             * @type{dict}
             * @see @ref Message::JSON */
            static ali::string_literal const smartMessage;
        };

        static Record fromJson(ali::json::dict const& json);
        ali::json::dict toJson() const;

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Call
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Call rate JSON items */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct JSON
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                /** @brief Fixed fee charged when the call gets established
                  * @type{double} */
                static ali::string_literal const fixed;

                /** @brief Price charged per each minute
                  * @type{double} */
                static ali::string_literal const perMinute;

                /** @brief Price charged per each second
                  * @type{double} */
                static ali::string_literal const perSecond;

                /** @brief Currency the rate is in
                  * @type{string} */
                static ali::string_literal const currency;

                /** @brief Formatted rate ready for display
                  * @type{string} */
                static ali::string_literal const formatted;
            };

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Call()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : fixed(.0)
                , perMinute(.0)
                , perSecond(.0)
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Call(double fixed,
                 double perMinute,
                 double perSecond,
                 ali::string const& currency,
                 ali::string const& formatted)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : fixed(fixed)
                , perMinute(perMinute)
                , perSecond(perSecond)
                , currency(currency)
                , formatted(formatted)
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void swap(Call & call)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                using ali::swap;
                swap(fixed, call.fixed);
                swap(perMinute, call.perMinute);
                swap(perSecond, call.perSecond);
                swap(currency, call.currency);
                swap(formatted, call.formatted);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            friend void swap(Call & first,
                             Call & second)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                first.swap(second);
            }

            static Call fromJson(ali::json::dict const& json);
            ali::json::dict toJson() const;

            double fixed;
            double perMinute;
            double perSecond;

            ali::string currency;
            ali::string formatted;
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Message
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Message rate JSON items */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct JSON
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                /** @brief Fixed fee charged when the message is sent
                  * @type{double} */
                static ali::string_literal const fixed;

                /** @brief Currency the rate is in
                  * @type{string} */
                static ali::string_literal const currency;

                /** @brief Formatted rate ready for display
                  * @type{string} */
                static ali::string_literal const formatted;
            };

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Message()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : fixed(.0)
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            Message(double fixed,
                    ali::string const& currency,
                    ali::string const& formatted)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                : fixed(fixed)
                , currency(currency)
                , formatted(formatted)
            {}

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            void swap(Message & message)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                using ali::swap;
                swap(fixed, message.fixed);
                swap(currency, message.currency);
                swap(formatted, message.formatted);
            }

            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            friend void swap(Message & first,
                             Message & second)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                first.swap(second);
            }

            static Message fromJson(ali::json::dict const& json);
            ali::json::dict toJson() const;

            double fixed;

            ali::string currency;
            ali::string formatted;
        };

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void swap(Record & record)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::swap;
            
            swap(call, record.call);
            swap(message, record.message);
            swap(smartCall, record.smartCall);
            swap(smartMessage, record.smartMessage);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        friend void swap(Record & first,
                         Record & second)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            first.swap(second);
        }

        ali::optional<Call> call;
        ali::optional<Message> message;

        ali::optional<Call> smartCall;
        ali::optional<Message> smartMessage;
    };

}
