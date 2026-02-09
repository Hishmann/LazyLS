#include "pch.h"

#include "objects.h"


void BoxRenderElement::make() {
    std::stringstream ss;
    for (int i = 0; i < h; i++) {
        ss << std::format("\e[{};{}H", coord.y+i, coord.x) + colour;
        for (int j = 0; j < w; j++) {
            ss << " ";
        }
        ss << "\e[0m";
    }
    rep = ss.str();
}

