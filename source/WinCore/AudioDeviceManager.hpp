// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_AudioDeviceManager_hpp
#define WinCore_AudioDeviceManager_hpp

#include <map>
#include <WinCore/WinMM.h>
#include <WinCore/AudioDevice.hpp>

class AudioDevice;

class AudioDeviceManager
{
public:
    AudioDeviceManager();
    ~AudioDeviceManager();

    HWAVEOUT Create(const WAVEFORMATEX* wfx, DWORD_PTR callback, DWORD_PTR instance, DWORD flags);
    AudioDevice* Find(HWAVEOUT hwo);
    BOOL Destroy(HWAVEOUT hwo);
    size_t Count() const;
    MMRESULT GetNumDevs();
private:
    AudioDeviceManager(const AudioDeviceManager&);
    AudioDeviceManager& operator=(const AudioDeviceManager&);

    typedef std::map<HWAVEOUT, AudioDevice*> container;
    container _devices;
    size_t    _nextHandle;
};

#endif
