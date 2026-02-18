#include "util.h"


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

    static termios orig_termios;
    static bool first_call = true;

    if (first_call) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        first_call = false;
    }

    if (enable) {
        termios raw = orig_termios;

        raw.c_lflag &= ~(ECHO | ICANON | IEXTEN); // Turn off echo, line buffering, ctrl-V special func
        raw.c_iflag &= ~(IXON | ICRNL); // Disable start/stop, Disable carraige mapping to newline
        raw.c_oflag &= ~(OPOST); // prevents \n being turned into \r\n)

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    } else {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

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
