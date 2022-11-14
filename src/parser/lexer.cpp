//
// Created by DanGuge on 2022/11/7.
//

#include "lexer.h"

#include "eagle.h"

namespace eagle {

Lexer::Lexer(std::string source) : source(std::move(source)), start(0), current(0), line(1) {}

std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"var", VAR},         {"and", AND},           {"or", OR},
    {"not", NOT},         {"true", BOOLEAN},      {"false", BOOLEAN},
    {"nil", NIL},         {"for", FOR},           {"while", WHILE},
    {"break", BREAK},     {"continue", CONTINUE}, {"if", IF},
    {"else", ELSE},       {"switch", SWITCH},     {"case", CASE},
    {"default", DEFAULT}, {"stream", STREAM},     {"def", DEF},
    {"class", CLASS},     {"super", SUPER},       {"this", THIS},
    {"extends", EXTENDS}, {"print", PRINT},       {"return", RETURN},
};

std::vector<TokenPtr>& Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(std::make_shared<Token>(END, line, "", std::make_shared<Null>()));
    return tokens;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '[': addToken(LEFT_BRACKET); break;
        case ']': addToken(RIGHT_BRACKET); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case '+': addToken(match('=') ? PLUS_EQUAL : PLUS); break;
        case '-': addToken(match('=') ? MINUS_EQUAL : MINUS); break;
        case '*': addToken(match('=') ? MULTI_EQUAL : MULTI); break;
        case '/':
            if (match('/')) {
                while (peekChar() != '\n' && !isAtEnd())
                    advance();
            } else {
                addToken(match('=') ? DIV_EQUAL : DIV);
            }
            break;
        case '%': addToken(match('=') ? MOD_EQUAL : MOD); break;
        case '=': addToken(match('=') ? EQUAL : ASSIGN); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
        case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
        case '?': addToken(QUESTION_MARK); break;
        case '!': addToken(match('=') ? NOT_EQUAL : EXCLAMATION_MARK); break;
        case ':': addToken(COLON); break;
        case ';': addToken(SEMICOLON); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': line++; break;
        // string
        case '"': addString(); break;
        default:
            if (isdigit(c)) {
                addNum();
            } else if (isAlpha(c)) {
                addIdentifier();
            } else {
                Eagle::error(line, "Unexpected character");
            }
            break;
    }
}

void Lexer::addNum() {
    while (isdigit(peekChar()))
        advance();

    if (peekChar() == '.' && isdigit(peekNextChar())) {
        advance();
        while (isdigit(peekChar()))
            advance();
    }

    addToken(NUMBER,
             std::make_shared<BigFloat>(std::string(source.substr(start, current - start))));
}

void Lexer::addString() {
    while (peekChar() != '"' && !isAtEnd()) {
        if (peekChar() == '\n')
            line++;
        advance();
    }

    if (isAtEnd()) {
        Eagle::error(line, "Expect '\"' at the end of string");
        return;
    }

    advance();
    addToken(STRING, std::make_shared<String>(source.substr(start + 1, current - start - 2)));
}

void Lexer::addIdentifier() {
    while (isAlphaOrDigit(peekChar()))
        advance();

    std::string identifier = source.substr(start, current - start);

    TokenType type;
    if (keywords.find(identifier) != keywords.end()) {
        type = keywords[identifier];
    } else {
        type = IDENTIFIER;
    }

    ObjectPtr object;
    if (identifier == "true")
        object = std::make_shared<Boolean>(true);
    else if (identifier == "false")
        object = std::make_shared<Boolean>(false);
    else
        object = std::make_shared<Null>();

    addToken(type, object);
}

void Lexer::addToken(TokenType type) {
    addToken(type, std::make_shared<Null>());
}

void Lexer::addToken(TokenType type, ObjectPtr literal) {
    std::string sub_str = source.substr(start, current - start);
    tokens.emplace_back(
        std::make_shared<Token>(type, line, std::move(sub_str), std::move(literal)));
}

bool Lexer::match(char expected) {
    if (peekChar() != expected)
        return false;

    advance();
    return true;
}

char Lexer::advance() {
    return source[current++];
}

bool Lexer::isAtEnd() {
    return current >= source.size();
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaOrDigit(char c) {
    return isAlpha(c) || isdigit(c);
}

char Lexer::peekChar() {
    if (isAtEnd())
        return '\0';
    return source[current];
}

char Lexer::peekNextChar() {
    if (current + 1 >= source.size())
        return '\0';
    return source[current + 1];
}

}  // namespace eagle