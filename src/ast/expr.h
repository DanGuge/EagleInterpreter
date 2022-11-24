//
// Created by Akman on 2022/11/11.
//

#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "object.h"
#include "parser/token.h"

namespace eagle {

class Expr;
using ExprPtr = std::shared_ptr<Expr>;

class Expr : public Object {
public:
    class Assign;
    class Ternary;
    class Compare;
    class Binary;
    class Unary;
    class Call;
    class Literal;
    class Variable;
    class Stream;
    class Switch;
    class InstanceSet;
    class InstanceGet;
    class ContainerSet;
    class ContainerGet;
    class This;
    class Super;
    class Sequence;
    class Associative;

    class Visitor {
    public:
        virtual ObjectPtr visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
        virtual ObjectPtr visitTernaryExpr(std::shared_ptr<Ternary> expr) = 0;
        virtual ObjectPtr visitCompareExpr(std::shared_ptr<Compare> expr) = 0;
        virtual ObjectPtr visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
        virtual ObjectPtr visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
        virtual ObjectPtr visitCallExpr(std::shared_ptr<Call> expr) = 0;
        virtual ObjectPtr visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
        virtual ObjectPtr visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
        virtual ObjectPtr visitStreamExpr(std::shared_ptr<Stream> expr) = 0;
        virtual ObjectPtr visitSwitchExpr(std::shared_ptr<Switch> expr) = 0;
        virtual ObjectPtr visitInstanceSetExpr(std::shared_ptr<InstanceSet> expr) = 0;
        virtual ObjectPtr visitInstanceGetExpr(std::shared_ptr<InstanceGet> expr) = 0;
        virtual ObjectPtr visitContainerSetExpr(std::shared_ptr<ContainerSet> expr) = 0;
        virtual ObjectPtr visitContainerGetExpr(std::shared_ptr<ContainerGet> expr) = 0;
        virtual ObjectPtr visitThisExpr(std::shared_ptr<This> expr) = 0;
        virtual ObjectPtr visitSuperExpr(std::shared_ptr<Super> expr) = 0;
        virtual ObjectPtr visitSequenceExpr(std::shared_ptr<Sequence> expr) = 0;
        virtual ObjectPtr visitAssociativeExpr(std::shared_ptr<Associative> expr) = 0;
    };

public:
    virtual ObjectPtr accept(Visitor &visitor) = 0;
};

class Expr::Assign : public Expr, public std::enable_shared_from_this<Assign> {
public:
    Assign(TokenPtr name, TokenPtr op, ExprPtr value);

    ObjectPtr accept(Visitor &visitor) override;

public:
    const TokenPtr name;
    const TokenPtr op;
    const ExprPtr value;
};

class Expr::Ternary : public Expr, public std::enable_shared_from_this<Ternary> {
public:
    Ternary(ExprPtr condition, ExprPtr then_expr, ExprPtr else_expr);

    ObjectPtr accept(Visitor &visitor) override;

public:
    const ExprPtr condition;
    const ExprPtr then_expr;
    const ExprPtr else_expr;
};

class Expr::Compare : public Expr, public std::enable_shared_from_this<Compare> {
public:
    Compare(ExprPtr first, std::vector<std::pair<TokenPtr, ExprPtr>> others);

    ObjectPtr accept(Visitor &visitor) override;

public:
    const ExprPtr first;
    const std::vector<std::pair<TokenPtr, ExprPtr>> others;
};

class Expr::Binary : public Expr, public std::enable_shared_from_this<Binary> {
public:
    Binary(ExprPtr left, TokenPtr op, ExprPtr right);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr left;
    TokenPtr op;
    ExprPtr right;
};

class Expr::Unary : public Expr, public std::enable_shared_from_this<Unary> {
public:
    Unary(TokenPtr op, ExprPtr expr);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr op;
    ExprPtr expr;
};

class Expr::Call : public Expr, public std::enable_shared_from_this<Call> {
public:
    Call(ExprPtr callee, std::vector<ExprPtr> arguments, int line);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr callee;
    std::vector<ExprPtr> arguments;
    int line;
};

class Expr::Literal : public Expr, public std::enable_shared_from_this<Literal> {
public:
    explicit Literal(ObjectPtr value);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ObjectPtr value;
};

class Expr::Variable : public Expr, public std::enable_shared_from_this<Variable> {
public:
    explicit Variable(TokenPtr name);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr name;
};

class Expr::Stream : public Expr, public std::enable_shared_from_this<Stream> {
public:
    Stream(ExprPtr expr, std::vector<std::pair<TokenPtr, ExprPtr>> operations);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr expr;
    std::vector<std::pair<TokenPtr, ExprPtr>> operations;
};

class Expr::Switch : public Expr, public std::enable_shared_from_this<Switch> {
public:
    Switch(ExprPtr expr, std::vector<std::pair<ExprPtr, ExprPtr>> case_results,
           ExprPtr default_results);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr expr;
    std::vector<std::pair<ExprPtr, ExprPtr>> case_results;
    ExprPtr default_results;
};

class Expr::InstanceSet : public Expr, public std::enable_shared_from_this<InstanceSet> {
public:
    InstanceSet(ExprPtr object, TokenPtr name, TokenPtr op, ExprPtr value);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr object;
    TokenPtr name;
    TokenPtr op;
    ExprPtr value;
};

class Expr::InstanceGet : public Expr, public std::enable_shared_from_this<InstanceGet> {
public:
    InstanceGet(ExprPtr object, TokenPtr name);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr object;
    TokenPtr name;
};

class Expr::ContainerSet : public Expr, public std::enable_shared_from_this<ContainerSet> {
public:
    ContainerSet(ExprPtr container, ExprPtr subscript, TokenPtr op, ExprPtr value);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr container;
    ExprPtr subscript;
    TokenPtr op;
    ExprPtr value;
};

class Expr::ContainerGet : public Expr, public std::enable_shared_from_this<ContainerGet> {
public:
    ContainerGet(ExprPtr container, ExprPtr subscript, int line);

    ObjectPtr accept(Visitor &visitor) override;

public:
    ExprPtr container;
    ExprPtr subscript;
    int line;
};

class Expr::This : public Expr, public std::enable_shared_from_this<This> {
public:
    explicit This(TokenPtr keyword);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr keyword;
};

class Expr::Super : public Expr, public std::enable_shared_from_this<Super> {
public:
    Super(TokenPtr keyword, TokenPtr method);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr keyword;
    TokenPtr method;
};

class Expr::Sequence : public Expr, public std::enable_shared_from_this<Sequence> {
public:
    Sequence(TokenPtr type, std::vector<ExprPtr> elements);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr type;
    std::vector<ExprPtr> elements;
};

class Expr::Associative : public Expr, public std::enable_shared_from_this<Associative> {
public:
    Associative(TokenPtr type, std::vector<std::pair<ExprPtr, ExprPtr>> elements);

    ObjectPtr accept(Visitor &visitor) override;

public:
    TokenPtr type;
    std::vector<std::pair<ExprPtr, ExprPtr>> elements;
};

}  // namespace eagle
