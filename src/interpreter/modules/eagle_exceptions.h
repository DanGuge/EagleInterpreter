//
// Created by Akman on 2022/11/18.
//

#pragma once
#include <exception>
#include <string>

namespace eagle {

class EagleRuntimeError : public std::exception {
public:
    std::string message;
};

}
