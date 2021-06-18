/*
 *  AccountValidator.h
 *
 *  Created by Jiri Kral on July 17th 2017
 *  Copyright 2017 Acrobits, s.r.o. All rights reserved.
 *
 */


#pragma once

#include "ali/ali_xml_tree2.h"
#include "ali/ali_utility.h"
#include "ali/ali_shared_ptr.h"
#include "Softphone/Mergeable/MergeableXmlValidator.h"

namespace Softphone
{
namespace Account
{

class Xml;
    
namespace Validator
{
    using namespace Mergeable::Validator;
    
    ali::shared_ptr<Base> parse(ali::xml::tree const& validator);
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Port : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Validator::Type TheType;
        static const FailureReason InvalidPort;
        
    public:
        explicit Port(ali::xml::tree const& xml);
        virtual ~Port(){}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<Port>::new_auto_ptr(mAllowEmpty,mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Mergeable::Xml const& account, FailureDetail & detail) const;

    private:
        explicit Port(bool allowEmpty, Localization::LocalizableString const& message)
        :Base(TheType),mAllowEmpty(allowEmpty),mMessage(message){}

    private:
        bool                            mAllowEmpty;
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<Port>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class BindPort : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Validator::Type TheType;
        static const FailureReason InvalidBindPort;
        
    public:
        explicit BindPort(ali::xml::tree const& xml);
        virtual ~BindPort(){}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<BindPort>::new_auto_ptr(mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Mergeable::Xml const& account, FailureDetail & detail) const;

    private:
        explicit BindPort(Localization::LocalizableString const& message)
        :Base(TheType),mMessage(message){}

    private:
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<BindPort>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class RtpRange : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Validator::Type TheType;
        static const FailureReason WrongRtpRange;
        
    public:
        explicit RtpRange(ali::xml::tree const& xml);
        virtual ~RtpRange(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<RtpRange>::new_auto_ptr(mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Mergeable::Xml const& account, FailureDetail & detail) const;
        
    private:
        explicit RtpRange(Localization::LocalizableString const& message)
        :Base(TheType),mMessage(message){}

    private:
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<RtpRange>;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class PortNotAllowed : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Validator::Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit PortNotAllowed(ali::xml::tree const& xml);
        virtual ~PortNotAllowed(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<PortNotAllowed>::new_auto_ptr(mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Mergeable::Xml const& account, FailureDetail & detail) const;

    private:
        explicit PortNotAllowed(Localization::LocalizableString const& message)
        :Base(TheType),mMessage(message){}
        
    private:
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<PortNotAllowed>;
    };
}
}
}
