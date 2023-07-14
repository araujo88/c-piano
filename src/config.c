#include "../include/config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
    configuration *pconfig = (configuration *)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("waveform", "sawtooth"))
    {
        pconfig->sawtooth = atof(value);
    }
    else if (MATCH("waveform", "sine"))
    {
        pconfig->sine = atof(value);
    }
    else if (MATCH("waveform", "triangle"))
    {
        pconfig->triangle = atof(value);
    }
    else if (MATCH("waveform", "square"))
    {
        pconfig->square = atof(value);
    }
    else if (MATCH("system", "sampling_rate"))
    {
        pconfig->sampling_rate = atoi(value);
    }
    else
    {
        return 0; /* unknown section/name, error */
    }
    return 1;
}
