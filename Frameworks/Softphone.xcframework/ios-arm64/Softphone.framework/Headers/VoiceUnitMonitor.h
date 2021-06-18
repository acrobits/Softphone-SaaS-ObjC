/*
 *  VoiceUnitMonitor.h
 *  softphone
 *
 *  Created by Jiri Kral on 5/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class VoiceUnitMonitor
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
{
public:
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    struct ProcessingResult
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        enum Type
        //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        {
            KeepData = 0,
            DiscardData = 1
        };

        ProcessingResult() = default;
        
        ProcessingResult(Type v)
        :value(v){}
        
        // ******************************************************************
        ali::c_string_const_ref toString()
        // ******************************************************************
        {
            using ali::operator""_s;

            switch ( value )
            {
                case KeepData:      return "KeepData"_s;
                case DiscardData:   return "DiscardData"_s;
            }

            return "<invalid>"_s;
        }

        // ******************************************************************
        ProcessingResult fromString(ali::string_const_ref v)
        // ******************************************************************
        {
            using ali::operator""_s;

            if (v == "DiscardData"_s)
                return {DiscardData};
            else
                return {KeepData};
        }
        
        bool isDiscardData() const {return value == DiscardData;}
        bool isKeepData() const {return value == KeepData;}

        Type value{KeepData};
    };

    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    virtual ~VoiceUnitMonitor()
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
    {}

    virtual ProcessingResult onInputData(int const samplingRate, short const * data, int len) = 0;
    virtual ProcessingResult onOutputData(int const samplingRate, short const * data, int len) = 0;
};


