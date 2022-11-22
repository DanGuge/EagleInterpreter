//
// Created by DanGuge on 2022/11/19.
//

#include "environment.h"

#include "modules/eagle_exceptions.h"
#include "util/error_reporter.h"

namespace eagle {

Environment::Environment(eagle::EnvironmentPtr enclosing) : enclosing(std::move(enclosing)) {}

void Environment::define(const std::string& name, const ObjectPtr& object) {
    name_object_map[name] = object;
}

ObjectPtr Environment::getAt(int distance, const std::string& name) {
    return ancestor(distance)->name_object_map[name];
}

ObjectPtr Environment::getAt(int distance, const TokenPtr& name) {
    return ancestor(distance)->name_object_map[name->text];
}

ObjectPtr Environment::get(const TokenPtr& name) {
    if (name_object_map.find(name->text) != name_object_map.end()) {
        return name_object_map[name->text];
    }

    if (enclosing != nullptr)
        return enclosing->get(name);

    environmentError(name, "Undefined variable '" + name->text + "'");
    return nullptr;
}

void Environment::assignAt(int distance, const TokenPtr& name, const ObjectPtr& object) {
    ancestor(distance)->name_object_map[name->text] = object;
}

void Environment::assign(const TokenPtr& name, const ObjectPtr& object) {
    if (name_object_map.find(name->text) != name_object_map.end()) {
        name_object_map[name->text] = object;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, object);
        return;
    }

    environmentError(name, "Undefined variable '" + name->text + "'");
}

EnvironmentPtr Environment::ancestor(int distance) {
    EnvironmentPtr target_env = shared_from_this();
    for (int i = 0; i < distance; i++) {
        target_env = target_env->enclosing;
    }
    return target_env;
}

void Environment::environmentError(const eagle::TokenPtr& name, std::string message) {
    throw EagleRuntimeError("[RuntimeError at line " + std::to_string(name->line) + "] " +
                            std::move(message));
}

ScopedEnvironment::ScopedEnvironment(EnvironmentPtr& current_env, EnvironmentPtr block_env)
    : reference_of_current_env(current_env), copy_of_previous_env(current_env) {
    reference_of_current_env = std::move(block_env);
}

ScopedEnvironment::~ScopedEnvironment() {
    reference_of_current_env = copy_of_previous_env;
}
}  // namespace eagle