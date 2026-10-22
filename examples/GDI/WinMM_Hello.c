/*
 * -----------------------------------------------------------------------------
 * This example is in the public domain (CC0 1.0 Universal).
 * You can copy, modify, use, and distribute it for any purpose.
 * -----------------------------------------------------------------------------
 */

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <WinCore/WinMM.h>

#define SAMPLE_RATE 44100
#define PI          3.14159265358979

static unsigned now_ms(void)
{
    return (unsigned)((clock() * 1000) / CLOCKS_PER_SEC);
}

static unsigned play_buffer(HWAVEOUT hwo, short* buf, int samples, const char* label)
{
    WAVEHDR hdr;
    unsigned start, elapsed;

    memset(&hdr, 0, sizeof(hdr));
    hdr.lpData = (LPSTR)buf;
    hdr.dwBufferLength = (DWORD)(samples * sizeof(short));

    if (waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
    {
        printf("  [%s] PrepareHeader FAILED\n", label);
        return 0;
    }

    if (waveOutWrite(hwo, &hdr, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
    {
        printf("  [%s] Write FAILED\n", label);
        waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
        return 0;
    }

    start = now_ms();
    while (!(hdr.dwFlags & WHDR_DONE))
    {
        Sleep(5);
    }
    elapsed = now_ms() - start;

    waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));

    printf("  [%s] played %d samples, elapsed=%u ms, final flags=0x%08X\n",
        label, samples, elapsed, (unsigned)hdr.dwFlags);

    return elapsed;
}

static void gen_tone(short* buf, int samples, double freq, double amp)
{
    int i;
    for (i = 0; i < samples; ++i)
    {
        double t = (double)i / (double)SAMPLE_RATE;
        buf[i] = (short)(sin(2.0 * PI * freq * t) * amp);
    }
}

static void gen_chord(short* buf, int samples, double root, double amp)
{
    int i;
    double f1 = root;
    double f2 = root * 5.0 / 4.0;
    double f3 = root * 3.0 / 2.0;

    for (i = 0; i < samples; ++i)
    {
        double t = (double)i / (double)SAMPLE_RATE;
        double s = sin(2.0 * PI * f1 * t)
            + sin(2.0 * PI * f2 * t)
            + sin(2.0 * PI * f3 * t);
        buf[i] = (short)((s / 3.0) * amp);
    }
}

static void gen_vibrato(short* buf, int samples, double center,
    double depth, double vibratoHz, double amp)
{
    int i;
    double phase = 0.0;

    for (i = 0; i < samples; ++i)
    {
        double t = (double)i / (double)SAMPLE_RATE;
        double f = center + depth * sin(2.0 * PI * vibratoHz * t);
        phase += 2.0 * PI * f / (double)SAMPLE_RATE;
        buf[i] = (short)(sin(phase) * amp);
    }
}

static void apply_envelope(short* buf, int samples, int attackMs, int releaseMs)
{
    int i;
    int attackN = (SAMPLE_RATE * attackMs) / 1000;
    int releaseN = (SAMPLE_RATE * releaseMs) / 1000;

    if (attackN > samples) attackN = samples;
    if (releaseN > samples) releaseN = samples;

    for (i = 0; i < attackN; ++i)
    {
        buf[i] = (short)((double)buf[i] * (double)i / (double)attackN);
    }
    for (i = 0; i < releaseN; ++i)
    {
        int idx = samples - 1 - i;
        buf[idx] = (short)((double)buf[idx] * (double)i / (double)releaseN);
    }
}

int main(void)
{
    WAVEFORMATEX wfx;
    HWAVEOUT hwo = NULL;
    int samples;
    short* buf;

    memset(&wfx, 0, sizeof(wfx));
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = SAMPLE_RATE;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (WORD)(wfx.nChannels * wfx.wBitsPerSample / 8);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    printf("=== waveOutGetNumDevs ===\n");
    printf("  -> %u\n", (unsigned)waveOutGetNumDevs());

    printf("=== waveOutOpen ===\n");
    if (waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
    {
        printf("  !!! waveOutOpen FAILED\n");
        return 1;
    }
    printf("  hwo = %p\n", (void*)hwo);

    samples = SAMPLE_RATE * 1;
    buf = (short*)malloc(samples * sizeof(short));
    if (!buf)
    {
        printf("  !!! malloc failed\n");
        waveOutClose(hwo);
        return 2;
    }

    printf("\n=== 1. pure tone 440 Hz ===\n");
    gen_tone(buf, samples, 440.0, 16000.0);
    apply_envelope(buf, samples, 20, 20);
    play_buffer(hwo, buf, samples, "tone");

    printf("\n=== 2. A major chord (440 + 550 + 660 Hz) ===\n");
    gen_chord(buf, samples, 440.0, 16000.0);
    apply_envelope(buf, samples, 20, 20);
    play_buffer(hwo, buf, samples, "chord");

    printf("\n=== 3. vibrato 660 Hz +/- 30 Hz @ 5 Hz ===\n");
    gen_vibrato(buf, samples, 660.0, 30.0, 5.0, 16000.0);
    apply_envelope(buf, samples, 20, 20);
    play_buffer(hwo, buf, samples, "vibrato");

    printf("\n=== 4. sweep 200 -> 1500 Hz ===\n");
    {
        int i;
        double phase = 0.0;
        for (i = 0; i < samples; ++i)
        {
            double t = (double)i / (double)SAMPLE_RATE;
            double f = 200.0 + (1500.0 - 200.0) * t;
            phase += 2.0 * PI * f / (double)SAMPLE_RATE;
            buf[i] = (short)(sin(phase) * 16000.0);
        }
        apply_envelope(buf, samples, 20, 20);
        play_buffer(hwo, buf, samples, "sweep");
    }

    printf("\n=== 5. white noise ===\n");
    {
        int i;
        unsigned int seed = 12345;
        for (i = 0; i < samples; ++i)
        {
            seed = seed * 1103515245u + 12345u;
            buf[i] = (short)((int)(seed >> 16) - 32768) / 2;
        }
        apply_envelope(buf, samples, 20, 20);
        play_buffer(hwo, buf, samples, "noise");
    }

    printf("\n=== waveOutClose ===\n");
    printf("  -> %u\n", (unsigned)waveOutClose(hwo));

    free(buf);
    printf("=== DONE ===\n");
    return 0;
}
