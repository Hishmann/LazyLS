#include "pch.h"


#include <poll.h>
#include <unistd.h>
#include <csignal>
#include <cstring>

#include "global.h"
#include "objects.h"
#include "util.h"


void input_handling() {

    struct pollfd my_pollfd;
    my_pollfd.fd = STDIN_FILENO;  // Watch Standard Input (0)
    my_pollfd.events = POLLIN;    // Alert me if there is data to read
    std::string accumulated;

    while (running) {  

        int ret = poll(&my_pollfd, 1, 500); // Poll the buffer and if no input then sleep for timeout (500 ms) 

        if (ret > 0) {

            if (my_pollfd.revents & POLLIN) { // Data is waiting!
                char buf[128];
                memset(buf, 0, sizeof(buf));
                int n = read(STDIN_FILENO, &buf, sizeof(buf)-1); // Read exactly 1 byte
                
                if (n > 0) {
                    buf[n] = '\0'; // Null-terminate the temp buffer
                    accumulated += buf; 
                }

                size_t mouse_end = accumulated.find_first_of("Mm", 3); 
        
                if (accumulated.find("\033[<") == 0 && mouse_end != std::string::npos) {
                    int btn, x, y;
                    char mode;

                    if (sscanf(accumulated.c_str() + 3, "%d;%d;%d%c", &btn, &x, &y, &mode) == 4) {
                        Event e;
                        e.type = (mode == 'M') ? EventType::MOUSE_PRESS : EventType::MOUSE_RELEASE;
                        e.mouse.x = x;
                        e.mouse.y = y;
                        e.mouse.button = btn;
                        g_event_queue.push(e);

                        accumulated.erase(0, mouse_end + 1);
                    }

                } else if (!accumulated.empty() && accumulated[0] == 'q') {
                    Event e; e.type = EventType::QUIT;
                    g_event_queue.push(e);
                    running = false;

                    accumulated.erase(0, 1);

                } else if (!accumulated.empty()) {
                    Event e; e.type = EventType::KEY_PRESS;
                    e.keyboard.key = accumulated[0]; e.keyboard.modifiers = 0;
                    g_event_queue.push(e);

                    accumulated.erase(0, 1);

                } else if (accumulated.size() > 32) {

                    accumulated.clear(); // If the buffer starts with something unrecognizable and grows too large

                }

            }
        } else if (ret == 0) {
            // Timeout reached, no input detected
        } else {
            perror("poll"); // An error occurred
            break;
        }

    }
    
}


class Screen {

    int row, col;
    std::vector<std::unique_ptr<BaseRenderElement>> elements;
    std::vector<Event> batch;

    public:
    
    Screen(int w_row, int w_col) : row{w_row}, col{w_col} {

        set_raw_mode(true);

        std::cout << "\033[?1049h" << "\033[?25l"; // Enter alternate buffer and hide cursor

        std::cout << "\033[?1003h\033[?1006h"; // Enable All Motion Mouse Tracking and SGR

        std::cout << std::flush;

    }

    ~Screen() {

        set_raw_mode(false);

        std::cout << "\033[?25h" << "\033[?1049l"; // Clean up: Show cursor and exit alternate buffer

        std::cout << "\033[?1003l\033[?1006l"; // Disable All Motion Mouse Tracking and SGR

        std::cout << std::flush;
    }

    int get_row() {return row;}

    int get_col() {return col;}

    void add_element(std::unique_ptr<BaseRenderElement> e) { elements.push_back(std::move(e)); }

    void render();
};


void Screen::render() {

    while (running) {  

        std::stringstream ss;
        ss << "\e[2J"; // clear screen (can cause flickering so alternate would be to maybe print the whole empty grid?)
        ss <<  "\e[H"; // reset cursor

        batch = g_event_queue.pop_all();
        if (!batch.empty()) {
            for (const auto& e : batch) {
                if (e.type == EventType::QUIT) { running = false; }
                for (auto& s : elements) {
                    s -> update(e);
                }
                batch.erase(batch.begin());
            }
        }

        for (auto& s : elements) {
            ss << s -> representation();
        }

        std::cout << ss.str() << std::flush;

        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(10));
    }

}


int main() {

    std::signal(SIGINT, [](int signum) {
        running = false;
    }); // Catch Ctrl+C

    WindowSize w = get_window_size();

    // Screen screen(w.r, w.c);

    // screen.add_element(
    //     std::make_unique<BoxRenderElement>(
    //         10, 5, PixelCoordinates{15,4}, style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}), 
    //         [&screen](BoxRenderElement& b, const Event& e) {

    //         if (e.type == EventType::MOUSE_PRESS && e.mouse.button == 0) {
    //             b.coord.x += 1;
    //             if (b.coord.x == screen.get_col() ) {
    //                 b.coord.x = 1;
    //             }
    //             return true;
    //         }

    //         return false;
    //     })
    // );

    // screen.add_element(
    //     std::make_unique<TextRenderElement>(
    //         "  To Lazy to LS  ",
    //         PixelCoordinates{1,1}, 
    //         style_rgb_code({PRG_CONST::BOLD, PRG_CONST::ITALIC}, RGB_FB{255,255,255}, RGB_FB{0,0,0}), 
    //         [&screen](TextRenderElement& b, const Event& e) {
                
    //             if (e.type == EventType::KEY_PRESS) {
    //                 if (e.keyboard.key == 127 && !b.text.empty()) {
    //                     b.text.pop_back();
    //                 } else {
    //                     b.text += e.keyboard.key;
    //                 }
    //                 return true;
    //             }

    //             return false;
    //         }
    //     )
    // );

    // std::thread inp_han(input_handling); // seperate polling thread for input handling

    // screen.render();   

    // inp_han.join();



    return 0;
}