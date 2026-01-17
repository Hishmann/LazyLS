#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <format>
#include <thread>
#include <chrono>
#include "inc/constants.h"

std::string draw_square_text_box(int row, int col, int sz, std::string box_content, const std::string& colour ) {

    std::string out_str;
    int text_row = sz / 2;
    int text_col =  sz - box_content.size() / 2;

    for (int i = 0; i < sz; i++) {
        out_str += std::format("\e[{};{}H", row+i, col) + colour;
        for (int j = 0; j < 2*sz; j++) {
            if (i == text_row && j >= text_col && j < text_col + box_content.size()) {
                out_str += box_content[j-text_col];
                continue;
            }
            out_str+= " ";
        }
        out_str += "\e[0m\n";
    }

    return out_str;
}

int main() {

    // std::cout << "Unicode: " << '\u03C0' << '\n';
    // // Grinning face
    // std::cout << "Beyond BMP: " << '\U0001F600' << '\n';
    // // Greek letter lambda
    // std::cout << "UTF-8: " << u8"\u03BB" << ", " << u8"\u03C0" << '\n';
    
    winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        // w.ws_col = columns, w.ws_row = rows
        std::printf("cols=%u rows=%u\n", w.ws_col, w.ws_row);
    } else {
        std::perror("ioctl(TIOCGWINSZ)");
    }

    int anim_i = 0;
    struct coord { int row; int col; } box_coord = {4, 15};
    std::string screen_output;

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        screen_output = "";

        screen_output += "\e[2J"; // clear screen
        screen_output += "\e[1;1H";
        screen_output += draw_square_text_box(box_coord.row, box_coord.col, 5, "Test", TERM_CONST_PRG::RED);
        screen_output += "\e[1;1H";

        std::cout << screen_output << std::endl;

        anim_i += 1;
        if (anim_i >= frameRate / 32) {
            anim_i = 0;
            box_coord.col += 1;
            if (box_coord.col >= w.ws_col - 10) {
                box_coord.col = 1;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        if (diff.count() < frameTime / 1000.0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(frameTime - diff.count() * 1000)));
        }
    }


    return 0;
}