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
                                         std::vector<ObjectPtr>& arguments, int line);

struct BuiltInClassMethodInfo {
    BuiltInClassMethod method;
    int method_arity;
};

class BuiltInClass : public Object {
public:
    virtual BuiltInClassMethodInfo GetMethod(const TokenPtr& method_name) = 0;
    virtual std::string GetBuiltInClassInfo() = 0;
};

class BuiltInClassCall : public EagleCallable {
public:
    BuiltInClassCall(BuiltInClassPtr instance, BuiltInClassMethod method, int method_arity)
        : instance(std::move(instance)), method(method), method_arity(method_arity) {}

    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override {
        return method(instance, arguments, call_line);
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
