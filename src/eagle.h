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

    static void run(std::string source);

private:
    Interpreter interpreter;
    Resolver resolver;
};
}  // namespace eagle