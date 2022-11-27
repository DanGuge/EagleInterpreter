//
// Created by DanGuge on 2022/11/22.
//

#pragma once

#include "eagle_callable.h"

namespace eagle {

class EagleFunction;
using EagleFunctionPtr = std::shared_ptr<EagleFunction>;
class EagleInstance;
using EagleInstancePtr = std::shared_ptr<EagleInstance>;

class EagleFunction : public EagleCallable {
public:
    EagleFunction(std::shared_ptr<Stmt::Function> declaration, EnvironmentPtr closure,
                  bool is_init);
    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;
    EagleFunctionPtr bind(const EagleInstancePtr& instance);
    std::string toString() override;

private:
    std::shared_ptr<Stmt::Function> declaration;
    EnvironmentPtr closure;
    bool is_init;
};

class EagleLambda : public EagleCallable {
public:
    EagleLambda(std::shared_ptr<Expr::Lambda> declaration, EnvironmentPtr closure);

    int arity() override;
    ObjectPtr call(std::vector<ObjectPtr> &arguments, int call_line) override;

    std::string toString() override;

private:
    std::shared_ptr<Expr::Lambda> declaration;
    EnvironmentPtr closure;
};
}  // namespace eagle