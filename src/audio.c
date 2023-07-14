#include "../include/audio.h"

struct note keyboard[] = {
    {'q', 261.63, "C4"},
    {'2', 277.18, "C#4"},
    {'w', 293.66, "D4"},
    {'3', 311.13, "D#4"},
    {'e', 329.63, "E4"},
    {'r', 349.23, "F4"},
    {'5', 369.99, "F#4"},
    {'t', 392.00, "G4"},
    {'6', 415.30, "G#4"},
    {'y', 440.00, "A4"},
    {'7', 466.16, "A#4"},
    {'u', 493.88, "B4"},
    {'i', 523.25, "C5"},
    {'9', 554.37, "C#5"},
    {'o', 587.34, "D5"},
    {'0', 622.26, "D#5"},

    {'z', 130.81, "C3"},
    {'s', 138.59, "C#3"},
    {'x', 146.83, "D3"},
    {'d', 155.56, "D#3"},
    {'c', 164.81, "E3"},
    {'v', 174.61, "F3"},
    {'g', 184.99, "F#3"},
    {'b', 195.99, "G3"},
    {'h', 207.65, "G#3"},
    {'n', 219.99, "A3"},
    {'j', 233.08, "A#3"},
    {'m', 246.93, "B3"},
    {',', 261.62, "C4"},
};

const int number_of_notes = (int)(sizeof(keyboard) / sizeof(struct note));

void *play(void *arg)
{
    struct params *args = (struct params *)arg;
    float freq = args->freq;
    float duration = args->duration;
    int sampling_rate = args->sampling_rate;

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    int size = (duration + RELEASE_TIME) * sampling_rate; // Extend buffer for release
    short buf[size];

    for (int i = 0; i < size; i++)
    {
        float volume = VOLUME;
        buf[i] = 0;

        if (i < ATTACK_TIME * sampling_rate)
        {
            volume = (i / (ATTACK_TIME * sampling_rate)) * VOLUME;
        }
        else if (i < duration * sampling_rate)
        {
            volume = VOLUME;
        }
        else
        {
            volume *= 1.0 - (i - duration * sampling_rate) / (RELEASE_TIME * sampling_rate);
        }

        // buf[i] += sine(i, volume, freq);

        buf[i] += piano(i, volume, freq, sampling_rate);

        // buf[i] += sawtooth(i, volume, freq);

        // buf[i] += triangle(i, volume, freq);

        // buf[i] += square(i, volume, freq);
    }

    snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, sampling_rate, 1, 50000); // 50ms

    frames = snd_pcm_writei(handle, buf, size);

    if (frames < 0)
    {
        frames = snd_pcm_recover(handle, frames, 0);
    }

    if (frames < 0)
    {
        printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
        exit(EXIT_FAILURE);
    }

    snd_pcm_close(handle);

    return NULL;
}
