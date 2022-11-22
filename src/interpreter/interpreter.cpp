//
// Created by Akman on 2022/11/6.
//

#include "interpreter.h"

#include "BigFloat.h"
#include "modules/eagle_class.h"
#include "modules/eagle_string.h"
#include "util/error_reporter.h"

namespace eagle {
Interpreter::Interpreter() {
    global_env = std::make_shared<Environment>(nullptr);
    current_env = global_env;
    local_variables = {};
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(statement);
        }
    } catch (EagleRuntimeError& runtime_error) {
        ErrorReporter::getInstance().error(runtime_error);
    }
}

// expressions
ObjectPtr Interpreter::visitAssignExpr(std::shared_ptr<Expr::Assign> expr) {
    ObjectPtr name_value = getVariable(expr->name, expr);
    ObjectPtr value = evaluate(expr->value);
    switch (expr->op->type) {
        case ASSIGN: return assignVariable(expr->name, expr, value);
        case PLUS_ASSIGN:
            return assignVariable(expr->name, expr, plus(name_value, expr->op, value));
        case MINUS_ASSIGN:
            return assignVariable(expr->name, expr, minus(name_value, expr->op, value));
        case MULTI_ASSIGN:
            return assignVariable(expr->name, expr, multi(name_value, expr->op, value));
        case DIV_ASSIGN: return assignVariable(expr->name, expr, div(name_value, expr->op, value));
        case MOD_ASSIGN: return assignVariable(expr->name, expr, mod(name_value, expr->op, value));
        default:
            throw interpreterRuntimeError(expr->op->line,
                                          "Undefined behavior of operator " + expr->op->text);
    }
}

ObjectPtr Interpreter::visitTernaryExpr(std::shared_ptr<Expr::Ternary> expr) {
    ObjectPtr condition = evaluate(expr->condition);
    if (isTruthy(condition)) {
        return evaluate(expr->then_expr);
    } else {
        return evaluate(expr->else_expr);
    }
}

ObjectPtr Interpreter::visitCompareExpr(std::shared_ptr<Expr::Compare> expr) {
    ObjectPtr left = evaluate(expr->first);
    bool is_true;
    for (const auto& other : expr->others) {
        ObjectPtr right = evaluate(other.second);
        switch (other.first->type) {
            case EQUAL: is_true = isEqual(left, right); break;
            case NOT_EQUAL: is_true = !isEqual(left, right); break;
            case LESS: is_true = less(left, other.first, right); break;
            case LESS_EQUAL: is_true = lessEqual(left, other.first, right); break;
            case GREATER: is_true = greater(left, other.first, right); break;
            case GREATER_EQUAL: is_true = greaterEqual(left, other.first, right); break;
            default:
                throw interpreterRuntimeError(
                    other.first->line, "Undefined behavior of operator " + other.first->text);
        }
        if (!is_true) {
            return std::make_shared<Boolean>(false);
        }
    }
    return std::make_shared<Boolean>(true);
}

ObjectPtr Interpreter::visitBinaryExpr(std::shared_ptr<Expr::Binary> expr) {
    ObjectPtr left = evaluate(expr->left);
    ObjectPtr right = evaluate(expr->right);
    switch (expr->op->type) {
        case OR:
            if (isTruthy(left))
                return left;
            else
                return right;
        case AND:
            if (!isTruthy(left))
                return left;
            else
                return right;
        case PLUS: return plus(left, expr->op, right);
        case MINUS: return minus(left, expr->op, right);
        case MULTI: return multi(left, expr->op, right);
        case DIV: return div(left, expr->op, right);
        case MOD: return mod(left, expr->op, right);
        default:
            throw interpreterRuntimeError(expr->op->line,
                                          "Undefined behavior of operator " + expr->op->text);
    }
}

ObjectPtr Interpreter::visitUnaryExpr(std::shared_ptr<Expr::Unary> expr) {
    ObjectPtr value = evaluate(expr->expr);
    switch (expr->op->type) {
        case EXCLAMATION_MARK:
        case NOT: return std::make_shared<Boolean>(!isTruthy(value));
        case MINUS:
            checkNumber(expr->op, value);
            return std::make_shared<BigFloat>(0 - (*cast<BigFloat>(value)));
        default:
            throw EagleRuntimeError("[RuntimeError at line " + std::to_string(expr->op->line) +
                                    "] " + "Undefined behavior of operator " + expr->op->text);
    }
}

ObjectPtr Interpreter::visitCallExpr(std::shared_ptr<Expr::Call> expr) {
    ObjectPtr callee = evaluate(expr->callee);

    std::vector<ObjectPtr> arguments{};
    for (const auto& argument : expr->arguments) {
        arguments.emplace_back(evaluate(argument));
    }

    if (!InstanceOf<EagleCallable>(callee)) {
        throw interpreterRuntimeError(expr->line, "Only functions and classes can be called");
    }

    EagleCallablePtr function = cast<EagleCallable>(callee);
    if (arguments.size() != function->arity()) {
        throw interpreterRuntimeError(
            expr->line, "Expect arguments size is" + std::to_string(function->arity()) + " But " +
                            std::to_string(arguments.size()) + " argument(s) is(are) given");
    }

    // TODO Built in Functions
    return function->call(*this, arguments);
}

ObjectPtr Interpreter::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) {
    return expr->value;
}

ObjectPtr Interpreter::visitVariableExpr(std::shared_ptr<Expr::Variable> expr) {
    return getVariable(expr->name, expr);
}

ObjectPtr Interpreter::visitStreamExpr(std::shared_ptr<Expr::Stream> expr) {
    return nullptr;
}

ObjectPtr Interpreter::visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) {
    return nullptr;
}

ObjectPtr Interpreter::visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) {
    ObjectPtr object = evaluate(expr->object);
    if (!InstanceOf<EagleInstance>(object)) {
        throw interpreterRuntimeError(expr->name->line,
                                      "Only instances have fields to set property");
    }

    ObjectPtr name_value = cast<EagleInstance>(object)->get(expr->name);
    ObjectPtr value = evaluate(expr->value);
    switch (expr->op->type) {
        case ASSIGN: break;
        case PLUS_ASSIGN: value = plus(name_value, expr->op, value); break;
        case MINUS_ASSIGN: value = minus(name_value, expr->op, value); break;
        case MULTI_ASSIGN: value = multi(name_value, expr->op, value); break;
        case DIV_ASSIGN: value = div(name_value, expr->op, value); break;
        case MOD_ASSIGN: value = mod(name_value, expr->op, value); break;
        default:
            throw interpreterRuntimeError(expr->op->line,
                                          "Undefined behavior of operator " + expr->op->text);
    }
    cast<EagleInstance>(object)->set(expr->name->text, value);
    return value;
}

ObjectPtr Interpreter::visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) {
    ObjectPtr object = evaluate(expr->object);
    if (!InstanceOf<EagleInstance>(object)) {
        throw interpreterRuntimeError(expr->name->line, "Only instances have properties");
    }
    return cast<EagleInstance>(object)->get(expr->name);
}

ObjectPtr Interpreter::visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) {
    return nullptr;
}

ObjectPtr Interpreter::visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) {
    return nullptr;
}

ObjectPtr Interpreter::visitThisExpr(std::shared_ptr<Expr::This> expr) {
    return getVariable(expr->keyword, expr);
}

ObjectPtr Interpreter::visitSuperExpr(std::shared_ptr<Expr::Super> expr) {
    int distance = local_variables[expr];
    EagleClassPtr super_class = cast<EagleClass>(current_env->getAt(distance, "super"));
    EagleInstancePtr instance = cast<EagleInstance>(current_env->getAt(distance - 1, "this"));
    EagleFunctionPtr method = super_class->getMethodRecursive(expr->method->text);

    if (method == nullptr) {
        throw interpreterRuntimeError(expr->method->line,
                                      "Undefined super class method '" + expr->method->text + "'");
    }
    return method->bind(instance);
}

ObjectPtr Interpreter::visitSequenceExpr(std::shared_ptr<Expr::Sequence> expr) {
    return nullptr;
}

ObjectPtr Interpreter::visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) {
    return nullptr;
}

// statements
ObjectPtr Interpreter::visitClassStmt(std::shared_ptr<Stmt::Class> stmt) {
    ObjectPtr super_class = nullptr;
    if (stmt->super_class != nullptr) {
        super_class = evaluate(stmt->super_class);
        if (!InstanceOf<EagleClass>(super_class)) {
            throw interpreterRuntimeError(stmt->super_class->name->line,
                                          "Superclass must be a class");
        }
    }
    current_env->define(stmt->name->text, nullptr);

    if (super_class != nullptr) {
        current_env = std::make_shared<Environment>(current_env);
        current_env->define("super", super_class);
    }

    // members
    std::unordered_map<std::string, ObjectPtr> members{};
    for (const auto& member : stmt->members) {
        ObjectPtr value;
        if (member->initializer != nullptr) {
            value = evaluate(member->initializer);
        } else {
            value = std::make_shared<Null>();
        }
        members[member->name->text] = value;
    }

    // methods
    std::unordered_map<std::string, EagleFunctionPtr> methods{};
    for (const auto& method : stmt->methods) {
        EagleFunctionPtr function =
            std::make_shared<EagleFunction>(method, current_env, method->name->text == "init");
        methods[method->name->text] = function;
    }

    EagleClassPtr klass = std::make_shared<EagleClass>(
        stmt->name->text, cast<EagleClass>(super_class), members, methods);

    if (super_class != nullptr) {
        current_env = current_env->enclosing;
    }

    current_env->assign(stmt->name, klass);
    return nullptr;
}

ObjectPtr Interpreter::visitFunctionStmt(std::shared_ptr<Stmt::Function> stmt) {
    EagleFunctionPtr function = std::make_shared<EagleFunction>(stmt, current_env, false);
    current_env->define(stmt->name->text, function);
    return nullptr;
}

ObjectPtr Interpreter::visitVarStmt(std::shared_ptr<Stmt::Var> stmt) {
    ObjectPtr value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
    current_env->define(stmt->name->text, value);
    return nullptr;
}

ObjectPtr Interpreter::visitIfStmt(std::shared_ptr<Stmt::If> stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->then_branch);
    } else {
        execute(stmt->else_branch);
    }
    return nullptr;
}

ObjectPtr Interpreter::visitWhileStmt(std::shared_ptr<Stmt::While> stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        try {
            execute(stmt->body);
        } catch (EagleBreak& eagle_break) {
            break;
        } catch (EagleContinue& eagle_continue) {
            continue;
        }
    }
    return nullptr;
}

ObjectPtr Interpreter::visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) {
    evaluate(stmt->expression);
    return nullptr;
}

ObjectPtr Interpreter::visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) {
    ObjectPtr value = evaluate(stmt->print_value);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

ObjectPtr Interpreter::visitReturnStmt(std::shared_ptr<Stmt::Return> stmt) {
    ObjectPtr return_value = nullptr;
    if (stmt->return_value != nullptr) {
        return_value = evaluate(stmt->return_value);
    }
    throw EagleReturn(return_value);
}

ObjectPtr Interpreter::visitBreakStmt(std::shared_ptr<Stmt::Break> stmt) {
    throw EagleBreak();
}

ObjectPtr Interpreter::visitContinueStmt(std::shared_ptr<Stmt::Continue> stmt) {
    throw EagleContinue();
}

ObjectPtr Interpreter::visitBlockStmt(std::shared_ptr<Stmt::Block> stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(current_env));
    return nullptr;
}

// help functions
void Interpreter::resolveLocal(const ExprPtr& expr, int distance) {
    local_variables[expr] = distance;
}

void Interpreter::executeBlock(const std::vector<StmtPtr>& statements,
                               const EnvironmentPtr& block_env) {
    ScopedEnvironment scoped(current_env, block_env);
    for (const auto& stmt : statements) {
        execute(stmt);
    }
}

ObjectPtr Interpreter::evaluate(const ExprPtr& expr) {
    return expr->accept(*this);
}

void Interpreter::execute(const StmtPtr& stmt) {
    stmt->accept(*this);
}

ObjectPtr Interpreter::assignVariable(const TokenPtr& name, const ExprPtr& expr,
                                      const ObjectPtr& value) {
    if (local_variables.find(expr) != local_variables.end()) {
        current_env->assignAt(local_variables[expr], name, value);
    } else {
        global_env->assign(name, value);
    }
    return value;
}

ObjectPtr Interpreter::getVariable(const TokenPtr& name, const ExprPtr& expr) {
    if (local_variables.find(expr) != local_variables.end()) {
        return current_env->getAt(local_variables[expr], name);
    } else {
        return global_env->get(name);
    }
}

bool Interpreter::checkNumber(const TokenPtr& op, const ObjectPtr& object) {
    if (InstanceOf<BigFloat>(object)) {
        return true;
    } else {
        throw interpreterRuntimeError(op->line,
                                      "Operator " + op->text + " needs object to be digit");
    }
}

bool Interpreter::checkTwoNumbers(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right,
                                  bool need_throw) {
    if (InstanceOf<BigFloat>(left) && InstanceOf<BigFloat>(right)) {
        return true;
    } else {
        if (need_throw) {
            throw interpreterRuntimeError(
                op->line, "Operator " + op->text + " needs both left and right to be digits");
        }
        return false;
    }
}

bool Interpreter::checkTwoStrings(const ObjectPtr& left, const TokenPtr& op,
                                  const ObjectPtr& right) {
    if (InstanceOf<String>(left) && InstanceOf<String>(right)) {
        return true;
    } else {
        return false;
    }
}
ObjectPtr Interpreter::plus(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return std::make_shared<BigFloat>((*cast<BigFloat>(left)) + (*cast<BigFloat>(right)));
    } else if (checkTwoStrings(left, op, right)) {
        return std::make_shared<String>(cast<String>(left)->str + cast<String>(right)->str);
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}

ObjectPtr Interpreter::minus(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<BigFloat>((*cast<BigFloat>(left)) - (*cast<BigFloat>(right)));
}
ObjectPtr Interpreter::multi(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<BigFloat>((*cast<BigFloat>(left)) * (*cast<BigFloat>(right)));
}

ObjectPtr Interpreter::div(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<BigFloat>((*cast<BigFloat>(left)) / (*cast<BigFloat>(right)));
}

ObjectPtr Interpreter::mod(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<BigFloat>((*cast<BigFloat>(left)) % (*cast<BigFloat>(right)));
}

bool Interpreter::less(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return (*cast<BigFloat>(left)) < (*cast<BigFloat>(right));
    } else if (checkTwoStrings(left, op, right)) {
        return cast<String>(left)->str < cast<String>(right)->str;
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}

bool Interpreter::lessEqual(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return (*cast<BigFloat>(left)) <= (*cast<BigFloat>(right));
    } else if (checkTwoStrings(left, op, right)) {
        return cast<String>(left)->str <= cast<String>(right)->str;
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}
bool Interpreter::greater(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return (*cast<BigFloat>(left)) > (*cast<BigFloat>(right));
    } else if (checkTwoStrings(left, op, right)) {
        return cast<String>(left)->str > cast<String>(right)->str;
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}
bool Interpreter::greaterEqual(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return (*cast<BigFloat>(left)) >= (*cast<BigFloat>(right));
    } else if (checkTwoStrings(left, op, right)) {
        return cast<String>(left)->str >= cast<String>(right)->str;
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}

bool Interpreter::isTruthy(const ObjectPtr& object) {
    if (InstanceOf<Null>(object))
        return false;

    if (InstanceOf<Boolean>(object))
        return cast<Boolean>(object)->value;

    if (InstanceOf<BigFloat>(object))
        return *cast<BigFloat>(object) == 0;

    if (InstanceOf<String>(object))
        return cast<String>(object)->str.empty();

    // TODO Containers

    return true;
}

bool Interpreter::isEqual(const ObjectPtr& left, const ObjectPtr& right) {
    if (InstanceOf<Null>(left) && InstanceOf<Null>(right))
        return true;

    if (InstanceOf<BigFloat>(left) && InstanceOf<BigFloat>(right))
        return *cast<BigFloat>(left) == *cast<BigFloat>(right);

    if (InstanceOf<String>(left) && InstanceOf<String>(right))
        return cast<String>(left)->str == cast<String>(right)->str;

    // TODO Containers

    return left == right;
}

std::string Interpreter::stringify(const ObjectPtr& object) {
    if (InstanceOf<Null>(object))
        return "nil";

    if (InstanceOf<BigFloat>(object))
        return cast<BigFloat>(object)->ToString();

    if (InstanceOf<Boolean>(object))
        return cast<Boolean>(object)->ToString();

    if (InstanceOf<String>(object))
        return cast<String>(object)->str;

    if (InstanceOf<EagleInstance>(object))
        return cast<EagleInstance>(object)->ToString();

    if (InstanceOf<EagleClass>(object))
        return cast<EagleClass>(object)->ToString();

    if (InstanceOf<EagleFunction>(object))
        return cast<EagleFunction>(object)->ToString();

    // TODO Containers && Function
    return "666";
}

EagleRuntimeError Interpreter::interpreterRuntimeError(int line, const std::string& message) {
    return EagleRuntimeError("[RuntimeError at line " + std::to_string(line) + "] " + message);
}

}  // namespace eagle