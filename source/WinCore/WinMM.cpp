// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Application.hpp>
#include <WinCore/AudioDevice.hpp>

MMRESULT waveOutGetNumDevs(void)
{
    return MainApplication()._audioDevices.GetNumDevs();
}

MMRESULT waveOutOpen(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen)
{
    if (!phwo || !pwfx)
    {
        MainApplication().SetLastError(ERROR_INVALID_PARAMETER);
        return MMSYSERR_INVALPARAM;
    }

    if (uDeviceID != WAVE_MAPPER)
    {
        MainApplication().SetLastError(ERROR_INVALID_PARAMETER);
        return MMSYSERR_BADDEVICEID;
    }

    if (pwfx->wFormatTag != WAVE_FORMAT_PCM || pwfx->wBitsPerSample != 16)
    {
        MainApplication().SetLastError(ERROR_NOT_SUPPORTED);
        return MMSYSERR_NOTSUPPORTED;
    }

    HWAVEOUT h = MainApplication()._audioDevices.Create(pwfx, dwCallback, dwInstance, fdwOpen);
    if (!h)
    {
        MainApplication().SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return MMSYSERR_NODRIVER;
    }

    *phwo = h;
    MainApplication().SetLastError(ERROR_SUCCESS);
    return MMSYSERR_NOERROR;
}

MMRESULT waveOutClose(HWAVEOUT hwo)
{
    if (!hwo)
    {
        return MMSYSERR_INVALHANDLE;
    }
    if (!MainApplication()._audioDevices.Destroy(hwo))
    {
        return MMSYSERR_INVALHANDLE;
    }
    return MMSYSERR_NOERROR;
}

MMRESULT waveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    if (!hwo || !pwh || cbwh < sizeof(WAVEHDR))
    {
        return MMSYSERR_INVALPARAM;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    return dev->PrepareHeader(pwh) ? MMSYSERR_NOERROR : MMSYSERR_INVALPARAM;
}

MMRESULT waveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    if (!hwo || !pwh || cbwh < sizeof(WAVEHDR))
    {
        return MMSYSERR_INVALPARAM;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    if (pwh->dwFlags & WHDR_INQUEUE)
    {
        return WAVERR_STILLPLAYING;
    }
    if (!(pwh->dwFlags & WHDR_PREPARED))
    {
        return WAVERR_UNPREPARED;
    }
    return dev->UnprepareHeader(pwh) ? MMSYSERR_NOERROR : MMSYSERR_INVALPARAM;
}

MMRESULT waveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh)
{
    if (!hwo || !pwh || cbwh < sizeof(WAVEHDR))
    {
        return MMSYSERR_INVALPARAM;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    if (!(pwh->dwFlags & WHDR_PREPARED))
    {
        return WAVERR_UNPREPARED;
    }
    if (pwh->dwFlags & WHDR_INQUEUE)
    {
        return WAVERR_STILLPLAYING;
    }
    return dev->Write(pwh) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;
}

MMRESULT waveOutReset(HWAVEOUT hwo)
{
    if (!hwo)
    {
        return MMSYSERR_INVALHANDLE;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    dev->Reset();
    return MMSYSERR_NOERROR;
}

MMRESULT waveOutGetPosition(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt)
{
    if (!hwo || !pmmt)
    {
        return MMSYSERR_INVALPARAM;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    return dev->GetPosition(pmmt, cbmmt) ? MMSYSERR_NOERROR : MMSYSERR_INVALPARAM;
}

MMRESULT waveOutSetVolume(HWAVEOUT hwo, DWORD dwVolume)
{
    if (!hwo)
    {
        return MMSYSERR_INVALHANDLE;
    }
    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }
    return dev->SetVolume(dwVolume) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;
}

MMRESULT waveOutGetVolume(HWAVEOUT hwo, LPDWORD pdwVolume)
{
    if (!hwo || !pdwVolume)
    {
        return MMSYSERR_INVALPARAM;
    }

    AudioDevice* dev = MainApplication()._audioDevices.Find(hwo);
    if (!dev)
    {
        return MMSYSERR_INVALHANDLE;
    }

    return dev->GetVolume(pdwVolume) ? MMSYSERR_NOERROR : MMSYSERR_ERROR;
}
