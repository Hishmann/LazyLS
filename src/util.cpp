#include "util.h"

#include "global.h"

WindowSize get_window_size() {
    winsize w{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return {w.ws_row, w.ws_col};
    } else {
        std::perror("ioctl(TIOCGWINSZ)");
        throw std::runtime_error("ioctl(TIOCGWINSZ)");
    }
}

void set_raw_mode(bool enable) {
    termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    if (enable) {
        raw.c_lflag &= ~(ECHO | ICANON); // Turn off echo and line buffering
    } else {
        raw.c_lflag |= (ECHO | ICANON);
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

std::string style_rgb_code(std::initializer_list<std::string_view> styles, std::optional<RGB_FB> fg = std::nullopt, std::optional<RGB_FB> bg = std::nullopt) {
    std::string result = "\e[";
    for (auto& s: styles) {
        result += s;
        result += ";";
    }

    if (fg) result += std::format("38;2;{};{};{};", fg->r, fg->g, fg->b);
    if (bg) result += std::format("48;2;{};{};{};", bg->r, bg->g, bg->b);

    if (result.back() == ';') result.pop_back(); // Clean up trailing semicolon
    return result + "m";
}
