#ifndef _WAVE_H
#define _WAVE_H 1

#include <math.h>
#include "helper.h"

short sine(int i, float volume, float freq, int sampling_rate);
short sawtooth(int i, float volume, float freq, int sampling_rate);
short triangle(int i, float volume, float freq, int sampling_rate);
short square(int i, float volume, float freq, int sampling_rate);
short piano(int i, float volume, float freq, int sampling_rate);

#endif
