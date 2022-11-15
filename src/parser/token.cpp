//
// Created by DanGuge on 2022/11/7.
//

#include "token.h"

#include <memory>
#include <utility>

namespace eagle {

std::unordered_map<TokenType, std::string> Token::type_to_string = {
    {MINUS, "MINUS"},
    {PLUS, "PLUS"},
    {MULTI, "MULTI"},
    {DIV, "DIV"},
    {MOD, "MOD"},
    {MINUS_ASSIGN, "MINUS_ASSIGN"},
    {PLUS_ASSIGN, "PLUS_ASSIGN"},
    {MULTI_ASSIGN, "MULTI_ASSIGN"},
    {DIV_ASSIGN, "DIV_ASSIGN"},
    {MOD_ASSIGN, "MOD_ASSIGN"},
    {EQUAL, "EQUAL"},
    {NOT_EQUAL, "NOT_EQUAL"},
    {LESS, "LESS"},
    {LESS_EQUAL, "LESS_EQUAL"},
    {GREATER, "GREATER"},
    {GREATER_EQUAL, "GREATER_EQUAL"},
    {IDENTIFIER, "IDENTIFIER"},
    {NUMBER, "NUMBER"},
    {STRING, "STRING"},
    {BOOLEAN, "BOOLEAN"},
    {LIST, "LIST"},
    {DICT, "DICT"},
    {TUPLE, "TUPLE"},
    {VAR, "VAR"},
    {AND, "AND"},
    {OR, "OR"},
    {NOT, "NOT"},
    {NIL, "NIL"},
    {FOR, "FOR"},
    {WHILE, "WHILE"},
    {BREAK, "BREAK"},
    {CONTINUE, "CONTINUE"},
    {IF, "IF"},
    {ELSE, "ELSE"},
    {SWITCH, "SWITCH"},
    {CASE, "CASE"},
    {DEFAULT, "DEFAULT"},
    {STREAM, "STREAM"},
    {DEF, "DEF"},
    {CLASS, "CLASS"},
    {SUPER, "SUPER"},
    {THIS, "THIS"},
    {EXTENDS, "EXTENDS"},
    {PRINT, "PRINT"},
    {RETURN, "RETURN"},
    {END, "END"},
    {ASSIGN, "ASSIGN"},
    {COMMA, "COMMA"},
    {DOT, "DOT"},
    {COLON, "COLON"},
    {SEMICOLON, "SEMICOLON"},
    {QUESTION_MARK, "QUESTION_MARK"},
    {EXCLAMATION_MARK, "EXCLAMATION_MARK"},
    {LEFT_PAREN, "LEFT_PAREN"},
    {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACKET, "LEFT_BRACKET"},
    {RIGHT_BRACKET, "RIGHT_BRACKET"},
    {LEFT_BRACE, "LEFT_BRACE"},
    {RIGHT_BRACE, "RIGHT_BRACE"},
};

Token::Token(eagle::TokenType type, int line, std::string text, ObjectPtr literal)
    : type(type), line(line), text(std::move(text)), literal(std::move(literal)) {}

std::string Token::toString() {
    return "[" + type_to_string[type] + "] " + text + " at line " + std::to_string(line);
}

}  // namespace eagle