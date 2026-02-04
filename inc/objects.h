#pragma once

#include "pch.h"

typedef struct PixelCoordinates {
    int x;
    int y;
};

class RenderElement {
    protected:
        std::string representation = "";
        std::function<bool(RenderElement&)> update_element;
        virtual void update_representation() = 0;

    public:
        RenderElement(std::function<bool(RenderElement&)> func) : update_element(std::move(func)) {}

        std::string representation_str();
}; 


class Box : public RenderElement {
    protected:
        void update_representation();
    public:
        int w, h;
        PixelCoordinates coord;
        std::string colour;

        Box(int width, int height, PixelCoordinates coordinates, const std::string& box_colour, std::function<bool(RenderElement&)> func) : 
        RenderElement(std::move(func)), w {width}, h {height}, coord {coordinates}, colour {box_colour} {}
};