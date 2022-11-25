//
// Created by DanGuge on 2022/11/25.
//

#pragma once

#include <string>
#include <vector>

namespace eagle {

class EagleShellHistory {
public:
    EagleShellHistory() = default;
    void reset();
    std::string& get();
    void forward();
    void backward();
    void append(std::string line);

private:
    static std::string empty_str;
    std::vector<std::string> lines;
    int index{};
};

class EagleShell {
public:
    std::string readLine();
    static void moveLeftToEndAndClear(int length);
    static void moveLeftOneAndClear();

private:
    EagleShellHistory history;
};

}  // namespace eagle