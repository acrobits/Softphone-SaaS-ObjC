#pragma once
#include "ali/ali_string.h"

namespace PreferenceKeys
{
	class BasicKey;
	/*
	An interface used by GenericBasicKey to read and write key-value pairs, where
	both key and value are ali::strings
	*/
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	class KeyStorage
	//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	{
    public:
        virtual ~KeyStorage()
        {}

		virtual ali::xml::string const& getValueForPreferenceKey(ali::string_const_ref key) const = 0;
		virtual void setValueForPreferenceKey(ali::string_const_ref key, ali::xml::string const& val) = 0;
	};
} // namespace Settings

