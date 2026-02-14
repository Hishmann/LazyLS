#pragma once

#include "pch.h"

#include <sys/ioctl.h>
#include <termios.h>

// rgb for foreground and background
struct RGB_FB { 
    int r;
    int g;
    int b;
};

struct WindowSize {
    int r;
    int c;
};

WindowSize get_window_size();

void set_raw_mode(bool enable);

std::string style_rgb_code(std::initializer_list<std::string_view> style, std::optional<RGB_FB> fg, std::optional<RGB_FB> bg);