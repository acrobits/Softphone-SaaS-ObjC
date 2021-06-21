/*
 *  Softphone.h
 *  libsoftphone
 *
 *  Copyright (c) 2010 - 2018 Acrobits, s.r.o. All rights reserved.
 */

#pragma once

#include "SdkFeatures.h"

#include "ali/ali_array.h"
#include "ali/ali_array_set.h"
#include "ali/ali_handle.h"
#include "ali/ali_optional.h"
#include "ali/ali_serializer.h"
#include "ali/ali_string.h"
#include "ali/ali_xml_tree2.h"
#include "ali/ali_net_interface_forward.h"

//  PK: Conflict during master -> merge_win_mac merge
#ifdef  ACROBITS_DESKTOP_APP

#include "AudioDevice.h"
#include "MicrophoneType.h"

#else   //  !ACROBITS_DESKTOP_APP

#include "AudioRoute.h"

#endif  //  ACROBITS_DESKTOP_APP

#include "Balance/BalanceRecord.h"
#include "Call/CallStatistics.h"
#include "Call/CallTypes.h"
#include "Call/DesiredMedia.h"
#include "Call/CallAudioHook.h"
#include "Call/CallActionSheetConfig.h"

#include "Softphone/Contacts/ContactBlock.h"
#include "Softphone/Contacts/ContactEnumeration.h"
#include "Softphone/Contacts/ContactSection.h"
#include "Softphone/Contacts/ContactSourceState.h"

#include "Softphone/DialAction/DialActionType.h"

#include "Softphone/Index/IndexQuery.h"
#include "Softphone/Index/IndexDocument.h"
#include "Softphone/Index/ContactIndexFields.h"

#include "Invite/InviteResult.h"

#include "Softphone/Account/AccountXml.h"
#include "Softphone/Account/AccountDefaults.h"

//  This seems obsolete:
//#if defined(SOFTPHONE_ACCOUNT_TEMPLATES)
//#   include "Registrator/AccountTemplate.h"
//#endif

#include "Softphone/EventHistory/CallEvent.h"
#include "Softphone/EventHistory/EventHistoryStorage.h"
#include "Softphone/EventHistory/EventHistory.h"
#include "Softphone/Messaging/MediaTransferHandle.h"
#include "Softphone/Messaging/ComposingInfo.h"
#include "Network.h"
#include "NetworkConditions.h"
#include "PushNotificationCompletion.h"
#include "PushTestScheduleResult.h"
#include "Softphone/PushToken/PushTokenUsage.h"
#include "Softphone/Rate/RateRecord.h"
#include "Registrator/RegTypes.h"
#include "Ringtone.h"
#include "Rtp_CodecInfo.h"
#include "SipMessage.h"
#include "Softphone_Preferences.h"
#include "DialogInfo.h"
#include "Softphone/Messaging/GroupManagementTransport.h"

#if defined(SOFTPHONE_VOICEMAIL)
#   include "Voicemail/VoicemailRecord.h"
#endif

#if defined(SOFTPHONE_VIDEO)
#   include "Video/CaptureTypes.h"
#endif

#if defined(ANDROID)
#   include "cz/acrobits/libsoftphone/Preferences.Factory.h"
#endif

/** @mainpage Acrobits libSoftphone SDK
  *
  * This is the Doxygen-generated documentation for libSoftphone SDK. Please
  * also check the content of doc/ folder for older documentation, there are
  * still valuable pieces of information (like the format of account XML). To
  * see libSoftphone in action, please try the included DemoPhone project which
  * demonstrates use of most of the libSoftphone APIs.
  *
  * The main API point is the @ref Softphone::Instance class which contains
  * several interfaces that control most of the libSoftphone functionality. */

namespace Softphone
{
    /** @brief Get whether this is a debug SDK build
      * @retval true This is a debug SDK build
      * @retval false This is a release SDK build */
    bool isDebug();

    /** @brief Get SDK build number
      * @return SDK build number */
    unsigned getBuild();

    /** @brief Get CPU version of the SDK
      * @return The CPU version as a string */
    ali::string_const_ref getCpuVersion();

    class Instance;

#if defined(ANDROID)
    using OverrideDefaults = cz::acrobits::libsoftphone::Preferences::Factory;
#else
    /** @brief Method to override default values of the preferences
      *
      * This method gets called during @ref init "initialization of the instance"
      */
    using OverrideDefaults = ali::callback<void(Softphone::Preferences &)>;
#endif

#if defined(SOFTPHONE_PROVISIONING)
    /** @brief Initialize the instance
      * @param tree XML tree for provisioning
      * @param overrideDefaults Method which is called during library
      *                         initialization that can override default values
      *                         of the preferences; not used if @c nullptr
      * @retval true Provisioned instance initialized
      * @retval false The instance is already initialized; assets in debug build
      *
      * This method is used for initializing a provisioned instance and
      * optionally overriding default values of the preferences. */
    bool init(ali::xml::tree const& tree,
              OverrideDefaults const& overrideDefaults = nullptr);

    /** @brief Initialize the instance
      * @param overrideDefaults Method which is called during library
      *                         initialization that can override default values
      *                         of the preferences; not used if @c nullptr
      * @retval true Provisioned instance initialized
      * @retval false The instance is already initialized; assets in debug build
      */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    static inline bool init(OverrideDefaults const& overrideDefaults = nullptr)
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        return init(ali::xml::tree(), overrideDefaults);
    }
#else
    /** @brief Initialize the instance
      * @param overrideDefaults Method which is called during library
      *                         initialization that can override default values
      *                         of the preferences; not used if @c nullptr
      * @retval true Provisioned instance initialized
      * @retval false The instance is already initialized; assets in debug build
      */
    bool init(OverrideDefaults const& overrideDefaults = nullptr);
#endif

    /** @brief Deinitialize the instance
      *
      * You should first @ref State.terminate "terminate the SDK"
      * before deinitializing it. In debug build, deinitializing an
      * unterminated SDK asserts.
      *
      * The library is not unloaded as Android does not support it.
      *
      * You do not need to call this method if you want to exit the process,
      * however you should @ref State.terminate "terminate the SDK" before
      * that. */
    void deinit();

    /** @brief Get the instance
      * @return The instance
      * @retval nullptr The instance was not initialized with @ref init
      *
      * This is the usual way of getting the instance.
      *
      * Do not forget to call @ref init prior to the first call of this
      * method! */
#ifdef ALI_DEBUG
    Instance * instanceDebug();
#else
    Instance * instanceRelease();
#endif

    static inline Instance * instance()
    {
#ifdef ALI_DEBUG
        return instanceDebug();
#else
        return instanceRelease();
#endif
    }

    bool instanceExists();

#if defined(SOFTPHONE_ADDONS)
    /** @brief Addon */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Addon
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Enumeration of add-ons
          *
          * Add-ons are divided into groups:
          * - 1xx for audio codecs
          * - 2xx for video codecs
          * - 9xx for security */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        enum Type
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            G729 = 101, ///< G.729 addon
            // 102 was AMR WB addon

#   if defined(SOFTPHONE_SECURITY_ZRTP)
            ZRTP = 901, ///< ZRTP addon
#   endif
        };
    };
#endif

    /** @brief DNS SRV record */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct DnsSrvRecord
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Constructor */
        DnsSrvRecord()
            : priority(0)
            , weight(0)
            , port(0)
        {}

        unsigned short  priority; ///< SRV priority
        unsigned short  weight; ///< SRV weight
        ali::string     host; ///< Host name
        unsigned short  port; ///< Port

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        friend int compare(
            DnsSrvRecord const& lhs,
            DnsSrvRecord const& rhs)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            using ali::compare;
            int result = compare(lhs.priority, rhs.priority);
            if (result != 0) return result;
            result = compare(lhs.weight, rhs.weight);
            if (result != 0) return result;
            result = compare(lhs.host, rhs.host);
            if (result != 0) return result;
            return result = compare(lhs.port, rhs.port);
        }

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool operator ==(DnsSrvRecord const& other) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return priority == other.priority
                    && weight == other.weight
                    && host == other.host
                    && port == other.port;
        }

        bool operator !=(DnsSrvRecord const& other) const
        {
            return !operator ==(other);
        }
    };

    /** @brief Set of DNS SRV records */
    typedef ali::array_set<DnsSrvRecord> DnsSrvRecords;

    /** @brief NAT traversal status */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct NatTraversalStatus
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Constructor */
        NatTraversalStatus()
            : detectedNat(false)
            , detectedSymmetricNat(false)
        {}

        ali::string                 networkInterfaceName; ///< System’s network interface name
        ali::string                 friendlyName; ///< Network interface user-friendly name

        bool                        detectedNat; ///< Whether NAT was detected
        bool                        detectedSymmetricNat; ///< Whether symmetric NAT was detected
    };

    /** @brief Response status line */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct ResponseStatusLine
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        ali::string code; ///< Response code
        ali::string phrase; ///< Response phrase
    };

    /** @brief Stream availability information */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct StreamAvailability
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        bool incoming; ///< Available for incoming direction
        bool outgoing; ///< Available for outgoing direction
    };

    /** @brief Transient attributes used in events */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Transients
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Call rate information
          * @type{dict}
          * @see @ref Call::Rate::Record::Call::JSON <br/>
          *      @ref Call::Rate::Record::Call::fromJson */
        static ali::string_literal const callRate;

        /** @brief RTP payload numbers of codecs used in call
         * @type{dict} */
        static ali::string_literal const codecs;
        static ali::string_literal const networkInterfaceName;

        /** @brief Desired media
          * @type{dict} */
        static ali::string_literal const desiredMedia;

        /** @brief Desired media attributes */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct DesiredMedia
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            /** @brief Incoming video
              * @type{bool} */
            static ali::string_literal const incomingVideo;

            /** @brief Outgoing video
              * @type{bool} */
            static ali::string_literal const outgoingVideo;
        };

        /** @brief Notifications transients */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Notifications
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Transient for the incoming call notification ID.
             * @type{string}
             *
             * you can use this convenience transient to be able to cancel the incoming call notification
             * from call state changed callback. It's here so you don't have to come up with your own
             * identification. It's based on the push call ID or the event ID in case of non-pushed calls
             */
            static ali::string_literal const incomingCallNotificationId;

            /** @brief Transient for the missed call notification ID.
             * @type{string}
             *
             * you can use this convenience transient to be able to cancel the previous missed call notification
             * when firing a new one with different missed call count for the same callee.
             * It's based on the callee URI
             */
            static ali::string_literal const missedCallNotificationId;

            /** @brief Transient for the missed call count per the callee
             * @type{string}
             *
             * you can use this convenience transient to be able to always display a single missed calls notification
             * per callee with the total number of missed calls for the particular person.
             * To be used with missedCallNotificationId transient to be able to cancel the previous notification
             */
            static ali::string_literal const missedCallCountForCallee;

            /** @brief Transient for whether the user should be notified about the incoming call
             * @type{string}
             *
             * this transient is false if the call is automatically rejected or
             * when the user is notified by the system (e.g. CallKit native incoming call screen)
             */

            static ali::string_literal const notifyUser;
        };

#if defined(SOFTPHONE_SECURITY)
        /** @brief Security status
          * @type{dict}
          * @see @ref Security */
        static ali::string_literal const security;

        /** @brief Security status attributes */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Security
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            /** @brief Audio stream status
              * @type{dict}
              * @see @ref Stream */
            static ali::string_literal const audio;

#   if defined(SOFTPHONE_VIDEO)
            /** @brief Video stream status
              * @type{dict}
              * @see @ref Stream */
            static ali::string_literal const video;
#   endif

            /** @brief Security status of a stream */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Stream
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                /** @brief Whether this stream is sending data
                  * @type{bool} */
                static ali::string_literal const active;

                /** @brief Stream encryption state
                  * @type{string}
                  * @see @ref State */
                static ali::string_literal const state;

                /** @brief Encryption state values */
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                struct State
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    /** @brief Stream is not encrypted */
                    static ali::string_literal const Unencrypted;

                    /** @brief Keys are exchanged at the moment */
                    static ali::string_literal const KeyExchange;

                    /** @brief Stream is encrypted */
                    static ali::string_literal const Encrypted;
                };

                /** @brief Stream key exchange mechanism
                  * @type{string}
                  * @see @ref KeyExchange */
                static ali::string_literal const keyExchange;

                /** @brief Key exchange mechanism values */
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                struct KeyExchange
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    /** @brief SDP SDES key exchange */
                    static ali::string_literal const SDES;

#   if defined(SOFTPHONE_SECURITY_ZRTP)
                    /** @brief ZRTP key exchange */
                    static ali::string_literal const ZRTP;
#   endif
                    /** @brief DTLS key exchange */
                    static ali::string_literal const DTLS;
                };

                /** @brief Cipher suite
                  * @type{dict}
                  * @see @ref CipherSuite */
                static ali::string_literal const cipherSuite;

                /** @brief Cipher suite information */
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                struct CipherSuite
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    /** @brief Cipher suite technical name
                      * @type{string} */
                    static ali::string_literal const name;

                    /** @brief Cipher suite human-readable description
                      * @type{string} */
                    static ali::string_literal const description;
                };

#   if defined(SOFTPHONE_SECURITY_ZRTP)
                /** @brief ZRTP status
                  * @type{dict}
                  * @see @ref ZRTP */
                static ali::string_literal const zrtp;

                /** @brief ZRTP key exchange status */
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                struct ZRTP
                //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                {
                    /** @brief ZRTP confirmInfo SAS
                      * @type{string} */
                    static ali::string_literal const confirmInfoSas;

                    /** @brief Remote ZID (hexadecimal string)
                      * @type{string} */
                    static ali::string_literal const remoteZid;

                    /** @brief Cache status
                      * @type{dict}
                      * @see @ref Cache */
                    static ali::string_literal const cache;

                    /** @brief Cache status */
                    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                    struct Cache
                    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
                    {
                        /** @brief Cache mismatch flag
                          * @type{bool} */
                        static ali::string_literal const mismatch;

                        /** @brief Cache entry flags
                         * @type{unsigned int} */
                        static ali::string_literal const flags;

                        struct Flags
                        {
                            static int const Rs1Valid;
                            static int const Rs2Valid;
                            static int const AuxValid;
                            static int const PbxValid;
                            static int const PbxAccepted;
                            static int const LocalSasVerified;
                            static int const RemoteSasVerified;
                        };
                    };

                    /** @brief Whether PBX enrollment is now in progress
                      * @type{bool} */
                    static ali::string_literal const pbxEnrollment;

                    /** @brief Man-in-the-Middle flag
                      *
                      * Whether there is a third party (usually the PBX)
                      * re-encrypting the stream and so the possibility of
                      * someone eavesdropping
                      * @type{bool} */
                    static ali::string_literal const mitm;

                    /** @brief ZRTP confirmInfoFromMitm SAS
                     * @type{string} */
                    static ali::string_literal const confirmInfoFromMitmSas;

                    /** @brief ZRTP confirmInfo Valid
                     * @type{string} */
                    static ali::string_literal const confirmInfoValid;

                    /** @brief ZRTP confirmInfoFromMitm Valid
                     * @type{string} */
                    static ali::string_literal const confirmInfoFromMitmValid;
                };
#   endif
            };
        };
#endif
    };

    /** @brief Interface of an AppInterface for the UI Related Events in the Acrobits libsoftphone library */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class AppInterface
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        virtual ~AppInterface()
        {}
        
        virtual void showMaxCallAlert(int /*maxCalls*/) = 0;
        
        virtual void showNoNetworkAlert() = 0;
        
        virtual void showMicrophoneAlert() = 0;
        
        virtual void showRegistrationAlert() = 0;
        
        virtual void presentDialActionSheet(CallActionSheetConfig const& /*config*/) = 0;
        
        virtual bool dialGsm(ali::string_const_ref /*dialString*/, Softphone::EventHistory::CallEvent::Pointer /*callEvent*/) = 0;
        
        virtual bool canPlaceGsmCalls() = 0;
        
        virtual void showCallWithId(Softphone::EventHistory::CallEvent::Pointer const& /*callEvent*/) = 0;
        
        virtual void composeMessageTo(Softphone::EventHistory::CallEvent::Pointer /*event*/) = 0;
        
        virtual bool composeMessage(Softphone::EventHistory::MessageEvent::Pointer /*event*/) = 0;
        
        virtual void messageSendingComplete(Softphone::EventHistory::MessageEvent::Pointer const message, bool /*result*/, ali::string const& /*messageFromTransport*/) = 0;
        
        virtual bool copyNumber(ali::string const& /*number*/) = 0;
        
        virtual bool openUrl(const ali::string &/*url*/) = 0;

        virtual bool isInTogetherConference() const = 0;
        
        virtual Softphone::EventHistory::CallEvent::Pointer getControlledCall() = 0;
    };

    /** @brief Interface of an observer of events in the Acrobits libsoftphone library */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class Observer
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        /** @brief Virtual destructor */
        virtual ~Observer()
        {}

        /** @brief Network change detected
          * @param network Current (new) network type */
        virtual void onNetworkChangeDetected(Network::Type network) = 0;

        /** @brief Invoked when a remote host supplies invalid TLS certificate.
          * @param host The domain name of the remote host
          * @param certificateHash The hash value of the certificate chain
          * 
          * You can use the Softphone::instance()->network()->certificates()
          * interface to manage exceptions to TLS certificate verification. */
        virtual void onNetworkCertificateVerificationFailed(
            ali::string const& /*host*/,
            ali::string const& /*certificateHash*/ )
        {}

        /** @brief SIP message has been sent or received
          * @param message The SIP message */
        virtual void onSipMessageCapture(SipMessage const& /*message*/)
        {}

        /** @brief New event arrived
          * @param event The newly arrived event
          *
          * if the event is a message received via SIMPLE, it's automatically accepted
          * unless you set rejectCode integer transient on the event. If you do you can also
          * set rejectPhrase string, rejectWarningCode integer and rejectWarningPhrase
          * string
         */

        virtual void onNewEvent(EventHistory::Event::Pointer event) = 0;

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Registration state changed
          * @param accountId Account that changed the registration state
          * @param state The new registration state */
        virtual void onRegistrationStateChanged(ali::string const& accountId,
                                                Registrator::State::Type state) = 0;

        /** @brief Registration failed
          * @param accountId Account that failed to register
          * @param message The registration error message */
        virtual void onRegistrationErrorMessage(ali::string const& /*accountId*/,
                                                ali::string const& /*message*/)
        {}
#else
        /** @brief Registration state changed
          * @param state The new registration state */
        virtual void onRegistrationStateChanged(Registrator::State::Type state) = 0;

        /** @brief Registration failed
          * @param message The registration error message */
        virtual void onRegistrationErrorMessage(ali::string const& message)
        {}
#endif

#if defined(SOFTPHONE_CALLTHROUGH)
        /** @brief Call-through result arrived
          * @param call The related call event
          * @param callee Callee to dial for the call-through if successful;
          *               @c nullptr otherwise
          * @retval true Successfully dialed
          * @retval false @p success if @c false or failed to dial */
        virtual bool onCallThroughResult(Softphone::EventHistory::CallEvent::Pointer /*call*/,
                                         ali::generic_peer_address const* /*callee*/)
        {
            return false;
        }
#endif

        /** @brief State of a call changed
          * @param call The call that changed state
          * @param state The new call state */
        virtual void onCallStateChanged(EventHistory::CallEvent::Pointer call,
                                        Call::State::Type state) = 0;

        /** @brief A hold state of a call changed
          * @param call The call that changed a hold state
          * @param states The new hold states */
        virtual void onCallHoldStateChanged(EventHistory::CallEvent::Pointer call,
                                            Call::HoldStates const& states) = 0;

//  PK: Conflict during master -> merge_win_mac merge
#ifdef  ACROBITS_DESKTOP_APP

        /** @brief Audio devices have changed
          * @param devices the current snapshot of audio devices available
          *
          * This method is called each time the audio input or output devices change.
          * However, when that occurs is platform-dependent. */
        virtual void onAudioDevicesChanged(Softphone::AudioDeviceSnapshot const& /*devices*/)
        {}

        /** @brief Default audio device has changed
         * @param device the currently set device
         * @param deviceType specifies whether default input or output has changed
         *
         * This method is called each time the default audio input or output devices change.
         * However, when that occurs is platform-dependent. */
        virtual void onCurrentAudioDeviceChanged(Softphone::AudioDevice const& /*device*/,
                                                 Softphone::AudioDeviceType /*deviceType*/)
        {}

#else   //  !ACROBITS_DESKTOP_APP

        /** @brief Audio route changed
          * @param route The new audio route
          *
          * This method is called each time the call audio route changes.
          * However, when that occurs is platform-dependent. */
        virtual void onAudioRouteChanged(AudioRoute::Type /*route*/)
        {}

        /** @brief Available audio routes changed
         *  @param routes Set of newly available audio routes
         *
         *  This method is called each time the available call audio routes change.
         *  However, when that occurs is platform-dependent. */
        virtual void onAudioAvailableRoutesChanged(ali::array_set<Softphone::AudioRoute::Type> /*routes*/)
        {}

#endif  //  ACROBITS_DESKTOP_APP

        /**
         * @brief Video flow changed
         * @param incoming Whether incoming video is flowing
         * @param outgoing Whether outgoing video is flowing * */
        virtual void onVideoFlowChanged(bool /*incoming*/, bool /*outgoing*/)
        {}

        /** @brief Mute changed
          * @param isMuted The current mute state
          *
          * This method is called each time the mute state changes.
          * This can happen as a result of calling setMuted() or from system events e.g. iOS CallKit in-call screen. */
        virtual void onMuteChanged(Microphone::Type /*microphone*/, bool /*isMuted*/)
        {}

        /** @brief Request for a ringtone for a call event
          * @param call The call event to get the ringtone for
          * @return The ringtone. @c nullptr if it should not ring.
          *
          * By default, it calls @ref getRingtoneFile and returns ringtone
          * for the obtained file.
          */
        virtual ali::auto_ptr<Ringtone::Base> getRingtone(EventHistory::CallEvent::Pointer call);

        /** @brief Request file for a ringtone for a call event
          * @param call The call event to get the ringtone for
          * @return File with the ringtone; empty if it should not ring.
          */
        virtual ali::filesystem2::path getRingtoneFile(EventHistory::CallEvent::Pointer /*call*/)
        {
            return {};
        }

        /** @brief Can automatic reprovisioning update the accounts right now
          * @return True if the app can be reprovisioned, false if not at the moment
          *
          * Return false for example if account settings page is open and automatic
          * reprovisioning would conflict with adjusting the settings from the GUI
          */
        virtual bool canReprovisionNow() const
        {
            return true;
        }

        /** @brief Can automatic dialAction reprovisioning be applied now?
         * @return True if the downloaded dialActions update can be applied,
         *         False otherwise
         *
         * Return false if the user is currently in process of editing dialActions
         * and applying the update could interfere with the changes he made.
         */
        virtual bool canApplyDialActionProvisioning() const
        {
            return true;
        }

        /** @brief Can automatic actionButtons reprovisioning be applied now?
         * @return True if the downloaded actionButtons update can be applied,
         *         False otherwise
         *
         * Return false if the user is currently in process of editing actionButtons
         * and applying the update could interfere with the changes he made.
         */
        virtual bool canApplyActionButtonsProvisioning() const
        {
            return true;
        }

        /** @brief DNS SRV query completed
          * @param queryId ID of the query
          * @param records Array of the found DNS SRV records */
        virtual void onDnsSrvQueryDone(unsigned long /*queryId*/,
                                       DnsSrvRecords const& /*records*/)
        {}

#if defined(SOFTPHONE_BALANCE)
        /** @brief Call rate information received or updated
          * @param call The call with appropriately filled rate transients
          * @param rate The call rate information, if known
          * @see @ref Transients::callRate */
        virtual void onCallRate(EventHistory::CallEvent::Pointer /*call*/,
                                ali::optional<Rate::Record::Call> const& /*rate*/)
        {}

#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Balance information received or changed
          * @param accountId ID of the account that changed balance
          * @param balance The new balance */
        virtual void onBalance(ali::string const& /*accountId*/,
                               Balance::Record const& /*balance*/)
        {}
#   else
        /** @brief Balance information received or changed
          * @param balance The new balance */
        virtual void onBalance(Balance::Record const& balance)
        {}
#   endif
#endif

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief DND activation changed
          * @param accountId ID of the account that changed status
          * @param active Whether DND is now active */
        virtual void onDndActiveChanged(ali::string const& /*accountId*/,
                                        bool /*active*/)
        {}
#else
        /** @brief DND activation changed
          * @param active Whether DND is now active */
        virtual void onDndActiveChanged(bool active)
        {}
#endif

        /** @brief Media status changed in a call
          * @param call The call that changed the media status
          * @param media The new media status */
        virtual void onMediaStatusChanged(EventHistory::CallEvent::Pointer /*call*/,
                                          Call::MediaStatus const& /*media*/)
        {}

#if defined(SOFTPHONE_VOICEMAIL)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief New voicemail arrived
          * @param accountId ID of the account the voicemail arrived to
          * @param voicemail The new voicemail */
        virtual void onVoicemail(ali::string const& /*accountId*/,
                                 Voicemail::Record const& /*voicemail*/)
        {}
#   else
        /** @brief New voicemail arrived
          * @param voicemail The new voicemail */
        virtual void onVoicemail(Voicemail::Record const& /*voicemail*/)
        {}
#   endif
#endif

#if defined(SOFTPHONE_CONFERENCES)
        /** @brief Transfer offered for a call
          * @param call The call the transfer was offered for
          * @see @ref Instance::Calls::getOfferedTransferInfo */
        virtual void onTransferOffered(EventHistory::CallEvent::Pointer /*call*/)
        {}

        /** @brief Transfer finished or failed
          * @param call The call the transfer finished or failed
          * @param success Whether the transfer finished or failed */
        virtual void onTransferResult(EventHistory::CallEvent::Pointer /*call*/,
                                      bool /*success*/)
        {}
#endif

        /** @brief Events changed
          * @param events The events that were changed
          * @param streams The event streams that were changed */
        virtual void onEventsChanged(EventHistory::ChangedEvents const& events,
                                     EventHistory::ChangedStreams const& streams) = 0;

#if defined(SOFTPHONE_SECURITY)
        /** @brief Security status changed on a call
          * @param call The call with the appropriate security status transients
          *             filled
          * @see @ref Transients::security */
        virtual void onSecurityStatusChanged(EventHistory::CallEvent::Pointer /*call*/)
        {}
#endif

        /** @brief Call repository changed
          *
          * Call repository holds all calls. A change in it means one or more
          * calls moved to/from a group, were transferred, new call(s) arrived
          * or call(s) were finished.
          * @see @ref onNewEvent gets called when a new call arrives */
        virtual void onCallRepositoryChanged()
        {}

        /** @brief Override priorities of network interfaces.
          *
          * @param network Snapshot of the network.
          *
          * The library decides which network interface by setting priorities
          * to detected interfaces. The priorities depend on the interface type
          * (Cellular vs. Wi-Fi), configuration (see networkUse preference key),
          * and a special boost is given to the interface which is used by the
          * signalling channel. This methods lets you override the priorities
          * or ban (remove) interfaces based on your own rules. The interface
          * with the highest priority will be picked. */
        virtual void overrideNetworkPriorities(ali::network::interface_snapshot & /*network*/)
        {}




#if defined(SOFTPHONE_PUSH)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief A push test was scheduled
          * @param accountId ID of the account the push test was scheduled on
          * @param result Result of the push test scheduling */
        virtual void onPushTestScheduled(ali::string const& /*accountId*/,
                                         PushTestScheduleResult::Type /*result*/)
        {}

        /** @brief A push test has arrived
          * @param accountId ID of the account the push test has arrived on */
        virtual void onPushTestArrived(ali::string const& /*accountId*/)
        {}
#   else
        /** @brief A push test was scheduled
          * @param result Result of the push test scheduling */
        virtual void onPushTestScheduled(PushTestScheduleResult::Type result)
        {}

        /** @brief A push test has arrived */
        virtual void onPushTestArrived()
        {}
#   endif

        /** @brief Information about a missed call received from SIPIS server */
        virtual void onSipisMissedCall(Softphone::EventHistory::CallEvent::Pointer const& /*call*/)
        {}
#endif

        /** @brief One or more contacts have been changed, added or removed. */
        virtual void onContactsChanged(ali::string const& /*source*/)
        {}

        /** @brief Called when the internal contact source state changes (for example when it
         scans the native address book, refreshes cardDav contacts etc.
         */
        virtual void onContactSourceStateChanged(ali::string const& /*source*/)
        {}

#if defined(SOFTPHONE_SMART_CONTACTS)
        /** @brief One or more contacts changed their smart status.
          *
          * @see @ref Instance::Contacts::Smart */
        virtual void onSmartContactsChanged()
        {}
#endif
        /** @brief Invoked when time-limited simulated microphone expires.   */
        virtual void onSimulatedMicrophoneStopped()
        {}

        /** @brief Invoked when there is a change in composing state for a stream party
         * @param composing info for a specific stream party
         *
         * SDK calls this method whenever composing state for a stream party changes.
         * Either because of a received isComposing message or when the auto-inactive timeout
         * expires.
         * To get the initial state @see @ref getActiveComposingInfos
         */

        virtual void onComposingInfo(Messaging::InboundComposingInfo const& /*info*/)
        {}

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        virtual void onDialogEvent(ali::string const& /*accountId*/)
        {}
#else
        virtual void onDialogEvent()
        {}
#endif
        virtual void onSettingsChanged()
        {}
    };

    /** @brief Instance of the Acrobits libsoftphone library */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    class Instance
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
    public:
        /** @brief Virtual destructor */
        virtual ~Instance()
        {}

        /** @brief Get current observer
          * @return The current observer
          * @retval nullptr No observer */
        virtual Observer * getObserver() const = 0;

        /** @brief Set an observer, replacing a previous one
          * @param observer The new observer */
        virtual void setObserver(Observer * observer) = 0;

        /** @brief Get current application interface
          * @return The current application interface
          * @retval nullptr No application interface */
        virtual AppInterface * getAppInterface() const = 0;

        /** @brief Set an application interface, replacing a previous one
          * @param appInterface The new application interface */
        virtual void setAppInterface(AppInterface * appInterface) = 0;

        struct Audio;
        struct Calls;
        struct Contacts;
        struct DNS;
        struct Events;
        struct GroupManagement;
        struct Log;
        struct DND;
        struct Network;
        struct Notifications;
        struct Playback;
        struct Recording;
        struct Registration;
#if defined(SOFTPHONE_SECURITY)
        struct Security;
#endif
        struct Settings;
        struct State;
        struct System;
        struct VoiceUnitMonitor;
#if defined(SOFTPHONE_VIDEO)
        struct Video;
#endif


        /** @brief Get methods for manipulating audio
          * @return The methods for manipulating audio */
        Audio * audio() const
        {
            return mAudio.get();
        }

        /** @brief Get methods for manipulating calls
          * @return The methods for manipulating calls*/
        Calls * calls() const
        {
            return mCalls.get();
        }

        /** @brief Get methods for contacts
          * @return The methods for contacts */
        Contacts * contacts() const
        {
            return mContacts.get();
        }

        /** @brief Get methods for querying the Domain Name System
          * @return The methods for querying the Domain Name System */
        DNS * dns() const
        {
            return mDns.get();
        }

        /** @brief Get methods manipulating event streams
          * @return The methods manipulating event streams */
        Events * events() const
        {
            return mEvents.get();
        }

        /** @brief Get methods manipulating group chat streams
         * @return The methods manipulating group chat streams */
        GroupManagement * groupManagement() const
        {
            return mGroupManagement.get();
        }

        /** @brief Get methods manipulating the traffic log
          * @return The methods manipulating the traffic log */
        Log * log() const
        {
            return mLog.get();
        }

        /** @brief Get methods for querying Do Not Disturb
         * @return The methods for querying Do Not Disturb */
        DND * dnd() const
        {
            return mDnd.get();
        }

        /** @brief Get network methods
          * @return The network methods */
        Network * network() const
        {
            return mNetwork.get();
        }

        /** @brief Get methods for notifications
          * @return The methods for notifications */
        Notifications * notifications() const
        {
            return mNotifications.get();
        }

        /** @brief Get methods for audio playback.
          * @return The methods for audio playback. */
        Playback * playback() const
        {
            return mPlayback.get();
        }

        /** @brief Get methods for audio recording.
          * @return The methods for audio recording. */
        Recording * recording() const
        {
            return mRecording.get();
        }

        /** @brief Get methods for manipulating registration of the accounts
          * @return The methods for manipulating registration of the accounts */
        Registration * registration() const
        {
            return mRegistration.get();
        }

#if defined(SOFTPHONE_SECURITY)
        /** @brief Get methods for call and message security
          * @return The methods for call and message security */
        Security * security() const
        {
            return mSecurity.get();
        }
#endif

#if defined(SOFTPHONE_ADDONS)
        /** @brief Get methods manipulating settings and add-ons database
          * @return The methods manipulating settings and add-ons database */
#else
        /** @brief Get methods manipulating settings database
          * @return The methods manipulating settings database */
#endif
        Settings * settings() const
        {
            return mSettings.get();
        }

        /** @brief Get state methods
          * @return The state methods */
        State * state() const
        {
            return mState.get();
        }

        /** @brief Get system-specific methods
          * @return The system-specific methods */
        System * system() const
        {
            return mSystem.get();
        }

        /** @brief Get voice unit monitor specific methods
          * @return The voice unit monitor specific methods */
        VoiceUnitMonitor * voiceUnitMonitor() const
        {
            return mVoiceUnitMonitor.get();
        }

#if defined(SOFTPHONE_VIDEO)
        /** @brief Get methods for video calls
          * @return The methods for video calls */
        Video * video() const
        {
            return mVideo.get();
        }
#endif

    protected:
        ali::auto_ptr<ali::handle> mContext; ///< The internal context of the Acrobits libsoftphone library
        ali::auto_ptr<Audio> mAudio; ///< Methods for manipulating audio
        ali::auto_ptr<Calls> mCalls; ///< Methods for manipulating calls
        ali::auto_ptr<Contacts> mContacts; ///< Methods for contacts
        ali::auto_ptr<DNS> mDns; ///< Methods for querying the Domain Name System
        ali::auto_ptr<Events> mEvents; ///< Methods manipulating event streams
        ali::auto_ptr<GroupManagement> mGroupManagement; ///< Methods manipulating group streams
        ali::auto_ptr<Log> mLog; ///< Methods manipulating the traffic log
        ali::auto_ptr<DND> mDnd; ///< Methods for quering Do Not Disturb
        ali::auto_ptr<Network> mNetwork; ///< Network methods
        ali::auto_ptr<Notifications> mNotifications; ///< Methods for notifications
        ali::auto_ptr<Playback> mPlayback; ///< Methods for audio playback
        ali::auto_ptr<Recording> mRecording; ///< Methods for audio recording
        ali::auto_ptr<Registration> mRegistration; ///< Methods for manipulating registration of the accounts
#if defined(SOFTPHONE_SECURITY)
        ali::auto_ptr<Security> mSecurity; ///< Methods for call and message security
#endif
#if defined(SOFTPHONE_ADDONS)
        ali::auto_ptr<Settings> mSettings; ///< Methods manipulating settings and add-ons database
#else
        ali::auto_ptr<Settings> mSettings; ///< Methods manipulating settings database
#endif
        ali::auto_ptr<State> mState; ///< State methods
        ali::auto_ptr<System> mSystem; ///< System-specific methods
        ali::auto_ptr<VoiceUnitMonitor> mVoiceUnitMonitor; ///< Voice unit monitor specific methods
#if defined(SOFTPHONE_VIDEO)
        ali::auto_ptr<Video> mVideo; ///< Methods for video calls
#endif
    };

    /** @brief Methods for manipulating audio
     *
     * These methods manipulate mute, audio routes and DTMF. */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Audio
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Audio()
        {}

        /** @brief Start generating DTMF tone for a digit
          * @param codeInAscii DTMF code in ASCII
          * @param appendDigit Append the digit for @ref Calls::getDtmfDigits
          *
          * Starts generating a DTMF tone, using the method described in
          * @ref AccountXML_dtmfOrder "account XML". The tone is generated until
          * @ref dtmfOff is called.
          *
          * In case no call is active, the method generates only a local beep.
          *
          * To stop the DTMF sound, use @ref dtmfOff. If @p appendDigit is
          * @c true, the digit is appended to the call's DTMF buffer returned by
          * @ref Calls::getDtmfDigits. If there is no call, @p appendDigit is
          * ignored and the digit is not stored. */
        virtual void dtmfOn(char codeInAscii,
                            bool appendDigit = true) = 0;

        /** @brief Start generating DTMF tone for a digit locally
          * @param codeInAscii DTMF code in ASCII
          *
          * Starts generating a DTMF tone locally. The tone is generated until
          * @ref dtmfOff is called.
          *
          * This method generates only a local beep.
          *
          * To stop the DTMF sound, use @ref dtmfOff.  */
        virtual void dtmfLocal(char codeInAscii) = 0;

        /** @brief Stop generating DTMF tone for a digit
          *
          * Stops generating the DTMF tone started with @ref dtmfOn. */
        virtual void dtmfOff() = 0;

        /** @brief Mute or unmute a microphone
          * @param microphone The microphone whose output should be muted
          * @param muted Whether the microphone should be muted */
        virtual void setMuted(Microphone::Type microphone, bool muted) = 0;

        /** @brief Check if the microphone is muted
          * @param microphone The microphone whose mute state to check
          * @retval true The microphone is muted
          * @retval false The microphone is not muted */
        virtual bool isMuted(Microphone::Type microphone) const = 0;

        /**
         Helper for mute functions for the most common phase - physical microphone
         */
        bool isMuted() const
        {
            return isMuted(Microphone::Physical);
        }

        void setMuted(bool muted)
        {
            setMuted(Microphone::Physical,muted);
        }

//  PK: Conflict during master -> merge_win_mac merge
#ifdef  ACROBITS_DESKTOP_APP

        /** @brief Set the audio device
         * @param device The audio device identifier to set
         * @param type Specify input or output device
          *
          * The observer is @ref Observer::onAudioDeviceChanged "notified about changes in the audio devices".
          *
          * @see @ref setObserver for setting the observer. */
        virtual void setAudioDevice(ali::string const& device, Softphone::AudioDeviceType type) = 0;

        /** @brief Get the input audio device
         * @param type Specify input or output device
          * @return The input audio record */
        virtual AudioDevice const& getAudioDevice(Softphone::AudioDeviceType type) const = 0;

        /** @brief Get available call audio devices
          * @return Set of available call audio routes
          *
          * This method returns only the readily-available audio devices,
          * e.g. Bluetooth devices will only  be present
          * when a Bluetooth hands-free is connected.
          *
          * This list is usually offered to the user in the audio input/output device
          * selection dialog. */
        virtual Softphone::AudioDeviceSnapshot getAvailableAudioDevices() const = 0;

#else   //  !ACROBITS_DESKTOP_APP

        /** @brief Set the call audio route
          * @param route The call audio route
          *
          * The observer is @ref Observer::onAudioRouteChanged "notified about changes in the audio routes".
          *
          * @see @ref setObserver for setting the observer. */
        virtual void setCallAudioRoute(AudioRoute::Type route) = 0;

        /** @brief Get the call audio route
          * @return The call audio route */
        virtual AudioRoute::Type getCallAudioRoute() const = 0;

        /** @brief Get the currently used audio route
          * @return The currently used audio route */
        virtual AudioRoute::Type getCurrentHardwareAudioRoute() const = 0;

        /** @brief Get available call audio routes
          * @return Set of available call audio routes
          *
          * This method returns only the readily-available audio routes,
          * e.g. @ref AudioRoute::BluetoothSCO "BluetoothSCO" will be present
          * only when a Bluetooth hands-free is connected.
          *
          * This list is usually offered to the user in the audio route
          * selection dialog. */
        virtual ali::array_set<AudioRoute::Type> getAvailableCallAudioRoutes() const = 0;

#endif  //  ACROBITS_DESKTOP_APP

        /** @brief Check if the device is muted
          * @retval true The device is muted
          * @retval false The device is not muted
          *
          * Checks if the device/machine as a whole is muted (or on vibrations). When
          * the device is muted, an incoming call should not ring. */
        virtual bool isDeviceMuted() const = 0;
        
//  PK: Conflict during master -> merge_win_mac merge
#ifdef  ACROBITS_DESKTOP_APP
#else   //  !ACROBITS_DESKTOP_APP

#if defined(__APPLE__)
        virtual void setExternalAudioPlaybackActive(bool active) = 0;
#endif

#endif  //  ACROBITS_DESKTOP_APP
    };

    /** @brief Methods for manipulating calls */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Calls
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        typedef ali::callback<void(bool)> CompletionCallback;

#if defined(SOFTPHONE_CONFERENCES)
        /** @brief Methods for manipulating call conferences */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Conferences
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            /** @brief Virtual destructor */
            virtual ~Conferences()
            {}

            /** @brief Get count of conferences
              * @return Count of conferences */
            virtual int count() const = 0;

            /** @brief Get identifier of conference a call is part of
              * @param call The call to search for
              * @return The identifier of the conference the call is part of */
            virtual ali::string get(EventHistory::CallEvent::Pointer const& call) const = 0;

            /** @brief Generate a conference identifier for a stream
              * @param key Key of the stream to generate the conference
              *            identifier for
              * @return The generated identifier
              *
              * The identifier is generated but the conference is not created.
              * It is automatically created when you call @ref move with this
              * identifier. */
            virtual ali::string generate(ali::string const& key) const = 0;

            /** @brief Generate a conference identifier for a stream
              * @param stream The stream to generate the conference identifier for
              * @return The generated identifier
              *
              * The identifier is generated but the conference is not created.
              * It is automatically created when you call @ref move with this
              * identifier. */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::string generate(EventHistory::EventStream::Pointer const& stream) const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return generate(stream->key);
            }

            /** @brief Get calls in a conference
              * @param conference ID of the conference to get the calls in
              * @return Set of the calls in the specified conference */
            virtual ali::array_set<EventHistory::CallEvent::Pointer> getCalls(ali::string const& conference) const = 0;

            /** @brief Get calls in a conference
              * @param call A call that is part of the conference
              * @return Set of the calls in the specified conference */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::array_set<EventHistory::CallEvent::Pointer> getCalls(EventHistory::CallEvent::Pointer const& call) const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getCalls(get(call));
            }

            /** @brief List all conferences
              * @return Set of all conferences */
            virtual ali::array_set<ali::string> list() const = 0;

            /** @brief List all conferences of a stream
              * @param key Key of the stream to list all conferences of
              * @return Set of all conferences of the specified stream */
            virtual ali::array_set<ali::string> list(ali::string const& key) const = 0;

            /** @brief List all conferences of a stream
              * @param stream The stream to list all conferences of
              * @return Set of all conferences of the specified stream */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            ali::array_set<ali::string> list(EventHistory::EventStream::Pointer const& stream) const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return list(stream->key);
            }

            /** @brief Get size of a conference
              * @param conference ID of the conference
              * @return The size of the conference */
            virtual int getSize(ali::string const& conference) const = 0;

            /** @brief Get size of a conference
              * @param call A call that is part of the conference
              * @return The size of the conference */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            int getSize(EventHistory::CallEvent::Pointer const& call) const
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return getSize(get(call));
            }

            /** @brief Get active conference
              * @return ID of the active conference;
              * @retval nullptr No active conference */
            virtual ali::opt_string getActive() const = 0;

            /** @brief Set active conference
              * @param conference ID of the conference to set active; no
              *                   conference will be active if @c nullptr
              * @retval true Specified conference set active; always if
              *              @p conference is @c nullptr
              * @retval false Specified conference does not exist */
            virtual bool setActive(ali::opt_string const& conference) = 0;

            /** @brief Set active conference
              * @param call A call that is part of the conference to set active;
              *             no conference will be active if @c nullptr
              * @retval true Specified conference set active; always if
              *              @p call is @c nullptr
              * @retval false Specified conference does not exist */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            bool setActive(EventHistory::CallEvent::Pointer const& call)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return setActive(call.is_null() ? ali::opt_string() : get(call));
            }

            /** @brief Move a call to a conference
              * @param call The call to move
              * @param conference ID of the conference to move the call to
              * @retval true The specified call was successfully moved to the
              *              specified conference
              * @retval false Cannot move; the call was moved to its own group
              *
              * A call cannot move to a default conference of another call as
              * that one is generated in the call's stream. Use @ref generate
              * to create a new conference identifier.
              *
              * If the target conference does not exist, it is created.
              *
              * If the moved call was the last call in its original conference,
              * that conference is destroyed. */
            virtual bool move(EventHistory::CallEvent::Pointer const& call,
                              ali::string const& conference) = 0;

            /** @brief Move a call to a conference
              * @param call The call to move
              * @param target A call that is part of the conference to move
              *               @p call to
              * @retval true The specified call was successfully moved to the
              *              specified conference
              * @retval false Cannot move; the call was moved to its own group
              *
              * A call cannot move to a default conference of another call as
              * that one is generated in the call's stream. Use @ref generate
              * to create a new conference identifier.
              *
              * If the moved call was the last call in its original conference,
              * that conference is destroyed.
              */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            bool move(EventHistory::CallEvent::Pointer const& call,
                      EventHistory::CallEvent::Pointer const& target)
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                return move(call, get(target));
            }

            /** @brief Split a call from its conference
              * @param call The call to split
              * @param activate Set to true to automatically activate the split call
              *
              * This is equivalent to move the call to conference with identifier
              * generated with @ref generate(@ref EventHistory::Event::getStreamKey
              * "call->getStreamKey()")
              *
              * If the activate is set to false, the original group is kept active and the
              * split call is put on hold
              */
            virtual void split(EventHistory::CallEvent::Pointer const& call,
                               bool activate) = 0;

            virtual void beginTransfer(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual bool isTransferring(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual void finishTransfer(EventHistory::CallEvent::Pointer const& call,
                                        EventHistory::CallEvent::Pointer target) = 0;
            virtual void cancelTransfer(EventHistory::CallEvent::Pointer const& call) = 0;

            virtual void beginForward(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual bool isForwarding(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual void finishForward(EventHistory::CallEvent::Pointer const& call,
                                       ali::generic_peer_address const& peer) = 0;
            virtual void cancelForward(EventHistory::CallEvent::Pointer const& call) = 0;

            virtual bool attendedTransfer(EventHistory::CallEvent::Pointer const& call,
                                          EventHistory::CallEvent::Pointer const& targetCall) = 0;

            virtual bool rejectOfferedTransfer(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual EventHistory::CallEvent::Pointer acceptOfferedTransfer(EventHistory::CallEvent::Pointer const& call,
                                                      Call::DesiredMedia const& media) = 0;
            virtual bool isTransferOffered(EventHistory::CallEvent::Pointer const& call) = 0;
            virtual EventHistory::CallEvent::Pointer getOfferedTransfer(EventHistory::CallEvent::Pointer const& callEvent) = 0;
        };
#endif

        /** @brief Virtual destructor */
        virtual ~Calls()
        {}

        /** @brief Check if a call is alive.
          *
          * Calls which are closed, or calls which have not been dialed by
          * calling @ref Events.post will return false.
          *
          * Calling methods of this interface with call events which are not
          * alive will assert in debug mode. In release builds, the library
          * should handle these cases gracefully.
          *
          * @param call The call to check.
          * @return Whether the call is currently alive. */
        virtual bool isAlive(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get active call(s).
          *
          * If feature Conferences is provided, this is identical to calling
          * @ref Conferences.getCalls(@ref Conferences.getActive).
          * However, this is the only way how to get the currently active
          * call without that feature.
          *
          * @return Set of active calls. Empty if none. Only one call can be
          *         returned without feature Conferences. */
        virtual ali::array_set<EventHistory::CallEvent::Pointer> getActive() const = 0;

        /** @brief Get state of a call
          * @param call The call to get the state for
          * @return Call state of the specified call */
        virtual Call::State::Type getState(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Answer an incoming call
          * @param call The call to answer
          * @param media Requested media for the call
          * @retval true Call answered
          * @retval false Answering the call failed and the call is terminated;
          *               or the call is already established or terminated; or
          *               @p call contains @c nullptr or an invalid call
          *
          * This method is asynchronous. The observer will be
          * @ref Observer::onCallStateChanged "notified when the call is actually answered".
          *
          * On SIP, this sends “200 OK”. On XMPP, this sends “session-accept”.
          *
          * @see @ref setObserver for setting the observer. */
        virtual bool answerIncoming(EventHistory::CallEvent::Pointer const& call,
                                    Call::DesiredMedia const& media) = 0;

        /** @brief Reject (hang-up) an incoming call on this particular device only
          * @param call The call to reject
          * @retval true Call rejected
          * @retval false The call is already established or terminated; or
          *               @p call contains @c nullptr or an invalid call
          *
          * The observer will be @ref Observer::onCallStateChanged
          * "notified about the call state change".
          *
          * On SIP, the call is rejected with “486 Busy Here”. On XMPP, it
          * is rejected with “busy”.
          *
          * After calling this method, the call should be @ref close "closed".
          *
          * @see @ref setObserver for setting the observer. */
        virtual bool rejectIncomingHere(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Reject/decline an incoming call everywhere.
         * @param call The call to reject
         * @retval true Call rejected
         * @retval false The call is already established or terminated; or
         *               @p call contains @c nullptr or an invalid call
         *
         * The observer will be @ref Observer::onCallStateChanged
         * "notified about the call state change".
         *
         * On SIP, the call is rejected with “603 Declined”. On XMPP, it
         * is rejected with “busy”.
         *
         * After calling this method, the call should be @ref close "closed". If
         * the call was ringing at other places (forking), the PBX should send
         * CANCEL to these endpoints.
         * @see @ref setObserver for setting the observer. */
        virtual bool rejectIncomingEverywhere(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Reject (hang-up) an incoming SIP call and specify the reason
          * @param call The call to reject
          * @param responseCode SIP Response code
          * @param phrase SIP phrase to use after the response code
          * @param warning A warning message to be included in SIP response. Can be empty.
          * @retval true Call rejected
          * @retval false The call is already established or terminated; or
          *               @p call contains @c nullptr or an invalid call
          *
          * The observer will be @ref Observer::onCallStateChanged
          * "notified about the call state change".
          *
          * After calling this method, the call should be @ref close "closed".
          *
          * @see @ref setObserver for setting the observer. */
        virtual bool rejectIncoming(EventHistory::CallEvent::Pointer const& call, ali::string const& responseCode, ali::string const& phrase, ali::string const& warning) = 0;

        bool rejectIncoming(EventHistory::CallEvent::Pointer const& call, ali::string const& responseCode, ali::string const& phrase)
        {
            return rejectIncoming(call, responseCode, phrase, ali::string{});
        }

        /** @brief Ignore an incoming call
          * @param call The call to ignore
          * @retval true Call ignored
          * @retval false The call is already established, ignored or
          *               terminated; or @p call contains @c nullptr or an
          *               invalid call
          *
          * Silences the ringing and stops vibrations for the incoming call.
          * Also prevents call waiting tone and disables auto-answer
          *
          * The call is kept ringing (on SIP in “180 Ringing”, on XMPP in
          * “ringing”) and still can be @ref answerIncoming "answered"
          * or @ref rejectIncoming "rejected". */
        virtual bool ignoreIncoming(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Hold or unhold a call
          * @param call The call to hold or unhold
          * @param held Whether the call should be held
          *
          * The observer will be @ref Observer::onCallHoldStateChanged
          * "notified that the call hold state changed".
          *
          * @see @ref setObserver for setting the observer. */
        virtual void setHeld(EventHistory::CallEvent::Pointer const& call,
                             bool held) = 0;

        /** @brief Check if a call is held
          * @param call The call to check
          * @return The hold states of the specified call */
        virtual Call::HoldStates isHeld(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Set simulated microphone
          * @param sound Sound to play; should be seekable. @c nullptr to stop.
          * @param playOnce Whether to play only once.
          *
          * If @p sound is @c nullptr, reading the real microphone resumes and
          * the played sound is stopped.
          *
          * If @p sound is not @c nullptr, the microphone signal is replaced by
          * the sound from the provided deserializer and the microphone is
          * no longer read. The sound is played in an infinite loop unless
          * @p playOnce is set to @c true. This is used e.g. when the call is
          * interrupted by a GSM call.
          *
          * If @p playOnce is @c true, the Observer
          * @ref Observer.onSimulatedMicrophoneStopped "will be notified" when
          * the playback is done.
          *
          * If there is already a simulated microphone, it is stopped before
          * the new is started. The @ref Observer.onSimulatedMicrophoneStopped
          * callback is not triggered in that case. */
        virtual void setSimulatedMicrophone(ali::auto_ptr<ali::deserializer> sound,
                                            bool playOnce) = 0;

        /** @brief Set simulated microphone
          * @param sound Sound to play; should be seekable. @c nullptr to stop.
          *
          * If @p sound is @c nullptr, reading the real microphone resumes and
          * the played sound is stopped.
          *
          * If @p sound is not @c nullptr, the microphone signal is replaced by
          * the sound from the provided deserializer and the microphone is
          * no longer read. The sound is played in an infinite loop if @p sound
          * is seekable, only once otherwise.
          *
          * If there is already a simulated microphone, it is stopped before
          * the new is started. The @ref Observer.onSimulatedMicrophoneStopped
          * callback is not triggered in that case. */
        void setSimulatedMicrophone(ali::auto_ptr<ali::deserializer> sound)
        {
            bool const playOnce = !sound.is_null();
            setSimulatedMicrophone(ali::move(sound), playOnce);
        }

        /** @brief Set simulated microphone from a file
          * @param file File with sound to play; empty to stop
          * @param playOnce Whether to play only once
          *
          * If @p file is empty, reading the real microphone resumes
          * and the played sound is stopped.
          *
          * If @p file is not empty, the microphone signal is replaced by sound
          * from the provided resource and the microphone is no longer read.
          * The sound is played in an infinite loop unless @p playOnce is set
          * to @c true. This is used e.g. when the call is interrupted by
          * a GSM call.
          *
          * If @p playOnce is @c true, the Observer @ref Observer.onSimulatedMicrophoneStopped
          * "will be notified" when the playback is done.
          *
          * If there is already a simulated microphone, it is stopped before
          * the new is started. The @ref Observer.onSimulatedMicrophoneStopped
          * callback is not triggered in that case. */
        void setSimulatedMicrophone(ali::filesystem2::path const& file,
                                    bool playOnce = false);

        /** @brief Check whether a simulated microphone is running
          * @retval true The microphone is simulated
          * @retval false The microphone is real */
        virtual bool isSimulatedMicrophone() const = 0;

        /** @brief Hangup a call
          * @param call The call to hangup
          * @param reason The hangup reason
          * @retval true Successfully closed
          * @retval false Invalid call or already terminated
          *
          * This method is asynchronous. The observer will be
          * @ref Observer::onCallStateChanged "notified that the call state changes". */
        virtual bool hangup(EventHistory::CallEvent::Pointer const& call, ali::string_const_ref reason = {}) = 0;

        /** @brief Close a call
          * @param call The call to close
          *
          * If the call is not yet terminated, closing it terminates it.
          *
          * Do not forget to close your calls, they use system resources and
          * are not closed automatically. The most convenient way to close them
          * is from @ref Observer::onCallStateChanged when the call state
          * @ref Call::State::isTerminal "is terminal".
          *
          * The call event associated with the call is retained. */
        virtual void close(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Get call statistics
          * @param call The call to get the statistics for
          * @return The call statistics */
        virtual Call::Statistics getStatistics(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get media status
         * @param call The call to get media information for (e.g. audio video codec types...)
         * @return The call media status */
        virtual Call::MediaStatus getMediaStatus(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get the default incoming audio energy update period
         * @param call The call to get the update period for
         * @return The default incoming audio energy update period in seconds */
        virtual double getDefaultIncomingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get the current incoming audio energy update period
         * @param call The call to get the update period for
         * @return The current incoming audio energy update period in seconds */
        virtual double getIncomingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Set the current incoming audio energy update period
         * @param call The call to set the update period for
         * @param updatePeriodInSeconds The new update period in seconds */
        virtual void setIncomingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call, double updatePeriodInSeconds) = 0;

        /** @brief Calculate at what intervals should the @ref getIncomingAudioEnergy
         * method be called for the specified incoming audio energy update period
         * @param call The call to calculate the sampling interval for
         * @param updatePeriodInSeconds The incoming audio energy update period
         * @return The recommended incoming audio energy sampling period in milliseconds */
        virtual int getRecommendedIncomingAudioEnergySamplingPeriodInMilliseconds(
            EventHistory::CallEvent::Pointer const& call, double updatePeriodInSeconds) const = 0;

        /** @brief Get incoming audio signal energy
         * @param call The call to get the incoming audio signal energy for
         * @return The actual incoming audio signal energy
         *
         * The return value is normalized into 0.0 - 1.0 range, and updated at regular intervals.
         *
         * @see @ref setIncomingAudioEnergyUpdatePeriod for changing the energy update period. */
        virtual float getIncomingAudioEnergy(
            EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get the default outgoing audio energy update period
         * @param call The call to get the update period for
         * @return The default outgoing audio energy update period in seconds */
        virtual double getDefaultOutgoingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get the current outgoing audio energy update period
         * @param call The call to get the update period for
         * @return The current outgoing audio energy update period in seconds */
        virtual double getOutgoingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Set the current outgoing audio energy update period
         * @param call The call to set the update period for
         * @param updatePeriodInSeconds The new update period in seconds */
        virtual void setOutgoingAudioEnergyUpdatePeriodInSeconds(
            EventHistory::CallEvent::Pointer const& call, double updatePeriodInSeconds) = 0;

        /** @brief Calculate at what intervals should the @ref getOutgoingAudioEnergy
         * method be called for the specified outgoing audio energy update period
         * @param call The call to calculate the sampling interval for
         * @param updatePeriodInSeconds The outgoing audio energy update period
         * @return The recommended outgoing audio energy sampling period in milliseconds */
        virtual int getRecommendedOutgoingAudioEnergySamplingPeriodInMilliseconds(
            EventHistory::CallEvent::Pointer const& call, double updatePeriodInSeconds) const = 0;

        /** @brief Get outgoing audio signal energy
         * @param call The call to get the outgoing audio signal energy for
         * @return The actual outgoing audio signal energy
         *
         * The return value is normalized into 0.0 - 1.0 range, and updated at regular intervals.
         *
         * @see @ref setOutgoingAudioEnergyUpdatePeriod for changing the energy update period. */
        virtual float getOutgoingAudioEnergy(
            EventHistory::CallEvent::Pointer const& call) const = 0;

#if defined(SOFTPHONE_RECORD)
        /** @brief Set call recording
          * @param call The call to set recording for
          * @param recording Whether the call recording should run
          * @retval true Recording state changed successfully
          * @retval false Changing the recording state failed */
#   if defined(SOFTPHONE_CONFERENCES)
        /** @note This starts recording the whole conference the call is part
          *       of. */
#   endif
        virtual bool setRecording(EventHistory::CallEvent::Pointer const& call,
                                  bool recording) = 0;

        /** @brief Check if a call is recording
          * @param call The call to check
          * @return The record state */
        virtual Call::RecordingStatus isRecording(EventHistory::CallEvent::Pointer const& call) = 0;
#endif

        /** @brief Find a SIP header in a call
          * @param call The call to search for the SIP header in
          * @param headerName Name of the header to search for
          * @retval nullptr No such header was received
          * @retval <HeaderName/> The header was empty
          *
          * The libsoftphone library maintains internally a map of all SIP
          * headers present in the call establishing dialog (incoming @c INVITE
          * messages and responses). In case the header already exists and
          * a new packet specifies new value for the header, the value in the
          * map is overwritten.
          *
          * The headers are stored in an internal XML format created when the
          * headers are parsed. Simple and non-standard headers usually have
          * a format of <tt>&lt;HeaderName>value&lt;/HeaderName></tt>, complex
          * headers like @c %Contact or @c Via can be stored as a non-flat XML
          * tree. */
        virtual ali::xml::tree const* findSipHeader(EventHistory::CallEvent::Pointer const& call,
                                                    ali::string_const_ref headerName) const = 0;

        /** @brief Return the status line from the response to @c INVITE
          * @param call The call to get the status line for
          * @return The exact status line from the SIP response to the
          *          @c INVITE request. */
        virtual ResponseStatusLine getInviteResponseStatusLine(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Get DTMF digits sent in a call
          * @param call The call to get the DTMF digits from
          * @return The DTMF digits
          * @retval "" No DTMS digits sent in the specified call or invalid call
          *
          * Useful if you want to display this information to user.
          * @see @ref Audio::dtmfOn  */
        virtual ali::string getDtmfDigits(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Clear the DTMF digits buffer of a call
          * @param call The call to clear the DTMF digits buffer
          *
          * This method clears the buffer filled by @ref Audio::dtmfOn and
          * returned by @ref getDtmfDigits. */
        virtual void clearDtmfDigits(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Set desired media for a call
          * @param call The call to set the desired media for
          * @param media The new desired media
          * @retval true Successfully changed
          * @retval false The account or the other side does not support the
          *               specified media or the call is invalid */
        virtual bool setDesiredMedia(EventHistory::CallEvent::Pointer const& call,
                                     Call::DesiredMedia const& media) = 0;

        /** @brief Get desired media of a call
          * @param call The call to get the desired media for
          * @return The desired media for the specified call */
        virtual Call::DesiredMedia getDesiredMedia(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Forces a new offer/answer negotiation for a call as soon as possible.
          * @param call The call */
        virtual void forceOurOffer(EventHistory::CallEvent::Pointer const& call) = 0;

        /** @brief Get count of calls not in terminal state
          * @return Number of calls not in terminal state */
        virtual int getNonTerminalCount() const = 0;

        /** @brief To check if a call is waiting for dtmf confirmation
         *  @return true if call is waiting for dtmf confirmation, false otherwise */
        virtual bool isCallWaitingForDtmfConfirmation(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Execute & clear dtmf */
        virtual void executeDtmfAndClear(EventHistory::CallEvent::Pointer const& call, bool clear) const = 0;

        /** @brief Get waiting DTMF to dial */
        virtual ali::string getDtmfToDial(EventHistory::CallEvent::Pointer const& call) const = 0;

#if defined(SOFTPHONE_VIDEO)
        /** @brief Check availability of a video stream
          * @param call The call to check the video stream availability for
          * @return Stream availability information */
        virtual StreamAvailability isVideoAvailable(EventHistory::CallEvent::Pointer const& call) const = 0;

        /** @brief Check if a call offers supported incoming video stream
          * @param call The call to check
          * @retval true The call offers incoming video stream that is supported
          *              with the current network type
          * @retval false The call offers no incoming video stream, or the
          *               offered video stream is not supported with the current
          *               network type
          * @see @ref System::getCurrentNetworkType */
        virtual bool offersIncomingVideo(EventHistory::CallEvent::Pointer const& call) const = 0;
#endif

#if defined(SOFTPHONE_CONFERENCES)
        /** @brief Get methods for call conferences
          * @return The methods for call conferences */
        Conferences * conferences() const
        {
            return mConferences.get();
        }

    protected:
        ali::auto_ptr<Conferences> mConferences;
#endif
    };

    /** @brief Methods for contacts */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Contacts
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Contacts()
        {}

#if defined(SOFTPHONE_NUMBER_REWRITING)
        /** @brief Methods for number rewriting
          *
          * These methods can perform number rewriting (like prepending country
          * code) and address book matching */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct NumberRewriting
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Result of rewriting */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            struct Result
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {
                ali::string number; ///< Rewritten (or original) number
                bool                        forceDialOut{false};
                bool                        showDialOutAlert{false};
                bool                        shouldAutoRecord{false};
                DialAction::Id              dialAction;
            };

            /** @brief Virtual destructor */
            virtual ~NumberRewriting()
            {}

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
            /** @brief Get rewriter rules
              * @param accountId Account ID; @c nullptr for main rewriter
              * @return The rewriter rules */
            virtual ali::xml::tree const& getRewriterRules(ali::opt_string const& accountId) const = 0;

            /** @brief Set rewriter rules
              * @param accountId Account ID; @c nullptr for main rewriter
              * @param rules The rewriter rules
              * @retval true Successfully set
              * @retval false Failed to set */
            virtual bool setRewriterRules(ali::opt_string const& accountId,
                                          ali::xml::tree const& rules) = 0;

            /** @brief Rewrite a number
              * @param accountId Account ID; @c nullptr for main rewriter
              * @param number Number to rewrite
              * @return The result of rewriting */
            virtual Result rewrite(ali::opt_string const& accountId,
                                   ali::string const& number) const = 0;
#else
            /** @brief Get rewriter rules
              * @return The rewriter rules */
            virtual ali::xml::tree const& getRewriterRules() const = 0;

            /** @brief Set rewriter rules
              * @param rules The rewriter rules
              * @retval true Successfully set
              * @retval false Failed to set */
            virtual bool setRewriterRules(ali::xml::tree const& rules) = 0;

            /** @brief Rewrite a number
              * @param number Number to rewrite
              * @return The result of rewriting */
            virtual Result rewrite(ali::string const& number) const = 0;
#endif
	    virtual ali::xml::tree const& getIncomingCallRewritingRules() const = 0;
	    virtual bool setIncomingCallRewritingRules(ali::xml::tree const& rules) = 0;
	    virtual ali::array<ali::string> matchIncomingCallRewriting(ali::string_const_ref number) const = 0;

        };
#endif

#if defined(SOFTPHONE_SMART_CONTACTS)
        /** @brief Methods for smart contacts. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Smart
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            /** @brief Virtual destructor */
            virtual ~Smart() {}

            /** @brief Find "smart" status for a number.
              * @param number Number in @ref canonize "canonical format". It should
              *               be number from one of the contacts in contact storage.
              * @return Dictionary with info about number, or @c nullptr if not smart. */
            virtual ali::json::dict const* find(ali::generic_peer_address const& uri) = 0;

            /** @brief Returns number in canonical format which corresponds to a SIP URI.
              * @param uri SIP URI to check.
              * @return Number in canonical format, or empty string if no smart number
              *         for the provided URI. */
            virtual ali::generic_peer_address canonize(ali::generic_peer_address const& uri) = 0;

            /** @brief Check whether smart contacts are enabled.
              * @return Whether smart contacts are enabled. */
            virtual bool isEnabled() = 0;

            /** @brief Get set of all smart contact IDs.
              * @return Set of all contact IDs which have at least one smart number. */
            virtual ali::array_set<Softphone::EventHistory::ContactId> const& list() = 0;

            /** @brief Sync smart contacts information from the contacts server.
             *
             * This is useful if the app somehow finds out (i.e. via push
             * message) that some of the contacts became smart. */
            virtual void sync() = 0;

            /** @brief Invite people to use the same app
             *
             * The phone numbers will be passed to server and marked as invited by current
             * user. The response callback will receive the numbers with information with
             * sucess/failure state. Invite may fail if the number has already been invited
             * by someone else. The app can then offer to send invitation SMS or email to
             * successfully invited users. */
            virtual ali::auto_ptr<ali::handle> invite(ali::array<ali::string> const& phoneNumbers,
                                                      ali::string_const_ref message,
                                                      Softphone::Invite::ResultCallback cb) = 0;
        };
#endif

        /** @brief Get the array of all contact sources which are currently configured.
          * @return array of source identifiers. */
        virtual ali::array<ali::string> const& getSources() const = 0;

        /** @brief Get number of contacts in the given source.
         * @return The number of contacts. */
        virtual int getCount(ali::string_const_ref source) const = 0;

        /** @brief Get number of contacts in the given contact source.
         * @param source the contact source to query
         * @param filter Callback function which will receive contactId and
         *               display name and return whether to include that
         *               contact in the enumeration.
         * @return The number of contacts. */
        virtual int getCount(ali::string_const_ref source,
                             Softphone::Contacts::QueryFilter filter) const = 0;

        /** Get contact from the given contact source.
          * @param source the contact source to query
          * @param index Index of the contact;
          *              0 ≤ @p index < @ref getCount.
          * Sequence generated by
          * @return The contact. */
        virtual ali::json::dict const& get(ali::string_const_ref source,
                                           int index) const = 0;

        /** Get contact from the given contact source.
          * @param source the contact source to query
          * @param id Contact ID.
         * @return The contact; @c nullptr if not found. */
        virtual ali::json::dict const* get(ali::string_const_ref source,
                                           ali::string_const_ref id) const = 0;

        /** Get contact by contactId
         * @param contactId the contact source and id
         * @return The contact; @c nullptr if not found. */
        virtual ali::json::dict const* get(Softphone::EventHistory::ContactId const& contactId) const
        {
            return get(contactId.source, contactId.id);
        }

        /** @brief Get path to contact avatar from the given contact source
          * @param source the contact source to query
          * @param avatar Avatar identifier. Can be obtain from contact JSON
          *               under key @ref Softphone::Contacts::Keyword::AVATAR.
          * @return Full path to the avatar image, or empty path if it doesn't exist. */
        virtual ali::filesystem2::path getAvatarPath(ali::string_const_ref source,
                                                     ali::string_const_ref avatar) const = 0;

        /** @brief Get checksum of all contacts in storage for the given contact source
          * @param source the contact source to query
          * @return Checksum of all contacts in storage.
          *
          * Any change in contacts should also change this checksum. */
        virtual ali::string getStorageChecksum(ali::string_const_ref source) const = 0;

        /** @brief Return how many bits are used to identify contact blocks in the given contact source
          * @param source the contact source to query
          * @return Number of bits used to identify contact blocks.
          *
          * When n bits are used, the internal storage has contacts divided into
          * 2ⁿ blocks. Blocks can be accessed by @ref copyBlock. */
        virtual int getBlockBitCount(ali::string_const_ref source) const = 0;

        /** @brief Get a copy of full block of contacts from the given contact source
          * @param source the contact source to query
          * @param bid Block ID to copy.
          * @return Block containing contacts.
          *
          * @note This method is thread-safe.*/
        virtual ali::auto_ptr<Softphone::Contacts::ContactBlock> copyBlock(ali::string_const_ref source,
                                                                           Softphone::Contacts::ContactBlockId const& bid) const = 0;

        /** @brief Enumerate contacts from the given contact source without any specific order
          * @param source the contact source to query
          * @param from Index of first contact to load.
          * @param limit Maximal number of contacts to load. @c -1 for no limit.
          * @return Contacts enumerator.
          *
          * Enumerating unordered is faster than alphabetical order and should
          * be preferred in case order is not important. */
        virtual Softphone::Contacts::Enumerator enumerateUnordered(ali::string_const_ref source,
                                                                   int from = 0,
                                                                   int limit = -1) const = 0;

        /** @brief Enumerate contacts from the given contact source in alphabetical order.
          * @param source the contact source to query
          * @param from Index of first contact to load.
          * @param limit Maximal number of contacts to load. @c -1 for no limit.
          * @return Contacts enumerator.
          *
          * Enumerating contacts by @p from … @p limit blocks is more efficient
          * than calling @ref get repeatedly. */
        virtual Softphone::Contacts::Enumerator enumerateAlphabetically(ali::string_const_ref source,
                                                                        int from = 0,
                                                                        int limit = -1) const = 0;

        /** @brief Enumerate contacts from the given contact source in alphabetical order with a filter.
          * @param source the contact source to query
          * @param filter Callback function which will receive contactId and
          *               display name and return whether to include that
          *               contact in the enumeration.
          * @param from Index of first contact to load.
          * @param limit Maximal number of contacts to load. @c -1 for no limit.
          * @return Contacts enumerator.
          *
          * Enumerating contacts by @p from … @p limit blocks is more efficient
          * than calling @ref get repeatedly. */
        virtual Softphone::Contacts::Enumerator enumerateAlphabetically(ali::string_const_ref source,
                                                                        Softphone::Contacts::QueryFilter filter,
                                                                        int from = 0,
                                                                        int limit = -1) const = 0;

        /** @brief Get sections for full alphabetical sequence of contacts in the given contact source
          * @param source the contact source to query
          * @return Array of section titles (based on current locale) and index
          *         of the first contact which belongs to this section. */
        virtual ali::array<Softphone::Contacts::Section> getSections(ali::string_const_ref source) const = 0;

        /** @brief Get sections for full alphabetical sequence of contacts from the given contact source
          * @param source the contact source to query
          * @param filter Callback function which will receive contactId and
          *               display name and return whether to include that
          *               contact in the enumeration.
          * @return Array of section titles (based on current locale) and index
          *         of the first contact which belongs to this section. */
        virtual ali::array<Softphone::Contacts::Section> getSections(ali::string_const_ref source,
                                                                     Softphone::Contacts::QueryFilter filter) const = 0;

        /** @brief Perform a full-text search in contact storage.
          * @param query Query to search for. Wildcards can be used in query terms.
          * @param limit Query result limit; @c 0 for no limit.
          * @param resolveSpans Whether to resolve information about match spans.
          *                     Using @c true can be pretty expensive.
          * @return Array of matched documents. Use @ref Index::Document::getId
          *         to get ID of the matched contact. */
        virtual Index::Matches search(Index::Query const& query,
                                      int limit = 0,
                                      bool resolveSpans = false) const = 0;

        /** @brief Set auxiliary data to a contact in the given contact source.
          * @param source the contact source where to set the data
          * @param id ID of contact to set the data for.
          * @param auxiliary JSON dictionary to store with the contact.
          * @retval true Successfully stored.
          * @retval false The specified contact can't be found.
          *
          * The stored JSON dictionary is accessible under key
          * @ref Softphone::Contacts::Keyword::AUXILIARY. */
        virtual bool setAuxiliary(ali::string_const_ref source,
                                  ali::string_const_ref id,
                                  ali::json::dict auxiliary) = 0;

        /** @brief Sync contacts information from the given contact source.
          *
          * This is useful if the app somehow finds out (for example, via
          * push notification or permission change) that the contact storage
          * was modified and should be synchronized. */
        virtual void sync(ali::string_const_ref source) = 0;

        /** @brief Returns the state of the specified contact source
         * This method helps to distinguish cases of "no contacts" (because the address
         * book is empty) and "no contacts yet" (because the contacts from address book
         * are being loaded). It may take some time (even a minute on very large address
         * books) before the contacts are read and processed, showing a message like
         * "You contacts are being processed..." and a spinner wheel when there are no
         * contacts and the source state is "QueryPending" gives a better user
         * experience than simply showing an empty list.
         */
        virtual Softphone::Contacts::Source::State getContactSourceState(ali::string_const_ref source) = 0;

#if defined(SOFTPHONE_NUMBER_REWRITING)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        NumberRewriting * numberRewriting() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return mNumberRewriting.get();
        }
#endif

#if defined(SOFTPHONE_SMART_CONTACTS)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Smart * smart() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return mSmart.get();
        }
#endif

    protected:
#if defined(SOFTPHONE_NUMBER_REWRITING)
        ali::auto_ptr<NumberRewriting> mNumberRewriting;
#endif

#if defined(SOFTPHONE_SMART_CONTACTS)
        ali::auto_ptr<Smart> mSmart;
#endif
    };

    /** @brief Methods for querying the Domain Name System */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::DNS
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~DNS()
        {}

        /** @brief Query SIP Service Record for a domain
          * @param domain Domain name to query
          * @return Query ID
          * @retval 0 Failed to start the DNS query
          *
          * This method is asynchronous. The observer will be
          * @ref Observer::onDnsSrvQueryDone "notified when the query is completed".
          *
          * @see @ref setObserver for setting the observer. */
        virtual unsigned long querySrv(ali::string_const_ref domain) = 0;

        /** @brief Cancel a DNS SRV query
          * @param queryId ID of the query to cancel
          * @retval true The specified query was canceled
          * @retval false The specified query has already completed or is not valid
          *
          * Cancels a query started by @ref querySrv. */
        virtual bool cancelQuery(unsigned long queryId) = 0;
    };

    /** @brief Methods manipulating event streams
      *
      * These methods allow querying the internal events database and
      * manipulating the event streams. */
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Instance::Events
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        /** @brief Post result */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct PostResult
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Post result enumeration
              *
              * Don’t panic
              *
              * The returned values are divided into multiple groups:
              * <dl>
              * <dt>@c 0 </dt>
              *     <dd>success</dd>
              * <dt>@c 1 to @c 999 </dt>
              *     <dd>the event was not posted due to a temporary failure and
              *         you can try reposting the same event later</dd>
              * <dt>@c 1000 to @c 1999 </dt>
              *     <dd>the event was not posted because of ambiguities or
              *         missing information and requires further processing
              *         before it can be reposted</dd>
              * <dt>@c 2000 to @c 2999 </dt>
              *     <dd>the event contains a major problem that should not
              *         happen; this causes assertion failure in the debug
              *         build</dd>
              * </dl>
              */
            enum Type
            {
                /** @brief Post successful
                  *
                  * The event was successfully posted to the specified stream.
                  * The result of the event is asynchronous and the observer
                  * @ref Observer::onEventsChanged "will be notified". */
                Success = 0,

                /** @brief No network connection
                  *
                  * The event was not posted because there is no network
                  * connection available at the moment. You can try reposting
                  * the same event later. */
                NoNetwork = 1,

                /** @brief The event is posted to a wrong stream
                  *
                  * You are trying to post an event that is not supported by the
                  * target stream. Posting a call event into a multiple users
                  * stream or posting an event to a single-user stream of
                  * another user are examples. */
                WrongStream = 1000,

                /** @brief No account to send the event with
                  *
                  * The event contains no account and there is no default
                  * account. */
                NoAccount = 1001,

                /** @brief The event has no recipient
                  *
                  * The event contains neither a stream key nor any remote
                  * user. You need to specify at least one of that before
                  * reposting.
                  *
                  * This code is also returned if the specified stream has no
                  * parties. */
                NoRecipient = 1002,

                /** @brief The event has too many recipients
                  *
                  * The event contains too many recipients. Eg. calls can
                  * contain only single recipient; to create a conference,
                  * create separate call for each recipient and
                  * @ref Instance.Calls.Conferences.move "move them together".
                  */
                TooManyRecipients = 1003,

                /** @brief The event has no stream
                  *
                  * The event contains multiple recipients but does not specify
                  * stream key. */
                NoStream = 1004,

                /** @brief The event type is not supported by the account
                  *
                  * This usually means some feature is not enabled in the
                  * account configuration. */
                UnsupportedByAccount = 1005,

                /** @brief Failed to process the event */
                ProcessingFailed = 1100,

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS) && defined(SOFTPHONE_NUMBER_REWRITING)
                /** @brief Number rewriting account override collision
                  *
                  * The event contains empty account ID, number rewriting
                  * account override matches the specified remote party and the
                  * overridden account ID is different from the current
                  * (default) account ID. You should decide what account to use,
                  * set the event's account ID appropriately and repost.
                  *
                  * The suggested account IDs are stored in
                  * <tt>event->transients[Transients::accountOverride]</tt>
                  * in an array
                  * {
                  *     default account ID,
                  *     account ID selected by the number rewriting
                  * }
                  */
                AccountOverride = 1500,
#endif

                /** @brief The event was already used
                  *
                  * The event has its event ID filled and therefore was already
                  * used. To repost the event, create a new event with
                  * @ref Softphone::EventHistory::Event::clone "clone".
                  *
                  * This error asserts in the debug build. */
                UsedEvent = 2000,

                /** @brief The event type cannot be posted
                  *
                  * You are trying to post an event of a type that cannot be
                  * posted. @ref Softphone::EventHistory::EventType::Type::Unknown
                  * "Unknown" is an example of such type.
                  *
                  * This error asserts in the debug build. */
                UnpostableType = 2001,

                /** @brief Unsupported feature required
                  *
                  * The event requires a feature that is not supported in your
                  * SDK build.
                  *
                  * This error asserts in the debug build. */
                UnsupportedFeature = 2002,

                /** @brief Invalid stream
                  *
                  * You are posting event to a stream that does not exist. */
                InvalidStream = 2003,

                /** @brief Invalid recipient
                  *
                  * You are posting event with a remote user that has no
                  * transport URI. */
                InvalidRecipient = 2004,
            };
        };

        /** @brief Keys for event transients */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Transients
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS) && defined(SOFTPHONE_NUMBER_REWRITING)
            /** @brief Transient for number rewriting account override
              * @type{array}
              * @see @ref PostResult::AccountOverride for description of its
              *      content */
            static ali::string_literal const accountOverride;
#endif
        };

        /** @brief Virtual destructor */
        virtual ~Events()
        {}

        /** @brief Post an event
          * @param event The event to post; its ID will be filled by this method
          * @return @ref PostResult::Type "Possible return values"
          *
          * The event is posted to the stream specified by the stream key
          * using the account ID specified in the event. What that means
          * depends on the event that is posted. The event
          * @ref EventHistory::Event::save "is saved" when it is posted
          * successfully.
          *
          * If the event's account ID is empty, the default account ID is used.
          */
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS) && defined(SOFTPHONE_NUMBER_REWRITING)
        /** Only in that case are the number rewriting account overrides
          * computed. The method then can return
          * @ref PostResult::AccountOverride. See the description of that
          * return value for more information. */
#endif
        /**
          * If the event's remote users are empty, all the stream parties are
          * used and filled into the event. Otherwise, the filled remote users
          * are used, regardless of the stream parties specified in the stream.
          *
          * If no stream key is filled, a stream matching the remote user is
          * found, or if no such stream exists, created automatically. The
          * event's stream key is then filled by the correct value. This does
          * not work when there are multiple remote users; in that case,
          * @ref PostResult::NoStream is returned.
          *
          * If neither a stream key nor any remote user is provided, the call
          * fails with @ref PostResult::NoRecipient.
          *
          * A call event cannot have multiple remote users. If you want to start
          * a conversation, post multiple call events and then
          * @ref Calls::Conferences::move "move them to an appropriate conference".
          * Note that the stream key must be either empty or match the remote
          * user's single-user stream, or legacy call stream if that is used.
          *
          * The event may be modified during processing or any time later. It is
          * guaranteed that the event will be modified in the main thread only.
          * You should not use the posted event in any other thread. */
        virtual PostResult::Type post(EventHistory::Event::Pointer event) = 0;

        /** @brief Repost a failed event
          * @param event Event to repost
          * @retval true Reposted
          * @retval false Failed to repost
          *
          * Only event that was successfully posted with @ref post and then
          * failed during processing (eg. a message failed to be delivered)
          * can be reposted. That event should have filled all information
          * necessary for posting, no account or stream matching is performed.
          *
          * Currently, only @ref EventHistory::MessageEvent can be reposted. */
        virtual bool repost(EventHistory::Event::Pointer event) = 0;

        /** @brief Fetch events from internal storage
          * @param query Query to fetch events
          * @param paging Paging of the fetched result
          * @return Result of the fetch */
        virtual EventHistory::FetchResult fetch(EventHistory::Query const& query,
                                                EventHistory::Paging const& paging = {}) = 0;

        /** @brief Fetch event streams from internal storage
          * @param query Query to fetch event streams
          * @param paging Paging of the fetched result
          * @return Result of the fetch */
        virtual EventHistory::StreamFetchResult fetch(EventHistory::StreamQuery const& query,
                                                      EventHistory::StreamPaging const& paging = {}) = 0;

        /** @brief Get unread events count
          * @param query Query to fetch event streams
          * @return total count of unread events */
        virtual int getUnreadCount(EventHistory::StreamQuery const& query) = 0;

        /** @brief Change stream key
          * @param currentKey Current stream key
          * @param newKey New stream key */
        virtual void changeStreamKey(ali::string const& currentKey,
                                     ali::string const& newKey) = 0;

        /** @brief Match stream parties in a stream
          *
          * @param streamKey Current stream key
          * @return The new stream key for the stream. May be the same as the
          *         original.
          *
          * Try to match stream parties of a stream against contacts. If there
          * is a match and the stream is a single-user stream, change the stream
          * key accordingly. */
        virtual ali::string matchStreamParties(ali::string const &streamKey) = 0;

        /** @brief Load events
          * @param events Set of events to load */
        virtual ali::array<EventHistory::Event::Pointer> load(ali::array<EventHistory::EventIdType> const& events) = 0;

        /** @brief Remove events
          * @param eventIds Set of IDs of events to remove */
        virtual void remove(ali::array_set<EventHistory::EventIdType> const& eventIds) = 0;

        /** @brief Remove an event
          * @param eventId ID of the event to remove */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void remove(EventHistory::EventIdType eventId)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            return remove(ali::array_set<EventHistory::EventIdType>(eventId));
        }

        /** @brief Remove events
          * @param events Set of events to remove */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void remove(ali::array_set<EventHistory::Event::Pointer> const& events)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            ali::array_set<EventHistory::EventIdType> ids;
            for (EventHistory::Event::Pointer const& event : events)
            {
                ali_assert(event->getEventId() > 0);
                ids.insert(event->getEventId());
            }
            remove(ids);
        }

        /** @brief Remove an event
          * @param event Event to remove */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void remove(EventHistory::Event::Pointer event)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            ali_assert(event->getEventId() > 0);
            remove(event->getEventId());
        }
        
        /** @brief Remove event attachment
         * @param event Event to remove attachment from
         * @param attachmentId Id of the attachment to remove
         */
        virtual void remove(EventHistory::Event::Pointer event, EventHistory::EventAttachmentIdType attachmentId) = 0;


        /** @brief Remove events
          * @param query Query on events to remove */
        virtual void remove(EventHistory::Query const& query) = 0;

        /** @brief Remove a stream
          * @param key Key of the stream to remove
          *
          * All events in the removed stream are removed as well */
        virtual void remove(ali::string const& key) = 0;

        /** @brief Remove a stream
          * @param stream The stream to remove
          *
          * All events in the removed stream are removed as well */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void remove(EventHistory::EventStream const& stream)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            remove(stream.key);
        }

        /** @brief Remove streams
          * @param query Query on streams to remove
          *
          * All events in the removed streams are removed as well */
        virtual void remove(EventHistory::StreamQuery const& query) = 0;

        /** @brief Remove all events and streams */
        virtual void removeAll() = 0;

        /** @brief Save an event as a draft
          * @param event Event to save
          *
          * This method saves a draft event. There can be one draft event saved
          * for each stream and one draft event with no associated stream. If
          * there already was a draft event for the stream specified in the
          * event, or if the event has no stream associated and there already
          * was a draft event without stream, that event is overwritten.
          * @see @ref fetchDraft <br/>
          *      @ref removeDraft */
        virtual void saveDraft(EventHistory::Event::Pointer event) = 0;

        /** @brief Fetch a draft event
          * @param key Key of the stream the draft event is associated
          *            with; @c nullptr for draft event without stream
          * @return A copy of the draft event
          * @retval nullptr No such draft event
          *
          * This method returns a copy of the stored draft event. It won’t
          * remove the draft event and you can call this method again with the
          * same stream key to obtain another copy.
          * @see @ref saveDraft <br/>
          *      @ref removeDraft */
        virtual EventHistory::Event::Pointer fetchDraft(ali::opt_string const& key) = 0;

        /** @brief Fetch all draft events.
          * @return Fetch result with all draft messages. */
        virtual EventHistory::FetchResult fetchAllDrafts() = 0;

        /** @brief Remove a draft event
          * @param key Key of the stream the draft event is associated
          *            with; @c nullptr for draft event without stream
          *
          * Removing a non-existent draft event is silently ignored.
          * @see @ref saveDraft <br/>
          *      @ref fetchDraft */
        virtual void removeDraft(ali::opt_string const& key) = 0;

        /** @brief Checks if the attached content can be downloaded via @ref downloadContent method
         * @param event event with attachments
         * @param attachmentId the attachment ID to query
         * @param automatically true if the attempt to automatically download the content is being made, false if the download is triggered as a user action
         * @return true if the attachment has downloadable content which has not been downloaded yet
         *
         * @see @ref downloadContent
         */
        virtual bool canDownloadContent(EventHistory::Event::Pointer event, EventHistory::EventAttachmentIdType attachmentId, bool automatically) = 0;

        /** @brief Starts download of the specified attachment
         * @param event event with attachmets
         * @param attachmentId the attachment ID to download
         * @param cb the callback to be called when the download completes sucessfully or fails
         * @return true if the attachment download started successfully
         *
         * @see @ref canDownloadContent
         */
        virtual ali::auto_ptr<Messaging::TransferHandle> downloadContent(EventHistory::Event::Pointer event, EventHistory::EventAttachmentIdType attachmentId, EventHistory::Event::ContentDownloadStatusCallback cb) = 0;

        /** @brief installs a callback which receives information about attachment upload progress
         * @param cb the callback
         * @return handle to the callback. It should be saved, deleting the handle will uninstall the callback
         *
         */
        virtual ali::auto_ptr<ali::handle> addContentUploadProgressCallback(EventHistory::Event::ContentUploadProgressCallback cb) = 0;

        /** @brief installs a callback which receives information about attachment download progress
         * @param cb the callback
         * @return handle to the callback. It should be saved, deleting the handle will uninstall the callback
         *
         */
        virtual ali::auto_ptr<ali::handle> addContentDownloadProgressCallback(EventHistory::Event::ContentDownloadProgressCallback cb) = 0;


        /** @brief sends isComposing notification to the stream
         * @param info the composing info data
         *
         * You can call this method as often as needed (typically after every character change in the
         * message edit line). The SDK only sends out the message if there's a change in active state
         * (active -> inactive or vice versa) or once in configured timeout (isComposingRefreshInterval)
         * You are not required to send the inactive composing info when the final message is being sent -
         * it is done automatically by the SDK.
         */
        virtual void sendComposingInfo(Messaging::ComposingInfo const& info) = 0;

        /** @brief get the info about the stream parties that are composing at the moment
         * @return an array of composing infos for all the stream parties that are currently active
         *
         * You can call this method the set the initial state of GUI. You will get a composing info
         * for all stream parties that should be displayed as "composing". The subsequent changes are
         * reported via @ref onComposingInfo
         */
        virtual ali::array<Messaging::InboundComposingInfo> getActiveComposingInfos() const = 0;
    };

    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::GroupManagement
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~GroupManagement()
        {};
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief get the transport interface for the account that allows you to manage the group streams
         * @param  accountId The account for which the group management transport interface is queried
         * @return the transport interface or nullptr
         *
         * You should not store the transport pointer anywhere. Always ask for a fresh pointer.
         */
        virtual Softphone::Messaging::GroupManagementTransport::Interface* getTransport(ali::string const& accountId) = 0;
#   else
        /** @brief get the transport interface that allows you to manage the group streams
         * @return the transport interface or nullptr
         *
         * You should not store the transport pointer anywhere. Always ask for a fresh pointer.
         */
        virtual Softphone::Messaging::GroupManagementTransport::Interface* getTransport() = 0;
#   endif
    };
    
    /** @brief Methods manipulating the traffic log
      *
      * All log methods are thread-safe. */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Log
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Log()
        {}

        /** @brief Set the custom sink for all log messages
         *
         * When set to non-null, the SDK will pass the logs to the provided serializer
         * object. */
        virtual void setCutomSink(ali::shared_ptr<ali::serializer> sink) = 0;

        /** @brief Get the traffic log
          * @return The traffic log
          * @retval "" The traffic log has not been enabled or nothing was
          *            logged since @ref clear or @ref seize was called
          *
          * The internal traffic log uses cyclic buffer so that it won’t
          * grow over an internally specified size. */
        virtual ali::string get() const = 0;

        /** @brief Clear the traffic log
          *
          * Clears the internal traffic log’s cyclic buffer. */
        virtual void clear() = 0;

        /** @brief Seize the traffic log
          * @return The traffic log
          * @retval "" The traffic log has not been enabled or nothing was
          *            logged since @ref clear or @ref seize was called
          *
          * This is equivalent to calling @ref get followed by @ref clear but
          * makes sure that nothing can be logged in between. */
        virtual ali::string seize() = 0;

        /** @brief Log an entry to the traffic log
          * @param entry Entry to log */
        virtual void log(ali::string entry) = 0;
    };

    /** @brief Methods for video calls */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::DND
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief DND result enumeration
         *
         * The result could be any one from:
         * <dl>
         * <dt>@c 0 </dt>
         *     <dd>PassNotSupported</dd>
         * <dt>@c 1 </dt>
         *     <dd>PassAllowed</dd>
         * <dt>@c 2 </dt>
         *     <dd>RejectImmediate/dd>
         * <dt>@c 3 </dt>
         *     <dd>RejectAccount</dd>
         * <dt>@c 4 </dt>
         *     <dd>RejectGlobal</dd>
         * </dl>
         */
        enum Result
        {
            /** @brief Pass Not Supported
             *
             * The client application dosn't have the Dnd feature enabled.*/
            PassNotSupported,

            /** @brief Pass Allowed
             *
             * The event successfully passed all the Dnd Rules. */
            PassAllowed,

            /** @brief Reject Immediate
             *
             * The Immediate Dnd is active, so any event whether it falls under any Dnd rule or not, it should be rejected.*/
            RejectImmediate,

            /** @brief Reject Account
             *
             * The event failed to pass the account level Dnd rules. */
            RejectAccount,

            /** @brief Reject Global
             *
             * The event failed to pass the global Dnd rules. */
            RejectGlobal
        };

        /** @brief Virtual Destructor */
        virtual ~DND()
        {}

        /** @brief Check whether event at specified time falls under any configured DND rule or immediate DND
          * @param time Time at which to verify the event
          * @param event Event to be verified
          * @return Result about the validation of event and event time against the configured DND rules */
        virtual Result applies(double time,
                               EventHistory::Event::Pointer event) const = 0;
    };


    /** @brief Network methods */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Network
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Methods for certificates */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Certificates
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            typedef ali::blob_const_ptr Certificate;
            typedef ali::array<Certificate> Chain;
            typedef ali::string Hash;
            typedef ali::array_set<Hash> Hashes;
            typedef ali::array_map<ali::string, Hashes> Exceptions;

            /** @brief Destructor */
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            virtual ~Certificates()
            //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
            {}

            virtual void setEnabled(bool enabled) = 0;

            virtual bool isEnabled() const = 0;

            virtual void addException(ali::string host,
                                    Hash const& hash) = 0;

            virtual void addException(ali::string host,
                                    Chain const& chain) = 0;

            virtual void removeException(ali::string host,
                                        Hash const& hash) = 0;

            virtual void removeException(ali::string host,
                                        Chain const& chain) = 0;

            virtual void removeAllExceptions(ali::string host) = 0;

            virtual bool isException(ali::string host,
                                    Hash const& hash) const = 0;

            virtual bool isException(ali::string host,
                                    Chain const& chain) const = 0;

            virtual Exceptions const& getExceptions() const = 0;

            virtual bool verify(ali::network::address const& host,
                                Chain const& chain) = 0;

            virtual bool load() = 0;
            virtual bool save() const = 0;
        };

        /** @brief Virtual destructor */
        virtual ~Network()
        {}

        /** @brief Get type of the currently used network (if any)
          * @return The type of the currently used network */
        virtual Softphone::Network::Type get() const = 0;

        /** @brief Get conditions of the currently used network
          * @return The conditions of the currently used network
          * @retval null The conditions are not known or monitored */
        virtual ali::optional<Softphone::NetworkConditions> getConditions() const = 0;

        /** @brief Rescan network.
          *
          * @param force Whether to force network switch.
          *
          * The SDK notices changes in networks that are reported by the
          * operating system. If you have special networks, like a VPN, that
          * does not integrate with the system notifications, you need to
          * notify the SDK of changes in it.
          *
          * This method can also be used when you need to change priorities
          * that were returned by @ref Observer.overrideNetworkPriorities. */
        virtual void rescan() = 0;

        /** @brief Get methods for certificates
          * @return The methods for certificates */
        Certificates * certificates() const
        {
            return mCertificates.get();
        }

    protected:
        ali::auto_ptr<Certificates> mCertificates;
    };

    /** @brief Methods for audio playback. */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Playback
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Playback()
        {}

        /** @brief Start a playback.
          * @param stream Stream to play; should support @c seek.
          * @param loop Whether to loop the stream.
          * @return Whether the stream is playing. */
        virtual bool start(ali::auto_ptr<ali::deserializer> stream,
                           bool loop = false) = 0;

        /** @brief Start a playback.
          * @param file File to load.
          * @param loop Whether to loop the stream.
          * @return Whether the file was loaded. */
        bool start(ali::filesystem2::path const& file,
                   bool loop = false);

        /** @brief Pause playback. */
        virtual void pause() = 0;

        /** @brief Resume playback from paused position. */
        virtual void resume() = 0;

        /** @brief Set playback speed (1x..16x) */
        virtual void setSpeed(int speed) = 0;

        /** @brief Stop playback. */
        virtual void stop() = 0;

        /** @brief Seek to a position.
          * @param position Position to seek to; 0 ≤ @c position < @ref getDuration. */
        virtual void seek(ali::time::milliseconds position) = 0;

        /** @brief Get length of the currently playing file.
          * @return Length of the currently playing file, in milliseconds. */
        virtual ali::time::milliseconds getDuration() const = 0;

        /** @brief Get position in the currently played file.
          * @return Position in the currently played file, in milliseconds. */
        virtual ali::time::milliseconds getPosition() const = 0;

        /** @brief Check whether playback is in progress.
          * @return Whether playback is in progress. */
        virtual bool isPlaying() const = 0;
    };

    /** @brief Methods for notifications */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Notifications
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** Virtual destructor */
        virtual ~Notifications()
        {}

#if defined(SOFTPHONE_PUSH)
        /** @brief Methods for push notifications */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        struct Push
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            
            typedef ali::callback<void(PushNotificationProcessor::PushNotificationCompletionInfo const&)> CompletionCallback;

            /** @brief Virtual destructor */
            virtual ~Push()
            {}

            /** @brief Set registration ID for push notifications
              * @param regId Registration ID for the push notifications
              * @param usage Intended usage for the push token.
              *
              * For android pass Softphone::PushToken::Usage::All
              * For iOS you may need to set pushkit VOIP token for Softphone::PushToken::Usage::IncomingCalls
              * and remote notification token for Softphone::PushToken::Usage::Other.
              * Starting on iOS 13 apps need to report the VOIP pushes to Callkit as incoming calls,
              * so the VOIP push tokens cannot be used for messages and other kinds of notifications.
             */
            
            virtual void setRegistrationId(ali::string_const_ref regId, PushToken::Usage usage) = 0;

            /** @brief Inform the library that a push message has arrived
              * @param message The incoming message, converted as XML
              * @return true/false based upon the notification gets handled or not. */
            virtual bool handle(ali::xml::tree const& message) = 0;

            /** @brief Inform the library that a push message has arrived
              * @param message The incoming message, converted as XML
              * @param completion The callback to call when handling of the push notification is complete
              * @return handle to store if we are interested in the callback */
            virtual ali::auto_ptr<ali::handle> handle(ali::xml::tree const& message, PushToken::Usage usage, CompletionCallback completion) = 0;

#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
            /** @brief Schedule a push test
              * @param accountId ID of the account to schedule the push test on
              * @param seconds How many seconds should the push arrive in
              * @retval true Push test request sent
              * @retval false Cannot schedule push test on this account
              *
              * This call is asynchronous. The observer will be
              * @ref Observer::onPushTestScheduled "notified when the push is actually scheduled or scheduling it failed".
              */
            virtual bool scheduleTest(ali::string const& accountId,
                                      int seconds) = 0;
#   else
            /** @brief Schedule a push test
              * @param seconds In how many seconds should the push arrive
              * @retval true Push test requested
              * @retval false Cannot schedule push test
              *
              * This call is asynchronous. The observer will be
              * @ref Observer::onPushTestScheduled "notified when the push is actually scheduled or scheduling it failed".
              */
            virtual bool scheduleTest(int seconds) = 0;
#   endif
        };
#endif



#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief create or update event for a missed call and add the notification related transients
         * @param callId The ID of the call, it's either some opaque identifier received
         from the push server or just the event ID in case of non-pushed calls
         * @param displayName The display name of the callee received via the push or retrieved from the event itself
         * @param callee The callee URI received via push or retrieved from the event itself
         * @param timestamp The timestamp of the missed call event
         * @param accountId ID of the account received via push or retrieved from the event itself
         * @retval nullptr The missed call for this callId has already been reported and no further action is required
         * @retval not-null CallEvent::Pointer enriched with the notification related transients
         *
         */
        virtual EventHistory::CallEvent::Pointer getMissedCallEvent(ali::string_const_ref callId,
                                                                    ali::string_const_ref displayName,
                                                                    ali::string_const_ref callee,
                                                                    EventHistory::TimestampType timestamp,
                                                                    ali::string_const_ref accountId) = 0;
#   else
        /** @brief create or update event for a missed call and add the notification related transients
         * @param callId The ID of the call, it's either some opaque identifier received
         from the push server or just the event ID in case of non-pushed calls
         * @param displayName The display name of the callee received via the push or retrieved from the event itself
         * @param callee The callee URI received via push or retrieved from the event itself
         * @param timestamp The timestamp of the missed call event
         * @retval nullptr The missed call for this callId has already been reported and no further action is required
         * @retval not-null CallEvent::Pointer enriched with the notification related transients
         *
         */
        virtual EventHistory::CallEvent::Pointer getMissedCallEvent(ali::string_const_ref callId,
                                                                    ali::string_const_ref displayName,
                                                                    ali::string_const_ref callee,
                                                                    EventHistory::TimestampType timestamp) = 0;
#   endif


#if defined(SOFTPHONE_PUSH)
        /** @brief Get methods for push notifications
          * @return The methods for push notifications */
        Push * push() const
        {
            return mPush.get();
        }
#endif

    protected:
#if defined(SOFTPHONE_PUSH)
        ali::auto_ptr<Push> mPush; ///< Methods for push notifications
#endif
    };

    /** @brief Methods for audio recording. */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Recording
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Recording()
        {}

        /** @brief Start recording.
          * @param file File to record to.
          * @param format Format of recording; @c nullptr for default.
          * @return Whether the recording started. */
        virtual bool start(ali::filesystem2::path const& file,
                           ali::opt_string const& format = nullptr) = 0;

        /** @brief Stop recording. */
        virtual void stop() = 0;

        /** @brief Get length of the current recording.
          * @return Length of the current recording, in milliseconds. */
        virtual ali::time::milliseconds getDuration() const = 0;

        /** @brief Check whether recording is in progress.
          * @return Whether recording is in progress. */
        virtual bool isRecording() const = 0;
    };

    /** @brief Methods for manipulating registration of the accounts */
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
    /**
      * All registered accounts are identified by either account ID or account
      * index. Whether it is better to use ID or index depends on the situation
      * as there are some advantages and some disadvantages for either approach.
      *
      * Account indexes have constant time complexity. However, they may not
      * identify the same account after some methods, which are marked as
      * having such side effect. Account indexes are generally preserved over
      * application restart but your code should not depend on that.
      *
      * Account IDs, on the other hand, have logarithmic time complexity but
      * are persistent and identify the same account until
      * @ref deleteAccount "the account is deleted". */
#endif
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Registration
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Registration()
        {}

        /** @brief Save an account
          * @param accountXML The @ref AccountXML "account XML" */
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @return Account ID of the saved account
          * @retval nullptr The XML in @p accountXML is not valid
          * */
#else
        /** @retval true The account was successfully saved
          * @retval false The XML in @p accountXML is not valid
          * */
#endif
        /** The account’s ID is read from @ref Account_id "attribute “id”". If
          * that attribute is missing, a new, pseudo-random ID composed of 16
          * alphanumeric characters is generated.
          * */
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** If the specified @p accountXML has ID of an already existing
          * account, that account is replaced and its
          * account index is preserved. Otherwise, a new account is created
          * with account index equal to @ref getAccountCount - 1. */
#else
        /** If there is an already existing account, it is replaced. */
#endif
        /**
          * If a new account is created, or an existing account is replaced and
          * the account XML is not same as the old account XML of that account,
          * the account is (re-)registered. Otherwise, if the new account XML
          * is same as the old account XML, the account is kept in the current
          * registration state.
          *
          * If the account is (re-)registered, the actual (re-)registration is
          * asynchronous. The observer is @ref Observer::onRegistrationStateChanged
          * "notified about any changes in the registration state" as well as
          * @ref Observer::onRegistrationErrorMessage "when the registration fails".
          *
          * @see @ref setObserver for setting the observer. */
#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        virtual ali::opt_string saveAccount(Account::Xml & accountXML) = 0;
#else
        virtual bool saveAccount(Account::Xml & accountXML) = 0;
#endif
        virtual bool validateAccount(Account::Xml & accountXml, Softphone::Account::Validator::FailureDetail *failureDetail ) = 0;

        virtual void loadValidatorsForAllAccounts() = 0;

        virtual void fillDefaultsForAccount(Account::Xml & accountXML) = 0;

        virtual void sanitizeAccount(Account::Xml & accountXML) = 0;

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        ali::opt_string saveAccount(ali::xml::tree & accountXML)
        {
            // create Account::Xml from plain xml document
            Account::Xml xml{accountXML,Account::NodeAttributes::gui()};

            // merge-in account defaults
            xml.merge(getAccountDefaults(xml.getType()));

            return saveAccount(xml);
        }
#else
        bool saveAccount(ali::xml::tree & accountXML)
        {
            // create Account::Xml from plain xml document
            Account::Xml xml{accountXML,Account::NodeAttributes::gui()};

            // merge-in account defaults
            xml.merge(getAccountDefaults(xml.getType()));

            return saveAccount(xml);
        }
#endif

        virtual Account::Xml const& getAccountDefaults(Account::Type type) = 0;

        virtual Account::Filter::Base const& getAccountFilter(Account::Type type) = 0;

        virtual void setProvisionedAccountDefaults(Account::Type type,
                                                   Account::Xml const& defaults,
                                                   ali::auto_ptr<Account::Validator::Array> validators,
                                                   Account::Filter::Base const& filter) = 0;

        /** @brief Update all accounts
          *
          * This is usually called after some preference keys that control the
          * way accounts work were altered. */
        virtual void updateAll() = 0;

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        virtual Softphone::BusyLampField getBlf(ali::string_const_ref accountId, ali::string_const_ref uri) = 0;
#else
        virtual Softphone::BusyLampField getBlf(ali::string_const_ref uri) = 0;
#endif

#if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Get index of an account
          * @param account ID of the account to get the index for
          * @return The index; @c 0 ≤ return < @ref getAccountCount
          * @retval -1 No such account ID */
        virtual int getAccountIndex(ali::string_const_ref accountId) const = 0;

        /** @brief Get number of accounts
          * @return Number of accounts */
        virtual int getAccountCount() const = 0;

        /** @brief Get account ID for an account index
          * @param index Account index; @c 0 ≤ @p index < @ref getAccountCount
          * @return Account ID */
        virtual ali::string_const_ref getAccountId(int index) const = 0;

        /** @brief Get IDs of all accounts
          * @return Set of IDs of all accounts */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::array_set<ali::string> getAccounts() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali::array_set<ali::string> ret;
            int const count = getAccountCount();
            ret.reserve(count);
            for (int i = 0; i < count; ++i)
                ret.insert(getAccountId(i));
            return ret;
        }

        /** @brief Get indexes of all enabled account
          * @return Set of indexes of all enabled accounts */
        virtual ali::array_set<int> getEnabledAccountIndexes() const = 0;

        /** @brief Get IDs of all enabled accounts
          * @return Set of IDs of all enabled accounts */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::array_set<ali::string> getEnabledAccounts() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali::array_set<ali::string> ret;
            ali::array_set<int> indexes = getEnabledAccountIndexes();
            ret.reserve(indexes.size());
            for (int i = 0; i < indexes.size(); ++i)
                ret.insert(getAccountId(indexes.at(i)));
            return ret;
        }

        /** @brief Get indexes of all disabled account
          * @return Set of indexes of all disabled accounts */
        virtual ali::array_set<int> getDisabledAccountIndexes() const = 0;

        /** @brief Get IDs of all disabled accounts
          * @return Set of IDs of all disabled accounts */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::array_set<ali::string> getDisabledAccounts() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            ali::array_set<ali::string> ret;
            ali::array_set<int> indexes = getDisabledAccountIndexes();
            ret.reserve(indexes.size());
            for (int i = 0; i < indexes.size(); ++i)
                ret.insert(getAccountId(indexes.at(i)));
            return ret;
        }

        /** @brief Check if an account is enabled
          * @param index Index of the account to check;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @retval true The account is enabled
          * @retval false The account is disabled */
        virtual bool isAccountEnabled(int index) const = 0;

        /** @brief Check if an account is enabled
          * @param accountId ID of the account to check
          * @retval true The account is enabled
          * @retval false The account is disabled */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool isAccountEnabled(ali::string_const_ref accountId) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return isAccountEnabled(getAccountIndex(accountId));
        }

        /** @brief Get account XML
          * @param index Account index; @c 0 ≤ @p index < @ref getAccountCount
          * @return The account XML
          *
          * The returned account XML is read-only and points directly to the
          * internal structures. Calling @ref saveAccount or @ref deleteAccount
          * may invalidate this reference so you should not call them. To make
          * the returned XML persistent or to make changes, make a copy. */
        virtual Account::Xml const& getAccount(int index) const = 0;

        /** @brief Get account XML
          * @param accountId ID of the account to get
          * @return The account XML
          * @retval nullptr No such account
          *
          * The returned account XML is read-only and points directly to the
          * internal structures. Calling @ref saveAccount or @ref deleteAccount
          * may invalidate this reference so you should not call them. To make
          * the returned XML persistent or to make changes, make a copy. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Account::Xml const* getAccount(ali::string_const_ref accountId) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            int const index = getAccountIndex(accountId);
            if (index < 0)
                return nullptr;
            return &getAccount(index);
        }

        /** @brief Delete an account
          * @param accountId ID of the account to delete
          *
          * Calling this method invalidates account indexes higher than
          * @ref getAccountIndex(@p accountId). */
        virtual void deleteAccount(ali::string accountId) = 0;

        /** @brief Delete an account
          * @param index Account index; @c 0 ≤ @p index < @ref getAccountCount
          *
          * Calling this method invalidates account indexes higher than
          * @p index. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void deleteAccount(int index)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            deleteAccount(getAccountId(index));
        }

        /** @brief Include non-standard SIP header
          * @param accountId ID of the account to include the non-standard SIP header to
          * @param method Method in which the SIP header should be included to;
          *               @c * for all methods
          * @param responseCode Response code the SIP header should be included to:
          *                      - @c xxx for all responses
          *                      - @c Axx for responses with code A00 to A99
          *                      - @c ABx for responses with code AB0 to AB9
          *                      - @c ABC for responses only with code ABC
          * @param name SIP header name; should start with “X-”
          * @param value SIP header value */
        virtual void includeNonStandardSipHeader(ali::string_const_ref accountId,
                                                 ali::string_const_ref method,
                                                 ali::string_const_ref responseCode,
                                                 ali::string_const_ref name,
                                                 ali::string_const_ref value) = 0;

        /** @brief Include non-standard SIP header
          * @param index Index of the account to include the non-standard SIP
          *              header to; @c 0 ≤ @p index < @ref getAccountCount
          * @param method Method in which the SIP header should be included to;
          *               @c * for all methods
          * @param responseCode Response code the SIP header should be included to:
          *                      - @c xxx for all responses
          *                      - @c Axx for responses with code A00 to A99
          *                      - @c ABx for responses with code AB0 to AB9
          *                      - @c ABC for responses only with code ABC
          * @param name SIP header name; should start with “X-”
          * @param value SIP header value */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void includeNonStandardSipHeader(int index,
                                         ali::string_const_ref method,
                                         ali::string_const_ref responseCode,
                                         ali::string_const_ref name,
                                         ali::string_const_ref value)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return includeNonStandardSipHeader(getAccountId(index),
                                               method,
                                               responseCode,
                                               name,
                                               value);
        }

        /** @brief Exclude non-standard SIP header
          * @param accountId ID of the account in which the non-standard SIP header
          *                  should not be included
          * @param method Method in which the SIP header should not be included
          * @param responseCode Response code the SIP header should not be included to;
          *                     see @ref includeNonStandardSipHeader for the description
          * @param name SIP header name; should start with “X-”
          *
          * This method excludes a non-standard SIP header included with
          * @ref includeNonStandardSipHeader. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        virtual void excludeNonStandardSipHeader(ali::string_const_ref accountId,
                                                 ali::string_const_ref method,
                                                 ali::string_const_ref responseCode,
                                                 ali::string_const_ref name) = 0;
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

        /** @brief Exclude non-standard SIP header
          * @param index Index of the account in which the non-standard SIP header
          *              should not be included; @c 0 ≤ @p index < @ref getAccountCount
          * @param method Method in which the SIP header should not be included
          * @param responseCode Response code the SIP header should not be included to;
          *                     see @ref includeNonStandardSipHeader for the description
          * @param name SIP header name; should start with “X-”
          *
          * This method excludes a non-standard SIP header included with
          * @ref includeNonStandardSipHeader. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void excludeNonStandardSipHeader(int index,
                                         ali::string_const_ref method,
                                         ali::string_const_ref responseCode,
                                         ali::string_const_ref name)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return excludeNonStandardSipHeader(getAccountId(index),
                                               method,
                                               responseCode,
                                               name);
        }

        /** @brief Get registration state of an account
          * @param accountId ID of the account to get the registration state of
          * @return The registration state of the specified account */
        virtual Registrator::State::Type getRegistrationState(ali::string_const_ref accountId) const = 0;

        /** @brief Get registration state of an account
          * @param index Index of the account to get the registration state of;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @return The registration state of the specified account */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Registrator::State::Type getRegistrationState(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getRegistrationState(getAccountId(index));
        }

#   if defined(SOFTPHONE_BALANCE)
        /** @brief Get balance record of an account
          * @param accountId ID of the account to get the balance record of
          * @return The balance record of the specified account */
        virtual Balance::Record getBalance(ali::string_const_ref accountId) const = 0;

        /** @brief Get balance record of an account
          * @param index Index of the account to get the balance record of;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @return The balance record of the specified account */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Balance::Record getBalance(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getBalance(getAccountId(index));
        }

        /** @brief Check balance of an account
          * @param accountId ID of the account to check the balance of
          *
          * The balance record is updated periodically and after each event but
          * if the user eg. buys credit using web view, you may want to trigger
          * immediate balance check.
          *
          * The balance is checked asynchronously. The observer
          * @ref Observer::onBalance "will be notified" when it is updated. */
        virtual void checkBalance(ali::string_const_ref accountId) const = 0;

        /** @brief Check balance of an account
          * @param index Index of the account to check the balance record of;
          *              @c 0 ≤ @p index < @ref getAccountCount
          *
          * The balance record is updated periodically and after each event but
          * if the user eg. buys credit using web view, you may want to trigger
          * immediate balance check.
          *
          * The balance is checked asynchronously. The observer
          * @ref Observer::onBalance "will be notified" when it is updated. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void checkBalance(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return checkBalance(getAccountId(index));
        }

        /** @brief Query rate record for a number
          * @param accountId ID of the account to query the rate for
          * @param number Number to query the rate record for
          * @param callback Callback called when the rate is available
          * @return Handle of the request
          * @retval nullptr Failed to query
          *
          * If the returned handle is destroyed, the query is aborted. */
        virtual ali::auto_ptr<ali::handle> queryRate(ali::string_const_ref accountId,
                                                     ali::generic_peer_address const& number,
                                                     Rate::RateCallback callback) const = 0;

        /** @brief Query rate record for a number
          * @param index Index of the account to query the rate for
          * @param number Number to query the rate record for
          * @param callback Callback called when the rate is available
          * @return Handle of the request
          * @retval nullptr Failed to query
          *
          * If the returned handle is destroyed, the query is aborted. */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        ali::auto_ptr<ali::handle> queryRate(int index,
                                             ali::generic_peer_address const& number,
                                             Rate::RateCallback callback) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return queryRate(getAccountId(index), number, callback);
        }

#   endif

        virtual bool isDndActive(ali::string_const_ref accountId) const = 0;

        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool isDndActive(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return isDndActive(getAccountId(index));
        }

        /** @brief Get default account ID
          * @return The ID of the default account
          * @retval nullptr No default account
          *
          * The returned account XML is read-only and points directly to the
          * internal structures. Calling @ref saveAccount or @ref deleteAccount
          * may invalidate this reference so you should not call them. To make
          * the returned XML persistent or to make changes, make a copy. */
        virtual ali::opt_string getDefaultAccountId() const = 0;

        /** @brief Get index of the default account
          * @return The index of the default account; @c 0 ≤ index < @ref getAccountCount
          * @retval -1 No default account */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        int getDefaultAccountIndex() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            auto const& id = getDefaultAccountId();
            if (id.is_null())
                return -1;
            return getAccountIndex(*id);
        }

        /** @brief Get default account XML
          * @return The account XML
          * @retval nullptr No such account */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Account::Xml const* getDefaultAccount() const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            auto const id = getDefaultAccountId();
            if (id == nullptr)
                return nullptr;
            return getAccount(*id);
        }

        /** @brief Set default account
          * @param accountId ID of the account to set as default; @c nullptr for none */
        virtual void setDefaultAccount(ali::opt_string const& accountId) = 0;

        /** @brief Set default account
          * @param index Index of the account to set as default;
          *              @p index < @c 0 for none; @p index < @ref getAccountCount */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        void setDefaultAccount(int index)
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return setDefaultAccount(index < 0 ? ali::opt_string{} : ali::opt_string{getAccountId(index)});
        }

        /** @brief Move an account to another index
          * @param from Index to move an account from; @c 0 ≤ @p from < @ref getAccountCount
          * @param to Index to move an account to; @c 0 ≤ @p to < @ref getAccountCount
          *
          * The index of the moved account is changed to @p to and indexes of
          * all other accounts between @p from and @p to inclusive are either
          * incremented if @p from > @p to or decremented otherwise. If
          * @p from = @p to, no change happens. */
        virtual void moveAccount(int from,
                                 int to) = 0;
#else
        /** @brief Get ID of the account
          * @return The ID of the account */
        virtual ali::string_const_ref getAccountId() const = 0;

        /** @brief Check if an account is enabled
          * @retval true The account is enabled
          * @retval false The account is disabled */
        virtual bool isAccountEnabled() const = 0;

        /** @brief Get account XML
         * @return The account XML */
        virtual Account::Xml const& getAccount() const = 0;

        /** @brief Delete the account */
        virtual void deleteAccount() = 0;

        /** @brief Include non-standard SIP header
          * @param method Method in which the SIP header should be included to; @c * for all methods
          * @param responseCode Response code the SIP header should be included to:
          *                      - @c xxx for all responses
          *                      - @c Axx for responses with code A00 to A99
          *                      - @c ABx for responses with code AB0 to AB9
          *                      - @c ABC for responses only with code ABC
          * @param name SIP header name; should start with “X-”
          * @param value SIP header value */
        virtual void includeNonStandardSipHeader(ali::string_const_ref method,
                                                 ali::string_const_ref responseCode,
                                                 ali::string_const_ref name,
                                                 ali::string_const_ref value) = 0;

        /** @brief Exclude non-standard SIP header
          * @param methods Method in which the SIP header should not be included
          * @param responseCode Response code the SIP header should not be included to;
          *                     see @ref includeNonStandardSipHeader for the description
          * @param name SIP header name; should start with “X-”
          *
          * This method excludes a non-standard SIP header included with
          * @ref includeNonStandardSipHeader. */
        virtual void excludeNonStandardSipHeader(ali::string_const_ref method,
                                                 ali::string_const_ref responseCode,
                                                 ali::string_const_ref name) = 0;

        /** @brief Get registration state
          * @return The registration state */
        virtual Registrator::State::Type getRegistrationState() const = 0;

#   if defined(SOFTPHONE_BALANCE)
        /** @brief Get the balance record
          * @return The balance record */
        virtual Balance::Record getBalance() const = 0;

        /** @brief Check balance
          *
          * The balance record is updated periodically and after each event but
          * if the user eg. buys credit using web view, you may want to trigger
          * immediate balance check.
          *
          * The balance is checked asynchronously. The observer
          * @ref Observer::onBalance "will be notified" when it is updated. */
        virtual void checkBalance() const = 0;

        /** @brief Query rate record for a number
         * @param number Number to query the rate record for
         * @param callback Callback called when the rate is available
         * @return Handle of the request
         * @retval nullptr Failed to query
         *
         * If the returned handle is destroyed, the query is aborted. */
        virtual ali::auto_ptr<ali::handle> queryRate(ali::generic_peer_address const& number,
                                                     Rate::RateCallback callback) const = 0;

#   endif

        virtual bool isDndActive() const = 0;

#endif

#if defined(SOFTPHONE_VOICEMAIL)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Get voicemail record for an account
          * @param accountId ID of the account to get the voicemail record for
          * @return The voicemail record for the specified account */
        virtual Voicemail::Record getVoicemail(ali::string_const_ref accountId) const = 0;

        /** @brief Get voicemail record for an account
          * @param index Index of the account to get the voicemail record for;
          *                  @c 0 ≤ @p index < @ref getAccountCount
          * @return The voicemail record for the specified account */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        Voicemail::Record getVoicemail(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return getVoicemail(getAccountId(index));
        }
#   else
        /** @brief Get voicemail record
          * @return The voicemail record */
        virtual Voicemail::Record getVoicemail() const = 0;
#   endif
#endif

#if defined(SOFTPHONE_MESSAGING)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Check if an account is ready for messaging
          * @param accountId ID of the account to check
          * @retval true The specified account is ready for messaging at the moment
          * @retval false The specified account is not ready for messaging;
          *               it might or might not be ready later */
        virtual bool readyForMessaging(ali::string_const_ref accountId) const = 0;

        /** @brief Check if an account is ready for messaging
          * @param index Index of the account to check;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @retval true The specified account is ready for messaging at the moment
          * @retval false The specified account is not ready for messaging;
          *               it might or might not be ready later */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool readyForMessaging(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return readyForMessaging(getAccountId(index));
        }
#   else
        /** @brief Check if the account is ready for messaging
          * @retval true The account is ready messaging
          * @retval false The account is not ready for messaging; it might or
          *               might not be ready later */
        virtual bool readyForMessaging() const = 0;
#   endif
#endif

#if defined(SOFTPHONE_VIDEO)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Check if an account is ready video
          * @param accountId ID of the account to check
          * @retval true The specified account is ready for video
          * @retval false The specified account is not ready for video; it might
          *               or might not be ready later */
        virtual bool readyForVideo(ali::string_const_ref accountId) const = 0;

        /** @brief Check if an account is ready video
          * @param index Index of the account to check;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @retval true The specified account is ready for video
          * @retval false The specified account is not ready for video; it might
          *               or might not be ready later */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool readyForVideo(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return readyForVideo(getAccountId(index));
        }
#   else
        /** @brief Check if the account is ready for video
          * @retval true The account is ready for video
          * @retval false The account is not ready for video; it might or might
          *               not be ready later */
        virtual bool readyForVideo() const = 0;
#   endif
#endif

#if defined(SOFTPHONE_CALLTHROUGH)
#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Check if an account is ready for call-through
          * @param accountId ID of the account to check
          * @retval true @todo
          * @retval false @todo */
        virtual bool readyForCallThrough(ali::string_const_ref accountId) const = 0;

        /** @brief Check if an account is ready for call-through
          * @param index Index of the account to check;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @retval true @todo
          * @retval false @todo */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool readyForCallThrough(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return readyForCallThrough(getAccountId(index));
        }
#   else
        /** @brief Check if the account is ready for call-through
          * @retval true @todo
          * @retval false @todo */
        virtual bool readyForCallThrough() const = 0;
#   endif
#endif

#if defined(SOFTPHONE_WEB_CALLBACK)
        /** @todo Document this */
        virtual bool supportsWebCallback(ali::xml::tree const& accountXML) const = 0;

#   if defined(SOFTPHONE_MULTIPLE_ACCOUNTS)
        /** @brief Check if an account is ready for web callback
          * @param accountId ID of the account to check
          * @retval true @todo
          * @retval false @todo */
        virtual bool readyForWebCallback(ali::string_const_ref accountId) const = 0;

        /** @brief Check if an account is ready for web callback
          * @param index Index of the account to check;
          *              @c 0 ≤ @p index < @ref getAccountCount
          * @retval true @todo
          * @retval false @todo */
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        bool readyForWebCallback(int index) const
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
        {
            return readyForWebCallback(getAccountId(index));
        }

#   else
        /** @brief Check if the account is ready for web callback
          * @retval true @todo
          * @retval false @todo */
        virtual bool readyForWebCallback() const = 0;
#   endif
#endif
    };

#if defined(SOFTPHONE_SECURITY)
    /** @brief Methods for call and message security */
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Instance::Security
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        /** @brief Virtual destructor */
        virtual ~Security()
        {}

#   if defined(SOFTPHONE_SECURITY_ZRTP)
        /** @brief ZRTP security methods */
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct ZRTP
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            /** @brief Virtual destructor */
            virtual ~ZRTP()
            {}

            /** @brief Dismiss ZRTP SAS exchange
              * @param call The call to dismiss the ZRTP SAS exchange
              * @retval true Successfully dismissed
              * @retval false Failed */
            virtual bool dismissSas(EventHistory::CallEvent::Pointer const& call) = 0;

            /** @brief Check whether ZRTP SAS is dismissed
              * @param call The call to check whether the ZRTP SAS exchange
              *             was dismissed
              * @retval true The ZRTP SAS exchange was dismissed
              * @retval false The ZRTP SAS exchange was not dismissed */
            virtual bool isSasDismissed(EventHistory::CallEvent::Pointer const& call) const = 0;

            /** @brief Set whether the ZRTP SAS is verified
              * @param call The call to check whether the ZRTP SAS is
              *             verified
              * @param verified Whether the ZRTP SAS is verified
              * @retval true The ZRTP SAS was verified
              * @retval false The ZRTP SAS failed to verify */
            virtual bool setSasVerified(EventHistory::CallEvent::Pointer const& call,
                                        bool verified) = 0;

            /** @brief Toggle whether ZRTP enrollment was accepted
              * @param call The call to toggle the ZRTP enrollment on
              * @retval true Successfully toggled
              * @retval false Failed to toggle */
            virtual bool toggleEnrollmentAccepted(EventHistory::CallEvent::Pointer const& call) = 0;

            /** @brief Check whether ZRTP enrollment is dismissed
              * @param call The call to check whether the ZRTP enrollment is
              *             dismissed
              * @retval true The ZRTP enrollment is dismissed for this call
              * @retval false The ZRTP enrollment is not dismissed for this call */
            virtual bool isEnrollmentDismissed(EventHistory::CallEvent::Pointer const& call) const = 0;

            /** @brief Get ZRTP caller ID
              * @param call The call to get the ZRTP caller ID from
              * @return The ZRTP caller ID of the specified call */
            virtual ali::string getCallerId(EventHistory::CallEvent::Pointer const& call) const = 0;

            /** @brief Set ZRTP caller ID
              * @param call The call to set the ZRTP caller ID to
              * @param name The ZRTP caller ID to set
              * @retval true Successfully set
              * @retval false Failed to set */
            virtual bool setCallerId(EventHistory::CallEvent::Pointer const& call,
                                     ali::string_const_ref name) = 0;

            /** @brief Clear ZRTP cache */
            virtual void clearCache() = 0;
        };

        /** @brief Get ZRTP security methods
          * @return The ZRTP security methods */
        ZRTP * zrtp() const
        {
            return mZrtp.get();
        }

    protected:
        ali::auto_ptr<ZRTP> mZrtp;
#   endif
    };
#endif

#if defined(SOFTPHONE_ADDONS)
    /** @brief Methods manipulating settings and add-ons database */
#else
    /** @brief Methods manipulating settings database */
#endif
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Settings
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~Settings()
        {}

#if defined(SOFTPHONE_ADDONS)
        /** @brief Check if an add-on is enabled
          * @param addon ID of the add-on to check
          * @retval true The specified add-on is enabled
          * @retval false The specified add-on is not enabled or does not exist */
        virtual bool isAddonEnabled(ali::string const& addon) const = 0;

        /** @brief Check if an add-on is enabled
          * @param addon Add-on to check
          * @retval true The specified add-on is enabled
          * @retval false The specified add-on is not enabled or does not exist */
        bool isAddonEnabled(Addon::Type addon) const
        {
            ali::opt_string id = getAddonId(addon);
            return id.is_null() ? false : isAddonEnabled(*id);
        }

        /** @brief Set whether an add-on is enabled
          * @param addon ID of the add-on to set whether it is enabled
          * @param enabled Whether the specified add-on is enabled */
        virtual void setAddonEnabled(ali::string const& addon,
                                     bool enabled) = 0;

        /** @brief Set whether an add-on is enabled
          * @param addon Add-on to set whether it is enabled
          * @param enabled Whether the specified add-on is enabled */
        void setAddonEnabled(Addon::Type addon,
                             bool enabled)
        {
            ali::opt_string id = getAddonId(addon);
            ali_assert(!id.is_null());
            if (id.is_null())
                return;
            return setAddonEnabled(*id, enabled);
        }

        /** @brief Get add-on ID
          * @param addon Add-on to get ID for
          * @return Add-on ID
          * @retval nullptr Add-on is not supported */
        virtual ali::opt_string getAddonId(Addon::Type addon) const = 0;
#endif

        /** @brief Get a value for a private option
          * @param hive The hive to get the value from
          * @param key The option’s name
          * @return The option’s value
          * @retval "" The option does not exist
          *
          * The value is stored persistently in a specified hive. Each hive
          * should use a valid XML tag name.
          *
          * The libSoftphone library does not use these values and you can
          * store any value you would like there. */
        virtual ali::xml::string const& getPrivateValue(ali::string_const_ref hive,
                                                        ali::string_const_ref key) = 0;

        /** @brief Set a value for a private option
          * @param hive The hive to store the value to
          * @param key The option’s name
          * @param value The option’s value
          *
          * The value is stored persistently in a specified hive. Each hive
          * should use a valid XML tag name.
          *
          * The libSoftphone library does not use these values and you can
          * store any value you would like there. */
        virtual void setPrivateValue(ali::string_const_ref hive,
                                     ali::string_const_ref key,
                                     ali::xml::string const& value) = 0;

        virtual Preferences & getPreferences() = 0;
    };

    /** @brief State methods */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::State
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~State()
        {}


        enum Value
        {
            /** Unknown state. */
            None,

            /** The application is in the background. */
            Background,

            /** The application is in the foreground but does not have focus. */
            Inactive,

            /** The application is in the foreground and has focus. */
            Active
        };

        /** @brief Get the current state.
          * @return The current state. */
        virtual State::Value get() const = 0;

        /** @brief Update the state.
          * @param state The new state. */
        virtual void update(State::Value state) = 0;

        /** @brief Terminate the SDK
          * @param timeoutMilis The timeout before which the SDK should terminate.
          *
          * This method tells the SDK to end all calls and unregister all
          * accounts. It should be called on exit.
          *
          * This method is asynchronous. The application can terminate after
          * @ref isTerminated returns true. The SDK will attempt to finish
          * all termination related tasks before the supplied timeout,
          * therefore the timeout should be reasonable - e.g. 10 000 miliseconds.
          *
          * This method does not exit the process or @ref deinit
          * "deinitialize the SDK". It is your responsibility to do so.
          *
          * The SDK @ref respawn "may respawn" if the application wishes to not
          * proceed with the termination. */
        virtual void terminate(unsigned int timeoutMilis = 20000) = 0;

        /** @brief Respawn the SDK
          *
          * This method tells the SDK to abort termination and restart all
          * processing. All accounts will re-register. */
        virtual void respawn() = 0;

        /** @brief Is the SDK terminating?
          * @retval true The SDK is terminating
          * @retval false The SDK is not terminating */
        virtual bool isTerminating() const = 0;

        /** @brief Is the SDK terminated?
          * @retval true The SDK is ready for the application to terminate. So
          *              long, and thanks for all the fish!
          * @retval false The SDK is not ready for the application to
          *               terminate; it is either still ending calls and
          *               unregistering accounts or @ref terminate was not
          *               called yet */
        virtual bool isTerminated() const = 0;
    };

    /** @brief System-specific methods */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::System
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Destructor */
        virtual ~System()
        {}

        /** @brief Get a unique alphanumeric device identifier
          * @return The unique alphanumeric device identifier
          *
          * The returned identifier is platform-specific. It might not be truly
          * unique if the platform, device model or the concrete device does
          * not offer one; in any case some mostly harmless identifier is
          * always returned
          *
          * The unique identifier is limited to 16 alphanumeric characters */
        virtual ali::string_const_ref getUniqueId() const = 0;

        /** @brief Get a set of all supported audio codecs
          * @return Set of all supported audio codecs */
        virtual Rtp::CodecInfoSet getAudioCodecs() const = 0;

#if defined(SOFTPHONE_VIDEO)
        /** @brief Get a set of all supported video codecs
          * @return Set of all supported video codecs */
        virtual Rtp::CodecInfoSet getVideoCodecs() const = 0;
#endif

        /** @brief Dump all settings and accounts stored in the SDK instance.
          * @return All settings and accounts as a XML string.
          *
          * Useful for debugging. */
        virtual ali::string dumpSettings() = 0;
    };

#if defined(SOFTPHONE_VIDEO)
    /** @brief Methods for video calls */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::Video
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        typedef ::Video::Capture::CameraInfo CameraInfo; ///< Camera Info type

        /** @brief Destructor */
        virtual ~Video()
        {}

        /** @brief Enumerate cameras
          * @return Set of all cameras */
        virtual ali::array_set<CameraInfo> enumerateCameras() const = 0;

        /** @brief Get current camera
          * @return Current camera information */
        virtual CameraInfo getCurrentCamera() const = 0;

        /** @brief Switch camera
          * @param cameraId ID of the camera to switch to
          * @retval true Successfully switched
          * @retval false Failed to switch */
        virtual bool switchCamera(ali::string_const_ref cameraId) = 0;

        /** @brief Switch camera
          * @param info Info of the camera to switch to
          * @retval true Successfully switched
          * @retval false Failed to switch */
        bool switchCamera(CameraInfo const& info)
        {
            return switchCamera(info.id);
        }
    };
#endif

    /** @brief Methods for voice unit monitor */
    //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    struct Instance::VoiceUnitMonitor
        //-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    {
        /** @brief Virtual destructor */
        virtual ~VoiceUnitMonitor()
        {}

        /** @brief Subscribe the callback for voice input data
          * @param callback Callback called when input voice data is available
          *
          **/
        virtual void subscribeCallRemoteAudio(Call::RemoteAudioHook callback) = 0;

        /** @brief Subscribe the callback for voice output data
          * @param callback Callback called when output voice data is available
          *
          **/
        virtual void subscribeCallLocalAudio(Call::LocalAudioHook callback) = 0;

        /** @brief Unsubscribe the callback for voice input data
          *
          **/
        virtual void cancelCallRemoteAudio() = 0;

        /** @brief Unsubscribe the callback for voice output data
          *
          **/
        virtual void cancelCallLocalAudio() = 0;
    };
}
