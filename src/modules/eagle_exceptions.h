//
// Created by Akman on 2022/11/18.
//

#pragma once

#include <exception>
#include <string>

#include "object.h"

namespace eagle {

class EagleRuntimeError : public std::exception {
public:
    explicit EagleRuntimeError(std::string message);

public:
    std::string message;
};

EagleRuntimeError RuntimeError(int line, const std::string& message);

class EagleReturn : public std::exception {
public:
    explicit EagleReturn(ObjectPtr object);

public:
    ObjectPtr return_value;
};

class EagleBreak : public std::exception {
public:
    EagleBreak() = default;
};

class EagleContinue : public std::exception {
public:
    EagleContinue() = default;
};

}  // namespace eagle
