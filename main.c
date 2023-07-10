#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include <termios.h>
#include <unistd.h>

#define SAMPLING_RATE 44100
#define VOLUME 10000 * 0.6
#define RELEASE_TIME 1 // Release time in seconds
#define ATTACK_TIME 0  // Attack time in seconds

typedef struct _params
{
    float freq;
    float duration;
} params;

struct note
{
    char key;
    float frequency;
    char *mkey;
};

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

#define NUMBER_OF_NOTES (int)(sizeof(keyboard) / sizeof(struct note))

int sign(double num)
{
    if (num > 0)
        return 1;
    else if (num < 0)
        return -1;
    else
        return 0;
}

// Function to set terminal to raw mode
void set_mode(int want_key)
{
    static struct termios old, new;

    if (!want_key)
    {
        // Restore the original settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        return;
    }

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &old);

    // Set the terminal to raw mode
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

short sine(int i, float volume, float freq)
{
    return volume * sin(2 * M_PI * freq * ((float)i / SAMPLING_RATE));
}

short sawtooth(int i, float volume, float freq)
{
    return volume * (2 / M_PI) * ((float)i / SAMPLING_RATE * freq - floor(0.5 + (float)i / SAMPLING_RATE * freq));
}

short triangle(int i, float volume, float freq)
{
    return volume * 2 * fabs(2 * (((float)i / SAMPLING_RATE * freq) - floor(0.5 + (float)i / SAMPLING_RATE * freq))) - 1;
}

short square(int i, float volume, float freq)
{
    return volume * sign(sin((float)2 * M_PI * freq * i / SAMPLING_RATE));
}

short piano(int i, float volume, float freq)
{
    return volume * pow(sin(2 * M_PI * freq * ((float)i / SAMPLING_RATE)), 3) + sin(2 * M_PI * freq * ((float)i / SAMPLING_RATE)) * exp(-0.1 * 2 * M_PI * freq * ((float)i / SAMPLING_RATE));
}

void *play_frequency(void *arg)
{
    params *args = (params *)arg;
    float freq = args->freq;
    float duration = args->duration;

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    int size = (duration + RELEASE_TIME) * SAMPLING_RATE; // Extend buffer for release
    short buf[size];

    for (int i = 0; i < size; i++)
    {
        float volume = VOLUME;
        buf[i] = 0;

        if (i < ATTACK_TIME * SAMPLING_RATE)
        {
            volume = (i / (ATTACK_TIME * SAMPLING_RATE)) * VOLUME;
        }
        else if (i < duration * SAMPLING_RATE)
        {
            volume = VOLUME;
        }
        else
        {
            volume *= 1.0 - (i - duration * SAMPLING_RATE) / (RELEASE_TIME * SAMPLING_RATE);
        }

        // buf[i] += sine(i, volume, freq);

        buf[i] += piano(i, volume, freq);

        buf[i] += 0.25 * piano(i, volume, 2 * freq);

        // buf[i] += sawtooth(i, volume, freq);

        // buf[i] += triangle(i, volume, freq);

        // buf[i] += square(i, volume, freq);
    }

    snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, SAMPLING_RATE, 1, 50000); // 50ms

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

int main()
{
    char key;
    params parameters;
    pthread_t t;

    // Set the terminal to raw mode
    set_mode(1);

    while (1)
    {
        if (!read(STDIN_FILENO, &key, 1))
            break;

        for (int i = 0; i < NUMBER_OF_NOTES; i++)
        {
            if (keyboard[i].key == key)
            {
                parameters.freq = keyboard[i].frequency;
                parameters.duration = 0.25; // 250 ms
                printf("key: %c - %s\n", key, keyboard[i].mkey);
            }
        }

        if (pthread_create(&t, NULL, &play_frequency, (void *)&parameters) != 0)
        {
            printf("\nError - pthread_create");
            return EXIT_FAILURE;
        }
    }

    set_mode(0);

    return 0;
}
