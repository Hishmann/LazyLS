#pragma once

#include <string_view>

#include "objects.h"


inline bool running = true; // Global or static flag

inline EventQueue g_event_queue; // for global usecase

namespace PRG_CONST {
    using namespace std::string_view_literals;

    // Reset
    inline constexpr std::string_view RESET = "0"sv;

    // 256 Colours (Format: 5;n)
    inline constexpr std::string_view BLACK       = "5;0"sv;
    inline constexpr std::string_view RED         = "5;1"sv;
    inline constexpr std::string_view GREEN       = "5;2"sv;
    inline constexpr std::string_view YELLOW      = "5;3"sv;
    inline constexpr std::string_view BLUE        = "5;4"sv;
    inline constexpr std::string_view PURPLE      = "5;5"sv;
    inline constexpr std::string_view CYAN        = "5;6"sv;
    inline constexpr std::string_view GRAY_LIGHT  = "5;7"sv;
    inline constexpr std::string_view GRAY        = "5;8"sv;
    inline constexpr std::string_view RED_HIGH    = "5;9"sv;
    inline constexpr std::string_view GREEN_HIGH  = "5;10"sv;
    inline constexpr std::string_view YELLOW_HIGH = "5;11"sv;
    inline constexpr std::string_view BLUE_HIGH   = "5;12"sv;
    inline constexpr std::string_view PINK        = "5;13"sv;
    inline constexpr std::string_view CYAN_HIGH   = "5;14"sv;
    inline constexpr std::string_view WHITE       = "5;15"sv;

    // Styles
    inline constexpr std::string_view BOLD           = "1"sv;
    inline constexpr std::string_view DIM            = "2"sv;
    inline constexpr std::string_view ITALIC         = "3"sv;
    inline constexpr std::string_view UNDERLINE      = "4"sv;
    inline constexpr std::string_view BLINK          = "5"sv;
    inline constexpr std::string_view INVERSE        = "7"sv;
    inline constexpr std::string_view INVISIBLE      = "8"sv;
    inline constexpr std::string_view STRIKETHROUGH  = "9"sv;

    // Style Resets
    inline constexpr std::string_view RESET_BOLD          = "21"sv;
    inline constexpr std::string_view RESET_DIM           = "22"sv;
    inline constexpr std::string_view RESET_ITALIC        = "23"sv;
    inline constexpr std::string_view RESET_UNDERLINE     = "24"sv;
    inline constexpr std::string_view RESET_BLINK         = "25"sv;
    inline constexpr std::string_view RESET_INVERSE       = "27"sv;
    inline constexpr std::string_view RESET_INVISIBLE     = "28"sv;
    inline constexpr std::string_view RESET_STRIKETHROUGH = "29"sv;
}