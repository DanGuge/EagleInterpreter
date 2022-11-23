//
// Created by Akman on 2022/11/11.
//

#include "stmt.h"

namespace eagle {

Stmt::Class::Class(TokenPtr name, std::shared_ptr<Expr::Variable> super_class,
                   std::vector<std::shared_ptr<Var>> members,
                   std::vector<std::shared_ptr<Function>> methods)
    : name(std::move(name))
    , super_class(std::move(super_class))
    , members(std::move(members))
    , methods(std::move(methods)) {}

ObjectPtr Stmt::Class::accept(Visitor &visitor) {
    return visitor.visitClassStmt(shared_from_this());
}

Stmt::Function::Function(TokenPtr name, std::vector<TokenPtr> params, std::vector<StmtPtr> body)
    : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

ObjectPtr Stmt::Function::accept(Visitor &visitor) {
    return visitor.visitFunctionStmt(shared_from_this());
}

Stmt::Var::Var(TokenPtr name, ExprPtr initializer)
    : name(std::move(name)), initializer(std::move(initializer)) {}

ObjectPtr Stmt::Var::accept(Visitor &visitor) {
    return visitor.visitVarStmt(shared_from_this());
}

Stmt::If::If(ExprPtr condition, StmtPtr then_branch, StmtPtr else_branch)
    : condition(std::move(condition))
    , then_branch(std::move(then_branch))
    , else_branch(std::move(else_branch)) {}

ObjectPtr Stmt::If::accept(Visitor &visitor) {
    return visitor.visitIfStmt(shared_from_this());
}

Stmt::While::While(ExprPtr condition, StmtPtr body)
    : condition(std::move(condition)), body(std::move(body)) {}

ObjectPtr Stmt::While::accept(Visitor &visitor) {
    return visitor.visitWhileStmt(shared_from_this());
}

Stmt::For::For(StmtPtr initializer, ExprPtr condition, StmtPtr increment, StmtPtr body)
    : initializer(std::move(initializer))
    , condition(std::move(condition))
    , increment(std::move(increment))
    , body(std::move(body)) {}

ObjectPtr Stmt::For::accept(Visitor &visitor) {
    return visitor.visitForStmt(shared_from_this());
}

Stmt::Expression::Expression(ExprPtr expression) : expression(std::move(expression)) {}

ObjectPtr Stmt::Expression::accept(Visitor &visitor) {
    return visitor.visitExpressionStmt(shared_from_this());
}

Stmt::Print::Print(ExprPtr print_value) : print_value(std::move(print_value)) {}

ObjectPtr Stmt::Print::accept(Visitor &visitor) {
    return visitor.visitPrintStmt(shared_from_this());
}

Stmt::Return::Return(ExprPtr return_value, int line)
    : return_value(std::move(return_value)), line(line) {}

ObjectPtr Stmt::Return::accept(Visitor &visitor) {
    return visitor.visitReturnStmt(shared_from_this());
}

Stmt::Break::Break(TokenPtr keyword) : keyword(std::move(keyword)) {}

ObjectPtr Stmt::Break::accept(Visitor &visitor) {
    return visitor.visitBreakStmt(shared_from_this());
}

Stmt::Continue::Continue(TokenPtr keyword) : keyword(std::move(keyword)) {}

ObjectPtr Stmt::Continue::accept(Visitor &visitor) {
    return visitor.visitContinueStmt(shared_from_this());
}

Stmt::Block::Block(std::vector<StmtPtr> statements) : statements(std::move(statements)) {}

ObjectPtr Stmt::Block::accept(Visitor &visitor) {
    return visitor.visitBlockStmt(shared_from_this());
}

}  // namespace eagle
