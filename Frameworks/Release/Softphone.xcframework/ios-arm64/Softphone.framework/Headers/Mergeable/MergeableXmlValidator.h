/*
 *  MergeableXmlValidator.h
 *
 *  Created by Jiri Kral on July 17th 2017
 *  Copyright 2017 Acrobits, s.r.o. All rights reserved.
 *
 */


#pragma once

#include "ali/ali_xml_tree2.h"
#include "ali/ali_utility.h"
#include "ali/ali_shared_ptr.h"
#include "Softphone/LocalizationHelpers.h"

namespace Softphone::Mergeable
{
class Xml;
    
namespace Validator
{
    class Base;
    struct Record;

    ali::shared_ptr<Base> parse(ali::xml::tree const& validator);

    using Type = ali::string_literal;
    using FailureReason = ali::string_literal;
    
    using Array = ali::array<Record>;
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct FailureDetail
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        FailureDetail() = default;
        
        FailureDetail(Type type, FailureReason reason, Localization::LocalizableString const& message = {})
        :mType{type},mReason{reason},mMessage{message}{}

        ~FailureDetail() = default;
        
        void setFieldName(ali::string const& fieldName)
        {
            mFieldName = fieldName;
        }
        
        ali::string const& validatorType() const {return mType;}
        ali::string const& reason() const {return mReason;}
        ali::string const& fieldName() const {return mFieldName;}
        Localization::LocalizableString const& message() const {return mMessage;}
        
    private:
        ali::string         mType;
        ali::string         mReason;
        ali::string         mFieldName;
        Localization::LocalizableString          mMessage;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        Base(Type type) : mType(type){}
        
        virtual ~Base() {}
        virtual ali::auto_ptr<Base> clone() = 0;
        
        Type const& getType() const {return mType;}
        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const = 0;

    private:
        Type mType;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Record
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        Record() = default;
                
        Record(ali::string_const_ref fieldName, ali::shared_ptr<Base> validator)
        :fieldName{fieldName},validator{validator}{}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Record(Record const& other)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            *this = other;
        }
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Record & operator =(Record const& other)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            if(&other == this) return *this;
            
            fieldName = other.fieldName;
            validator = other.validator;
            
            return *this;
        }
        
        ali::string fieldName;
        ali::shared_ptr<Base> validator;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Required : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason ValueMissing;
 
    public:
        explicit Required(ali::xml::tree const& xml);
        virtual ~Required(){}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<
                Required>::new_auto_ptr(mAllowWhitespace,mMessage);
        }
        
        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;

    private:
        Required(bool allowWhitespace, Localization::LocalizableString const& message)
        :Base(TheType),mAllowWhitespace(allowWhitespace),mMessage(message){}
        
    private:
        bool mAllowWhitespace;
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<Required>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Numeric : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason NotANumber;
        static const FailureReason TooLow;
        static const FailureReason TooHigh;

    public:
        explicit Numeric(ali::xml::tree const& xml);
        virtual ~Numeric(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<Numeric>::new_auto_ptr(
                mCheckMin,
                mCheckMax,
                mMin,
                mMax,
                mNotANumberMessage,
                mTooLowMessage,
                mTooHighMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        Numeric(bool checkMin, bool checkMax, int min, int max,
                Localization::LocalizableString const& notANumberMessage,
                Localization::LocalizableString const& tooLowMessage,
                Localization::LocalizableString const& tooHighMessage)
        :Base(TheType)
        ,mCheckMin(checkMin)
        ,mCheckMax(checkMax)
        ,mMin(min)
        ,mMax(max)
        ,mNotANumberMessage(notANumberMessage)
        ,mTooLowMessage(tooLowMessage)
        ,mTooHighMessage(tooHighMessage){}
        
    private:
        bool    mCheckMin{false};
        bool    mCheckMax{false};
        int     mMin;
        int     mMax;
        
        Localization::LocalizableString mNotANumberMessage;
        Localization::LocalizableString mTooLowMessage;
        Localization::LocalizableString mTooHighMessage;
        friend ali::private_auto_ptr_factory<Numeric>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Host : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongHost;
        
    public:
        explicit Host(ali::xml::tree const& xml);
        virtual ~Host(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<Host>::new_auto_ptr(mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        explicit Host(Localization::LocalizableString const& message)
        :Base(TheType),mMessage(message){}

    private:
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<Host>;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Email : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongEmail;
        
    public:
        explicit Email(ali::xml::tree const& xml);
        virtual ~Email(){}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<Email>::new_auto_ptr(mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        explicit Email(Localization::LocalizableString const& message)
        :Base(TheType),mMessage(message){}

    private:
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<Email>;
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class EndsWith : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit EndsWith(ali::xml::tree const& xml);
        virtual ~EndsWith(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<EndsWith>::new_auto_ptr(
                mText, mIgnoreCase, mAllowEmpty, mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;

    private:
        EndsWith(ali::string const& text, bool ignoreCase, bool allowEmpty,Localization::LocalizableString const& message)
        :Base(TheType),mText(text),mIgnoreCase(ignoreCase),mAllowEmpty(allowEmpty),mMessage(message){}

    private:
        ali::string mText;
        bool mIgnoreCase{false};
        bool mAllowEmpty{false};
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<EndsWith>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class OneOf : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit OneOf(ali::xml::tree const& xml);
        virtual ~OneOf(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<OneOf>::new_auto_ptr(
                mList, mIgnoreCase, mAllowEmpty, mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        OneOf(ali::array<ali::string> const& list,
              bool ignoreCase,
              bool allowEmpty,
              Localization::LocalizableString const& message)
        :Base(TheType),mList(list)
        ,mIgnoreCase(ignoreCase)
        ,mAllowEmpty(allowEmpty)
        ,mMessage(message){}

    private:
        ali::array<ali::string> mList;
        bool mIgnoreCase{false};
        bool mAllowEmpty{false};
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<OneOf>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class SameAsOther : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit SameAsOther(ali::xml::tree const& xml);
        virtual ~SameAsOther(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<SameAsOther>::new_auto_ptr(mProp, mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;

    private:
        SameAsOther(ali::string prop,
              Localization::LocalizableString const& message)
        :Base(TheType),mProp(prop)
        ,mMessage(message){}

    private:
        ali::string mProp;
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<SameAsOther>;
    };
 
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Length : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason TooLow;
        static const FailureReason TooHigh;
        
    public:
        explicit Length(ali::xml::tree const& xml);
        virtual ~Length(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<Length>::new_auto_ptr(
                mCheckMin,
                mCheckMax,
                mMin,
                mMax,
                mTooLowMessage,
                mTooHighMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        Length(bool checkMin, bool checkMax, int min, int max,
                Localization::LocalizableString const& tooLowMessage,
                Localization::LocalizableString const& tooHighMessage)
        :Base(TheType),mCheckMin(checkMin)
        ,mCheckMax(checkMax)
        ,mMin(min)
        ,mMax(max)
        ,mTooLowMessage(tooLowMessage)
        ,mTooHighMessage(tooHighMessage){}
        
    private:
        bool    mCheckMin{false};
        bool    mCheckMax{false};

        int mMin;
        int mMax;
        
        Localization::LocalizableString mTooLowMessage;
        Localization::LocalizableString mTooHighMessage;
        friend ali::private_auto_ptr_factory<Length>;
    };
 
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class HostOneOf : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit HostOneOf(ali::xml::tree const& xml);
        virtual ~HostOneOf(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<HostOneOf>::new_auto_ptr(
                mList, mAllowEmpty, mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;
        
    private:
        HostOneOf(ali::array<ali::string> const& list,
              bool allowEmpty,
              Localization::LocalizableString const& message)
        :Base(TheType),mList(list)
        ,mAllowEmpty(allowEmpty)
        ,mMessage(message){}

    private:
        ali::array<ali::string> mList;
        bool mAllowEmpty{false};
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<HostOneOf>;
    };
    
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class HostEndsWith : public Base
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        static const Type TheType;
        static const FailureReason WrongValue;
        
    public:
        explicit HostEndsWith(ali::xml::tree const& xml);
        virtual ~HostEndsWith(){}
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        virtual ali::auto_ptr<Base> clone()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ali::private_auto_ptr_factory<HostEndsWith>::new_auto_ptr(
                mText, mAllowEmpty, mMessage);
        }

        virtual bool validate(ali::xml::tree const& value, Xml const& account, FailureDetail & detail) const;

    private:
        HostEndsWith(ali::string text,
                  bool allowEmpty,
                  Localization::LocalizableString const& message)
        :Base(TheType),mText(text)
        ,mAllowEmpty(allowEmpty)
        ,mMessage(message){}

    private:
        ali::string mText;
        bool mAllowEmpty{false};
        Localization::LocalizableString mMessage;
        friend ali::private_auto_ptr_factory<HostEndsWith>;
    };
}
}
