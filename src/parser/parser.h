//
// Created by Akman on 2022/11/6.
//

#pragma once

#include "token.h"

#include "ast/expr.h"
#include "ast/stmt.h"
#include "object.h"

#include <vector>

namespace eagle {

class Parser {
public:
    explicit Parser(std::vector<TokenPtr> tokens);
    std::vector<ast::StmtPtr> parse();
private:
    class ParserError: std::exception {
    };
private:
    ast::StmtPtr declarationOrStatement();
    // declaration
    ast::StmtPtr classDecl();
    ast::StmtPtr function(std::string kind);
    ast::StmtPtr varDecl();
    // statement
    ast::StmtPtr ifStmt();
    ast::StmtPtr forStmt();
    ast::StmtPtr whileStmt();
    ast::StmtPtr expressionStmt();
    ast::StmtPtr printStmt();
    ast::StmtPtr returnStmt();
    ast::StmtPtr breakStmt();
    ast::StmtPtr continueStmt();
    std::vector<ast::StmtPtr> block();
    // expression
    ast::ExprPtr expression();
    ast::ExprPtr assignmentExpr();
    ast::ExprPtr ternary();
    ast::ExprPtr logicOr();
    ast::ExprPtr logicAnd();
    ast::ExprPtr equality();
    ast::ExprPtr comparison();
    ast::ExprPtr term();
    ast::ExprPtr factor();
    ast::ExprPtr unary();
    ast::ExprPtr call();
    ast::ExprPtr subscript();
    ast::ExprPtr primary();
    ast::ExprPtr streamExpr();
    ast::ExprPtr switchExpr();
    // help function
    bool match(const std::vector<TokenType>& types);
    TokenPtr consume(TokenType type, std::string message);
    bool check(TokenType type);
    TokenPtr advance();
    bool isAtEnd();
    TokenPtr peek();
    TokenPtr previous();
    // error handling
    ParserError error(TokenPtr token, std::string message);
private:
    std::vector<ast::StmtPtr> stmts;
    std::vector<TokenPtr> tokens;
    int current;
};

}  // namespace eagle
