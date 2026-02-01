#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <format>
#include <thread>
#include <chrono>
#include <functional>
#include <csignal>
#include "inc/constants.h"


// Global or static flag
bool running = true;

void signalHandler(int signum) {
    running = false;
}

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
        RenderElement(std::function<bool(RenderElement&)> func) 
        : update_element(std::move(func)) {}

        std::string representation_str() {
            if (representation.empty() || update_element && update_element(*this)) {
                update_representation();
            }
            return representation;
        };
}; 

class Box : public RenderElement {
    public:
        int w, h;
        PixelCoordinates coord;
        std::string colour;

        Box(int width, int height, PixelCoordinates coordinates, const std::string& box_colour, std::function<bool(RenderElement&)> func) : 
        RenderElement(std::move(func)), w {width}, h {height}, coord {coordinates}, colour {box_colour} {}

    protected:

        void update_representation() {
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

};

class Screen {

    int row, col;
    std::vector<std::unique_ptr<RenderElement>> elements;

    public:
    
    Screen() {
        winsize w{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            row = w.ws_row;
            col = w.ws_col;
        } else {
            std::perror("ioctl(TIOCGWINSZ)");
            throw std::runtime_error("ioctl(TIOCGWINSZ)");
        }

        // Enter alternate buffer and hide cursor
        std::cout << "\033[?1049h" << "\033[?25l" << std::flush;
    }

    ~Screen() {
        // Clean up: Show cursor and exit alternate buffer
        std::cout << "\033[?25h" << "\033[?1049l" << std::flush;
    }

    int get_row() {return row;}

    int get_col() {return col;}

    void add_element(std::unique_ptr<RenderElement> e) {
        elements.push_back(std::move(e));
    }

    void render() {

        while (running) {
            std::stringstream ss;
            ss << "\e[2J"; // clear screen (can cause flickering so alternate would be to maybe print the whole empty grid?)
            ss <<  "\e[H"; // reset cursor

            for (auto& s : elements) {
                ss << s -> representation_str();
            }

            std::cout << ss.str() << std::flush;
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(10));
        }
    }

};

int main() {

    std::signal(SIGINT, signalHandler); // Catch Ctrl+C

    Screen screen;

    screen.add_element(
        std::make_unique<Box>(10, 5, PixelCoordinates{15,4}, TERM_CONST_PRG::RED, [&screen](RenderElement& e) {

            Box& b = static_cast<Box&>(e);
            static int box_anim;

            box_anim += 1;
            if (box_anim > 50) {
                b.coord.x += 1;
                box_anim = 0;
                if (b.coord.x == screen.get_col() ) {
                    b.coord.x = 1;
                }
                return true;
            }

            return false;
        })
    );

    screen.render();    

    return 0;
}