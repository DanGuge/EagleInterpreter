//
// Created by DanGuge on 2022/11/19.
//
#include "eagle_exceptions.h"

#include <utility>

namespace eagle {

EagleRuntimeError::EagleRuntimeError(std::string message) : message(std::move(message)) {}

EagleRuntimeError RuntimeError(int line, const std::string& message) {
    return EagleRuntimeError("[RuntimeError at line " + std::to_string(line) + "] " + message);
}

EagleRuntimeError RuntimeError(const std::string& message) {
    return EagleRuntimeError("[RuntimeError] " + message);
}

EagleReturn::EagleReturn(ObjectPtr object) : return_value(std::move(object)) {}
}  // namespace eagle
