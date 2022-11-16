//
// Created by DanGuge on 2022/11/16.
//

#include "resolver.h"

namespace eagle {

Resolver::Resolver(std::shared_ptr<Interpreter>& interpreter) : interpreter(interpreter) {}

// expressions
ObjectPtr Resolver::visitAssignExpr(std::shared_ptr<Expr::Assign> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitTernaryExpr(std::shared_ptr<Expr::Ternary> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitCompareExpr(std::shared_ptr<Expr::Compare> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitCallExpr(std::shared_ptr<Expr::Call> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitVariableExpr(std::shared_ptr<Expr::Variable> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitStreamExpr(std::shared_ptr<Expr::Stream> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitThisExpr(std::shared_ptr<Expr::This> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitSuperExpr(std::shared_ptr<Expr::Super> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitSequenceExpr(std::shared_ptr<Expr::Sequence> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) {
    return nullptr;
}

// statements
ObjectPtr Resolver::visitClassStmt(std::shared_ptr<Stmt::Class> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitFunctionStmt(std::shared_ptr<Stmt::Function> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitVarStmt(std::shared_ptr<Stmt::Var> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitIfStmt(std::shared_ptr<Stmt::If> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitWhileStmt(std::shared_ptr<Stmt::While> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitExpressionStmt(std::shared_ptr<Stmt::Expression> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitPrintStmt(std::shared_ptr<Stmt::Print> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitReturnStmt(std::shared_ptr<Stmt::Return> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitBreakStmt(std::shared_ptr<Stmt::Break> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitContinueStmt(std::shared_ptr<Stmt::Continue> expr) {
    return nullptr;
}
ObjectPtr Resolver::visitBlockStmt(std::shared_ptr<Stmt::Block> expr) {
    return nullptr;
}
}  // namespace eagle