//
// Created by DanGuge on 2022/11/7.
//

#pragma once

#include <any>
#include <string>
#include <unordered_map>

#include "object.h"

namespace eagle {

class Token;
using TokenPtr = std::shared_ptr<Token>;

enum TokenType {
    // Arithmetic operators
    MINUS,
    PLUS,
    MULTI,
    DIV,
    MOD,
    ASSIGN,
    MINUS_ASSIGN,
    PLUS_ASSIGN,
    MULTI_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,

    // Logical operators
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    // Literals
    IDENTIFIER,
    NUMBER,
    STRING,
    BOOLEAN,
    LIST,
    DICT,
    TUPLE,

    // Keywords
    VAR,
    AND,
    OR,
    NOT,
    NIL,
    FOR,
    WHILE,
    BREAK,
    CONTINUE,
    IF,
    ELSE,
    SWITCH,
    CASE,
    DEFAULT,
    STREAM,
    DEF,
    CLASS,
    SUPER,
    THIS,
    EXTENDS,
    PRINT,
    RETURN,

    // Single character
    COMMA,
    DOT,
    COLON,
    SEMICOLON,
    QUESTION_MARK,
    EXCLAMATION_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,

    // Two characters
    RIGHT_ARROW,

    // Help
    END,
};

class Token {
public:
    Token(TokenType type, int line, std::string text, ObjectPtr literal);
    std::string toString();

public:
    const TokenType type;
    const int line;
    const std::string text;
    const ObjectPtr literal;

    static std::unordered_map<TokenType, std::string> type_to_string;
};

}  // namespace eagle