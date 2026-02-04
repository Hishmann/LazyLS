#include "util.h"

#include "constants.h"

void set_raw_mode(bool enable) {
    termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    if (enable) {
        raw.c_lflag &= ~(ECHO | ICANON); // Turn off echo and line buffering
    } else {
        raw.c_lflag |= (ECHO | ICANON);
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void signalHandler(int signum) {
    running = false;
}