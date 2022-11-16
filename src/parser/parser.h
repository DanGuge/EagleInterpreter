//
// Created by Akman on 2022/11/6.
//

#pragma once

#include <vector>

#include "ast/expr.h"
#include "ast/stmt.h"
#include "object.h"
#include "token.h"

namespace eagle {



class Parser {
public:
    explicit Parser(std::vector<TokenPtr> tokens);
    std::vector<StmtPtr> parse();

private:
    class ParserError : std::exception {};

private:
    StmtPtr declarationOrStatement();
    // declaration
    StmtPtr classDecl();
    StmtPtr function(const std::string& kind);
    StmtPtr varDecl();
    // statement
    StmtPtr statement();
    StmtPtr ifStmt();
    StmtPtr forStmt();
    StmtPtr whileStmt();
    StmtPtr expressionStmt();
    StmtPtr printStmt();
    StmtPtr returnStmt();
    StmtPtr breakStmt();
    StmtPtr continueStmt();
    std::vector<StmtPtr> block();
    // expression
    ExprPtr expression();
    ExprPtr assignmentExpr();
    ExprPtr ternary();
    ExprPtr logicOr();
    ExprPtr logicAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr call();
    ExprPtr primary();
    ExprPtr parsePrimaryParen();
    ExprPtr streamExpr();
    ExprPtr switchExpr();
    // help function
    bool match(const TokenType& type);
    bool match(const std::vector<TokenType>& types);
    TokenPtr consume(TokenType type, const std::string& message);
    bool check(TokenType type);
    TokenPtr advance();
    bool isAtEnd();
    TokenPtr peek();
    TokenPtr previous();
    // error handling
    static ParserError error(const TokenPtr& token, const std::string& message);

private:
    std::vector<StmtPtr> stmts;
    std::vector<TokenPtr> tokens;
    int current;
};

}  // namespace eagle
