//
// Created by DanGuge on 2022/11/7.
//

#pragma once

#include <vector>

#include "BigFloat.h"
#include "token.h"

namespace eagle {

class Lexer {
public:
    explicit Lexer(std::string source);
    std::vector<std::shared_ptr<Token>>& scanTokens();

private:
    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, std::shared_ptr<Object> literal);
    void addNum();
    void addString();
    void addIdentifier();
    bool isAtEnd();
    static bool isAlpha(char c);
    static bool isAlphaOrDigit(char c);
    bool match(char expected);
    char advance();
    char peekChar();
    char peekNextChar();

private:
    const std::string source;
    std::vector<std::shared_ptr<Token>> tokens;
    int start;
    int current;
    int line;

    static std::unordered_map<std::string, TokenType> keywords;
};

}  // namespace eagle