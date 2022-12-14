//
// Created by Akman on 2022/11/6.
//

#include "interpreter.h"

#include "built_in_functions/eagle_built_in_functions.h"
#include "modules/eagle_class.h"
#include "modules/eagle_container.h"
#include "modules/eagle_stream.h"
#include "modules/eagle_string.h"
#include "Number.h"
#include "pretty_print/pretty_print.h"
#include "util/error_reporter.h"

namespace eagle {

Interpreter& Interpreter::getInstance() {
    return interpreter;
}

Interpreter Interpreter::interpreter{};

Interpreter::Interpreter() {
    global_env = std::make_shared<Environment>(nullptr);
    current_env = global_env;
    running_mode = RunningMode::SHELL_MODE;
    local_variables = {};
    init_built_in();
}

void Interpreter::init_built_in() {
    // built-in functions
    global_env->define("str", std::make_shared<built_in_functions::Str>());
    global_env->define("num", std::make_shared<built_in_functions::Num>());
    global_env->define("read_from_file", std::make_shared<built_in_functions::ReadFromFile>());
    global_env->define("write_to_file", std::make_shared<built_in_functions::WriteToFile>());
    global_env->define("input", std::make_shared<built_in_functions::Input>());
    global_env->define("help", std::make_shared<built_in_functions::Help>());
    global_env->define("bool", std::make_shared<built_in_functions::Bool>());
    global_env->define("class_method", std::make_shared<built_in_functions::ClassMethod>());
    global_env->define("globals", std::make_shared<built_in_functions::Globals>());
    global_env->define("id", std::make_shared<built_in_functions::Id>());
    global_env->define("len", std::make_shared<built_in_functions::Len>());
    // built-in class
    global_env->define("list_info", std::make_shared<EagleList>(std::vector<ObjectPtr>{}));
    global_env->define("tuple_info", std::make_shared<EagleTuple>(std::vector<ObjectPtr>{}));
    global_env->define("dict_info", std::make_shared<EagleDict>(std::vector<EagleDictEntry>{}));
    global_env->define("stream_info", std::make_shared<EagleStream>(
                                          std::make_shared<EagleList>(std::vector<ObjectPtr>{}),
                                          std::vector<std::pair<TokenPtr, ObjectPtr>>{}));
}

void Interpreter::set_running_mode(RunningMode mode) {
    running_mode = mode;
}

EagleDictPtr Interpreter::getGlobals() {
    std::vector<EagleDictEntry> elements;
    for (const auto& entry : global_env->get_name_object_map()) {
        ObjectPtr key = std::make_shared<String>(entry.first);
        ObjectPtr value = entry.second;
        elements.emplace_back(std::move(key), std::move(value));
    }
    return std::make_shared<EagleDict>(std::move(elements));
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
        left = right;
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
            return std::make_shared<Number>(0 - (*cast<Number>(value)));
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
    // It means do not check parameters number when function->arity() return -1
    if (arguments.size() != function->arity() && function->arity() != -1) {
        throw interpreterRuntimeError(
            expr->line, "Expect arguments size is " + std::to_string(function->arity()) + " But " +
                            std::to_string(arguments.size()) + " argument(s) is(are) given");
    }
    return function->WrapperCall(arguments, expr->line);
}

ObjectPtr Interpreter::visitLiteralExpr(std::shared_ptr<Expr::Literal> expr) {
    return expr->value;
}

ObjectPtr Interpreter::visitVariableExpr(std::shared_ptr<Expr::Variable> expr) {
    return getVariable(expr->name, expr);
}

ObjectPtr Interpreter::visitStreamExpr(std::shared_ptr<Expr::Stream> expr) {
    ObjectPtr object = evaluate(expr->expr);
    if (!InstanceOf<EagleContainer>(object)) {
        throw interpreterRuntimeError(expr->line,
                                      "Only containers can be executed 'stream()' operation");
    }
    EagleContainerPtr init_expr = cast<EagleContainer>(object);
    std::vector<std::pair<TokenPtr, ObjectPtr>> operations;
    for (auto& expr_operation : expr->operations) {
        ObjectPtr para =
            expr_operation.second == nullptr ? nullptr : evaluate(expr_operation.second);
        operations.emplace_back(
            std::pair<TokenPtr, ObjectPtr>{expr_operation.first, std::move(para)});
    }
    EagleStreamPtr stream =
        std::make_shared<EagleStream>(std::move(init_expr), std::move(operations));
    return stream->run(expr->line);
}

ObjectPtr Interpreter::visitSwitchExpr(std::shared_ptr<Expr::Switch> expr) {
    ObjectPtr condition = evaluate(expr->expr);
    ObjectPtr value = nullptr;
    for (const auto& case_result : expr->case_results) {
        if (isEqual(condition, evaluate(case_result.first))) {
            value = evaluate(case_result.second);
            break;
        }
    }
    return value != nullptr ? value : evaluate(expr->default_results);
}

ObjectPtr Interpreter::visitLambdaExpr(std::shared_ptr<Expr::Lambda> expr) {
    return std::make_shared<EagleLambda>(expr, current_env);
}

ObjectPtr Interpreter::visitInstanceSetExpr(std::shared_ptr<Expr::InstanceSet> expr) {
    ObjectPtr object = evaluate(expr->object);
    if (!InstanceOf<EagleInstance>(object)) {
        throw interpreterRuntimeError(expr->name->line,
                                      "Only instances have fields to set property");
    }
    EagleInstancePtr instance = cast<EagleInstance>(object);

    ObjectPtr value = evaluate(expr->value);
    if (expr->op->type != ASSIGN) {
        ObjectPtr name_value = instance->get(expr->name->text, instance);
        if (name_value == nullptr) {
            throw interpreterRuntimeError(expr->name->line,
                                          "Undefined property '" + expr->name->text + "'.");
        }
        switch (expr->op->type) {
            case PLUS_ASSIGN: value = plus(name_value, expr->op, value); break;
            case MINUS_ASSIGN: value = minus(name_value, expr->op, value); break;
            case MULTI_ASSIGN: value = multi(name_value, expr->op, value); break;
            case DIV_ASSIGN: value = div(name_value, expr->op, value); break;
            case MOD_ASSIGN: value = mod(name_value, expr->op, value); break;
            default:
                throw interpreterRuntimeError(expr->op->line,
                                              "Undefined behavior of operator " + expr->op->text);
        }
    }
    cast<EagleInstance>(object)->set(expr->name->text, value);
    return value;
}

ObjectPtr Interpreter::visitInstanceGetExpr(std::shared_ptr<Expr::InstanceGet> expr) {
    ObjectPtr object = evaluate(expr->object);
    if (InstanceOf<EagleInstance>(object)) {
        EagleInstancePtr instance = cast<EagleInstance>(object);
        ObjectPtr value = instance->get(expr->name->text, instance);
        if (value == nullptr) {
            throw interpreterRuntimeError(expr->name->line,
                                          "Undefined property '" + expr->name->text + "'.");
        }
        return value;
    } else if (InstanceOf<BuiltInClass>(object)) {
        BuiltInClassPtr instance = cast<BuiltInClass>(object);
        BuiltInClassMethodInfo method_info = instance->GetMethod(expr->name);
        return std::make_shared<BuiltInClassCall>(std::move(instance), method_info.method,
                                                  method_info.method_arity);
    } else if (InstanceOf<EagleStream>(object)) {
        EagleStreamPtr stream = cast<EagleStream>(object);
        EagleStreamPtr new_stream = stream->copy(expr->name->line);
        stream->execute();
        return std::make_shared<EagleStreamCall>(new_stream, expr->name);
    } else {
        throw interpreterRuntimeError(expr->name->line, "Only instances have properties");
    }
}

ObjectPtr Interpreter::visitContainerSetExpr(std::shared_ptr<Expr::ContainerSet> expr) {
    ObjectPtr object = evaluate(expr->container);
    if (!InstanceOf<EagleContainer>(object)) {
        throw interpreterRuntimeError(expr->op->line, "Only containers have elements");
    }
    EagleContainerPtr container = cast<EagleContainer>(object);
    ObjectPtr subscript = evaluate(expr->subscript);
    ObjectPtr value = evaluate(expr->value);
    if (expr->op->type != ASSIGN) {
        ObjectPtr stored_value = container->get(subscript, expr->op->line);
        switch (expr->op->type) {
            case PLUS_ASSIGN: value = plus(stored_value, expr->op, value); break;
            case MINUS_ASSIGN: value = minus(stored_value, expr->op, value); break;
            case MULTI_ASSIGN: value = multi(stored_value, expr->op, value); break;
            case DIV_ASSIGN: value = div(stored_value, expr->op, value); break;
            case MOD_ASSIGN: value = mod(stored_value, expr->op, value); break;
            default:
                throw interpreterRuntimeError(expr->op->line,
                                              "Undefined behavior of operator " + expr->op->text);
        }
    }
    container->set(subscript, value, expr->op->line);
    return value;
}

ObjectPtr Interpreter::visitContainerGetExpr(std::shared_ptr<Expr::ContainerGet> expr) {
    ObjectPtr object = evaluate(expr->container);
    if (!InstanceOf<EagleContainer>(object)) {
        throw interpreterRuntimeError(expr->line, "Only containers have elements");
    }
    EagleContainerPtr container = cast<EagleContainer>(object);
    ObjectPtr subscript = evaluate(expr->subscript);
    return container->get(subscript, expr->line);
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
    std::vector<ObjectPtr> elements;
    for (auto& ele_expr : expr->elements) {
        elements.emplace_back(evaluate(ele_expr));
    }
    if (expr->type->type == LIST) {
        return std::make_shared<EagleList>(std::move(elements));
    } else if (expr->type->type == TUPLE) {
        return std::make_shared<EagleTuple>(std::move(elements));
    } else {
        throw interpreterRuntimeError(expr->type->line,
                                      "No sequence container called: " + expr->type->text);
    }
}

ObjectPtr Interpreter::visitAssociativeExpr(std::shared_ptr<Expr::Associative> expr) {
    std::vector<EagleDictEntry> elements;
    for (auto& entry_expr : expr->elements) {
        ObjectPtr key = evaluate(entry_expr.first);
        ObjectPtr value = evaluate(entry_expr.second);
        elements.emplace_back(EagleDictEntry{std::move(key), std::move(value)});
    }
    if (expr->type->type == DICT) {
        return std::make_shared<EagleDict>(std::move(elements));
    } else {
        throw interpreterRuntimeError(expr->type->line,
                                      "No associative container called: " + expr->type->text);
    }
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
    ObjectPtr value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    } else {
        value = std::make_shared<Null>();
    }
    current_env->define(stmt->name->text, value);
    return nullptr;
}

ObjectPtr Interpreter::visitIfStmt(std::shared_ptr<Stmt::If> stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->then_branch);
    } else if (stmt->else_branch != nullptr) {
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

ObjectPtr Interpreter::visitForStmt(std::shared_ptr<Stmt::For> stmt) {
    EnvironmentPtr for_env = std::make_shared<Environment>(current_env);
    ScopedEnvironment scoped(current_env, for_env);
    if (stmt->initializer != nullptr) {
        execute(stmt->initializer);
    }
    while (isTruthy(evaluate(stmt->condition))) {
        try {
            execute(stmt->body);
        } catch (EagleBreak& eagle_break) {
            break;
        } catch (EagleContinue& eagle_continue) {}

        if (stmt->increment != nullptr) {
            execute(stmt->increment);
        }
    }
    return nullptr;
}

ObjectPtr Interpreter::visitExpressionStmt(std::shared_ptr<Stmt::Expression> stmt) {
    ObjectPtr value = evaluate(stmt->expression);
    if (running_mode == RunningMode::SHELL_MODE) {
        if (InstanceOf<Expr::Assign>(stmt->expression) ||
            InstanceOf<Expr::InstanceSet>(stmt->expression) ||
            InstanceOf<Expr::ContainerSet>(stmt->expression)) {
            return nullptr;
        } else if (value != nullptr && !InstanceOf<Null>(value)) {
            pretty_print::PrettyPrint::print(pretty_print::Front::BLUE, stringify(value));
        }
    }
    return nullptr;
}

ObjectPtr Interpreter::visitPrintStmt(std::shared_ptr<Stmt::Print> stmt) {
    ObjectPtr value = evaluate(stmt->print_value);
    std::string print_str;
    if (InstanceOf<String>(value)) {
        print_str = cast<String>(value)->str;
    } else {
        print_str = stringify(value);
    }
    pretty_print::PrettyPrint::print(pretty_print::Front::BLUE, print_str);
    return nullptr;
}

ObjectPtr Interpreter::visitReturnStmt(std::shared_ptr<Stmt::Return> stmt) {
    ObjectPtr return_value;
    if (stmt->return_value != nullptr) {
        return_value = evaluate(stmt->return_value);
    } else {
        return_value = std::make_shared<Null>();
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

ObjectPtr Interpreter::visitEmptyStmt(std::shared_ptr<Stmt::Empty> stmt) {
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

void Interpreter::execute(const StmtPtr& statement, const EnvironmentPtr& stmt_env) {
    ScopedEnvironment scoped{current_env, stmt_env};
    execute(statement);
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
    if (InstanceOf<Number>(object)) {
        return true;
    } else {
        throw interpreterRuntimeError(op->line,
                                      "Operator " + op->text + " needs object to be digit");
    }
}

bool Interpreter::checkTwoNumbers(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right,
                                  bool need_throw) {
    if (InstanceOf<Number>(left) && InstanceOf<Number>(right)) {
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
        return std::make_shared<Number>((*cast<Number>(left)) + (*cast<Number>(right)));
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
    return std::make_shared<Number>((*cast<Number>(left)) - (*cast<Number>(right)));
}

ObjectPtr Interpreter::multi(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<Number>((*cast<Number>(left)) * (*cast<Number>(right)));
}

ObjectPtr Interpreter::div(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<Number>((*cast<Number>(left)) / (*cast<Number>(right)));
}

ObjectPtr Interpreter::mod(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    checkTwoNumbers(left, op, right, true);
    return std::make_shared<Number>((*cast<Number>(left)) % (*cast<Number>(right)));
}

bool Interpreter::less(const ObjectPtr& left, const TokenPtr& op, const ObjectPtr& right) {
    if (checkTwoNumbers(left, op, right, false)) {
        return (*cast<Number>(left)) < (*cast<Number>(right));
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
        return (*cast<Number>(left)) <= (*cast<Number>(right));
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
        return (*cast<Number>(left)) > (*cast<Number>(right));
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
        return (*cast<Number>(left)) >= (*cast<Number>(right));
    } else if (checkTwoStrings(left, op, right)) {
        return cast<String>(left)->str >= cast<String>(right)->str;
    } else {
        throw interpreterRuntimeError(
            op->line,
            "Operator " + op->text + " needs both left and right to be digits or strings");
    }
}

/* isTruthy(), isEqual(), stringify()
 * Need to support:
 * 1. Null & Number & Boolean
 * 2. String
 * 3. Class & Instance
 * 4. Function
 * 5. List & Tuple & Dict
 */
bool Interpreter::isTruthy(const ObjectPtr& object) {
    return object->WrapIsTruthy();
}

bool Interpreter::isEqual(const ObjectPtr& left, const ObjectPtr& right) {
    return left->WrapEquals(right);
}

std::string Interpreter::stringify(const ObjectPtr& object) {
    return object->WrapToString();
}

EagleRuntimeError Interpreter::interpreterRuntimeError(int line, const std::string& message) {
    return EagleRuntimeError("[RuntimeError at line " + std::to_string(line) + "] " + message);
}

}  // namespace eagle