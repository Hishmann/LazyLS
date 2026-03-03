#pragma once

#include <string_view>


namespace PRG_CONST {

    constexpr int MOUSE_LC = 0;
    constexpr int MOUSE_MC = 1;
    constexpr int MOUSE_RC = 2;
    constexpr int MOUSE_RELEASE = 3; // old protocol
    constexpr int MOUSE_MOVE = 32;
    constexpr int MOUSE_SCROLL_U = 64;
    constexpr int MOUSE_SCROLL_D = 65;
    constexpr int MOUSE_BTN_4 = 128;
    constexpr int MOUSE_BTN_5 = 129;

    constexpr int MOUSE_MOD_SHIFT = 4;
    constexpr int MOUSE_MOD_META = 8;
    constexpr int MOUSE_MOD_CTRL = 16;

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

namespace PRG_DRAW {

    inline constexpr std::string_view BOX_TL = "┌";
    inline constexpr std::string_view BOX_TL_H = "┏";

    inline constexpr std::string_view BOX_TR = "┐";
    inline constexpr std::string_view BOX_TR_H = "┓";

    inline constexpr std::string_view BOX_BL = "└";           
    inline constexpr std::string_view BOX_BL_H = "┗";

    inline constexpr std::string_view BOX_BR = "┘";
    inline constexpr std::string_view BOX_BR_H = "┛";

    inline constexpr std::string_view BOX_HOR = "─";
    inline constexpr std::string_view BOX_HOR_H = "━";

    inline constexpr std::string_view BOX_VER = "│";
    inline constexpr std::string_view BOX_VER_H = "┃";

    inline constexpr std::string_view BOX_ML = "├";
    inline constexpr std::string_view BOX_ML_H = "┣";

    inline constexpr std::string_view BOX_MR = "┤";
    inline constexpr std::string_view BOX_MR_H = "┫";

    inline constexpr std::string_view BOX_MT = "┬";
    inline constexpr std::string_view BOX_MT_H = "┳";

    inline constexpr std::string_view BOX_MB = "┴";
    inline constexpr std::string_view BOX_MB_H = "┻";

    inline constexpr std::string_view BOX_M = "┼";
    inline constexpr std::string_view BOX_M_H = "╋";

}