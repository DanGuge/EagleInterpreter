//
// Created by Akman on 2022/11/21.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "eagle_callable.h"
#include "object.h"

namespace eagle {

class BuiltInClass;
using BuiltInClassPtr = std::shared_ptr<BuiltInClass>;
using BuiltInClassMethod = ObjectPtr (*)(const BuiltInClassPtr& instance,
                                         std::vector<ObjectPtr>& arguments);

struct BuiltInClassMethodInfo {
    BuiltInClassMethod method;
    int method_arity;
};

class BuiltInClass : public Object {
public:
    virtual BuiltInClassMethodInfo GetMethod(const std::string& method_name) = 0;
};

class BuiltInClassCallable : public EagleCallable {
public:
    BuiltInClassCallable(BuiltInClassPtr instance, BuiltInClassMethod method, int method_arity)
        : instance(std::move(instance)), method(method), method_arity(method_arity) {}

    ObjectPtr call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments) override {
        return method(instance, arguments);
    }

    int arity() override {
        return method_arity;
    }

private:
    BuiltInClassPtr instance;
    BuiltInClassMethod method;
    int method_arity;
};

}  // namespace eagle
