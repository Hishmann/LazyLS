#include "pch.h"

#include <sys/ioctl.h>
#include <poll.h>
#include <unistd.h>
#include <csignal>
#include <cstring>

#include "constants.h"
#include "objects.h"
#include "util.h"


void input_handling() {

    struct pollfd my_pollfd;
    my_pollfd.fd = STDIN_FILENO;  // Watch Standard Input (0)
    my_pollfd.events = POLLIN;    // Alert me if there is data to read

    while (running) {  

        int ret = poll(&my_pollfd, 1, 500); // Poll the buffer (timeout is in milliseconds)

        if (ret > 0) {
            if (my_pollfd.revents & POLLIN) { // Data is waiting!
                char buf[128];
                memset(buf, 0, sizeof(buf));
                int n = read(STDIN_FILENO, &buf, sizeof(buf)-1); // Read exactly 1 byte

                if (n > 3 && buf[0] == '\033' && buf[1] == '[' && buf[2] == '<') {
                    int btn, x, y;
                    char mode;

                    // sscanf parses the pattern: button;x;y followed by a character
                    // buf + 3 skips the "\033[<" prefix
                    if (sscanf(buf + 3, "%d;%d;%d%c", &btn, &x, &y, &mode) == 4) {
                        bool isPress = (mode == 'M');
                        
                        // Clear screen and print status
                        std::cout << "\e[2J\e[H";
                        std::cout << "--- Mouse Event ---" << std::endl;
                        std::cout << "Button ID: " << btn << std::endl;
                        std::cout << "Coords:    (" << x << ", " << y << ")" << std::endl;
                        std::cout << "State:     " << (isPress ? "Pressed/Moving" : "Released") << std::endl;
                        std::cout << std::flush;
                    }
                }

                else if (buf[0] == 'q') {
                    running = false;
                    break;
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

    void add_element(std::unique_ptr<RenderElement> e) { elements.push_back(std::move(e)); }

    void mouse();

    void render();
};


void Screen::render() {
    std::thread inp_han(input_handling); // seperate polling thread for input handling

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

    inp_han.join();
}

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