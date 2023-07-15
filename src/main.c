#include <stdio.h>
#include <pthread.h>
#include "../include/inih/ini.h"
#include "../include/config.h"
#include "../include/helper.h"
#include "../include/audio.h"

int main()
{
    char key;
    struct params parameters;
    pthread_t t;

    configuration config;

    if (ini_parse("piano.ini", handler, &config) < 0)
    {
        printf("Can't load 'piano.ini'\n");
        return EXIT_FAILURE;
    }
    printf("Config loaded from 'piano.ini': sawtooth=%lf, sine=%lf, square=%lf, triangle=%lf, sampling_rate=%d\n",
           config.sawtooth, config.sine, config.square, config.triangle, config.sampling_rate);

    // Set the terminal to raw mode
    set_mode(1);

    while (1)
    {
        if (!read(STDIN_FILENO, &key, 1))
            break;

        for (int i = 0; i < number_of_notes; i++)
        {
            if (keyboard[i].key == key)
            {
                parameters.freq = keyboard[i].frequency;
                parameters.duration = 0.25; // 250 ms
                parameters.sampling_rate = config.sampling_rate;
                printf("key: %c - %s\n", key, keyboard[i].mkey);
            }
        }

        if (pthread_create(&t, NULL, &play, (void *)&parameters) != 0)
        {
            printf("\nError - pthread_create");
            return EXIT_FAILURE;
        }
    }

    set_mode(0);

    return 0;
}
