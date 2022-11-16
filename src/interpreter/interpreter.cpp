//
// Created by Akman on 2022/11/6.
//

#include "interpreter.h"

namespace eagle {
Interpreter::Interpreter() {}
// expressions
ObjectPtr Interpreter::visitAssignExpr(std::shared_ptr<Expr::Assign> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitTernaryExpr(std::shared_ptr<Expr::Ternary> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitCompareExpr(std::shared_ptr<Expr::Compare> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitCallExpr(std::shared_ptr<Expr::Call> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitVariableExpr(std::shared_ptr<Expr::Variable> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitStreamExpr(std::shared_ptr<Expr::Stream> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitThisExpr(std::shared_ptr<Expr::This> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitSuperExpr(std::shared_ptr<Expr::Super> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitSequenceExpr(std::shared_ptr<Expr::Sequence> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) {
    return nullptr;
}

// statements
ObjectPtr Interpreter::visitClassStmt(std::shared_ptr<Stmt::Class> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitFunctionStmt(std::shared_ptr<Stmt::Function> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitVarStmt(std::shared_ptr<Stmt::Var> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitIfStmt(std::shared_ptr<Stmt::If> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitWhileStmt(std::shared_ptr<Stmt::While> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitExpressionStmt(std::shared_ptr<Stmt::Expression> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitPrintStmt(std::shared_ptr<Stmt::Print> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitReturnStmt(std::shared_ptr<Stmt::Return> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitBreakStmt(std::shared_ptr<Stmt::Break> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitContinueStmt(std::shared_ptr<Stmt::Continue> expr) {
    return nullptr;
}
ObjectPtr Interpreter::visitBlockStmt(std::shared_ptr<Stmt::Block> expr) {
    return nullptr;
}
}  // namespace eagle