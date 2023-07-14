# c-piano

a simple piano programmed in C

# Requirements

`sudo apt-get install libasound2 libasound2-dev`

# External libraries

- [inih](https://github.com/benhoyt/inih)

# Cloning

`git clone --recurse-submodules https://github.com/your_project`

# Updating

`git submodule update --init`

# Compiling

`make`

# Running

`./piano`

# Notes mapping

```c
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
```

# Features

## Waveforms

- sine
- sawtooth
- triangle
- square

## Effects

- release
- attack
