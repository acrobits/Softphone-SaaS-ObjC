/*
 *  DialActionType.h
 *  softphone
 *
 *  Copyright (c) 2012 - 2018 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "ali/ali_string.h"
#include "ali/ali_rb_tree_map_forward.h"
#include "Softphone/PreferenceKeys/KeyValidator.h"
#include "Softphone/PreferenceKeys/KeyValueConvertor.h"
#include "Softphone/Account/AccountType.h"

#include "ali/ali_json.h"

namespace Softphone::DialAction
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Id
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        Id() = default;

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        explicit Id(ali::string_const_ref id)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            :id{id}
        {}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        bool isEmpty() const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return id.is_empty();
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        friend int compare(Id const& lhs, Id const& rhs)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            using ali::compare;
            return compare(lhs.id, rhs.id);
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        inline bool operator == (Id const& rhs) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return id == rhs.id;
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        inline bool operator !=(Id const& rhs) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return !operator == (rhs);
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        inline bool oneOf(ali::initializer_list<Id> const& ids) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            for (auto const& rhs : ids)
                if (operator ==(rhs))
                    return true;
            return false;
        }

        ali::string id;
    };

    using Set = ali::rb_tree_map<ali::string, Id>;

    extern const Id AUTO_CALL;
    extern const Id AUTO_MEDIA_CALL;
    extern const Id VOICE_CALL;
    extern const Id VIDEO_CALL;
    extern const Id GSM_CALL;
    extern const Id FORCE_OFFNET_CALL;

    extern const Id WEB_CALLBACK;
    extern const Id CALL_THROUGH;

    extern const Id TEXT;
    extern const Id FORCE_OFFNET_TEXT;

    extern const Id SHOW_OPTIONS;
    extern const Id SHOW_GSM_OPTIONS;
    extern const Id SHOW_CUSTOM_OPTIONS;

    extern const Id SHARE_IMAGE;
    extern const Id SHARE_AUDIO;
    extern const Id SHARE_VIDEO;
    extern const Id SHARE_LOCATION;

    extern const Id COPY_NUMBER;
    extern const Id OPEN_URL;
    extern const Id OFF;


    Set setFromJson(ali::json::object const& json);
    Set setForSingleTap();
    Set setForDoubleTap();
    Set setForLongPress();

    Set setForDialAction(Id dialAction);
    Set setForMessageAction(Id messageActionId);

    Id dialActionForAccountType(Set const& set, Account::Type type);

}

namespace PreferenceKeys
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    template<>
    class KeyValueConvertor<Softphone::DialAction::Id>
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static ali::string_literal const typeName;

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        static ali::string toString(Softphone::DialAction::Id const& v)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return v.id;
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        static Softphone::DialAction::Id fromString(ali::string_const_ref str)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return Softphone::DialAction::Id{str};
        }
    };
}


