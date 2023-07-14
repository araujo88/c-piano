#include "../include/wave.h"

short sine(int i, float volume, float freq, int sampling_rate)
{
    return volume * sin(2 * M_PI * freq * ((float)i / sampling_rate));
}

short sawtooth(int i, float volume, float freq, int sampling_rate)
{
    return volume * (2 / M_PI) * ((float)i / sampling_rate * freq - floor(0.5 + (float)i / sampling_rate * freq));
}

short triangle(int i, float volume, float freq, int sampling_rate)
{
    return volume * 2 * fabs(2 * (((float)i / sampling_rate * freq) - floor(0.5 + (float)i / sampling_rate * freq))) - 1;
}

short square(int i, float volume, float freq, int sampling_rate)
{
    return volume * sign(sin((float)2 * M_PI * freq * i / sampling_rate));
}

short piano(int i, float volume, float freq, int sampling_rate)
{
    return volume * pow(sin(2 * M_PI * freq * ((float)i / sampling_rate)), 3) + sin(2 * M_PI * freq * ((float)i / sampling_rate)) * exp(-0.1 * 2 * M_PI * freq * ((float)i / sampling_rate));
}
