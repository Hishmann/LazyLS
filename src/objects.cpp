#include "pch.h"

#include "objects.h"


void BoxRenderElement::make() {
    std::stringstream ss;
    for (int i = 0; i < h; i++) {
        ss << std::format("\e[{};{}H", coord.y+i, coord.x) + style_colour;
        for (int j = 0; j < w; j++) {
            ss << " ";
        }
        ss << "\e[0m";
    }
    rep = ss.str();
}

void TextRenderElement::make() {
    std::stringstream ss;
    ss << std::format("\e[{};{}H", coord.y, coord.x) << style_colour << text << "\e[0m";
    rep = ss.str();
}
