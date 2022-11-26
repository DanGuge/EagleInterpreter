//
// Created by DanGuge on 2022/11/26.
//

#pragma once

#include "modules/eagle_callable.h"

namespace eagle::built_in_functions {

class BuiltInFunction : public EagleCallable {
public:
    virtual std::string GetBuiltInFunctionInfo() = 0;
};

class ReadFromFile : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class WriteToFile : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Input : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Str : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Num : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Help : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Bool : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class ClassMethod : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Globals : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Id : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

class Len : public BuiltInFunction {
public:
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    std::string toString() override;
    std::string GetBuiltInFunctionInfo() override;
};

}  // namespace eagle::built_in_functions