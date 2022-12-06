//
// Created by DanGuge on 2022/11/22.
//

#include "eagle_class.h"

#include <utility>

#include "eagle_function.h"
#include "eagle_string.h"

namespace eagle {

EagleClass::EagleClass(std::string name, EagleClassPtr super_class,
                       std::unordered_map<std::string, ObjectPtr> members,
                       std::unordered_map<std::string, EagleFunctionPtr> methods)
    : name(std::move(name))
    , super_class(std::move(super_class))
    , members(std::move(members))
    , methods(std::move(methods)) {}

std::string EagleClass::toString() {
    return "<class " + name + ">";
}

int EagleClass::arity() {
    EagleFunctionPtr init = getMethodLocal("init");
    if (init == nullptr)
        return 0;
    return init->arity();
}

ObjectPtr EagleClass::call(std::vector<ObjectPtr>& arguments, int call_line) {
    EagleInstancePtr instance = instanceVarInit();
    EagleFunctionPtr init = getMethodLocal("init");
    if (init != nullptr) {
        return init->bind(instance)->WrapperCall(arguments, call_line);
    }
    return instance;
}

EagleInstancePtr EagleClass::instanceVarInit() {
    EagleInstancePtr super_instance = nullptr;
    if (super_class != nullptr) {
        super_instance = super_class->instanceVarInit();
    }

    EagleInstancePtr instance = std::make_shared<EagleInstance>(shared_from_this(), super_instance);
    for (const auto& member : members) {
        instance->set(member.first, member.second);
    }
    return instance;
}

EagleFunctionPtr EagleClass::getMethodRecursive(const std::string& method_name) {
    if (methods.find(method_name) != methods.end()) {
        return methods[method_name];
    }

    if (super_class != nullptr) {
        return super_class->getMethodRecursive(method_name);
    }

    return nullptr;
}

EagleFunctionPtr EagleClass::getMethodLocal(const std::string& method_name) {
    if (methods.find(method_name) != methods.end()) {
        return methods[method_name];
    }

    return nullptr;
}

std::string EagleClass::getLocalMethodInfo() {
    std::string method_str;
    bool is_last = true;
    for (const auto& method : methods) {
        if (!is_last) {
            method_str.append("\n");
        }
        method_str.append("method " + method.first + " with " +
                          std::to_string(method.second->arity()) + " parameter(s)");
        is_last = false;
    }
    return std::move(method_str);
}

EagleInstance::EagleInstance(EagleClassPtr klass, EagleInstancePtr super_instance)
    : klass(std::move(klass)), super_instance(std::move(super_instance)), fields({}) {}

ObjectPtr EagleInstance::get(const std::string& name, const EagleInstancePtr& instance) {
    // 1. find in field
    if (fields.find(name) != fields.end()) {
        return fields[name];
    }

    // 2. find in local method
    if (klass->getMethodLocal(name) != nullptr) {
        return klass->getMethodLocal(name)->bind(instance);
    }

    // 3. find super instance
    if (super_instance != nullptr) {
        return super_instance->get(name, instance);
    }
    return nullptr;
}

void EagleInstance::set(const std::string& name, ObjectPtr value) {
    fields[name] = std::move(value);
}

std::string EagleInstance::toString() {
    auto method = get("toString", shared_from_this());
    if (InstanceOf<EagleCallable>(method) && cast<EagleCallable>(method)->arity() == 0) {
        std::vector<ObjectPtr> arguments;
        ObjectPtr result = cast<EagleCallable>(method)->WrapperCall(arguments, -1);
        if (InstanceOf<String>(result)) {
            return cast<String>(result)->str;
        }
        return result->toString();
    }
    return "<instance of class " + klass->name + ">";
}

bool EagleInstance::equals(eagle::ObjectPtr other) {
    if (other.get() == this) {
        return true;
    }
    if (!InstanceOf<EagleInstance>(other)) {
        return false;
    }
    EagleInstancePtr another = cast<EagleInstance>(other);
    if (this->klass != another->klass) {
        return false;
    }
    auto method = get("equals", shared_from_this());
    if (InstanceOf<EagleCallable>(method) && cast<EagleCallable>(method)->arity() == 1) {
        std::vector<ObjectPtr> arguments{another};
        ObjectPtr result = cast<EagleCallable>(method)->WrapperCall(arguments, -1);
        return result->isTruthy();
    }
    return false;
}

size_t EagleInstance::hashcode() {
    auto method = get("hashcode", shared_from_this());
    if (InstanceOf<EagleCallable>(method) && cast<EagleCallable>(method)->arity() == 0) {
        std::vector<ObjectPtr> arguments;
        ObjectPtr result = cast<EagleCallable>(method)->WrapperCall(arguments, -1);
        if (InstanceOf<Number>(result) && cast<Number>(result)->isInteger()) {
            return static_cast<size_t>(cast<Number>(result)->ToInt());
        }
    }
    return reinterpret_cast<size_t>(this);
}

bool EagleInstance::isTruthy() {
    auto method = get("isTruthy", shared_from_this());
    if (InstanceOf<EagleCallable>(method) && cast<EagleCallable>(method)->arity() == 0) {
        std::vector<ObjectPtr> arguments;
        ObjectPtr result = cast<EagleCallable>(method)->WrapperCall(arguments, -1);
        if (InstanceOf<Boolean>(result)) {
            return cast<Boolean>(result)->value;
        }
    }
    return true;
}

}  // namespace eagle
