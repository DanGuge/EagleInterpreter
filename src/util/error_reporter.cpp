//
// Created by Akman on 2022/11/18.
//

#include "error_reporter.h"

#include "pretty_print/pretty_print.h"

namespace eagle {

ErrorReporter &ErrorReporter::getInstance() {
    return error_reporter;
}

ErrorReporter ErrorReporter::error_reporter{};

void ErrorReporter::error(const EagleRuntimeError &error) {
    error_info.emplace_back(error.message);
}

void ErrorReporter::error(int line, const std::string &message) {
    errorReport(line, "", message);
}

void ErrorReporter::error(const TokenPtr &token, const std::string &message) {
    if (token->type == TokenType::END) {
        errorReport(token->line, "At the end:", message);
    } else {
        errorReport(token->line, "At '" + token->text + "':", message);
    }
}

void ErrorReporter::errorReport(int line, const std::string &where, const std::string &message) {
    error_info.emplace_back("[Error at line " + std::to_string(line) + "] " + where + " " +
                            message);
}

bool ErrorReporter::hasError() {
    return !error_info.empty();
}

void ErrorReporter::reset() {
    error_info.clear();
}

void ErrorReporter::printErrors() {
    for (auto &info : error_info) {
        pretty_print::PrettyPrint::print(
            pretty_print::Front::Color::RED,
            {pretty_print::BOLD, pretty_print::ITALIC, pretty_print::UNDERLINE}, info + "\n");
    }
}

}  // namespace eagle
