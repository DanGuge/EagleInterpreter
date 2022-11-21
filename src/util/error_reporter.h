//
// Created by Akman on 2022/11/18.
//

#pragma once
#include <string>
#include <vector>

#include "modules/eagle_exceptions.h"
#include "parser/token.h"

namespace eagle {

class ErrorReporter {
public:
    static ErrorReporter& getInstance();

private:
    static ErrorReporter error_reporter;

public:
    void error(int line, const std::string& message);

    void error(const TokenPtr& token, const std::string& message);

    void error(const EagleRuntimeError& error);

    bool hasError();

    void printErrors();

private:
    ErrorReporter() = default;

    void errorReport(int line, const std::string& where, const std::string& message);

    std::vector<std::string> error_info;
};

}  // namespace eagle
