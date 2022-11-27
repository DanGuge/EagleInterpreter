//
// Created by Akman on 2022/11/11.
//

#include "expr.h"

#include "stmt.h"

namespace eagle {

Expr::Assign::Assign(TokenPtr name, TokenPtr op, ExprPtr value)
    : name(std::move(name)), op(std::move(op)), value(std::move(value)) {}

ObjectPtr Expr::Assign::accept(Visitor &visitor) {
    return visitor.visitAssignExpr(shared_from_this());
}

Expr::Ternary::Ternary(ExprPtr condition, ExprPtr then_expr, ExprPtr else_expr)
    : condition(std::move(condition))
    , then_expr(std::move(then_expr))
    , else_expr(std::move(else_expr)) {}

ObjectPtr Expr::Ternary::accept(Visitor &visitor) {
    return visitor.visitTernaryExpr(shared_from_this());
}

Expr::Compare::Compare(ExprPtr first, std::vector<std::pair<TokenPtr, ExprPtr>> others)
    : first(std::move(first)), others(std::move(others)) {}

ObjectPtr Expr::Compare::accept(Visitor &visitor) {
    return visitor.visitCompareExpr(shared_from_this());
}

Expr::Binary::Binary(ExprPtr left, TokenPtr op, ExprPtr right)
    : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

ObjectPtr Expr::Binary::accept(Visitor &visitor) {
    return visitor.visitBinaryExpr(shared_from_this());
}

Expr::Unary::Unary(TokenPtr op, ExprPtr expr) : op(std::move(op)), expr(std::move(expr)) {}

ObjectPtr Expr::Unary::accept(Visitor &visitor) {
    return visitor.visitUnaryExpr(shared_from_this());
}

Expr::Call::Call(ExprPtr callee, std::vector<ExprPtr> arguments, int line)
    : callee(std::move(callee)), arguments(std::move(arguments)), line(line) {}

ObjectPtr Expr::Call::accept(Visitor &visitor) {
    return visitor.visitCallExpr(shared_from_this());
}

Expr::Literal::Literal(ObjectPtr value) : value(std::move(value)) {}

ObjectPtr Expr::Literal::accept(Visitor &visitor) {
    return visitor.visitLiteralExpr(shared_from_this());
}

Expr::Variable::Variable(TokenPtr name) : name(std::move(name)) {}

ObjectPtr Expr::Variable::accept(Visitor &visitor) {
    return visitor.visitVariableExpr(shared_from_this());
}

Expr::Stream::Stream(ExprPtr expr, std::vector<std::pair<TokenPtr, ExprPtr>> operations, int line)
    : expr(std::move(expr)), operations(std::move(operations)), line(line) {}

ObjectPtr Expr::Stream::accept(Visitor &visitor) {
    return visitor.visitStreamExpr(shared_from_this());
}

Expr::Switch::Switch(ExprPtr expr, std::vector<std::pair<ExprPtr, ExprPtr>> case_results,
                     ExprPtr default_results)
    : expr(std::move(expr))
    , case_results(std::move(case_results))
    , default_results(std::move(default_results)) {}

ObjectPtr Expr::Switch::accept(Visitor &visitor) {
    return visitor.visitSwitchExpr(shared_from_this());
}

Expr::Lambda::Lambda(std::vector<TokenPtr> params, StmtPtr body)
    : params(std::move(params)), body(std::move(body)) {}

ObjectPtr Expr::Lambda::accept(Visitor &visitor) {
    return visitor.visitLambdaExpr(shared_from_this());
}

Expr::InstanceSet::InstanceSet(ExprPtr object, TokenPtr name, TokenPtr op, ExprPtr value)
    : object(std::move(object))
    , name(std::move(name))
    , op(std::move(op))
    , value(std::move(value)) {}

ObjectPtr Expr::InstanceSet::accept(Visitor &visitor) {
    return visitor.visitInstanceSetExpr(shared_from_this());
}

Expr::InstanceGet::InstanceGet(ExprPtr object, TokenPtr name)
    : object(std::move(object)), name(std::move(name)) {}

ObjectPtr Expr::InstanceGet::accept(Visitor &visitor) {
    return visitor.visitInstanceGetExpr(shared_from_this());
}

Expr::ContainerSet::ContainerSet(ExprPtr container, ExprPtr subscript, TokenPtr op, ExprPtr value)
    : container(std::move(container))
    , subscript(std::move(subscript))
    , op(std::move(op))
    , value(std::move(value)) {}

ObjectPtr Expr::ContainerSet::accept(Visitor &visitor) {
    return visitor.visitContainerSetExpr(shared_from_this());
}

Expr::ContainerGet::ContainerGet(ExprPtr container, ExprPtr subscript, int line)
    : container(std::move(container)), subscript(std::move(subscript)), line(line) {}

ObjectPtr Expr::ContainerGet::accept(Visitor &visitor) {
    return visitor.visitContainerGetExpr(shared_from_this());
}

Expr::This::This(TokenPtr keyword) : keyword(std::move(keyword)) {}

ObjectPtr Expr::This::accept(Visitor &visitor) {
    return visitor.visitThisExpr(shared_from_this());
}

Expr::Super::Super(TokenPtr keyword, TokenPtr method)
    : keyword(std::move(keyword)), method(std::move(method)) {}

ObjectPtr Expr::Super::accept(Visitor &visitor) {
    return visitor.visitSuperExpr(shared_from_this());
}

Expr::Sequence::Sequence(TokenPtr type, std::vector<ExprPtr> elements)
    : type(std::move(type)), elements(std::move(elements)) {}

ObjectPtr Expr::Sequence::accept(Visitor &visitor) {
    return visitor.visitSequenceExpr(shared_from_this());
}

Expr::Associative::Associative(TokenPtr type, std::vector<std::pair<ExprPtr, ExprPtr>> elements)
    : type(std::move(type)), elements(std::move(elements)) {}

ObjectPtr Expr::Associative::accept(Visitor &visitor) {
    return visitor.visitAssociativeExpr(shared_from_this());
}

}  // namespace eagle
