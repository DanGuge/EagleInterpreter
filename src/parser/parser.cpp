//
// Created by Akman on 2022/11/6.
//
#include "parser.h"

#include <utility>

#include "util/error_reporter.h"

namespace eagle {

std::vector<StmtPtr> Parser::parse(std::vector<TokenPtr> tokens_) {
    reset();
    tokens = std::move(tokens_);

    while (!isAtEnd()) {
        StmtPtr stmt = declarationOrStatement();
        if (stmt != nullptr)
            stmts.emplace_back(stmt);
    }
    return std::move(stmts);
}

void Parser::reset() {
    current = 0;
    tokens.clear();
}

/*
 * declaration  ::= class-declaration | function-declaration | variable-declaration;
 * statement    ::= if-statement | for-statement | while-statement | expression-statement |
 *                  print-statement | return-statement | break-statement | continue-statement |
 *                  block-statement ;
 */
StmtPtr Parser::declarationOrStatement() {
    try {
        if (match(CLASS))
            return classDecl();
        if (match(VAR))
            return varDecl();
        if (match(DEF))
            return function("function");
        return statement();
    } catch (ParserError& parser_exception) {
        synchronize();
        return nullptr;
    }
}

// declarations
/*
 * have processed token "class"
 * class-declaration    ::= "class" identifier ("extends" identifier)?
 *                          "{" (variable-declaration|function-declaration)* "}" ;
 */
StmtPtr Parser::classDecl() {
    TokenPtr name = consume(IDENTIFIER, "Expect class name after 'class' keyword");

    std::shared_ptr<Expr::Variable> superclass = nullptr;
    if (match(EXTENDS)) {
        consume(IDENTIFIER, "Expect superclass name after 'extends' keyword");
        superclass = std::make_shared<Expr::Variable>(previous());
    }

    consume(LEFT_BRACE, "Expect '{' before class body");
    std::vector<std::shared_ptr<Stmt::Var>> members{};
    std::vector<std::shared_ptr<Stmt::Function>> methods{};
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        if (match(VAR)) {
            members.emplace_back(cast<Stmt::Var>(varDecl()));
        } else if (match(DEF)) {
            methods.emplace_back(cast<Stmt::Function>(function("method")));
        } else {
            throw error(peek(), "Illegal token '" + peek()->text +
                                    "' in class declaration, Expect 'def' or 'var'");
        }
    }
    consume(RIGHT_BRACE, "Expect '}' after class body");

    return std::make_shared<Stmt::Class>(name, superclass, std::move(members), std::move(methods));
}

/*
 * have processed token "var"
 * variable-declaration ::= "var" identifier ("=" expression)? ";" ;
 */
StmtPtr Parser::varDecl() {
    TokenPtr name = consume(IDENTIFIER, "Expect variable name after 'var' keyword");

    ExprPtr initializer = nullptr;
    if (match(ASSIGN)) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration");

    return std::make_shared<Stmt::Var>(name, initializer);
}

/*
 * have processed token "def"
 * function-declaration ::= "def" identifier "(" parameters? ")" block-statement ;
 * parameters           ::= identifier ("," identifier)* ;
 */
StmtPtr Parser::function(const std::string& kind) {
    TokenPtr name = consume(IDENTIFIER, "Expect function name after 'def' keyword");

    consume(LEFT_PAREN, "Expect '(' after " + kind + " name");
    std::vector<TokenPtr> parameters{};
    if (!check(RIGHT_PAREN)) {
        do {
            parameters.emplace_back(consume(IDENTIFIER, "Expect function parameters name or ')'"));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters");

    consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<StmtPtr> body = block();

    return std::make_shared<Stmt::Function>(name, std::move(parameters), body);
}

// statements
StmtPtr Parser::statement() {
    if (match(IF))
        return ifStmt();
    if (match(FOR))
        return forStmt();
    if (match(WHILE))
        return whileStmt();
    if (match(PRINT))
        return printStmt();
    if (match(RETURN))
        return returnStmt();
    if (match(BREAK))
        return breakStmt();
    if (match(CONTINUE))
        return continueStmt();
    if (match(LEFT_BRACE))
        return std::make_shared<Stmt::Block>(block());
    if (match(SEMICOLON))
        return nullptr;
    return expressionStmt();
}

/*
 * have processed token "if"
 * if-statement ::= "if" "(" condition ")" statement ("else" statement)? ;
 */
StmtPtr Parser::ifStmt() {
    consume(LEFT_PAREN, "Expect '(' after 'if' keyword");
    ExprPtr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after 'if' condition");

    StmtPtr then_branch = statement();
    StmtPtr else_branch = nullptr;
    if (match(ELSE)) {
        else_branch = statement();
    }

    return std::make_shared<Stmt::If>(condition, then_branch, else_branch);
}

/*
 * have processed token "for"
 * for-statement    ::= "for" "(" (variable-declaration | expression-statement | ";")?
 *                      expression? ";" expression? ")"
 *                      statement ;
 */
StmtPtr Parser::forStmt() {
    consume(LEFT_PAREN, "Expect '(' after 'for' keyword");

    StmtPtr initializer;
    if (match(SEMICOLON)) {
        initializer = nullptr;
    } else if (match(VAR)) {
        initializer = varDecl();
    } else {
        initializer = expressionStmt();
    }

    ExprPtr condition;
    if (!check(SEMICOLON)) {
        condition = expression();
    } else {
        condition = std::make_shared<Expr::Literal>(std::make_shared<Boolean>(true));
    }
    consume(SEMICOLON, "Expect ';' after 'for' condition");

    StmtPtr increment = nullptr;
    if (!check(RIGHT_PAREN)) {
        increment = std::make_shared<Stmt::Expression>(expression());
    }
    consume(RIGHT_PAREN, "Expect ')' after 'for' declaration");

    StmtPtr body = statement();

    return std::make_shared<Stmt::For>(initializer, condition, increment, body);
}

/*
 * have processed token "while"
 * while-statement  ::= "while" "(" expression ")" statement ;
 */
StmtPtr Parser::whileStmt() {
    consume(LEFT_PAREN, "Expect '(' after 'while' keyword");
    ExprPtr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after 'while' condition");

    StmtPtr body = statement();

    return std::make_shared<Stmt::While>(condition, body);
}

/*
 * expression-statement ::= expression ";" ;
 */
StmtPtr Parser::expressionStmt() {
    ExprPtr expr = expression();
    consume(SEMICOLON, "Expect ';' after expression statement");

    return std::make_shared<Stmt::Expression>(expr);
}

/*
 * have processed token "print"
 * print-statement  ::= "print" expression ";" ;
 */
StmtPtr Parser::printStmt() {
    ExprPtr print_value = expression();
    consume(SEMICOLON, "Expect ';' after print statement");

    return std::make_shared<Stmt::Print>(print_value);
}

/*
 * have processed token "return"
 * return-statement ::= "return" expression ";" ;
 */
StmtPtr Parser::returnStmt() {
    int line = previous()->line;
    ExprPtr return_value = nullptr;
    if (!check(SEMICOLON)) {
        return_value = expression();
    }
    consume(SEMICOLON, "Expect ';' after return statement");

    return std::make_shared<Stmt::Return>(return_value, line);
}

/*
 * have processed token "break"
 * break-statement  ::= "break" ";" ;
 */
StmtPtr Parser::breakStmt() {
    TokenPtr keyword = previous();
    consume(SEMICOLON, "Expect ';' after break statement");

    return std::make_shared<Stmt::Break>(keyword);
}

/*
 * have processed token "continue"
 * continue-statement   ::= "continue" ";" ;
 */
StmtPtr Parser::continueStmt() {
    TokenPtr keyword = previous();
    consume(SEMICOLON, "Expect ';' after continue statement");

    return std::make_shared<Stmt::Continue>(keyword);
}

/*
 * have processed token "{"
 * block-statement  ::= "{" (declaration|statement)* "}" ;
 */
std::vector<StmtPtr> Parser::block() {
    std::vector<StmtPtr> statements{};
    while (!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.emplace_back(declarationOrStatement());
    }
    consume(RIGHT_BRACE, "Expect '}' after block statement");

    return statements;
}

// expressions
/*
 * expression   ::= assignment_expression ;
 */
ExprPtr Parser::expression() {
    return assignmentExpr();
}

/*
 * assignment_expression    ::= call ("="|"+" "="|"-" "="|"*" "="|"/" "="|"%" "=")
 *                              assignment_expression | ternary_if_else ;
 */
ExprPtr Parser::assignmentExpr() {
    ExprPtr expr = ternary();
    if (match({ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTI_ASSIGN, DIV_ASSIGN, MOD_ASSIGN})) {
        TokenPtr op = previous();
        ExprPtr value = assignmentExpr();
        if (InstanceOf<Expr::Variable>(expr)) {
            TokenPtr name = cast<Expr::Variable>(expr)->name;
            return std::make_shared<Expr::Assign>(name, op, value);
        } else if (InstanceOf<Expr::InstanceGet>(expr)) {
            ExprPtr object = cast<Expr::InstanceGet>(expr)->object;
            TokenPtr name = cast<Expr::InstanceGet>(expr)->name;
            return std::make_shared<Expr::InstanceSet>(object, name, op, value);
        } else if (InstanceOf<Expr::ContainerGet>(expr)) {
            ExprPtr container = cast<Expr::ContainerGet>(expr)->container;
            ExprPtr subscript = cast<Expr::ContainerGet>(expr)->subscript;
            return std::make_shared<Expr::ContainerSet>(container, subscript, op, value);
        } else {
            throw error(op, "Invalid assignment target");
        }
    }

    return expr;
}

/*
 * ternary_if_else  ::= logic_or ("?" ternary_if_else ":" ternary_if_else)? ;
 */
ExprPtr Parser::ternary() {
    ExprPtr expr = logicOr();

    if (match(QUESTION_MARK)) {
        ExprPtr then_expr = ternary();
        consume(COLON, "Expect ':' in ternary expression");
        ExprPtr else_expr = ternary();
        return std::make_shared<Expr::Ternary>(expr, then_expr, else_expr);
    }

    return expr;
}

/*
 * logic_or ::= logic_and ("or" logic_and)* ;
 */
ExprPtr Parser::logicOr() {
    ExprPtr expr = logicAnd();

    while (match(OR)) {
        TokenPtr op = previous();
        ExprPtr right = logicAnd();
        expr = std::make_shared<Expr::Binary>(expr, op, right);
    }

    return expr;
}

/*
 * logic_and    ::= comparison ("and" comparison)* ;
 */
ExprPtr Parser::logicAnd() {
    ExprPtr expr = comparison();

    while (match(AND)) {
        TokenPtr op = previous();
        ExprPtr right = comparison();
        expr = std::make_shared<Expr::Binary>(expr, op, right);
    }

    return expr;
}

/*
 * comparison   ::= term (("!="|"=="|">"|">="|"<"|"<=") term)* ;
 */
ExprPtr Parser::comparison() {
    ExprPtr expr = term();

    if (match({EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        std::vector<std::pair<TokenPtr, ExprPtr>> others{};
        do {
            TokenPtr op = previous();
            ExprPtr right = term();
            others.emplace_back(std::pair<TokenPtr, ExprPtr>{op, right});
        } while (match({EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}));
        expr = std::make_shared<Expr::Compare>(expr, others);
    }

    return expr;
}

/*
 * term ::= factor (("-"|"+") factor)* ;
 */
ExprPtr Parser::term() {
    ExprPtr expr = factor();

    while (match({MINUS, PLUS})) {
        TokenPtr op = previous();
        ExprPtr right = factor();
        expr = std::make_shared<Expr::Binary>(expr, op, right);
    }

    return expr;
}

/*
 * factor ::= unary (("*"|"/"|"%") unary)* ;
 */
ExprPtr Parser::factor() {
    ExprPtr expr = unary();

    while (match({MULTI, DIV, MOD})) {
        TokenPtr op = previous();
        ExprPtr right = unary();
        expr = std::make_shared<Expr::Binary>(expr, op, right);
    }

    return expr;
}

/*
 * unary    ::= ("!"|"-"|"not") unary | call ;
 */
ExprPtr Parser::unary() {
    TokenPtr op = nullptr;
    if (match({EXCLAMATION_MARK, MINUS, NOT})) {
        op = previous();
        return std::make_shared<Expr::Unary>(op, unary());
    }
    return call();
}

/*
 * call         ::= primary ("." identifier | ("[" expression "]")+ | "(" arguments? ")")* ;
 * arguments    ::= expression ("," expression) ;
 */
ExprPtr Parser::call() {
    ExprPtr expr = primary();

    while (true) {
        if (match(DOT)) {
            TokenPtr name = consume(IDENTIFIER, "Expect identifier after '.' operator");
            expr = std::make_shared<Expr::InstanceGet>(expr, name);
        } else if (match(LEFT_BRACKET)) {
            do {
                int line = peek()->line;
                ExprPtr subscript = expression();
                consume(RIGHT_BRACKET, "Expect ']' after subscript");
                expr = std::make_shared<Expr::ContainerGet>(expr, subscript, line);
            } while (match(LEFT_BRACKET));
        } else if (match(LEFT_PAREN)) {
            std::vector<ExprPtr> arguments{};
            if (!check(RIGHT_PAREN)) {
                do {
                    arguments.emplace_back(expression());
                } while (match(COMMA));
            }
            consume(RIGHT_PAREN, "Expect ')' after arguments");
            expr = std::make_shared<Expr::Call>(expr, std::move(arguments), previous()->line);
        } else {
            break;
        }
    }

    return expr;
}

/*
 * primary          ::= "true" | "false" | "nil" | "this" | "super" "." identifier |
 *                      number | string | identifier | "(" expression ")" |
 *                      "[" list_elements? "]" | "{" dict_elements? "}" | "(" tuple_elements? ")" |
 *                      stream_expression | switch_expression ;
 * list_elements    ::= expression ("," expression)* ;
 * dict_elements    ::= expression ":" expression ("," expression ":" expression)* ;
 * tuple_elements   ::= (expression ",")? | expression ("," expression)+ ;
 */
ExprPtr Parser::primary() {
    if (match({BOOLEAN, NIL, NUMBER, STRING})) {
        return std::make_shared<Expr::Literal>(previous()->literal);
    } else if (match(THIS)) {
        return std::make_shared<Expr::This>(previous());
    } else if (match(SUPER)) {
        TokenPtr keyword = previous();
        consume(DOT, "Expect '.' after 'super' keyword");
        TokenPtr method = consume(IDENTIFIER, "Expect an identifier after 'super.'");
        return std::make_shared<Expr::Super>(keyword, method);
    } else if (match(IDENTIFIER)) {
        return std::make_shared<Expr::Variable>(previous());
    } else if (match(LEFT_PAREN)) {
        return parsePrimaryParen();
    } else if (match(LEFT_BRACKET)) {
        TokenPtr type = std::make_shared<Token>(LIST, previous()->line, "", nullptr);
        std::vector<ExprPtr> elements{};
        if (!check(RIGHT_BRACKET)) {
            do {
                elements.emplace_back(expression());
            } while (match(COMMA));
        }
        consume(RIGHT_BRACKET, "Expect ']' at the end of list");
        return std::make_shared<Expr::Sequence>(type, std::move(elements));
    } else if (match(LEFT_BRACE)) {
        TokenPtr type = std::make_shared<Token>(DICT, previous()->line, "", nullptr);
        std::vector<std::pair<ExprPtr, ExprPtr>> elements{};
        if (!check(RIGHT_BRACE)) {
            do {
                ExprPtr key = expression();
                consume(COLON, "Expect ':' after key in dict");
                ExprPtr value = expression();
                elements.emplace_back(std::pair<ExprPtr, ExprPtr>{key, value});
            } while (match(COMMA));
        }
        consume(RIGHT_BRACE, "Expect '}' at the end of dict");
        return std::make_shared<Expr::Associative>(type, std::move(elements));
    } else if (match(STREAM)) {
        return streamExpr();
    } else if (match(SWITCH)) {
        return switchExpr();
    } else {
        throw error(peek(), "Unexpected token: " + peek()->text);
    }
}

ExprPtr Parser::parsePrimaryParen() {
    TokenPtr type = std::make_shared<Token>(TUPLE, previous()->line, "", nullptr);
    if (match(RIGHT_PAREN)) {
        return std::make_shared<Expr::Sequence>(type, std::vector<ExprPtr>{});
    } else {
        ExprPtr expr = expression();
        if (match(COMMA)) {
            std::vector<ExprPtr> elements{};
            elements.emplace_back(expr);
            if (!check(RIGHT_PAREN)) {
                do {
                    elements.emplace_back(expression());
                } while (match(COMMA));
            }
            consume(RIGHT_PAREN, "Expect ')' at the end of tuple");
            return std::make_shared<Expr::Sequence>(type, std::move(elements));
        } else if (match(RIGHT_PAREN)) {
            return expr;
        } else {
            throw error(peek(), "Unexpected token: " + peek()->text);
        }
    }
}

/*
 * have processed token "stream"
 * stream_expression ::= "stream" "(" expression ")" ("." identifier "(" call? ")")* ;
 */
ExprPtr Parser::streamExpr() {
    consume(LEFT_PAREN, "Expect '(' after 'stream'");
    ExprPtr expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after stream initial expression");
    std::vector<std::pair<TokenPtr, ExprPtr>> operations;
    while (match(DOT)) {
        TokenPtr stream_method = consume(IDENTIFIER, "Expect an identifier after '.' in stream");
        consume(LEFT_PAREN, "Expect '(' after identifier in stream");
        ExprPtr inner_method;
        if (match(RIGHT_PAREN)) {
            inner_method = nullptr;
        } else {
            inner_method = call();
            consume(RIGHT_PAREN, "Expect ')' after expression in stream");
        }
        operations.emplace_back(std::pair<TokenPtr, ExprPtr>(stream_method, inner_method));
    }
    return std::make_shared<Expr::Stream>(expr, std::move(operations));
}

/*
 * have processed token "switch"
 * switch_expression ::= "switch" "(" expression ")" "{" ("case" expression ":" expression ",")*
 *                       "default" ":" expression "}" ;
 */
ExprPtr Parser::switchExpr() {
    consume(LEFT_PAREN, "Expect '(' after 'switch'");
    ExprPtr expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after switch expression");
    consume(LEFT_BRACE, "Expect '{' after ')' in switch");
    std::vector<std::pair<ExprPtr, ExprPtr>> case_results;
    while (!match(DEFAULT)) {
        consume(CASE, "Expect 'case'");
        ExprPtr case_condition = expression();
        consume(COLON, "Expect ':' after case condition expression");
        ExprPtr case_result = expression();
        consume(COMMA, "Expect ',' after case result expression");
        case_results.emplace_back(std::pair<ExprPtr, ExprPtr>(case_condition, case_result));
    }
    consume(COLON, "Expect ':' after 'default'");
    ExprPtr default_result = expression();
    consume(RIGHT_BRACE, "Expect '}' at the end of switch expression");
    return std::make_shared<Expr::Switch>(expr, std::move(case_results), default_result);
}

bool Parser::match(const TokenType& type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

TokenPtr Parser::consume(TokenType type, const std::string& message) {
    if (check(type))
        return advance();

    throw error(peek(), message);
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

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        switch (peek()->type) {
            case SEMICOLON: advance(); return;
            case CLASS:
            case DEF:
            case VAR:
            case IF:
            case WHILE:
            case FOR:
            case PRINT:
            case RETURN:
            case BREAK:
            case CONTINUE: return;
            default: advance();
        }
    }
}

Parser::ParserError Parser::error(const TokenPtr& token, const std::string& message) {
    ErrorReporter::getInstance().error(token, message);
    return {};
}

}  // namespace eagle
