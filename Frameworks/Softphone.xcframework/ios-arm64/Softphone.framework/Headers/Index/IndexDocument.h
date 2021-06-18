/*
 *  IndexDocument.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 26/10/15.
 *  Copyright 2015 Acrobits, s.r.o. All rights reserved.
 *
 */

#pragma once

#include "ali/ali_array_set.h"
#include "ali/ali_string.h"

namespace Softphone
{
namespace Index
{
    typedef ali::pair<ali::string, ali::string> Term;

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class Document
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        struct Field
        {
            struct Store
            {
                enum Type
                {
                    No,
                    Yes
                };
            };

            struct Index
            {
                enum Type
                {
                    No,
                    Tokenized,
                    Untokenized
                };
            };

            struct TermVector
            {
                enum Type
                {
                    No,
                    Yes,
                    YesWithPositions,
                    YesWithOffsets,
                    YesWithPositionsAndOffsets
                };
            };

            Field(ali::string name, ali::string value)
            :name(name)
            ,value(value)
            {}

            Field(ali::string name, ali::string value,
                  Store::Type store, Index::Type index, TermVector::Type tv)
            :name(name)
            ,value(value)
            ,store(store)
            ,index(index)
            ,termVector(tv)
            {}

            friend int compare(Field const& lhs, Field const& rhs)
            {
                using ali::compare;
                return compare(lhs.name, rhs.name);
            }

            friend int compare(Field const& lhs, ali::string_const_ref rhs)
            {
                using ali::compare;
                return compare(lhs.name, rhs);
            }

            friend int compare(ali::string_const_ref lhs, Field const& rhs)
            {
                using ali::compare;
                return compare(lhs, rhs.name);
            }

            ali::string         name;
            ali::string         value;

            Store::Type         store      {Store::No};
            Index::Type         index      {Index::Tokenized};
            TermVector::Type    termVector {TermVector::No};
        };

        using Fields = ali::array_set<Field>;

    public:
        Document(ali::string type, ali::string id)
        :mType(type),mId(id){}

        ~Document(){}

        ali::string const& getType() const
        {
            return mType;
        }

        ali::string const& getId() const
        {
            return mId;
        }

        Field const& getField(ali::string_const_ref name) const
        {
            Field const* field = findField(name);
            ali_assert(field != nullptr);
            return *field;
        }

        Field const* findField(ali::string_const_ref name) const
        {
            int index = mFields.index_of(name);
            if (index == mFields.size())
                return nullptr;
            return &mFields.at(index);
        }

        bool setField(Field const& field)
        {
            return mFields.insert_or_update(field);
        }

        int getFieldCount() const
        {
            return mFields.size();
        }

        Field const& getFieldAt(int pos) const
        {
            return mFields.at(pos);
        }

        Fields::const_range_type iterateFields() const
        {
            return mFields.range();
        }

        ali::string toString() const;
        
    private:
        ali::string         mType;
        ali::string         mId;

        Fields mFields;
    };
}
}

