//
// Created by Akman on 2022/11/20.
//

#pragma once

#include <memory>
#include <vector>

#include "object.h"

namespace eagle {

class EagleCallable : public Object {
public:
    virtual int arity() = 0;
    virtual ObjectPtr call(std::vector<ObjectPtr>& arguments) = 0;
};

using EagleCallablePtr = std::shared_ptr<EagleCallable>;

}  // namespace eagle
