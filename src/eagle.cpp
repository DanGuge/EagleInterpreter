//
// Created by DanGuge on 2022/11/10.
//

#include "eagle.h"

#include <iomanip>
#include <iostream>

#include "parser/parser.h"

namespace eagle {
bool Eagle::had_error = false;
bool Eagle::had_runtime_error = false;

void Eagle::run(std::string source) {

    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(std::move(source));
    std::vector<std::shared_ptr<Token>>& tokens = lexer->scanTokens();

    const int print_literal_len = 30;
    for (const auto& token : tokens) {
        if (InstanceOf<String>(token->literal)) {
            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
                      << "String Literal " + cast<String>(token->literal)->str;
        } else if (InstanceOf<Boolean>(token->literal)) {
            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
                      << "Boolean Literal " + cast<Boolean>(token->literal)->ToString();
        } else if (InstanceOf<BigFloat>(token->literal)) {
            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
                      << "BigFloat Literal " + cast<BigFloat>(token->literal)->ToString();
        } else if (InstanceOf<Null>(token->literal)) {
            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
                      << "Null Literal ";
        } else {
            std::cerr << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
                      << "Error";
        }
        std::cout << token->toString() << std::endl;
    }

    // if had lexer error then return
    if (had_error)
        return;

    std::shared_ptr<Parser> parser = std::make_shared<Parser>(tokens);
    std::vector<StmtPtr> statements = parser->parse();
    // if had parser error then return
    if (had_error)
        return;
}

void Eagle::error(int line, const std::string& message) {
    errorReport(line, "", message);
}

void Eagle::error(const TokenPtr& token, const std::string& message) {
    if (token->type == TokenType::END) {
        errorReport(token->line, "At the end", message);
    } else {
        errorReport(token->line, "At '" + token->text + "'", message);
    }
}

void Eagle::errorReport(int line, const std::string& where, const std::string& message) {
    had_error = true;
    std::cerr << "[Error at line " << std::to_string(line) << "] " + where + " " + message
              << std::endl;
}

}  // namespace eagle