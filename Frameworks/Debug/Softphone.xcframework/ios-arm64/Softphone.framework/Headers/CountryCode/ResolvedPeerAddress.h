//
//  ResolvedPeerAddress.h
//  libsoftphone
//
//  Created by Jiri Kral on Jun 21st 2016.
//
//

#pragma once

#include "ali/ali_generic_peer_address.h"

#include "DetectionResult.h"

namespace NumberRewriting
{
    struct Context;
}

namespace Softphone
{
namespace CountryCode
{
    class Detector;

    extern ali::string codeForIso2(ali::string const& iso2);
    extern ali::string olympicForIso2(ali::string const& iso2);
    
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class ResolvedPeerAddress : public ali::generic_peer_address
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        ResolvedPeerAddress() = default;

        ResolvedPeerAddress(ali::string address,
                            Detector const& detector,
                            NumberRewriting::Context const* nrContext = nullptr);

        ResolvedPeerAddress(ali::generic_peer_address const& address,
                            Detector const& detector,
                            NumberRewriting::Context const* nrContext = nullptr);

        static ResolvedPeerAddress fromString(ali::string address);
        static ResolvedPeerAddress fromPeerAddress(ali::generic_peer_address const& address);

        /**
         * Returns true if it was possible to detect country information from input. Detection
         * is only performed on numerical addresses, which are considered phone numbers. 
         * The detection is performed on rewritten number; the number rewriter may prepend some
         * default country code if the input didn't have any.
         * @returns true if any country was detected.
         */
        bool isCountryResolved() const {return mCountryResolved;}

        /**
         * Returns the address in a formatted form. In case it's recognized as a number, it is
         * formatted based on the detected country. In case detection fails, the method returns
         * the address as it was passed to constructor.
         * @returns human-readable representation of this address
         */
        ali::string const& getHumanReadable() const
        {
            return mInfo.formattedNumber;
        }

        /**
         * Returns the regional prefix as it was recognized in country resolution process. The
         * returned value is a country code, possibly followed by more digits which define some
         * more specific area or service within the country.
         *
         * The code is returned without the leading +. Example return value is "1613" (Canada, Ontario).
         * @return recognized country code prefix if the country was successfully resolved, otherwise 
         *         empty string.
         */
        ali::string const& getRegionalPrefix() const
        {
            return mInfo.regionalPrefix;
        }

        /**
         * @return country ISO2 code in case country resolution was successful, otherwise empty string.
         */
        ali::string const& getCountryIso2() const
        {
            return mInfo.iso2Code;
        }

        /**
         * @return country 3-digit olympic code in case country resolution was successful, otherwise 
         * empty string.
         */
        ali::string const& getCountryOlympicCode() const
        {
            return mInfo.olympicCode;
        }

        /**
         * Returns the title which corresponds to the detected country code prefix. It can be a country
         * name, possibly followed by a more specific geographic region. Example: "Canada, Ontario"
         * @return title of the detected country or region, or empty string if detection fails.
         */
        ali::string const& getCountryTitle() const
        {
            return mInfo.title;
        }

        /**
         * Returns the number in E.164 format, if at all possible. Number rewriting rules are applied
         * to the input to create E.164 form; in case the input number does not have any country code,
         * a default country code may pre-pended.
         * @return number in E.164 format as output from number rewriter, or empty string if input is 
                   not a number.
         */
        ali::string const& getE164() const
        {
            return mInfo.e164Number;
        }

    private:
        CountryCode::DetectionResult     mInfo;
        bool                             mCountryResolved;
    };

}
}

