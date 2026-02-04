#include "pch.h"

#include "objects.h"


std::string RenderElement::representation_str() {
    if (representation.empty() || update_element && update_element(*this)) {
        update_representation();
    }
    return representation;
};

void Box::update_representation() {
    std::stringstream ss;
    for (int i = 0; i < h; i++) {
        ss << std::format("\e[{};{}H", coord.y+i, coord.x) + colour;
        for (int j = 0; j < w; j++) {
            ss << " ";
        }
        ss << "\e[0m";
    }
    representation = ss.str();
}

