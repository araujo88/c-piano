#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include <termios.h>
#include <unistd.h>

#define SAMPLING_RATE 44100
#define VOLUME 10000

typedef struct _params
{
    float freq;
    float duration;
} params;

struct note
{
    char key;
    float frequency;
};

struct note keyboard[] = {
    {'a', 261.63}, // C4
    {'w', 277.18}, // C#4
    {'s', 293.66}, // D4
    {'e', 311.13}, // D#4
    {'d', 329.63}, // E4
    {'f', 349.23}, // F4
    {'t', 369.99}, // F#4
    {'g', 392.00}, // G4
    {'y', 415.30}, // G#4
    {'h', 440.00}, // A4
    {'u', 466.16}, // A#4
    {'j', 493.88}, // B4
    {'k', 523.25}  // C5
};

#define NUMBER_OF_NOTES (sizeof(keyboard) / sizeof(struct note))

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

void *play_frequency(void *arg)
{
    params *args = (params *)arg;
    float freq = args->freq;
    float duration = args->duration;

    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    int size = duration * SAMPLING_RATE;
    short buf[size];

    for (int i = 0; i < size; i++)
    {
        buf[i] = VOLUME * sin(2 * M_PI * freq * ((float)i / SAMPLING_RATE));
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
}

int main()
{
    char key;
    int rc;
    params parameters;

    // Set the terminal to raw mode
    set_mode(1);

    while (1)
    {
        pthread_t t;
        if (!read(STDIN_FILENO, &key, 1))
            break;

        for (int i = 0; i < NUMBER_OF_NOTES; i++)
        {
            if (keyboard[i].key == key)
            {
                parameters.freq = keyboard[i].frequency;
                parameters.duration = 0.25; // 250 ms
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