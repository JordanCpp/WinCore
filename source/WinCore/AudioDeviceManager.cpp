// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/AudioDeviceManager.hpp>
#include <WinCore/AudioDevice.hpp>

AudioDeviceManager::AudioDeviceManager() :
    _nextHandle(0x20000)
{
}

AudioDeviceManager::~AudioDeviceManager()
{
    for (container::iterator i = _devices.begin(); i != _devices.end(); ++i)
    {
        delete i->second;
    }
    _devices.clear();
}

HWAVEOUT AudioDeviceManager::Create(const WAVEFORMATEX* wfx, DWORD_PTR callback, DWORD_PTR instance, DWORD flags)
{
    AudioDevice* dev = new (std::nothrow) AudioDevice();
    if (!dev)
    {
        return NULL;
    }

    if (!dev->Open(wfx, callback, instance, flags))
    {
        delete dev;
        return NULL;
    }

    HWAVEOUT handle = (HWAVEOUT)_nextHandle++;
    _devices.insert(std::make_pair(handle, dev));

    return handle;
}

AudioDevice* AudioDeviceManager::Find(HWAVEOUT hwo)
{
    container::iterator i = _devices.find(hwo);
    return (i != _devices.end()) ? i->second : NULL;
}

BOOL AudioDeviceManager::Destroy(HWAVEOUT hwo)
{
    container::iterator i = _devices.find(hwo);
    if (i == _devices.end())
    {
        return FALSE;
    }

    AudioDevice* dev = i->second;
    _devices.erase(i);
    delete dev;

    return TRUE;
}

size_t AudioDeviceManager::Count() const
{
    return _devices.size();
}

MMRESULT AudioDeviceManager::GetNumDevs()
{
    return AudioDevice::GetNumDevs();
}