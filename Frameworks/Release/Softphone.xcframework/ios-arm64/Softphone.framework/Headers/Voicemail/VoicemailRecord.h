/*
 *  VoicemailRecord.h
 *  libsoftphone
 *
 *  Created by jiri on 6/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_string.h"

namespace Voicemail
{
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	class Record
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
	protected:
		Record(int newMessages, int oldMessages, bool msgsWaiting)
		:_newMessages(newMessages),_oldMessages(oldMessages),_messagesWaiting(msgsWaiting),_valid(true)
		{}
		
	public:
		Record()
		:_newMessages(0),_oldMessages(0),_messagesWaiting(false),_valid(false)
		{}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        bool operator == (Record const& lhs) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return _newMessages == lhs._newMessages
            && _oldMessages == lhs._oldMessages
            && _valid == lhs._valid;
        }
        
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        bool operator != (Record const& lhs) const
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return ! operator==(lhs);
        }

    protected:
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void setMessageAccount(ali::string const& acct)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            _messageAccount = acct;
        }
        
	public:
		bool isValid() const			{return _valid;}
		bool messagesWaiting() const	{return _messagesWaiting;}
		int getOldMessages() const		{return _oldMessages;}
		int getNewMessages() const		{return _newMessages;}
		ali::string const& getMessageAccount() const {return _messageAccount;}
		
	private:
		int _newMessages;
		int _oldMessages;
		bool _messagesWaiting;
		bool _valid;
		ali::string _messageAccount;
	};
}



