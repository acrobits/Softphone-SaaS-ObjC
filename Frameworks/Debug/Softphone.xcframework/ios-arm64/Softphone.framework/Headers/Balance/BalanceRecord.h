/*
 *  BalanceRecord.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 4/25/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_string.h"
#include "ali/ali_auto_ptr.h"

namespace ali
{
    namespace xml
    {
        struct tree;
    }
}

namespace Balance
{
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	struct Record
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
		Record();
		~Record();

        Record(Record const& other);
        Record & operator =(Record const& other);
        
        friend ali::string& format(
            ali::string& str,
            Record const& value,
            ali::string_const_ref /*formatString*/ )
        {
            return str.append(value.balanceString);
        }
        
    public:
		bool valid;
		
		ali::string balanceString;
		float balance;

        ali::xml::tree & info();
        ali::xml::tree const& info() const;
        
    private:
        mutable ali::auto_ptr<ali::xml::tree> _info;
	};
}
