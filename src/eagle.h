//
// Created by DanGuge on 2022/11/10.
//

#pragma once

#include "interpreter/interpreter.h"
#include "interpreter/resolver.h"
#include "parser/lexer.h"
#include "parser/parser.h"

namespace eagle {

class Interpreter;

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
    static std::shared_ptr<Interpreter> interpreter;
};
}  // namespace eagle