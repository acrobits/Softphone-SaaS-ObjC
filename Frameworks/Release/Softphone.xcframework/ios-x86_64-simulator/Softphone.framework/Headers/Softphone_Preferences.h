/*
 *  Softphone_Preferences.h
 *  libsoftphone
 *
 *  Copyright (c) 2013 - 2018 Acrobits s.r.o. All rights reserved.
 *
 */

#pragma once

#include "RingerSetting.h"


namespace PreferenceKeys
{
    struct Keys;
    template <typename Type>
    class GenericBasicROKey;
    template <typename Type>
    class GenericBasicKey;
}

namespace Softphone
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Preferences
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        template <typename Type>
        class ROKey
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
        public:
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            ROKey(PreferenceKeys::GenericBasicROKey<Type> & key)
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                : mKey{key}
            {}

            Type get() const;
            Type getDefault( void ) const;
            void overrideDefault(Type const& value);

        protected:
            PreferenceKeys::GenericBasicROKey<Type> & mKey;
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        template <typename Type>
        class Key
            : public ROKey<Type>
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
        public:
            typedef ROKey<Type> Super;

            Key(PreferenceKeys::GenericBasicKey<Type> & key)
                : Super{key}
            {}

            void set(Type const& value);
            void reset();
        };


        explicit Preferences(PreferenceKeys::Keys & keys);

        /**
         * Whether to log traffic.
         * Default to true in debug build or false in release build.
         */
        Key<bool>                trafficLogging;
        
        /**
         * Whether to capture SIP messages.
         * Default to false.
         */
        Key<bool>                sipMessageCapture;
        
        Key<IncomingCallsMode>   incomingCallsMode;
        
        /**
         * Whether to enable acoustic echo suppression.
         * Defaults to true.
         */
        Key<bool>                echoSuppressionEnabled;
        
        Key<ali::string>         networkUse;
        
        /**
         * Microphone volume.
         */
        Key<int>                 volumeBoostMicrophone;
        
        /**
         * Call playback volume.
         */
        Key<int>                 volumeBoostPlayback;
        
        /**
         * Keypad volume
         * Defaults to 10.
         */
        Key<int>                 keypadVolume;

        /**
         * User agent used in network traffic. When empty, the default is used.
         * This value can be logged on the network and is visible in the traffic log.
         */
        ROKey<ali::string>       userAgentOverride;
        
        /**
         * Whether to enable voicemail.
         * Defaults to false.
         */
        ROKey<bool>              voicemailEnabled;
        
        /**
         * DSCP 46 (EF)
         */
        Key<int>                 audioRtpTosByte;
        
        /**
         * DSCP 34 (AF41)
         */
        Key<int>                 videoRtpTosByte;
        
        /**
         * DSCP 26 (AF31)
         */
        Key<int>                 rtcpTosByte;

        /**
         * Whether to use proximity sensor to switch to speaker mode during a call.
         * Possible values on, off and video
         */
        Key<ali::string>         proximityBasedSpeakerMode;

        /**
         * List of contact sources.
         * A comma separated string.
         * Possible values ab, mockup, dav and ws
         */
        Key<ali::string>         contactSources;

        /**
         * Value used to sort the contacts
         * Possible values lastFirst and firstLast
         */
        Key<ali::string>         contactSortOrder;

        /**
         * Do not disturb rules.
         */
        Key<ali::xml::tree>      doNotDisturb;

        /**
         * Whether immediate Do not disturb applies.
         */
        Key<bool>                immediateDnd;

        /**
         * Whether to enable Background Noise Suppression.
         * Defaults to true on ARM with NEON extension, false otherwise.
         */
        Key<bool>                backgroundNoiseSuppression;

        /**
         * Whether to play dtmf if the call is muted.
         * Defaults to false
         */
        Key<bool>                playDtmfEvenIfMuted;

        /**
         * A unique string generated when the app is first installed.
         * It can be used to identify this particular installation.
         * When the app is updated, this value is preserved, but uninstall followed by reinstall will change this ID.
         * It is not based on any hardware device identifier.
         */
        Key<ali::string>         installationSalt;

        //ROKey<bool>            switchAudioSessionDuringTrying;

        Key<RingerSetting>       ringerSettingOverride;

        /**
         * Whether to verify certificates.
         * If set to false, all SSL/TLS certificates are trusted.
         * Defaults to true.
         */
        Key<bool>                verifyCertificates;
        
        /**
         * Whether usage of the SIPIS server is disabled.
         * When this is set to true, push won't work but the user's
         * credentials won't be transferred to the SIPIS server.
         */
        ROKey<bool>              sipisDisabled;
        
        /**
         * Whether push notifications are enabled.
         * Defaults to true.
         */
        ROKey<bool>              pushNotificationsEnabled;
        
        /**
         * Maximum number of concurrent calls.
         */
        ROKey<int>               maxNumberOfConcurrentCalls;
        
#if defined(__APPLE__)
        Key<bool>                bgrUsesPushRingtone;

        /**
         * Whether to use PushKit for push notifications
         * <p>
         * Defaults to true.
         */
        ROKey<bool>              pushNotificationsUsePushKit;
        
        /**
         * Use pushkit pushes for incoming calls and remote-notification pushes for other events
         * <p>
         * Defaults to true on ios13+ and false (use pushkit for everything) on older iOSes
         */
        ROKey<bool>              dualPushes;
#endif

        /**
         * Whether to play warning beep when recording.
         * When recording a call, a warning beep will be inserted into the outgoing audio stream to
         * warn the other side that they are recorded.
         * Local legislation may require audible notification when recording calls. Check the
         * applicable law before turning off.
         * Defaults to true.
         */
        Key<bool>                recordWarningBeep;

        /**
         * The duration for dtmf tone to be played.
         * Defaults to 1000.
         */
        Key<int>                 dtmfDurationInMilliseconds;

        /**
         * The gap between dtmf tones played via dtmf program
         * Defaults to 125.
         */
        Key<int>                 dtmfMinimumGapInMilliseconds;

        /**
         * Duration for pause symbol
         * Defaults to 1000.
         */
        Key<int>                 dtmfPauseInMilliseconds;
#if defined(__APPLE__)
        /**
         * Whether to use CallKit or not.
         * CallKit handles the action by presenting the system interface for answering the call.
         * Defaults to true.
         */
        Key<bool>                useCallKit;

        /**
         * Whether to report call that are unanswered while using CallKit.
         * Defaults to true.
         */
        ROKey<bool>              reportCallKitFailuresAsUnanswered;
        
        /**
         * Sets the preferred IO buffer duration in milliseconds for calls.
         * Defaults to 12.
         */
        Key<int>                audioSessionIOBufferDurationInMs;

#endif
        /**
         * Missed call count.
         */
        Key<int>                 missedCount;
    };

    extern template class Preferences::ROKey<bool>;
    extern template class Preferences::ROKey<int>;
    extern template class Preferences::ROKey<ali::string>;
    extern template class Preferences::ROKey<RingerSetting>;
    extern template class Preferences::Key<bool>;
    extern template class Preferences::Key<int>;
    extern template class Preferences::Key<ali::string>;
    extern template class Preferences::Key<RingerSetting>;
}

