#pragma once

#include "ali/ali_callback.h"
#include "Softphone/PreferenceKeys/KeyStorage.h"

namespace PreferenceKeys
{
	class BasicKey;
	
	/*
	 KeyRing is a container for BasicKeys. In BasicKey constructor, the key adds itself
	 into its KeyRing. KeyRing can clear the "changed" flag for all keys and provides
	 a const "forEach" callback
	 */
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	class KeyRing
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
	public:
		typedef ali::callback<void(BasicKey const&)> ConstKeyCallback;
		
	public:
        virtual ~KeyRing()
        {}

		virtual BasicKey * findKey(ali::string_const_ref key) = 0;
		virtual void addKey(BasicKey * key) = 0;
		virtual void clearChanges() = 0;
		virtual void forEachKey(ConstKeyCallback cb) = 0;
		
	protected:
		void clearKeyChange(BasicKey & key);
	};
	
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	class ContainerBase : public KeyRing, public KeyStorage
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
    public:
        virtual ~ContainerBase()
        {}
	};
    
} // namespace Preferences


