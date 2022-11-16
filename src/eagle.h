//
// Created by DanGuge on 2022/11/10.
//

#pragma once

#include "parser/lexer.h"

namespace eagle {

class Eagle {
public:
    static void run(std::string source);
    static void error(int line, const std::string& message);
    static void error(const TokenPtr& token, const std::string& message);

private:
    static void errorReport(int line, const std::string& where, const std::string& message);

private:
    static bool had_error;
    static bool had_runtime_error;
};
}  // namespace eagle