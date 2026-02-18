#pragma once

#include "pch.h"

#include "input.h"


struct TermCell {
    std::string character = " ";
    std::string style = " ";

    bool operator!=(const TermCell& other) const {
        return character != other.character || style != other.style;
    }
};


using TermCellGrid = std::vector<std::vector<TermCell>>;


struct ScreenConstraints {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};


struct PixelCoordinates {
    int x;
    int y;
};


class BaseRenderElement {  
    public:
        virtual ~BaseRenderElement() = default; // Compiler will generate the destructor
        virtual void update(const Event&) = 0;
        virtual void render(const ScreenConstraints&, TermCellGrid&) = 0;
};


template <typename Derived>
class RenderElement : public BaseRenderElement {
    protected:
        std::function<void(Derived&, const Event&)> update_func;

    public:
        PixelCoordinates coord;

        RenderElement(
            PixelCoordinates coordinates, 
            std::function<void(Derived&, const Event&)> func = [](Derived&, const Event&){}
        ) : 
            update_func(std::move(func)) { 
                this -> coord = coordinates;
            }
        
        void update(const Event& e) override { 
            update_func(*static_cast<Derived*>(this), e);
        } 

}; 


class EmptyRenderElement: public RenderElement<EmptyRenderElement> {

    public:
        EmptyRenderElement(
            PixelCoordinates coordinates,
            std::function<void(EmptyRenderElement&, const Event&)> func = [](EmptyRenderElement&, const Event&){}
        ) : 
            RenderElement<EmptyRenderElement>(coordinates, std::move(func)) {}
};


class BoxRenderElement : public RenderElement<BoxRenderElement> {

    public:
        int w, h;
        std::string style_colour;

        BoxRenderElement(
            int width, int height, const std::string& box_style_colour, 
            PixelCoordinates coordinates,
            std::function<void(BoxRenderElement&, const Event&)> func = [](BoxRenderElement&, const Event&){}
        ) : 
            RenderElement<BoxRenderElement>(coordinates, std::move(func)), 
            w {width}, h {height}, style_colour {box_style_colour} {}
        
        void render(const ScreenConstraints& s_c, TermCellGrid& buf) override;
};


class TextRenderElement: public RenderElement<TextRenderElement> {

    public:
        std::string text;
        std::string style_colour;

        TextRenderElement(
            const std::string& txt, const std::string& txt_style_colour,
            PixelCoordinates coordinates, 
            std::function<void(TextRenderElement&, const Event&)> func = [](TextRenderElement&, const Event&){}
        ) :
            RenderElement<TextRenderElement>(coordinates, std::move(func)),
            text {txt}, style_colour {txt_style_colour} {}
        
        void render(const ScreenConstraints& s_c, TermCellGrid& buf) override;
};


class TableRenderElement: public RenderElement<TableRenderElement> {
        using grid = std::vector<std::vector<std::string>>;
        grid grid_content; 
        std::vector<u_int> max_col_widths;
        u_int grid_row;
        u_int grid_col;

    public:
        std::string style_colour;

        void set_content(grid data);

        TableRenderElement(
            grid data, const std::string& txt_style_colour,
            PixelCoordinates coordinates, 
            std::function<void(TableRenderElement&, const Event&)> func  = [](TableRenderElement&, const Event&){}
        ) : 
            RenderElement<TableRenderElement>(coordinates, std::move(func)),
            style_colour {txt_style_colour} 
        {
            set_content(std::move(data));
        }

        void render(const ScreenConstraints& s_c, TermCellGrid& buf) override;
};