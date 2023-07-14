#ifndef _CONFIG_H
#define _CONFIG_H 1

#include <stdlib.h>
#include <string.h>

typedef struct
{
    double sawtooth;
    double sine;
    double triangle;
    double square;
    int sampling_rate;
} configuration;

int handler(void *user, const char *section, const char *name, const char *value);

#endif
