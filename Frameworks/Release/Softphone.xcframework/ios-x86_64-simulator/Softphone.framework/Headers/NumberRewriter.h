/*
 *  NumberRewriter.h
 *  softphone
 *
 *  Copyright (c) 2010 - 2018 Acrobits, s.r.o. All rights reserved.
 *
 */
#pragma once

#include "Network.h"
#include "ali/ali_string.h"
#include "ali/ali_array.h"
#include "ali/ali_array_utils.h"
#include "ali/ali_xml_tree2_forward.h"
#include "ali/ali_json.h"
#include "Softphone/DialAction/DialActionType.h"

namespace NumberRewriting
{
    struct IncomingCallAction
    {
        enum Type
        {
            None,
            RejectCall,
            ForwardCall,
            AnswerImmediately
        };

        static Type fromString(ali::string const& type);

        static ali::string toString(Type);
        
        IncomingCallAction() : actionType(Type::None) {};
        
        IncomingCallAction(Type aType) : actionType(aType) {};
        
        IncomingCallAction(Type aType, ali::string const& aParam) : actionType(aType), actionParam(aParam) {};
        
        Type actionType;
        ali::string actionParam;
    };


    typedef ali::pair<int,int> match_t;

    struct State
    {
        State(ali::string const& result, ali::string const& cc)
        : result(result),defaultCountryCode(cc) {};

        IncomingCallAction incomingCallAction;
        bool            recordCall{false};
        bool            forceDialOut{false};
        bool            showDialOutAlert{true};
        Softphone::DialAction::Id     newDialAction;
        match_t         match;
        ali::string     result;
        bool            shouldEnd{false};
        ali::json::dict headers;
        
        ali::string     defaultCountryCode;
        ali::xml::tree  extras;
        
        ali::string     newLocationPolicy;
        ali::string     alert;
    };

    struct Context
    {
        virtual ~Context(){}

        virtual ali::string getCurrentWifiSsid() const = 0;
        virtual ali::string getDefaultCountryCode() const = 0;
        virtual bool isRecordingEnabled() const = 0;
        virtual Softphone::Network::Type getCurrentNetwork() const = 0;
    };

    struct StoredDataContext : public Context
    {
        Context& setNetworkType(
            Softphone::Network::Type value )
        {
            mNetworkType = value;
            return *this;
        }

        Context& setWifiSSID(
            ali::string const& value )
        {
            mWifiSsid = value;
            return *this;
        }

        Context& setDefaultCountryCode(
            ali::string const& value )
        {
            mDefaultCountryCode = value;
            return *this;
        }

        Context& setIsRecordingEnabled( bool value )
        {
            mIsRecordingEnabled = value;
            return *this;
        }

        virtual ali::string getCurrentWifiSsid()  const override
        {
            return mWifiSsid;
        }

        virtual ali::string getDefaultCountryCode() const override
        {
            return mDefaultCountryCode;
        }

        virtual bool isRecordingEnabled() const override
        {
            return mIsRecordingEnabled;
        }
        
        virtual Softphone::Network::Type getCurrentNetwork() const override
        {
            return mNetworkType;
        }

    private:
        Softphone::Network::Type    mNetworkType{Softphone::Network::None};
                //  Softphone::context->getCurrentNetworkType()
        ali::string                 mWifiSsid{};
                //  Softphone::context->getCurrentWifiSSID()
        ali::string                 mDefaultCountryCode{};
                //  Softphone::context->_prefKeys->defaultCountryCode.getCurrentValue()
        bool                        mIsRecordingEnabled{};
                //  !Softphone::context->_prefKeys->recordingDisabled.getCurrentValue()
    };

    struct Condition
    {
        enum Type
        {
            Undef,
            Min,
            StartsWith=Min,
            DoesNotStartWith,
            Equals,
            LengthEquals,
            ShorterThan,
            LongerThan,
            NetworkType,
            SSID,
            Contains,
            Numeric,
            Max=Numeric
        };

        Type            ctype;
        ali::string        param;

        Condition() : ctype(Undef) {};
        Condition(Condition::Type ctype) : ctype(ctype) {};
        Condition(Condition::Type ctype, ali::string_const_ref param) : ctype(ctype),param(param) {};

        bool matches(State& state, Context const& context) const;

        static ali::string toString(Condition::Type cType);
        static Type fromString(ali::string_const_ref str);

        void swap( Condition& b )
        {
            using ali::swap;
            ali::swap_by_copy(ctype, b.ctype);
            swap(param, b.param);
        }

        friend void swap( Condition& a, Condition& b )
        {
            a.swap(b);
        }

        bool operator==( Condition const& b ) const
        {
            return (ctype == b.ctype) && (param == b.param);
        }

        bool operator!=( Condition const& b ) const
        {
            return !operator==(b);
        }
    };

    struct Action
    {
        enum Type
        {
            Undef,
            Min,
            Replace=Min,
            Prepend,
            Append,
            Continue,
            CallThrough,
            DialOut,
            RecordCall,
            OverrideDialAction,
            SetHeader,
            RejectCall,
            ForwardCall,
            AnswerImmediately,
            OverrideLocationPolicy,
            ShowAlert,
            Max=ShowAlert
        };

        Type            atype;
        ali::string     param;
        ali::xml::tree  root;
        
        Action() : atype(Undef) {};
        Action(Action::Type atype);
        Action(Action::Type atype, ali::string_const_ref param);
        Action(ali::xml::tree const& xml);

        static ali::string toString(Action::Type aType);
        static Type fromString(ali::string_const_ref str);
        
        void apply(State& state) const;

        void swap( Action& b )
        {
            using ali::swap;
            ali::swap_by_copy(atype, b.atype);
            swap(param, b.param);
            swap(root, b.root);
        }

        friend void swap( Action& a, Action& b )
        {
            a.swap(b);
        }

        bool operator==( Action const& b ) const
        {
            return (atype == b.atype) && (param == b.param) && (root == b.root);
        }

        bool operator!=( Action const& b ) const
        {
            return !operator==(b);
        }
        
    private:
        void sanitize();
    };

    struct Rule
    {
        typedef ali::array<Condition>::const_iterator cond_it_t;
        typedef ali::array<Action>::const_iterator action_it_t;

        ali::array<Condition>    conditions;
        ali::array<Action>        actions;
        
        bool                    forIncomingCall;

        ali::array<Condition::Type> applicableConditions() const;
        ali::array<Action::Type> applicableActions(Context const& context) const;

    private:
        bool _accountSpecific;
    public:
        Rule(bool accountSpecific);

        void sortByTypes();

        int findCondition(Condition::Type t) const;
        int findAction(Action::Type t) const;

        void rewrite(State& state, Context const& context) const;
        bool matches(State& state, Context const& context) const;
        void apply(State& state) const;

        bool operator==( Rule const& b ) const
        {
            return (conditions == b.conditions) && (actions == b.actions);
        }

        bool operator!=( Rule const& b ) const
        {
            return !operator==(b);
        }

    private:
        bool hasCondition(Condition::Type t) const;
        bool hasAction(Action::Type t) const;

    };

    class Rewriter
    {
    public:
        typedef ali::array<Rule>::const_iterator rule_it_t;
        ali::array<Rule> rules;
    private:
        bool _accountSpecific;
        mutable ali::array<Rule> _backupRules;

    public:
        struct Result
        {
            IncomingCallAction          incomingCallAction;
            ali::string                 number;
            bool                        forceDialOut{false};
            bool                        showDialOutAlert{false};
            bool                        shouldAutoRecord{false};
            Softphone::DialAction::Id   dialAction;
            ali::json::dict             headers;
            ali::xml::tree              extras;
            ali::string                 locationPolicy;
            ali::string                 alert;
        };

    public:
        Rewriter(bool accountSpecific=true);
        virtual ~Rewriter() {};

        Rule& newRule();

        void clearAllRules()
        {
            rules.erase();
        }

        bool isDirty() const
        {
            return !(_backupRules == rules);
        }

        void load(ali::xml::tree const& tree);
        void save(ali::xml::tree& tree) const;

        bool forcesDialOut(
            ali::string const& src,
            Context const& context,
            bool& showDialOutAlert) const;

        bool shouldOverrideDialAction(
            ali::string const& src,
            Softphone::DialAction::Id & newDialAction,
            Context const& context) const;

        bool shouldRecordCall(
            ali::string const& src,
            Context const& context) const;

        Result rewrite(
            ali::string const& src,
            Context const& context) const;

        ali::array<ali::string> rewriteProgressively(
            ali::string const& src,
            Context const& context) const;

        bool containsForcedDialOut() const;
        
        bool containsWiFiSSID() const;
        
        bool requiresLocation() const;
        
        bool shouldOverrideLocationPolicy(
                                          ali::string const& src,
                                          ali::string & newLoactionPolicy,
                                          Context const& context) const;
   };
}
