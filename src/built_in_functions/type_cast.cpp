//
// Created by DanGuge on 2022/11/26.
//

#include "type_cast.h"

#include "modules/eagle_string.h"

namespace eagle::built_in_functions {

int Str::arity() {
    return 1;
}

ObjectPtr Str::call(eagle::Interpreter &interpreter, std::vector<ObjectPtr> &arguments,
                    int call_line) {
    if (InstanceOf<String>(arguments[0])) {
        return arguments[0];
    } else if (InstanceOf<Object>(arguments[0])) {
        return std::make_shared<String>(arguments[0]->toString());
    } else {
        throw RuntimeError(call_line, "str() need one 'Object' argument");
    }
}

std::string Str::toString() {
    return "<built in function str>\n"
           "usage: str(variable: Object)->String";
}

int Num::arity() {
    return 1;
}

ObjectPtr Num::call(eagle::Interpreter &interpreter, std::vector<ObjectPtr> &arguments,
                    int call_line) {
    if (InstanceOf<BigFloat>(arguments[0])) {
        return arguments[0];
    } else if (InstanceOf<String>(arguments[0])) {
        return std::make_shared<BigFloat>(cast<String>(arguments[0])->str);
    } else {
        throw RuntimeError(call_line, "num() need one 'Number' or 'String' argument");
    }
}

std::string Num::toString() {
    return "<built in function num>\n"
           "usage: num(variable: Number/String)->Number";
}

}  // namespace eagle::built_in_functions