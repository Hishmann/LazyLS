#include "pch.h"

#include "objects.h"
#include "global.h"

bool constraint_obeyed(int x, int y, ScreenConstraints s_c) {
    if ((x >= s_c.min_x && x < s_c.max_x) && (y >= s_c.min_y && y < s_c.max_y)) {
        return true;
    }
    return false;
}

void BoxRenderElement::render(const ScreenConstraints& s_c, TermCellGrid& buf) {

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (!constraint_obeyed(coord.x + x, coord.y + y, s_c)) continue;

            int target_y = coord.y + y - s_c.min_y;
            int target_x = coord.x + x - s_c.min_x;

            buf[target_y][target_x] = TermCell{" ", style_colour};
        }
    }

}

void TextRenderElement::render(const ScreenConstraints& s_c, TermCellGrid& buf) {
    std::stringstream ss;
    ss << std::format("\e[{};{}H", coord.y, coord.x) << style_colour << text << "\e[0m";

}


void TableRenderElement::set_content(grid data) { 
    if (data.empty() || data[0].empty()) {
        throw std::invalid_argument("TableRenderElement: Grid dimensions cannot be zero.");
    }

    size_t width = data[0].size();
    for (const auto& row : data) {
        if (row.size() != width) {
            throw std::invalid_argument("TableRenderElement: Irregular grid provided.");
        }
    }

    grid_content = std::move(data);
    grid_row = grid_content.size();
    grid_col = width;

    max_col_widths.assign(grid_col, 0);

    for (int j = 0; j < grid_col; j++) {
        for (int i = 0; i < grid_row; i++) {
            uint sz = grid_content[i][j].size();
            if (max_col_widths[j] < sz) {
                max_col_widths[j] = sz;
            }
        }
    }
}

void TableRenderElement::render(const ScreenConstraints& s_c, TermCellGrid& buf) {

    using namespace PRG_DRAW;

    std::stringstream ss;  

    auto make_line = [&](std::string_view left, std::string_view mid, std::string_view right) {
        std::string line;
        line += left;
        for (int j = 0; j < grid_col; ++j) {
            int w = max_col_widths[j];
            for (int t = 0; t < w; t++) {
                line += BOX_HOR;
            }
            if (j < grid_col - 1) line += mid;
        }
        line += right;
        return line;
    };

    std::string top_line = make_line(BOX_TL, BOX_MT, BOX_TR);
    std::string mid_line = make_line(BOX_ML, BOX_M,  BOX_MR);
    std::string bot_line = make_line(BOX_BL, BOX_MB, BOX_BR);

   for (int i = 0; i < grid_row; ++i) {
        ss << std::format("\e[{};{}H", coord.y + (i * 2), coord.x);
        ss << (i == 0 ? top_line : mid_line);

        ss << std::format("\e[{};{}H", coord.y + (i * 2) + 1, coord.x);
        for (int j = 0; j < grid_col; ++j) {
            const auto& s = grid_content[i][j];
            ss << BOX_VER << s << std::string(max_col_widths[j] - s.size(), ' ');
        }
        ss << BOX_VER;
    }

    ss << std::format("\e[{};{}H", coord.y + (grid_row * 2), coord.x);
    ss << bot_line;

}
