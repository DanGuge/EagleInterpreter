//
// Created by DanGuge on 2022/11/7.
//

#pragma once

#include <vector>

#include "Number.h"
#include "token.h"

namespace eagle {

class Lexer {
public:
    Lexer() = default;
    std::vector<TokenPtr> scanTokens(std::string source);

private:
    void reset();

    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, ObjectPtr literal);
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
    std::string source;
    std::vector<TokenPtr> tokens;
    int start;
    int current;
    int line;

    static std::unordered_map<std::string, TokenType> keywords;
};

}  // namespace eagle