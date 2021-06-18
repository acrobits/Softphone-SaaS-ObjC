//
//  PushNotificationCompletion.h
//  libsoftphone
//
//  Created by Stanislav Kutil on 19/08/2019.
//

#pragma once

namespace Softphone
{
    namespace PushNotificationProcessor
    {
        struct PushNotificationCompletionInfo
        {
            enum Result
            {
                Failed = 0,
                NoData = 1,
                NewData = 2
            } result{Failed};
            
            PushNotificationCompletionInfo() {};
            PushNotificationCompletionInfo(Result result) : result{result} {};
            
            PushNotificationCompletionInfo& operator +=(PushNotificationCompletionInfo const& info)
            {
                result = ali::maxi(result, info.result);
                return *this;
            }
        };        
    }
}
