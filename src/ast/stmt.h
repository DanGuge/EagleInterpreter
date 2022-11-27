//
// Created by Akman on 2022/11/11.
//

#pragma once

#include "expr.h"

namespace eagle {

class Stmt;
using StmtPtr = std::shared_ptr<Stmt>;

class Stmt : public Object {
public:
    class Class;
    class Function;
    class Var;
    class If;
    class While;
    class For;
    class Expression;
    class Print;
    class Return;
    class Break;
    class Continue;
    class Block;
    class Empty;

    class Visitor {
    public:
        virtual ObjectPtr visitClassStmt(std::shared_ptr<Class> stmt) = 0;
        virtual ObjectPtr visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
        virtual ObjectPtr visitVarStmt(std::shared_ptr<Var> stmt) = 0;
        virtual ObjectPtr visitIfStmt(std::shared_ptr<If> stmt) = 0;
        virtual ObjectPtr visitWhileStmt(std::shared_ptr<While> stmt) = 0;
        virtual ObjectPtr visitForStmt(std::shared_ptr<For> stmt) = 0;
        virtual ObjectPtr visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
        virtual ObjectPtr visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
        virtual ObjectPtr visitReturnStmt(std::shared_ptr<Return> stmt) = 0;
        virtual ObjectPtr visitBreakStmt(std::shared_ptr<Break> stmt) = 0;
        virtual ObjectPtr visitContinueStmt(std::shared_ptr<Continue> stmt) = 0;
        virtual ObjectPtr visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
        virtual ObjectPtr visitEmptyStmt(std::shared_ptr<Empty> stmt) = 0;
    };

public:
    virtual ObjectPtr accept(Visitor &visitor) = 0;
};

class Stmt::Class : public Stmt, public std::enable_shared_from_this<Class> {
public:
    Class(TokenPtr name, std::shared_ptr<Expr::Variable> super_class,
          std::vector<std::shared_ptr<Var>> members,
          std::vector<std::shared_ptr<Function>> methods);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr name;
    std::shared_ptr<Expr::Variable> super_class;
    std::vector<std::shared_ptr<Var>> members;
    std::vector<std::shared_ptr<Function>> methods;
};

class Stmt::Function : public Stmt, public std::enable_shared_from_this<Function> {
public:
    Function(TokenPtr name, std::vector<TokenPtr> params, std::vector<StmtPtr> body);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr name;
    std::vector<TokenPtr> params;
    std::vector<StmtPtr> body;
};

class Stmt::Var : public Stmt, public std::enable_shared_from_this<Var> {
public:
    Var(TokenPtr name, ExprPtr initializer);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr name;
    ExprPtr initializer;
};

class Stmt::If : public Stmt, public std::enable_shared_from_this<If> {
public:
    If(ExprPtr condition, StmtPtr then_branch, StmtPtr else_branch);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr condition;
    StmtPtr then_branch;
    StmtPtr else_branch;
};

class Stmt::While : public Stmt, public std::enable_shared_from_this<While> {
public:
    While(ExprPtr condition, StmtPtr body);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr condition;
    StmtPtr body;
};

class Stmt::For : public Stmt, public std::enable_shared_from_this<For> {
public:
    For(StmtPtr initializer, ExprPtr condition, StmtPtr increment, StmtPtr body);

    ObjectPtr accept(Visitor &visitor) override;

public:
    StmtPtr initializer;
    ExprPtr condition;
    StmtPtr increment;
    StmtPtr body;
};

class Stmt::Expression : public Stmt, public std::enable_shared_from_this<Expression> {
public:
    explicit Expression(ExprPtr expression);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr expression;
};

class Stmt::Print : public Stmt, public std::enable_shared_from_this<Print> {
public:
    explicit Print(ExprPtr print_value);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr print_value;
};

class Stmt::Return : public Stmt, public std::enable_shared_from_this<Return> {
public:
    explicit Return(ExprPtr return_value, int line);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr return_value;
    int line;
};

class Stmt::Break : public Stmt, public std::enable_shared_from_this<Break> {
public:
    explicit Break(TokenPtr keyword);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr keyword;
};

class Stmt::Continue : public Stmt, public std::enable_shared_from_this<Continue> {
public:
    explicit Continue(TokenPtr keyword);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr keyword;
};

class Stmt::Block : public Stmt, public std::enable_shared_from_this<Block> {
public:
    explicit Block(std::vector<StmtPtr> statements);

    ObjectPtr accept(Visitor &visitor) override;

public:
    std::vector<StmtPtr> statements;
};

class Stmt::Empty : public Stmt, public std::enable_shared_from_this<Empty> {
public:
    Empty() = default;

    ObjectPtr accept(eagle::Stmt::Visitor &visitor) override;
};

}  // namespace eagle
