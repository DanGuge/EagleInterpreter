//
// Created by DanGuge on 2022/11/16.
//

#include "resolver.h"

namespace eagle {

Resolver::Resolver(std::shared_ptr<Interpreter>& interpreter) : interpreter(interpreter) {
    current_class_type = ClassType::NONE;
    current_function_type = FunctionType::NONE;
    current_loop_type = LoopType::NONE;
}

// entrance function
void Resolver::resolve(const std::vector<StmtPtr>& stmts) {
    for (const auto& stmt : stmts) {
        resolve(stmt);
    }
}

// expressions
ObjectPtr Resolver::visitAssignExpr(std::shared_ptr<Expr::Assign> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return nullptr;
}

ObjectPtr Resolver::visitTernaryExpr(std::shared_ptr<Expr::Ternary> expr) {
    resolve(expr->condition);
    resolve(expr->then_expr);
    resolve(expr->else_expr);
    return nullptr;
}

ObjectPtr Resolver::visitCompareExpr(std::shared_ptr<Expr::Compare> expr) {
    resolve(expr->first);
    for (const auto& op_expr : expr->others) {
        resolve(op_expr.second);
    }
    return nullptr;
}

ObjectPtr Resolver::visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return nullptr;
}

ObjectPtr Resolver::visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) {
    resolve(expr->expr);
    return nullptr;
}

ObjectPtr Resolver::visitCallExpr(std::shared_ptr<Expr::Call> expr) {
    resolve(expr->callee);
    for (const auto& argument : expr->arguments) {
        resolve(argument);
    }
    return nullptr;
}

ObjectPtr Resolver::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) {
    return nullptr;
}

ObjectPtr Resolver::visitVariableExpr(std::shared_ptr<Expr::Variable> expr) {
    if (!scopes.empty() && !scopes.back()[expr->name->text]) {
        Eagle::error(expr->name,
                     "local variable \'" + expr->name->text + "\' referenced before assignment");
    } else {
        resolveLocal(expr, expr->name);
    }
    return nullptr;
}
ObjectPtr Resolver::visitStreamExpr(std::shared_ptr<Expr::Stream> expr) {
    for (const auto& operation : expr->operations) {
        if (operation.second != nullptr)
            resolve(operation.second);
    }
    return nullptr;
}
ObjectPtr Resolver::visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) {
    resolve(expr->expr);
    for (const auto& case_result : expr->case_results) {
        resolve(case_result.first);
        resolve(case_result.second);
    }
    resolve(expr->default_results);
    return nullptr;
}
ObjectPtr Resolver::visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) {
    resolve(expr->object);
    resolve(expr->value);
    return nullptr;
}
ObjectPtr Resolver::visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) {
    resolve(expr->object);
    return nullptr;
}
ObjectPtr Resolver::visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) {
    resolve(expr->container);
    resolve(expr->subscript);
    resolve(expr->value);
    return nullptr;
}
ObjectPtr Resolver::visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) {
    resolve(expr->container);
    resolve(expr->subscript);
    return nullptr;
}
ObjectPtr Resolver::visitThisExpr(std::shared_ptr<Expr::This> expr) {
    if (current_class_type == ClassType::NONE) {
        Eagle::error(expr->keyword, "Can't use 'this' keyword outside class");
    } else {
        resolveLocal(expr, expr->keyword);
    }
    return nullptr;
}
ObjectPtr Resolver::visitSuperExpr(std::shared_ptr<Expr::Super> expr) {
    if (current_class_type == ClassType::NONE) {
        Eagle::error(expr->keyword, "Can't use 'super' keyword outside class");
    } else if (current_class_type == ClassType::CLASS) {
        Eagle::error(expr->keyword, "Can't use 'super' keyword in a class without superclass");
    } else {
        resolveLocal(expr, expr->keyword);
    }
    return nullptr;
}
ObjectPtr Resolver::visitSequenceExpr(std::shared_ptr<Expr::Sequence> expr) {
    for (const auto& element : expr->elements) {
        resolve(element);
    }
    return nullptr;
}
ObjectPtr Resolver::visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) {
    for (const auto& key_value : expr->elements) {
        resolve(key_value.first);
        resolve(key_value.second);
    }
    return nullptr;
}

// statements
ObjectPtr Resolver::visitClassStmt(std::shared_ptr<Stmt::Class> stmt) {
    ClassType enclosing_class_type = current_class_type;
    current_class_type = ClassType::CLASS;

    declareIdentifier(stmt->name);
    defineIdentifier(stmt->name);

    std::shared_ptr<Expr::Variable> super_class = stmt->super_class;

    if (super_class != nullptr && (stmt->name->text == super_class->name->text)) {
        Eagle::error(stmt->super_class->name, "Class can't inherit from itself");
        return nullptr;
    }

    if (super_class != nullptr) {
        current_class_type = ClassType::SUBCLASS;
        resolve(super_class);
    }

    if (super_class != nullptr) {
        beginScope();
        scopes.back()["super"] = true;
    }

    beginScope();
    scopes.back()["this"] = true;

    for (const auto& member : stmt->members) {
        resolve(member);
    }

    for (const auto& method : stmt->methods) {
        FunctionType method_type = FunctionType::METHOD;

        if (method->name->text == "init") {
            method_type = FunctionType::INITIALIZER;
        }
        resolveFunction(method, method_type);
    }

    endScope();

    if (super_class != nullptr)
        endScope();

    current_class_type = enclosing_class_type;
    return nullptr;
}
ObjectPtr Resolver::visitFunctionStmt(std::shared_ptr<Stmt::Function> stmt) {
    declareIdentifier(stmt->name);
    defineIdentifier(stmt->name);
    resolveFunction(stmt, FunctionType::FUNCTION);
    return nullptr;
}
ObjectPtr Resolver::visitVarStmt(std::shared_ptr<Stmt::Var> stmt) {
    declareIdentifier(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer);
    }
    defineIdentifier(stmt->name);
    return nullptr;
}
ObjectPtr Resolver::visitIfStmt(std::shared_ptr<Stmt::If> stmt) {
    resolve(stmt->condition);
    resolve(stmt->then_branch);
    if (stmt->else_branch != nullptr)
        resolve(stmt->else_branch);
    return nullptr;
}
ObjectPtr Resolver::visitWhileStmt(std::shared_ptr<Stmt::While> stmt) {
    resolve(stmt->condition);
    LoopType enclosing_loop_type = current_loop_type;
    current_loop_type = LoopType::LOOP;
    resolve(stmt->body);
    current_loop_type = enclosing_loop_type;
    return nullptr;
}
ObjectPtr Resolver::visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) {
    resolve(stmt->expression);
    return nullptr;
}
ObjectPtr Resolver::visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) {
    resolve(stmt->print_value);
    return nullptr;
}
ObjectPtr Resolver::visitReturnStmt(std::shared_ptr<Stmt::Return> stmt) {
    if (current_function_type == FunctionType::NONE) {
        Eagle::error(stmt->line, "Can't use 'return' keyword outside function(method)");
    }
    if (stmt->return_value != nullptr) {
        if (current_function_type == FunctionType::INITIALIZER) {
            Eagle::error(stmt->line, "Can't return value from an initializer method");
        }
        resolve(stmt->return_value);
    }
    return nullptr;
}
ObjectPtr Resolver::visitBreakStmt(std::shared_ptr<Stmt::Break> stmt) {
    if (current_loop_type != LoopType::LOOP)
        Eagle::error(stmt->keyword, "Can't use 'break' keyword outside loop");
    return nullptr;
}
ObjectPtr Resolver::visitContinueStmt(std::shared_ptr<Stmt::Continue> stmt) {
    if (current_loop_type != LoopType::LOOP)
        Eagle::error(stmt->keyword, "Can't use 'continue' keyword outside loop");
    return nullptr;
}
ObjectPtr Resolver::visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return nullptr;
}

// help functions
void Resolver::resolve(const StmtPtr& stmt) {
    stmt->accept(*this);
}

void Resolver::resolve(const ExprPtr& expr) {
    expr->accept(*this);
}

void Resolver::beginScope() {
    scopes.emplace_back(Scope{});
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declareIdentifier(const TokenPtr& token) {
    if (scopes.empty())
        return;

    Scope top_scope = scopes.back();
    if (top_scope.find(token->text) != top_scope.end()) {
        Eagle::error(token, "Redeclaration of variable \'" + token->text + "\'");
    }

    top_scope[token->text] = false;
}

void Resolver::defineIdentifier(const TokenPtr& token) {
    if (scopes.empty())
        return;
    scopes.back()[token->text] = true;
}

void Resolver::resolveLocal(const ExprPtr& expr, const TokenPtr& name) {
    for (int i = (int)scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name->text) != scopes[i].end()) {
            interpreter->resolveLocal(expr, (int)scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::resolveFunction(const std::shared_ptr<Stmt::Function>& function, FunctionType type) {
    FunctionType enclosing_function_type = current_function_type;
    current_function_type = type;
    beginScope();
    for (const auto& param : function->params) {
        declareIdentifier(param);
        defineIdentifier(param);
    }
    resolve(function->body);
    endScope();
    current_function_type = enclosing_function_type;
}
}  // namespace eagle