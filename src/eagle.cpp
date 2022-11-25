//
// Created by DanGuge on 2022/11/10.
//

#include "eagle.h"

#include <iomanip>
#include <iostream>

#include "modules/eagle_string.h"
#include "util/error_reporter.h"

namespace eagle {

Eagle::Eagle() : lexer(), parser(), interpreter(), resolver(interpreter) {}

void Eagle::run(std::string source) {
    std::vector<TokenPtr> tokens = lexer.scanTokens(std::move(source));

//    const int print_literal_len = 30;
//    for (const auto& token : tokens) {
//        if (InstanceOf<String>(token->literal)) {
//            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
//                      << "String Literal " + cast<String>(token->literal)->str;
//        } else if (InstanceOf<Boolean>(token->literal)) {
//            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
//                      << "Boolean Literal " + cast<Boolean>(token->literal)->ToString();
//        } else if (InstanceOf<BigFloat>(token->literal)) {
//            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
//                      << "BigFloat Literal " + cast<BigFloat>(token->literal)->ToString();
//        } else if (InstanceOf<Null>(token->literal)) {
//            std::cout << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
//                      << "Null Literal ";
//        } else {
//            std::cerr << std::setiosflags(std::ios::left) << std::setw(print_literal_len)
//                      << "Error";
//        }
//        std::cout << token->toString() << std::endl;
//    }

    std::vector<StmtPtr> statements = parser.parse(std::move(tokens));
    // if had lexer or parser error then return
    if (ErrorReporter::getInstance().hasError())
        return;

    resolver.resolve(statements);
    // if had resolver error then return
    if (ErrorReporter::getInstance().hasError())
        return;

    interpreter.interpret(statements);
}

}  // namespace eagle