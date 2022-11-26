//
// Created by DanGuge on 2022/11/25.
//

#pragma once

#include <string>
#include <vector>

namespace pretty_print {

#ifdef _WIN32
namespace Front {
enum Color {
    NONE = 15,
    BLACK = 0,
    RED = 12,
    GREEN = 10,
    YELLOW = 14,
    BLUE = 9,
    PINK = 13,
    CYAN = 11,
    WHITE = 15
};
}

namespace Back {
enum Color {
    NONE = 0,
    BLACK = 0,
    RED = 12,
    GREEN = 10,
    YELLOW = 14,
    BLUE = 9,
    PINK = 13,
    CYAN = 11,
    WHITE = 15
};
}
#elif (__APPLE__) || (__unix__)
namespace Front {
enum Color {
    NONE = -1,
    BLACK = 90,
    RED = 91,
    GREEN = 92,
    YELLOW = 93,
    BLUE = 94,
    PINK = 95,
    CYAN = 96,
    WHITE = 97
};
}

namespace Back {
enum Color {
    NONE = -1,
    BLACK = 100,
    RED = 101,
    GREEN = 102,
    YELLOW = 103,
    BLUE = 104,
    PINK = 105,
    CYAN = 106,
    WHITE = 107
};
}
#endif

enum DisplayStyle { NORMAL = 0, BOLD = 1, ITALIC = 3, UNDERLINE = 4, SLOW_BLINK = 5 };

class PrettyPrint {
public:
    static void print(const std::string& output, bool newline = true);
    static void print(const std::vector<DisplayStyle>& display_styles, const std::string& output,
                      bool newline = true);
    static void print(Front::Color front_color, const std::string& output, bool newline = true);
    static void print(Back::Color back_color, const std::string& output, bool newline = true);
    static void print(Front::Color front_color, Back::Color back_color, const std::string& output,
                      bool newline = true);
    static void print(Front::Color front_color, const std::vector<DisplayStyle>& display_styles,
                      const std::string& output, bool newline = true);
    static void print(Back::Color back_color, const std::vector<DisplayStyle>& display_styles,
                      const std::string& output, bool newline = true);
    static void print(Front::Color front_color, Back::Color back_color,
                      const std::vector<DisplayStyle>& display_styles, const std::string& output,
                      bool newline = true);

private:
    static std::string ansi_prefix;
    static std::string ansi_suffix;
    static std::string ansi_csi;
    static std::string ansi_delimiter;
};
}  // namespace pretty_print