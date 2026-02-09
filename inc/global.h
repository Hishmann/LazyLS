#pragma once

#include "objects.h"

inline bool running = true; // Global or static flag

inline EventQueue g_event_queue;

namespace TERM_CONST_PRG {

    inline constexpr char BLACK[] = "\e[48;5;0m";
    inline constexpr char RED[] = "\e[48;5;1m";
    inline constexpr char GREEN[] = "\e[48;5;2m";
    inline constexpr char YELLOW[] = "\e[48;5;3m";
    inline constexpr char BLUE[] = "\e[48;5;4m";
    inline constexpr char PURPLE[] = "\e[48;5;5m";
    inline constexpr char CYAN[] = "\e[48;5;6m";
    inline constexpr char GRAY_LIGHT[] = "\e[48;5;7m";
    inline constexpr char GRAY[] = "\e[48;5;8m";
    inline constexpr char RED_HIGH[] = "\e[48;5;9m";
    inline constexpr char GREEN_HIGH[] = "\e[48;5;10m";
    inline constexpr char YELLOW_HIGH[] = "\e[48;5;11m";
    inline constexpr char BLUE_HIGH[] = "\e[48;5;12m";
    inline constexpr char PINK[] = "\e[48;5;13m";
    inline constexpr char CYAN_HIGH[] = "\e[48;5;14m";
    inline constexpr char WHITE[] = "\e[48;5;15m";

}