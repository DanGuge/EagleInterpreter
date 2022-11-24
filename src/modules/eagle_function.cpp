//
// Created by DanGuge on 2022/11/22.
//

#include "eagle_function.h"

#include "eagle_callable.h"
#include "eagle_class.h"

namespace eagle {
EagleFunction::EagleFunction(std::shared_ptr<Stmt::Function> declaration, EnvironmentPtr closure,
                             bool is_init)
    : declaration(std::move(declaration)), closure(std::move(closure)), is_init(is_init) {}

int EagleFunction::arity() {
    return (int)declaration->params.size();
}

ObjectPtr EagleFunction::call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments) {
    EnvironmentPtr function_env = std::make_shared<Environment>(closure);
    for (int i = 0; i < this->arity(); i++) {
        function_env->define(declaration->params[i]->text, arguments[i]);
    }

    try {
        interpreter.executeBlock(declaration->body, function_env);
    } catch (EagleReturn& eagle_return) {
        if (is_init)
            return closure->getAt(0, "this");
        return eagle_return.return_value;
    }

    if (is_init)
        return closure->getAt(0, "this");
    return std::make_shared<Null>();
}

EagleFunctionPtr EagleFunction::bind(const EagleInstancePtr& instance) {
    EnvironmentPtr instance_env = std::make_shared<Environment>(closure);
    instance_env->define("this", instance);
    return std::make_shared<EagleFunction>(declaration, instance_env, is_init);
}

std::string EagleFunction::ToString() {
    return "<function " + declaration->name->text + ">";
}
}  // namespace eagle