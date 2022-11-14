//
// Created by Akman on 2022/11/6.
//
#include "parser.h"

#include <utility>

namespace eagle {

Parser::Parser(std::vector<TokenPtr> tokens) : tokens(std::move(tokens)), current(0) {}

std::vector<ast::StmtPtr> Parser::parse() {
    while (!isAtEnd()) {
        stmts.emplace_back(declarationOrStatement());
    }
    return stmts;
}

/*
 * declaration ::= class-declaration | function-declaration | variable-declaration;
 * statement ::= if-statement | for-statement | while-statement | expression-statement |
 * print-statement | return-statement | break-statement | continue-statement | block-statement ;
 */
ast::StmtPtr Parser::declarationOrStatement() {
    try {
        if (match({CLASS}))
            return classDecl();
        if (match({VAR}))
            return varDecl();
        if (match({DEF}))
            return function("function");
        if (match({IF}))
            return ifStmt();
        if (match({FOR}))
            return forStmt();
        if (match({WHILE}))
            return whileStmt();
        if (match({PRINT}))
            return printStmt();
        if (match({RETURN}))
            return returnStmt();
        if (match({BREAK}))
            return breakStmt();
        if (match({CONTINUE}))
            return continueStmt();
        if (match({LEFT_BRACE}))
            return std::make_shared<ast::Stmt::Block>(block());
        return expressionStmt();
    } catch (std::exception parser_exception) {}
}
// declaration
//
ast::StmtPtr Parser::classDecl() {}
ast::StmtPtr Parser::varDecl() {}
ast::StmtPtr Parser::function(std::string kind) {}
// statement
ast::StmtPtr Parser::ifStmt() {}
ast::StmtPtr Parser::forStmt() {}
ast::StmtPtr Parser::whileStmt() {}
ast::StmtPtr Parser::expressionStmt() {}
ast::StmtPtr Parser::printStmt() {}
ast::StmtPtr Parser::returnStmt() {}
ast::StmtPtr Parser::breakStmt() {}
ast::StmtPtr Parser::continueStmt() {}
std::vector<ast::StmtPtr> Parser::block() {}
// expression
ast::ExprPtr Parser::expression() {}
ast::ExprPtr Parser::assignmentExpr() {}
ast::ExprPtr Parser::ternary() {}
ast::ExprPtr Parser::logicOr() {}
ast::ExprPtr Parser::logicAnd() {}
ast::ExprPtr Parser::equality() {}
ast::ExprPtr Parser::comparison() {}
ast::ExprPtr Parser::term() {}
ast::ExprPtr Parser::factor() {}
ast::ExprPtr Parser::unary() {}
ast::ExprPtr Parser::call() {}
ast::ExprPtr Parser::subscript() {}
ast::ExprPtr Parser::primary() {}
ast::ExprPtr Parser::streamExpr() {}
ast::ExprPtr Parser::switchExpr() {}

bool Parser::match(const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

TokenPtr Parser::consume(TokenType type, std::string message) {
    if (check(type))
        return advance();

    error(peek(), std::move(message));
}

bool Parser::check(TokenType type) {
    if (isAtEnd())
        return false;
    return peek()->type == type;
}

TokenPtr Parser::advance() {
    if (!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek()->type == END;
}

TokenPtr Parser::peek() {
    return tokens[current];
}

TokenPtr Parser::previous() {
    return tokens[current - 1];
}

Parser::ParserError Parser::error(TokenPtr token, std::string message) {
    throw Parser::ParserError();
}

}  // namespace eagle
