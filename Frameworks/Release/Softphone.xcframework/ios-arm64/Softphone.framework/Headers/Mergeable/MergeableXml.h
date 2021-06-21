#pragma once

#include "ali/ali_xml_tree2.h"
#include "Softphone/Mergeable/MergeableXmlFilter.h"
#include "Softphone/Mergeable/MergeableXmlNodeAttributes.h"
#include "Softphone/Mergeable/MergeableXmlValidator.h"

namespace Softphone::Mergeable
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Xml
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        /** @brief constructs empty account Xml
         */
        Xml(ali::string_const_ref rootNodeName)
        :mXml(rootNodeName){}

        /** @brief constructs Xml from plain xml tree.
         * Nodes without "priority" and "source" will have these attributes added, with values
         * taken from defaultAttributes
         */
        Xml(ali::xml::tree const& xml, NodeAttributes const& defaultAttributes = {});

        ~Xml() = default;

        /** @brief uses validator from account defaults to validate stored values
         * @param failureDetail When return value is false, it contains error code and message
         *
         */
        bool validate(Validator::Array const& validators,
                      Validator::FailureDetail & failureDetail) const;

        /** @brief returns plain xml tree (read-only) with account values.
         */
        ali::xml::tree const& get() const { return mXml;}

        /** @brief helper method which returns string value stored in account Xml node
         */
        ali::xml::string const& getString(ali::string_const_ref key) const
        {
            return mXml.nodes[key].data;
        }
        
        /** @brief helper method which looks for a localized version of string in account Xml node.
         * If not found, returns getString()
         *
         *  <node priority="10" source="whatever">
         *   <value lang="cs">czech</value>
         *   <value>default</value>
         *  </node>
         */
        ali::xml::string getLocalizedString(ali::string_const_ref key) const;

        /** @brief merges the modifications into our account XML, if the priorities
         * of modifications are higher or equal and if filter allows it.
         * @param modifications The Xml to merge in
         * @param filter Filter which is used to check whether modification is allowed
         *
         */
        void merge(Xml const& modifications, Filter::Base const& filter = Filter::AllAllowed{});

        /** @brief merges single value into our account XML, if the priorities
         * of modifications are higher or equal and if filter allows it.
         * @param key The node in XML where to set the value
         * @param value The value to set
         * @param defaultAttributes Attributes to set in case the node doesn't exist yet
         * @param filter Filter which is used to check whether modification is allowed
         *
         */
        bool mergeValue(ali::xml::string key,
                        ali::xml::string value,
                        NodeAttributes const& defaultAttributes = {},
                        Filter::Base const& filter = Filter::AllAllowed{});

        bool mergeValue(ali::xml::string key,
                        ali::string_const_ref value,
                        NodeAttributes const& defaultAttributes = {},
                        Filter::Base const& filter = Filter::AllAllowed{})
        {
            return mergeValue(
                ali::move(key), ali::xml::string{value},
                defaultAttributes, filter);
        }

        bool mergeValue(ali::string_const_ref key,
                        ali::xml::string value,
                        NodeAttributes const& defaultAttributes = {},
                        Filter::Base const& filter = Filter::AllAllowed{})
        {
            return mergeValue(
                ali::xml::string{key}, ali::move(value),
                defaultAttributes, filter);
        }

        bool mergeValue(ali::string_const_ref key,
                        ali::string_const_ref value,
                        NodeAttributes const& defaultAttributes = {},
                        Filter::Base const& filter = Filter::AllAllowed{})
        {
            return mergeValue(
                ali::xml::string{key}, ali::xml::string{value},
                defaultAttributes, filter);
        }

        /** @brief a version of mergeValue which merges the whole subnode into our account XML
         */
        bool mergeValue(ali::xml::tree const& value,
                        NodeAttributes const& defaultAttributes = {},
                        Filter::Base const& filter = Filter::AllAllowed{});

        /** @brief determines whether the value can be merged.
         * canMergeValue uses the same logic as mergeValue to determine whether the value can be
         * merged, depending on priorities and filters. Returns boolean result.
         */
        bool canMergeValue(ali::xml::string key,
                           ali::xml::string value,
                           NodeAttributes const& defaultAttributes = {},
                           Filter::Base const& filter = Filter::AllAllowed{}) const;

        bool canMergeValue(ali::xml::string key,
                           ali::string_const_ref value,
                           NodeAttributes const& defaultAttributes = {},
                           Filter::Base const& filter = Filter::AllAllowed{}) const
        {
            return canMergeValue(
                ali::move(key), ali::xml::string{value},
                defaultAttributes, filter);
        }

        bool canMergeValue(ali::string_const_ref key,
                           ali::xml::string value,
                           NodeAttributes const& defaultAttributes = {},
                           Filter::Base const& filter = Filter::AllAllowed{}) const
        {
            return canMergeValue(
                ali::xml::string{key}, ali::move(value),
                defaultAttributes, filter);
        }

        bool canMergeValue(ali::string_const_ref key,
                           ali::string_const_ref value,
                           NodeAttributes const& defaultAttributes = {},
                           Filter::Base const& filter = Filter::AllAllowed{}) const
        {
            return canMergeValue(
                ali::xml::string{key}, ali::xml::string{value},
                defaultAttributes, filter);
        }

        /** @brief a version of canMergeValue which does the check for the whole subnode
         */
        bool canMergeValue(ali::xml::tree const& value,
                           NodeAttributes const& defaultAttributes = {},
                           Filter::Base const& filter = Filter::AllAllowed{}) const;

        /** @brief removes all nodes with the given name, if the priorities
         * in provided attributes are higher or equal and if filter allows it.
         * @param key The name of nodes to ne deleted
         * @param attributes Attributes used to check whether the operation is permitted
         * @param filter Filter which is used to check whether modification is allowed
         *
         */
        void removeAll(ali::string_const_ref key,
                       NodeAttributes const& attributes = {},
                       Filter::Base const& filter = Filter::AllAllowed{});

        
        NodeAttributes getNodeAttributes(ali::string_const_ref key,
                                         NodeAttributes const& defaultAttributes = {}) const;
        
        ali::xml::string const& getId() const;
        void setId(ali::xml::string id);
        void setId(ali::string_const_ref id)
        {
            setId(ali::xml::string{id});
        }

        ali::string dump(bool pretty = true) const;

        friend ali::string& format(
            ali::string& str,
            Xml const& value,
            ali::string_const_ref formatString )
        {
            return ali::xml::format(str, value.mXml, formatString);
        }

    protected:
        ali::xml::tree & getXml()
        {
            return mXml;
        }

        ali::xml::tree const& getXml() const
        {
            return mXml;
        }

    private:
        bool mergeNode(ali::xml::tree const& node,
                       Filter::Base const& filter,
                       bool dryRun);

    protected:
        /** @brief set value directly into our account XML, without checking priorities
         * The value can be either overwritten,or newly created, in which case it's created
         * with the provided attributes. This private method can only be used by friend classes.
         * @param subNode The node in XML where to set
         * @param defaultAttributes Attributes to set in case the node doesn't exist yet
         *
         */
        void setValue(ali::xml::tree const& subNode,
                      NodeAttributes const& defaultAttributes = {});

        /** @brief helper method for simple nodes with string value
         */
        void setValue(ali::xml::string key,
                      ali::xml::string value,
                      NodeAttributes const& defaultAttributes = {})
        {
            setValue({ali::move(key), ali::move(value)}, defaultAttributes);
        }

        void setValue(ali::xml::string key,
                      ali::string_const_ref value,
                      NodeAttributes const& defaultAttributes = {})
        {
            setValue(ali::move(key), ali::xml::string{value}, defaultAttributes);
        }

        void setValue(ali::string_const_ref key,
                      ali::xml::string value,
                      NodeAttributes const& defaultAttributes = {})
        {
            setValue(ali::xml::string{key}, ali::move(value), defaultAttributes);
        }

        void setValue(ali::string_const_ref key,
                      ali::string_const_ref value,
                      NodeAttributes const& defaultAttributes = {})
        {
            setValue(ali::xml::string{key}, ali::xml::string{value}, defaultAttributes);
        }

    private:
        ali::xml::tree      mXml;
    };

}
