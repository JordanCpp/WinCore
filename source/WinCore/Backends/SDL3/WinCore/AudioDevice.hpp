// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#ifndef WinCore_SDL3_AudioDevice_hpp
#define WinCore_SDL3_AudioDevice_hpp

#include <deque>
#include <SDL3/SDL_audio.h>
#include <WinCore/WinMM.h>

class AudioDevice
{
public:
    AudioDevice();
    ~AudioDevice();

    static MMRESULT GetNumDevs();

    bool Open(const WAVEFORMATEX* wfx, DWORD_PTR callback,
        DWORD_PTR instance, DWORD flags);
    void Close();

    bool PrepareHeader(WAVEHDR* wh);
    bool UnprepareHeader(WAVEHDR* wh);
    bool Write(WAVEHDR* wh);
    void Reset();
    bool GetPosition(MMTIME* mmt, UINT cbmmt);
    bool SetVolume(DWORD vol);
    bool GetVolume(DWORD* vol);

private:
    AudioDevice(const AudioDevice&);
    AudioDevice& operator=(const AudioDevice&);

    static void SDLCALL StreamCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount);

    void NotifyDone(WAVEHDR* wh);

    struct QueuedHeader
    {
        WAVEHDR* wh;
        DWORD    remaining;
        LPSTR    cursor;
    };

    struct PendingDone
    {
        WAVEHDR* wh;
        Uint64   atMs;
    };

    SDL_AudioStream* _stream;
    SDL_AudioDeviceID  _dev;
    SDL_AudioSpec      _spec;
    bool               _opened;
    int                _deviceBufferFrames;

    DWORD_PTR          _callback;
    DWORD_PTR          _instance;
    DWORD              _flags;

    std::deque<QueuedHeader> _inFlight;
    std::deque<PendingDone>  _pendingDone;

    Uint64             _totalWritten;
    Uint32             _byteRate;
};

#endif