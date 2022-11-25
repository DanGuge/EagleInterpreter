//
// Created by DanGuge on 2022/11/25.
//

#include <pretty_print/pretty_print.h>

#include <iostream>

namespace pretty_print {

std::string PrettyPrint::ansi_prefix = "\e";
std::string PrettyPrint::ansi_suffix = "m";
std::string PrettyPrint::ansi_csi = "[";
std::string PrettyPrint::ansi_delimiter = ";";

void PrettyPrint::print(const std::string& output) {
    print(Front::Color::NONE, Back::Color::NONE, {}, output);
}

void PrettyPrint::print(const std::vector<DisplayStyle>& display_styles,
                        const std::string& output) {
    print(Front::Color::NONE, Back::Color::NONE, display_styles, output);
}

void PrettyPrint::print(Front::Color front_color, const std::string& output) {
    print(front_color, Back::Color::NONE, {}, output);
}

void PrettyPrint::print(Back::Color back_color, const std::string& output) {
    print(Front::Color::NONE, back_color, {}, output);
}

void PrettyPrint::print(Front::Color front_color, Back::Color back_color,
                        const std::string& output) {
    print(front_color, back_color, {}, output);
}

void PrettyPrint::print(Front::Color front_color, const std::vector<DisplayStyle>& display_styles,
                        const std::string& output) {
    print(front_color, Back::Color::NONE, display_styles, output);
}

void PrettyPrint::print(Back::Color back_color, const std::vector<DisplayStyle>& display_styles,
                        const std::string& output) {
    print(Front::Color::NONE, back_color, display_styles, output);
}

void PrettyPrint::print(Front::Color front_color, Back::Color back_color,
                        const std::vector<DisplayStyle>& display_styles,
                        const std::string& output) {
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

    std::cout << prefix << output << "\e[95;21;23;24m";
}

}  // namespace pretty_print