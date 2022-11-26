//
// Created by Akman on 2022/11/20.
//

#pragma once

#include <vector>

#include "eagle_stack.h"
#include "interpreter/interpreter.h"
#include "object.h"

namespace eagle {

class EagleCallable : public Object {
public:
    virtual int arity() = 0;
    virtual ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) = 0;
    ObjectPtr WrapperCall(std::vector<ObjectPtr>& arguments, int call_line) {
        EagleStack::Push();
        ObjectPtr value = this->call(arguments, call_line);
        EagleStack::Pop();
        return value;
    }
};

using EagleCallablePtr = std::shared_ptr<EagleCallable>;

}  // namespace eagle
