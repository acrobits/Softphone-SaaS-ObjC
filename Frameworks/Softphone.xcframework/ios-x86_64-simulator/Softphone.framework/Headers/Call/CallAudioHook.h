#pragma once

#include "Softphone/EventHistory/CallEvent.h"
#include "Softphone/VoiceUnitMonitor.h"
#include "ali/ali_string.h"
#include "ali/ali_callback.h"

namespace Call
{
    typedef ali::callback<VoiceUnitMonitor::ProcessingResult(Softphone::EventHistory::CallEvent::Pointer const& callId,
                                                             int samplingRate, short const* data, int len)> RemoteAudioHook;
    typedef ali::callback<VoiceUnitMonitor::ProcessingResult(ali::string const& groupId,
                                                             int samplingRate, short const* data, int len)> LocalAudioHook;
}
