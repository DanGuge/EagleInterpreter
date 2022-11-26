//
// Created by DanGuge on 2022/11/16.
//

#pragma once

#include "eagle_callable.h"
#include "eagle_function.h"
#include "interpreter/interpreter.h"
#include "object.h"

namespace eagle {

class EagleClass;
class EagleInstance;
using EagleClassPtr = std::shared_ptr<EagleClass>;
using EagleInstancePtr = std::shared_ptr<EagleInstance>;

class EagleClass : public EagleCallable, public std::enable_shared_from_this<EagleClass> {
public:
    EagleClass(std::string name, EagleClassPtr super_class,
               std::unordered_map<std::string, ObjectPtr> members,
               std::unordered_map<std::string, EagleFunctionPtr> methods);
    std::string toString() override;
    int arity() override;
    ObjectPtr call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments, int call_line) override;
    EagleInstancePtr instanceVarInit();
    EagleFunctionPtr getMethodRecursive(const std::string& method_name);
    EagleFunctionPtr getMethodLocal(const std::string& method_name);
    std::string getLocalMethodInfo();

public:
    std::string name;
    EagleClassPtr super_class;
    std::unordered_map<std::string, ObjectPtr> members;
    std::unordered_map<std::string, EagleFunctionPtr> methods;
};

class EagleInstance : public Object {
public:
    EagleInstance(EagleClassPtr klass, EagleInstancePtr super_instance);
    ObjectPtr get(const TokenPtr& name, const EagleInstancePtr& instance);
    void set(const std::string& name, ObjectPtr value);
    std::string toString() override;

private:
    EagleClassPtr klass;
    EagleInstancePtr super_instance;
    std::unordered_map<std::string, ObjectPtr> fields;
};

}  // namespace eagle