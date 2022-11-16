//
// Created by DanGuge on 2022/11/16.
//

#pragma once

#include <stack>

#include "ast/expr.h"
#include "ast/stmt.h"
#include "interpreter.h"

namespace eagle {

class Resolver : Expr::Visitor, Stmt::Visitor {
public:
    explicit Resolver(std::shared_ptr<Interpreter>& interpreter);
    // expressions
    ObjectPtr visitAssignExpr(std::shared_ptr<Expr::Assign> expr) override;
    ObjectPtr visitTernaryExpr(std::shared_ptr<Expr::Ternary> expr) override;
    ObjectPtr visitCompareExpr(std::shared_ptr<Expr::Compare> expr) override;
    ObjectPtr visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) override;
    ObjectPtr visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) override;
    ObjectPtr visitCallExpr(std::shared_ptr<Expr::Call> expr) override;
    ObjectPtr visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) override;
    ObjectPtr visitVariableExpr(std::shared_ptr<Expr::Variable> expr) override;
    ObjectPtr visitStreamExpr(std::shared_ptr<Expr::Stream> expr) override;
    ObjectPtr visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) override;
    ObjectPtr visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) override;
    ObjectPtr visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) override;
    ObjectPtr visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) override;
    ObjectPtr visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) override;
    ObjectPtr visitThisExpr(std::shared_ptr<Expr::This> expr) override;
    ObjectPtr visitSuperExpr(std::shared_ptr<Expr::Super> expr) override;
    ObjectPtr visitSequenceExpr(std::shared_ptr<Expr::Sequence> expr) override;
    ObjectPtr visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) override;
    // statements
    ObjectPtr visitClassStmt(std::shared_ptr<Stmt::Class> expr) override;
    ObjectPtr visitFunctionStmt(std::shared_ptr<Stmt::Function> expr) override;
    ObjectPtr visitVarStmt(std::shared_ptr<Stmt::Var> expr) override;
    ObjectPtr visitIfStmt(std::shared_ptr<Stmt::If> expr) override;
    ObjectPtr visitWhileStmt(std::shared_ptr<Stmt::While> expr) override;
    ObjectPtr visitExpressionStmt(std::shared_ptr<Stmt::Expression> expr) override;
    ObjectPtr visitPrintStmt(std::shared_ptr<Stmt::Print> expr) override;
    ObjectPtr visitReturnStmt(std::shared_ptr<Stmt::Return> expr) override;
    ObjectPtr visitBreakStmt(std::shared_ptr<Stmt::Break> expr) override;
    ObjectPtr visitContinueStmt(std::shared_ptr<Stmt::Continue> expr) override;
    ObjectPtr visitBlockStmt(std::shared_ptr<Stmt::Block> expr) override;

private:
    std::shared_ptr<Interpreter> interpreter;
    std::stack<std::unordered_map<std::string, bool>> scopes;
};

}  // namespace eagle