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

ObjectPtr EagleFunction::call(std::vector<ObjectPtr>& arguments, int call_line) {
    EnvironmentPtr function_env = std::make_shared<Environment>(closure);
    for (int i = 0; i < this->arity(); i++) {
        function_env->define(declaration->params[i]->text, arguments[i]);
    }

    try {
        Interpreter::getInstance().executeBlock(declaration->body, function_env);
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

std::string EagleFunction::toString() {
    return "<function " + declaration->name->text + ">";
}

EagleLambda::EagleLambda(std::shared_ptr<Expr::Lambda> declaration, EnvironmentPtr closure)
    : declaration(std::move(declaration)), closure(std::move(closure)) {}

int EagleLambda::arity() {
    return (int)declaration->params.size();
}

ObjectPtr EagleLambda::call(std::vector<ObjectPtr>& arguments, int call_line) {
    EnvironmentPtr lambda_env = std::make_shared<Environment>(closure);
    for (int i = 0; i < this->arity(); i++) {
        lambda_env->define(declaration->params[i]->text, arguments[i]);
    }

    try {
        Interpreter::getInstance().execute(declaration->body, lambda_env);
    } catch (EagleReturn& eagle_return) { return eagle_return.return_value; }
    return std::make_shared<Null>();
}

std::string EagleLambda::toString() {
    return "<lambda at " + std::to_string(reinterpret_cast<size_t>(this)) + ">";
}

}  // namespace eagle