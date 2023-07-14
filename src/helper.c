#include "../include/helper.h"

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
