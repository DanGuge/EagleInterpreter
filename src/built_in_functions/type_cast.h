//
// Created by DanGuge on 2022/11/26.
//

#pragma once

#include "modules/eagle_callable.h"

namespace eagle::built_in_functions {

class Str : public EagleCallable {
public:
    int arity() override;
    ObjectPtr call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments,
                   int call_line) override;
    std::string toString() override;
};

class Num : public EagleCallable {
public:
    int arity() override;
    ObjectPtr call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments,
                   int call_line) override;
    std::string toString() override;
};

}  // namespace eagle::built_in_functions