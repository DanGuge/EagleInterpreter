//
// Created by DanGuge on 2022/11/16.
//

#pragma once

#include "ast/expr.h"
#include "ast/stmt.h"
#include "eagle.h"
#include "interpreter/interpreter.h"

namespace eagle {

using Scope = std::unordered_map<std::string, bool>;

class Resolver : Expr::Visitor, Stmt::Visitor {
    enum class ClassType;
    enum class FunctionType;
    enum class LoopType;

public:
    explicit Resolver(std::shared_ptr<Interpreter>& interpreter);
    // entrance function
    void resolve(const std::vector<StmtPtr>& stmts);
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
    ObjectPtr visitClassStmt(std::shared_ptr<Stmt::Class> stmt) override;
    ObjectPtr visitFunctionStmt(std::shared_ptr<Stmt::Function> stmt) override;
    ObjectPtr visitVarStmt(std::shared_ptr<Stmt::Var> stmt) override;
    ObjectPtr visitIfStmt(std::shared_ptr<Stmt::If> stmt) override;
    ObjectPtr visitWhileStmt(std::shared_ptr<Stmt::While> stmt) override;
    ObjectPtr visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) override;
    ObjectPtr visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) override;
    ObjectPtr visitReturnStmt(std::shared_ptr<Stmt::Return> stmt) override;
    ObjectPtr visitBreakStmt(std::shared_ptr<Stmt::Break> stmt) override;
    ObjectPtr visitContinueStmt(std::shared_ptr<Stmt::Continue> stmt) override;
    ObjectPtr visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) override;

private:
    // help functions
    void resolve(const StmtPtr& stmt);
    void resolve(const ExprPtr& expr);
    void beginScope();
    void endScope();
    void declareIdentifier(const TokenPtr& token);
    void defineIdentifier(const TokenPtr& token);
    void resolveLocal(const ExprPtr& expr, const TokenPtr& name);
    void resolveFunction(const std::shared_ptr<Stmt::Function>& function, FunctionType type);

private:
    enum class ClassType { NONE, CLASS, SUBCLASS };
    enum class FunctionType { NONE, FUNCTION, METHOD, INITIALIZER };
    enum class LoopType { NONE, LOOP };
    std::shared_ptr<Interpreter> interpreter;
    std::vector<Scope> scopes;
    ClassType current_class_type;
    FunctionType current_function_type;
    LoopType current_loop_type;
};

}  // namespace eagle