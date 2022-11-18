//
// Created by DanGuge on 2022/11/10.
//

#pragma once

#include "interpreter/interpreter.h"
#include "interpreter/resolver.h"
#include "parser/lexer.h"
#include "parser/parser.h"

namespace eagle {

class Eagle {
public:
    Eagle();

    void run(std::string source);

private:
    Lexer lexer;
    Parser parser;
    Interpreter interpreter;
    Resolver resolver;
};
}  // namespace eagle