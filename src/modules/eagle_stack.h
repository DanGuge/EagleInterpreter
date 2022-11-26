//
// Created by Akman on 2022/11/26.
//

#pragma once
#include "eagle_exceptions.h"

namespace eagle {

class EagleStack {
public:
    static void Push();
    static void Pop();

private:
    static const int MAX_STACK_DEPTH = 500;
    static int stack_depth;
};

}  // namespace eagle
