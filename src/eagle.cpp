//
// Created by DanGuge on 2022/11/10.
//

#include "eagle.h"

#include <iostream>

namespace eagle {
bool Eagle::had_error = false;
bool Eagle::had_runtime_error = false;

void Eagle::run(std::string source) {
    had_error = false;

    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(std::move(source));
    std::vector<std::shared_ptr<Token>> tokens = lexer->scanTokens();

    for (const auto& token : tokens) {
        if (InstanceOf<String>(token->literal)) {
            std::cout << "String Literal " << cast<String>(token->literal)->str << "\t";
        } else if (InstanceOf<Boolean>(token->literal)) {
            std::cout << "Boolean Literal " << cast<Boolean>(token->literal)->value << "\t";
        } else if (InstanceOf<BigFloat>(token->literal)) {
            std::cout << "BigFloat Literal " << cast<BigFloat>(token->literal)->ToString() << "\t";
        } else if (InstanceOf<Null>(token->literal)) {
            std::cout << "Null Literal "
                      << "\t";
        } else {
            std::cerr << "Error"
                      << "\t";
        }
        std::cout << token->toString() << std::endl;
    }

    if (had_error)
        return;
}

void Eagle::error(int line, const std::string& message) {
    errorReport(line, "", message);
}

void Eagle::error(const Token& token, const std::string& message) {
    if (token.type == TokenType::END) {
        errorReport(token.line, "At the end", message);
    } else {
        errorReport(token.line, "At '" + token.text + "'", message);
    }
}

void Eagle::errorReport(int line, const std::string& where, const std::string& message) {
    had_error = true;
    std::cerr << "[Error at line " << std::to_string(line) << "] " + where + " " + message
              << std::endl;
}

}  // namespace eagle