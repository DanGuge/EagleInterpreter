//
// Created by DanGuge on 2022/11/25.
//

#include <pretty_print/pretty_print.h>

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

namespace pretty_print {

#if (__APPLE__) || (__unix__)
std::string PrettyPrint::ansi_prefix = "\e";
std::string PrettyPrint::ansi_suffix = "m";
std::string PrettyPrint::ansi_csi = "[";
std::string PrettyPrint::ansi_delimiter = ";";
#endif

void PrettyPrint::print(const std::string& output, bool newline) {
    print(Front::Color::NONE, Back::Color::NONE, {}, output, newline);
}

void PrettyPrint::print(const std::vector<DisplayStyle>& display_styles, const std::string& output,
                        bool newline) {
    print(Front::Color::NONE, Back::Color::NONE, display_styles, output, newline);
}

void PrettyPrint::print(Front::Color front_color, const std::string& output, bool newline) {
    print(front_color, Back::Color::NONE, {}, output, newline);
}

void PrettyPrint::print(Back::Color back_color, const std::string& output, bool newline) {
    print(Front::Color::NONE, back_color, {}, output, newline);
}

void PrettyPrint::print(Front::Color front_color, Back::Color back_color, const std::string& output,
                        bool newline) {
    print(front_color, back_color, {}, output, newline);
}

void PrettyPrint::print(Front::Color front_color, const std::vector<DisplayStyle>& display_styles,
                        const std::string& output, bool newline) {
    print(front_color, Back::Color::NONE, display_styles, output, newline);
}

void PrettyPrint::print(Back::Color back_color, const std::vector<DisplayStyle>& display_styles,
                        const std::string& output, bool newline) {
    print(Front::Color::NONE, back_color, display_styles, output, newline);
}

void PrettyPrint::print(Front::Color front_color, Back::Color back_color,
                        const std::vector<DisplayStyle>& display_styles, const std::string& output,
                        bool newline) {
#ifdef _WIN32
    int text_attribute = (back_color << 4) + front_color;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text_attribute);
    if (newline) {
        printf("%s\n", output.c_str());
    } else {
        printf("%s", output.c_str());
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
#elif (__APPLE__) || (__unix__)
    std::string prefix = ansi_prefix;
    std::string tmp = ansi_csi;

    if (front_color != Front::Color::NONE) {
        prefix += tmp + std::to_string(front_color);
        tmp = ansi_delimiter;
    }

    if (back_color != Back::Color::NONE) {
        prefix += tmp + std::to_string(back_color);
        tmp = ansi_delimiter;
    }

    for (const auto& display_style : display_styles) {
        prefix += tmp + std::to_string(display_style);
    }

    prefix += ansi_suffix;
    if (newline) {
        std::cout << prefix << output << "\e[95;23;24;49m"
                  << "\n";
    } else {
        std::cout << prefix << output << "\e[95;23;24;49m";
    }
#endif
}

}  // namespace pretty_print