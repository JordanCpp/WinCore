// Copyright (C) 2026 Evgeny Zoshchuk (JordanCpp). Licensed under LGPL-3.0-or-later.

#include <WinCore/Backends/SDL3/WinCore/AudioDevice.hpp>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_timer.h>

AudioDevice::AudioDevice() :
    _stream(NULL),
    _dev(0),
    _opened(false),
    _deviceBufferFrames(0),
    _callback(0),
    _instance(0),
    _flags(0),
    _totalWritten(0),
    _byteRate(0)
{
    SDL_zero(_spec);
}

AudioDevice::~AudioDevice()
{
    Close();
}

MMRESULT AudioDevice::GetNumDevs()
{
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        return 0;
    }

    int count = 0;
    SDL_AudioDeviceID* devices = SDL_GetAudioPlaybackDevices(&count);

    if (devices)
    {
        SDL_free(devices);
    }

    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    return (count > 0) ? (MMRESULT)count : 0;
}

bool AudioDevice::Open(const WAVEFORMATEX* wfx, DWORD_PTR callback, DWORD_PTR instance, DWORD flags)
{
    if (_opened || !wfx)
    {
        return false;
    }

    if (wfx->wFormatTag != WAVE_FORMAT_PCM)
    {
        return false;
    }

    if (wfx->wBitsPerSample != 16)
    {
        return false;
    }

    if (wfx->nChannels != 1 && wfx->nChannels != 2)
    {
        return false;
    }

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        return false;
    }

    SDL_AudioSpec want;
    SDL_zero(want);
    want.format = SDL_AUDIO_S16;
    want.channels = (int)wfx->nChannels;
    want.freq = (int)wfx->nSamplesPerSec;

    _callback = callback;
    _instance = instance;
    _flags = flags;

    _stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &want, StreamCallback, this);
    if (!_stream)
    {
        SDL_Log("WinCore: SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    _dev = SDL_GetAudioStreamDevice(_stream);

    SDL_AudioSpec have;
    SDL_zero(have);
    int sampleFrames = 0;

    if (!SDL_GetAudioDeviceFormat(_dev, &have, &sampleFrames))
    {
        SDL_Log("WinCore: SDL_GetAudioDeviceFormat failed: %s", SDL_GetError());
        SDL_DestroyAudioStream(_stream);
        _stream = NULL;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    _spec = have;
    _deviceBufferFrames = sampleFrames;

    _byteRate = (Uint32)have.freq * (Uint32)have.channels * 2u;
    _totalWritten = 0;

    if (!SDL_ResumeAudioStreamDevice(_stream))
    {
        SDL_Log("WinCore: SDL_ResumeAudioStreamDevice failed: %s", SDL_GetError());
        SDL_DestroyAudioStream(_stream);
        _stream = NULL;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }

    _opened = true;

    if (_callback && (_flags & CALLBACK_FUNCTION))
    {
        typedef void (CALLBACK* WOMCALLBACK)(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
        ((WOMCALLBACK)_callback)((HWAVEOUT)this, WOM_OPEN, 0, 0, _instance);
    }

    return true;
}

void AudioDevice::Close()
{
    if (!_opened)
    {
        return;
    }

    Reset();

    if (_callback && (_flags & CALLBACK_FUNCTION))
    {
        typedef void (CALLBACK* WOMCALLBACK)(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
        ((WOMCALLBACK)_callback)((HWAVEOUT)this, WOM_CLOSE, 0, 0, _instance);
    }

    SDL_DestroyAudioStream(_stream);
    _stream = NULL;
    _dev = 0;
    _opened = false;

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool AudioDevice::PrepareHeader(WAVEHDR* wh)
{
    if (!wh || !wh->lpData || wh->dwBufferLength == 0)
    {
        return false;
    }

    if (wh->dwFlags & WHDR_PREPARED)
    {
        return true;
    }

    wh->dwFlags |= WHDR_PREPARED;
    wh->dwFlags &= ~(WHDR_DONE | WHDR_INQUEUE);

    return true;
}

bool AudioDevice::UnprepareHeader(WAVEHDR* wh)
{
    if (!wh)
    {
        return false;
    }
    if (!(wh->dwFlags & WHDR_PREPARED))
    {
        return false;
    }
    if (wh->dwFlags & WHDR_INQUEUE)
    {
        return false;
    }
    wh->dwFlags &= ~WHDR_PREPARED;

    return true;
}

bool AudioDevice::Write(WAVEHDR* wh)
{
    if (!_opened || !wh || !wh->lpData || wh->dwBufferLength == 0)
    {
        return false;
    }

    if (!(wh->dwFlags & WHDR_PREPARED))
    {
        return false;
    }

    if (wh->dwFlags & WHDR_INQUEUE)
    {
        return false;
    }

    QueuedHeader qh;
    qh.wh = wh;
    qh.remaining = wh->dwBufferLength;
    qh.cursor = wh->lpData;

    _inFlight.push_back(qh);

    wh->dwFlags |= WHDR_INQUEUE;
    wh->dwFlags &= ~WHDR_DONE;

    _totalWritten += wh->dwBufferLength;

    SDL_Log("WinCore: Write queued %p len=%u inflight=%u", (void*)wh, (unsigned)wh->dwBufferLength, (unsigned)_inFlight.size());

    return true;
}

void AudioDevice::Reset()
{
    if (!_stream)
    {
        return;
    }

    SDL_ClearAudioStream(_stream);

    while (!_pendingDone.empty())
    {
        WAVEHDR* wh = _pendingDone.front().wh;
        _pendingDone.pop_front();

        wh->dwFlags &= ~WHDR_INQUEUE;
        wh->dwFlags |= WHDR_DONE;
        NotifyDone(wh);
    }

    while (!_inFlight.empty())
    {
        WAVEHDR* wh = _inFlight.front().wh;
        _inFlight.pop_front();

        wh->dwFlags &= ~WHDR_INQUEUE;
        wh->dwFlags |= WHDR_DONE;
        NotifyDone(wh);
    }

    _totalWritten = 0;
}

bool AudioDevice::GetPosition(MMTIME* mmt, UINT cbmmt)
{
    if (!mmt || cbmmt < sizeof(MMTIME))
    {
        return false;
    }

    Uint64 inStream = _stream ? (Uint64)SDL_GetAudioStreamQueued(_stream) : 0;

    Uint64 inFlightBytes = 0;
    for (std::deque<QueuedHeader>::const_iterator it = _inFlight.begin(); it != _inFlight.end(); ++it)
    {
        inFlightBytes += it->remaining;
    }

    Uint64 pendingBytes = 0;
    for (std::deque<PendingDone>::const_iterator it = _pendingDone.begin(); it != _pendingDone.end(); ++it)
    {
        pendingBytes += it->wh->dwBufferLength;
    }

    Uint64 inWork = inStream + inFlightBytes + pendingBytes;
    Uint64 played = (_totalWritten > inWork) ? (_totalWritten - inWork) : 0;

    if (mmt->wType == TIME_BYTES)
    {
        mmt->u.cb = (DWORD)played;
        return true;
    }
    if (mmt->wType == TIME_MS)
    {
        mmt->u.ms = (DWORD)((played * 1000ull) / (_byteRate ? _byteRate : 1));
        return true;
    }
    if (mmt->wType == TIME_SAMPLES)
    {
        UINT block = (_spec.channels > 0) ? (UINT)_spec.channels * 2u : 2u;
        mmt->u.sample = (DWORD)(played / block);
        return true;
    }
    return false;
}

bool AudioDevice::SetVolume(DWORD vol)
{
    if (!_stream)
    {
        return false;
    }

    WORD left = (WORD)(vol & 0xFFFF);
    WORD right = (WORD)((vol >> 16) & 0xFFFF);

    float fl = (float)left / 65535.0f;
    float fr = (float)right / 65535.0f;
    float avg = (fl + fr) * 0.5f;

    return SDL_SetAudioStreamGain(_stream, avg);
}

bool AudioDevice::GetVolume(DWORD* vol)
{
    if (!_stream || !vol)
    {
        return false;
    }

    float gain = SDL_GetAudioStreamGain(_stream);
    if (gain < 0.0f) gain = 0.0f;
    if (gain > 1.0f) gain = 1.0f;

    WORD w = (WORD)(gain * 65535.0f + 0.5f);
    *vol = ((DWORD)w << 16) | (DWORD)w;
    return true;
}

void SDLCALL AudioDevice::StreamCallback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount)
{
    (void)total_amount;

    AudioDevice* self = static_cast<AudioDevice*>(userdata);
    if (!self || additional_amount <= 0)
    {
        return;
    }

    Uint32 deviceLatencyMs = 0;
    if (self->_spec.freq > 0 && self->_deviceBufferFrames > 0)
    {
        deviceLatencyMs = (Uint32)((Uint64)self->_deviceBufferFrames * 1000 / (Uint64)self->_spec.freq);
    }

    int remaining = additional_amount;

    while (remaining > 0 && !self->_inFlight.empty())
    {
        QueuedHeader& qh = self->_inFlight.front();

        int take = (int)qh.remaining;
        if (take > remaining) take = remaining;

        if (!SDL_PutAudioStreamData(stream, qh.cursor, take))
        {
            SDL_Log("WinCore: PutAudioStreamData in callback failed: %s", SDL_GetError());
            break;
        }

        qh.cursor += take;
        qh.remaining -= (DWORD)take;
        remaining -= take;

        if (qh.remaining == 0)
        {
            WAVEHDR* wh = qh.wh;
            self->_inFlight.pop_front();

            PendingDone pd;
            pd.wh = wh;
            pd.atMs = SDL_GetTicks() + deviceLatencyMs;
            self->_pendingDone.push_back(pd);
        }
    }

    Uint64 nowMs = SDL_GetTicks();

    while (!self->_pendingDone.empty())
    {
        PendingDone& pd = self->_pendingDone.front();
        if (pd.atMs > nowMs)
        {
            break;
        }

        WAVEHDR* wh = pd.wh;
        self->_pendingDone.pop_front();

        wh->dwFlags &= ~WHDR_INQUEUE;
        wh->dwFlags |= WHDR_DONE;

        self->NotifyDone(wh);
    }
}

void AudioDevice::NotifyDone(WAVEHDR* wh)
{
    if (!_callback || !(_flags & CALLBACK_FUNCTION))
    {
        return;
    }

    typedef void (CALLBACK* WOMCALLBACK)(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
    ((WOMCALLBACK)_callback)((HWAVEOUT)this, WOM_DONE, (DWORD_PTR)wh, 0, _instance);
}
