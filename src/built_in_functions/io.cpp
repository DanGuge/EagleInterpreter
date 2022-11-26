//
// Created by DanGuge on 2022/11/26.
//

#include "io.h"

#include <iostream>
#include <string>

#include "modules/eagle_string.h"

namespace eagle::built_in_functions {

int Input::arity() {
    return 0;
}

ObjectPtr Input::call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments, int call_line) {
    std::string input;
    getline(std::cin, input);
    return std::make_shared<String>(input);
}

std::string Input::toString() {
    return "<built in function input>\n"
           "usage: input()->String";
}

}  // namespace eagle::built_in_functions