#ifndef _AUDIO_H
#define _AUDIO_H 1

#include <alsa/asoundlib.h>
#include "wave.h"

#define VOLUME 10000 * 0.6
#define RELEASE_TIME 1 // Release time in seconds
#define ATTACK_TIME 0  // Attack time in seconds

struct note
{
    char key;
    float frequency;
    char *mkey;
};

extern struct note keyboard[];

struct params
{
    float freq;
    float duration;
    int sampling_rate;
};

extern const int number_of_notes;

void *play(void *arg);

#endif