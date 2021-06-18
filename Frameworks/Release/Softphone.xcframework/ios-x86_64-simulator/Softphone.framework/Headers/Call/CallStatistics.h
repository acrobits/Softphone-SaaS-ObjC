/*
 *  CallStatistics.h
 *  libsoftphone
 *
 *  Created by Jiri Kral on 8/9/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ali/ali_utility.h"

namespace Call
{
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct Statistics
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        struct Traffic
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            struct Counts
            //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            {
                //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                Counts()
                //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                    : packetCount(0)
                    , octetCount(0)
                {}

                //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                Counts(unsigned long packetCount,
                       unsigned long octetCount)
                //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
                    : packetCount(packetCount)
                    , octetCount(octetCount)
                {}

                unsigned long packetCount;
                unsigned long octetCount;
            };

            Counts incoming;
            Counts outgoing;
        };

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Statistics()
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            : clockRateInHertz(0)
            , maxJitter(0)
            , avgJitter(0)
            , networkPacketLossPercentage(0)
            , jitterBufferPacketLossPercentage(0)
            , meteredNetworkJitter(0)
            , meteredAudioJitter(0)
            , lastRoundTripTimeInMilliseconds(-1)
            , avgRoundTripTimeInMilliseconds(-1)
        {}

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        Statistics(Traffic traffic,
                   int networkPacketLossPercentage,
                   int jitterBufferPacketLossPercentage,
                   int clockRateInHertz,
                   double maxJitter,
                   double avgJitter,
                   long long meteredNetworkJitter,
                   long long meteredAudioJitter,
                   int roundTripTimeInMilliseconds)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
            : traffic(traffic)
            , clockRateInHertz(clockRateInHertz)
            , maxJitter(maxJitter)
            , avgJitter(avgJitter)
            , networkPacketLossPercentage(networkPacketLossPercentage)
            , jitterBufferPacketLossPercentage(jitterBufferPacketLossPercentage)
            , meteredNetworkJitter(meteredNetworkJitter)
            , meteredAudioJitter(meteredAudioJitter)
            , lastRoundTripTimeInMilliseconds(roundTripTimeInMilliseconds)
            , avgRoundTripTimeInMilliseconds(roundTripTimeInMilliseconds)
        {}

        Traffic traffic;

        // defines timestamp unit for the following jitter statistics
        int clockRateInHertz;
		
        // jitter calculated by methods described at http://tools.ietf.org/html/rfc3550#appendix-A.8
        // the values are in timestamp units
        double maxJitter;
        double avgJitter;

        // packet loss detected only by examining 'holes' in packet sequence numbers sequence
        int networkPacketLossPercentage;

        // packet loss percentage as discovered by adaptive jitter buffer. Includes packets
        // which arrived too late to be played
        int jitterBufferPacketLossPercentage;

        // network jitter detected by adaptive jitter buffer. In microseconds. It contains the maximum
        // difference between time when the packet was expected and time it actually arrived,
        // measured over a time period
        long long meteredNetworkJitter;

        // audio jitter detected by adaptive jitter buffer. In microseconds. It contains the maximum
        // difference between time when we expected audio subsystem will ask for chunk of audio data
        // to be played and the actual time it happened
        long long meteredAudioJitter;

        // round-trip time obtained from an exchange of RTCP packets. In case it can't be obtained,
        // the value is -1. This is the last measured value.
        int lastRoundTripTimeInMilliseconds;

        // round-trip time obtained from an exchange of RTCP packets. In case it can't be obtained,
        // the value is -1. This is the average from the last 12 values.
        int avgRoundTripTimeInMilliseconds;

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        void takeWorseValues(Statistics const& other)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            if ( other.avgJitter > avgJitter )
            {
                clockRateInHertz = other.clockRateInHertz;
                maxJitter = other.maxJitter;
			    avgJitter = other.avgJitter;
            }

            networkPacketLossPercentage = ali::maxi(networkPacketLossPercentage, other.networkPacketLossPercentage);
            jitterBufferPacketLossPercentage = ali::maxi(jitterBufferPacketLossPercentage, other.jitterBufferPacketLossPercentage);

            meteredAudioJitter = ali::maxi(meteredAudioJitter, other.meteredAudioJitter);
            meteredNetworkJitter = ali::maxi(meteredNetworkJitter, other.meteredNetworkJitter);
            
            lastRoundTripTimeInMilliseconds = ali::maxi(lastRoundTripTimeInMilliseconds,other.lastRoundTripTimeInMilliseconds);
            avgRoundTripTimeInMilliseconds = ali::maxi(avgRoundTripTimeInMilliseconds,other.avgRoundTripTimeInMilliseconds);
        }

        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        static Statistics takeWorseValues(Statistics first,
                                          Statistics const& second)
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            first.takeWorseValues(second);
            return first;
        }
    };
}
