# c-piano

a simple piano programmed in C

# Requirements

`sudo apt-get install libasound2 libasound2-dev`

# Compiling

`make`

# Running

`./sin_wave`

# Notes mapping

```c
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
```
