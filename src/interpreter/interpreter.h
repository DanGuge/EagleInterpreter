//
// Created by Akman on 2022/11/6.
//

#pragma once

#include "ast/expr.h"
#include "ast/stmt.h"
#include "environment.h"
#include "modules/eagle_exceptions.h"

namespace eagle {

class EagleDict;
using EagleDictPtr = std::shared_ptr<EagleDict>;

class Interpreter : public Expr::Visitor, public Stmt::Visitor {
public:
    static Interpreter& getInstance();

private:
    static Interpreter interpreter;

private:
    explicit Interpreter();

public:
    void interpret(const std::vector<StmtPtr>& statements);
    void init_built_in();
    EagleDictPtr getGlobals();
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
    ObjectPtr visitLambdaExpr(std::shared_ptr<Expr::Lambda> expr) override;
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
    ObjectPtr visitForStmt(std::shared_ptr<Stmt::For> stmt) override;
    ObjectPtr visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) override;
    ObjectPtr visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) override;
    ObjectPtr visitReturnStmt(std::shared_ptr<Stmt::Return> stmt) override;
    ObjectPtr visitBreakStmt(std::shared_ptr<Stmt::Break> stmt) override;
    ObjectPtr visitContinueStmt(std::shared_ptr<Stmt::Continue> stmt) override;
    ObjectPtr visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) override;
    ObjectPtr visitEmptyStmt(std::shared_ptr<Stmt::Empty> stmt) override;
    // help functions
    void resolveLocal(const ExprPtr& expr, int distance);
    void executeBlock(const std::vector<StmtPtr>& statements, const EnvironmentPtr& block_env);
    void execute(const StmtPtr& statement, const EnvironmentPtr& stmt_env);

private:
    // help functions
    ObjectPtr evaluate(const ExprPtr& expr);
    void execute(const StmtPtr& stmt);
    ObjectPtr assignVariable(const TokenPtr& name, const ExprPtr& expr, const ObjectPtr& value);
    ObjectPtr getVariable(const TokenPtr& name, const ExprPtr& expr);
    static bool checkNumber(const TokenPtr& op, const ObjectPtr& object);
    static bool checkTwoNumbers(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right,
                                bool need_throw);
    static bool checkTwoStrings(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static ObjectPtr plus(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static ObjectPtr minus(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static ObjectPtr multi(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static ObjectPtr div(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static ObjectPtr mod(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static bool less(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static bool lessEqual(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static bool greater(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static bool greaterEqual(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right);
    static bool isTruthy(const ObjectPtr& object);
    static bool isEqual(const ObjectPtr& left, const ObjectPtr& right);
    static std::string stringify(const ObjectPtr& object);
    static EagleRuntimeError interpreterRuntimeError(int line, const std::string& message);

private:
    EnvironmentPtr global_env;
    EnvironmentPtr current_env;
    std::unordered_map<ExprPtr, int> local_variables;
    int stack_depth;
};

}  // namespace eagle
