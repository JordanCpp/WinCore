/* Copyright(C) 2026 Evgeny Zoshchuk(JordanCpp).Licensed under LGPL - 3.0 - or -later. */

#ifndef WinCore_WinMM_h
#define WinCore_WinMM_h

#include <WinCore/Config.h>
#include <WinCore/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct HWAVEOUT__* HWAVEOUT;
    typedef HWAVEOUT* LPHWAVEOUT;

    typedef struct WAVEFORMATEX_
    {
        WORD  wFormatTag;
        WORD  nChannels;
        DWORD nSamplesPerSec;
        DWORD nAvgBytesPerSec;
        WORD  nBlockAlign;
        WORD  wBitsPerSample;
        WORD  cbSize;
    } WAVEFORMATEX, * LPWAVEFORMATEX, * LPCWAVEFORMATEX;

    typedef struct WAVEHDR_
    {
        LPSTR  lpData;
        DWORD  dwBufferLength;
        DWORD  dwBytesRecorded;
        DWORD_PTR dwUser;
        DWORD  dwFlags;
        DWORD  dwLoops;
        struct WAVEHDR_* lpNext;
        DWORD_PTR reserved;
    } WAVEHDR, * LPWAVEHDR;

    typedef struct MMTIME_
    {
        UINT wType;
        union
        {
            DWORD ms;
            DWORD sample;
            DWORD cb;
            DWORD ticks;
            struct
            {
                BYTE hour;
                BYTE min;
                BYTE sec;
                BYTE frame;
                BYTE fps;
                BYTE dummy;
                BYTE pad[2];
            } smpte;
            struct
            {
                DWORD songptrpos;
            } midi;
        } u;
    } MMTIME, * LPMMTIME;

    typedef struct _MMTIME_BYTES { UINT wType; DWORD cb;  DWORD pad; } MMTIME_BYTES;
    typedef struct _MMTIME_MS { UINT wType; DWORD ms;  DWORD pad; } MMTIME_MS;
    typedef struct _MMTIME_SAMPLE { UINT wType; DWORD sample; DWORD pad; } MMTIME_SAMPLE;

    typedef UINT MMRESULT;

#define WAVE_MAPPER             ((UINT)-1)
#define MAXPNAMELEN             32

#define WAVE_FORMAT_PCM         0x0001

#define WHDR_DONE               0x00000001
#define WHDR_PREPARED           0x00000002
#define WHDR_BEGINLOOP          0x00000004
#define WHDR_ENDLOOP            0x00000008
#define WHDR_INQUEUE            0x00000010

#define CALLBACK_NULL           0x00000000
#define CALLBACK_WINDOW         0x00010000
#define CALLBACK_TASK           0x00020000
#define CALLBACK_FUNCTION       0x00030000
#define CALLBACK_THREAD         (CALLBACK_TASK)
#define CALLBACK_EVENT          0x00050000

#define WAVE_FORMAT_QUERY       0x0001
#define WAVE_ALLOWSYNC          0x0002
#define WAVE_MAPPED             0x0004
#define WAVE_FORMAT_DIRECT      0x0008
#define WAVE_FORMAT_DIRECT_QUERY (WAVE_FORMAT_QUERY | WAVE_FORMAT_DIRECT)

#define WOM_OPEN                0x3BB
#define WOM_CLOSE               0x3BC
#define WOM_DONE                0x3BD

/* MMRESULT */
#define MMSYSERR_NOERROR        0
#define MMSYSERR_ERROR          1
#define MMSYSERR_BADDEVICEID    2
#define MMSYSERR_NOTENABLED     3
#define MMSYSERR_ALLOCATED      4
#define MMSYSERR_INVALHANDLE    5
#define MMSYSERR_NODRIVER       6
#define MMSYSERR_NOMEM          7
#define MMSYSERR_NOTSUPPORTED   8
#define MMSYSERR_BADERRNUM      9
#define MMSYSERR_INVALFLAG      10
#define MMSYSERR_INVALPARAM     11
#define MMSYSERR_HANDLEBUSY     12
#define MMSYSERR_INVALIDALIAS   13

#define WAVERR_BADFORMAT        32
#define WAVERR_STILLPLAYING     33
#define WAVERR_UNPREPARED       34
#define WAVERR_SYNC             35

/* MMTIME wType */
#define TIME_MS                 0x0001
#define TIME_SAMPLES            0x0002
#define TIME_BYTES              0x0004
#define TIME_SMPTE              0x0008
#define TIME_MIDI               0x0010
#define TIME_TICKS              0x0020

/* Timer */
#define TIMERR_NOERROR          0
#define TIMERR_NOCANDO          97

/* Volume (16-bit per channel packed) */
#define WAVE_VOLUME_LEFT(vol)   ((WORD)((vol) & 0xFFFF))
#define WAVE_VOLUME_RIGHT(vol)  ((WORD)(((vol) >> 16) & 0xFFFF))

/* ---- API ---- */
    WINCORE_API MMRESULT waveOutGetNumDevs(void);
    WINCORE_API MMRESULT waveOutOpen(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
    WINCORE_API MMRESULT waveOutClose(HWAVEOUT hwo);
    WINCORE_API MMRESULT waveOutPrepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
    WINCORE_API MMRESULT waveOutUnprepareHeader(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
    WINCORE_API MMRESULT waveOutWrite(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
    WINCORE_API MMRESULT waveOutReset(HWAVEOUT hwo);
    WINCORE_API MMRESULT waveOutGetPosition(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt);
    WINCORE_API MMRESULT waveOutSetVolume(HWAVEOUT hwo, DWORD dwVolume);
    WINCORE_API MMRESULT waveOutGetVolume(HWAVEOUT hwo, LPDWORD pdwVolume);

#ifdef __cplusplus
}
#endif

#endif
