//
// Created by Akman on 2022/11/26.
//

#include "eagle_stack.h"

namespace eagle {

int EagleStack::stack_depth = 0;

void EagleStack::Push() {
    stack_depth++;
    if (stack_depth > MAX_STACK_DEPTH) {
        stack_depth = 0;
        throw RuntimeError("Maximum recursion depth exceeded");
    }
}

void EagleStack::Pop() {
    if (stack_depth > 0) {
        stack_depth--;
    }
}

}
