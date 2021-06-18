#pragma once

#include "Softphone/Mergeable/MergeableXml.h"

#include "Softphone/Account/AccountDefaults.h"
#include "Softphone/Account/AccountValidator.h"
#include "Softphone/Account/AccountType.h"
#include "Softphone/Account/AccountXmlNodeAttributes.h"
#include "Softphone/Account/AccountFilter.h"

namespace Softphone
{
namespace Account
{
    class Migrations;
        
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    class Xml : public Mergeable::Xml
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
    public:
        /** @brief constructs empty account Xml
         */
        Xml(Type type);

        /** @brief constructs Xml from plain xml tree.
         * Nodes without "priority" and "source" will have these attributes added, with values
         * taken from defaultAttributes
         */
        Xml(ali::xml::tree const& xml, NodeAttributes const& defaultAttributes = {})
        :Mergeable::Xml{xml,defaultAttributes}{}

        /** @brief constructs Xml from Defaults.
         * Here we assume that XML coming from Defaults will have attributes "priority" and "source"
         * filled in
         */
        Xml(Defaults const& defaults)
        :Mergeable::Xml{defaults.getAccountXml().getXml()}{}

        ~Xml() = default;

        bool isEnabled() const;
        void setEnabled(bool enabled = true);
        
        bool isImportPending() const;
        void setImportPending(bool pending = true);

        Type getType() const;

    private:
        friend class Sanitization;
        friend class Migration;

        bool isMigrationApplied(ali::string_const_ref uid) const;
        void setMigrationApplied(ali::string_const_ref uid);
    };

}
}
