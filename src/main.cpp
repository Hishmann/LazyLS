#include "pch.h"

#include <csignal>

#include "global.h"
#include "objects.h"
#include "input.h"
#include "util.h"
#include "file.h"

bool running = true; // Global or static flag

EventQueue g_event_queue; // for global usecase

class Screen {

        int row, col;
        std::vector<std::unique_ptr<BaseRenderElement>> elements;
        std::vector<Event> batch;
        TermCellGrid front_buffer;
        TermCellGrid back_buffer;
        ScreenConstraints constraint;

        void clear_back_buffer() {
            for (auto& row : back_buffer) {
                std::fill(row.begin(), row.end(), TermCell{" ", ""});
            }
        }

        void present() {
            std::string active_style = "INITIAL"; // Has to be different from empty in the start for edge case scenario with (back.style != active_style)

            for (int y = 0; y < row; y++) {
                for (int x = 0; x < col; x++) {
                    TermCell& back = back_buffer[y][x];
                    TermCell& front = front_buffer[y][x];

                    if (back != front) {
                        std::cout << "\e[" << (y + 1) << ";" << (x + 1) << "H";

                        if (back.style != active_style) {
                            if (back.style.empty()) {
                                std::cout << "\e[0m"; // Reset to default
                                active_style = "";
                            } else {
                                std::cout << back.style; // Apply new color
                                active_style = back.style;
                            }
                        }
                        std::cout << back.character;
                        front = back;
                    }
                }
            }
            std::cout << std::flush;
        }

        void offset_constraint(int x_off, int y_off) {
            auto upd_func = [](int& val, int off, int cmp){
                val += off;
                if (val < cmp) val = cmp;
            };
            upd_func(constraint.min_x, x_off, 0); 
            upd_func(constraint.max_x, x_off, col); 
            upd_func(constraint.min_y, y_off, 0); 
            upd_func(constraint.max_y, y_off, row); 
        }

    public:
    
        Screen(int w_row, int w_col) : row{w_row}, col{w_col} {
            front_buffer.assign(row, std::vector<TermCell>(col, TermCell{" ", ""}));
            back_buffer.assign(row, std::vector<TermCell>(col, TermCell{" ", ""}));
            constraint = ScreenConstraints{0, 0, col, row};
        }

        int get_col() {return col;}

        int get_row() {return row;}

        void add_element(std::unique_ptr<BaseRenderElement> e) { elements.push_back(std::move(e)); }

        void render();
};


void Screen::render() {

    while (running) {  

        batch = g_event_queue.pop_all();
        if (!batch.empty()) {
            for (const auto& e : batch) {

                if (e.type == EventType::QUIT) { 
                    running = false; 
                }

                if (e.type == EventType::MOUSE_PRESS && e.mouse.button == PRG_CONST::MOUSE_SCROLL_U) {
                    offset_constraint(0,1);
                }

                if (e.type == EventType::MOUSE_PRESS && e.mouse.button == PRG_CONST::MOUSE_SCROLL_D) {
                    offset_constraint(0,-1);
                }

                for (auto& s : elements) {
                    s -> update(e);
                }

            }
        }
        batch.clear();

        clear_back_buffer();

        for (auto& s : elements) {
            s -> render(constraint, back_buffer);
        }

        present();

        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(10));
    }

}


int main() {

    std::signal(SIGINT, [](int signum) {
        running = false;
    }); // Catch Ctrl+C

    WindowSize w = get_window_size();

    set_raw_mode(true);
    std::cout << "\033[?1049h" << "\033[?25l"; // Enter alternate buffer and hide cursor
    std::cout << "\033[?1003h\033[?1006h"; // Enable All Motion Mouse Tracking and SGR
    std::cout << std::flush;

    Screen screen(w.r, w.c);

    // screen.add_element(
    //     std::make_unique<BoxRenderElement>(
    //         10, 5, style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}), PixelCoordinates{15,4},  
    //         [&screen](BoxRenderElement& b, const Event& e) {

    //         if (e.type == EventType::KEY_PRESS && e.keyboard.type == KeyType::RIGHT_ARROW) {
    //             b.coord.x += 1;
    //             if (b.coord.x == screen.get_col() ) {
    //                 b.coord.x = 1;
    //             }
    //         }

    //     })
    // );

    // screen.add_element(
    //     std::make_unique<TextRenderElement>(
    //         "  To Lazy to LS  ",
    //         style_rgb_code({PRG_CONST::BOLD, PRG_CONST::ITALIC}, RGB_FB{255,255,255}, RGB_FB{0,0,0}), 
    //         PixelCoordinates{1,1}, 
    //         [&screen](TextRenderElement& b, const Event& e) {
                
    //             if (e.type == EventType::KEY_PRESS) {
    //                 if (e.keyboard.key == 127 && !b.text.empty()) {
    //                     b.text.pop_back();
    //                 } else {
    //                     b.text += e.keyboard.key;
    //                 }
    //             }

    //         }
    //     )
    // );

    std::vector<std::vector<std::string>> test_vec_str = {
        {"hmmmmm", "np"}, 
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"yes?", "why not"},
        {"hmmmmm", "np"},
    };

    // screen.add_element(
    //     std::make_unique<TableRenderElement>(
    //         test_vec_str,
    //         PixelCoordinates{2,2}, style_rgb_code({}, std::nullopt, std::nullopt), 
    //         [&screen](TableRenderElement& b, const Event& e) {

    //         return false;
    //     })
    // );

    std::thread inp_han(input_handling, std::ref(running), std::ref(g_event_queue)); // seperate polling thread for input handling

    screen.render();   

    inp_han.join();

    set_raw_mode(false);
    std::cout << "\033[?25h" << "\033[?1049l"; // Clean up: Show cursor and exit alternate buffer
    std::cout << "\033[?1003l\033[?1006l"; // Disable All Motion Mouse Tracking and SGR
    std::cout << std::flush;
    
    return 0;
}