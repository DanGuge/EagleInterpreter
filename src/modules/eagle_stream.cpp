//
// Created by Akman on 2022/11/25.
//

#include "eagle_stream.h"

#include <utility>

#include "eagle_container.h"
#include "Number.h"

namespace eagle {

const std::unordered_map<std::string, StreamOneToOneMethod> EagleStream::one2one_methods = {
    {"map", map}, {"filter", filter}};
const std::unordered_map<std::string, StreamManyToManyMethod> EagleStream::many2many_methods = {
    {"limit", limit}};
const std::unordered_map<std::string, StreamFinalMethod> EagleStream::final_methods = {
    {"to_list", to_list},
    {"to_tuple", to_tuple},
    {"to_dict", to_dict},
    {"for_each", for_each},
    {"count", count}};

EagleStream::EagleStream(eagle::EagleContainerPtr init_expr,
                         std::vector<std::pair<TokenPtr, ObjectPtr>> operations)
    : init_expr(std::move(init_expr)), operations(std::move(operations)), executed(false) {}

ObjectPtr EagleStream::run(int line) {
    if (executed) {
        throw RuntimeError(line, "Stream should not be executed repeatedly");
    }
    if (operations.empty() || !isFinalMethod(operations[operations.size() - 1].first->text)) {
        return shared_from_this();
    }

    auto process_segments = GetProcessSegments();
    std::vector<ObjectPtr> elements = init_expr->iterator();
    std::vector<ObjectPtr> results;
    ObjectPtr final_result = std::make_shared<Null>();
    for (auto &segment : process_segments) {
        int seg_start = segment.first, seg_end = segment.second;
        // hand ont-to-one methods
        for (auto &ele : elements) {
            ObjectPtr ele_r = ele;
            for (int i = seg_start; i < seg_end; i++) {
                if (ele_r == nullptr) {
                    break;
                }
                auto func = operations[i].first;
                auto para = operations[i].second;
                auto method = one2one_methods.find(func->text)->second;
                ele_r = method(para, ele_r, func->line);
            }
            if (ele_r != nullptr) {
                results.emplace_back(ele_r);
            }
        }
        // hand many-to-many or final methods
        auto last_func = operations[seg_end].first;
        auto last_para = operations[seg_end].second;
        if (isManyToManyMethod(last_func->text)) {
            auto method = many2many_methods.find(last_func->text)->second;
            elements = method(last_para, results, last_func->line);
            results.clear();
        } else {
            auto method = final_methods.find(last_func->text)->second;
            final_result = method(last_para, results, last_func->line);
        }
    }
    return final_result;
}

void EagleStream::append(std::pair<TokenPtr, ObjectPtr> operation) {
    if (executed) {
        throw RuntimeError(operation.first->line, "Stream should not be executed repeatedly");
    }
    operations.emplace_back(std::move(operation));
}

void EagleStream::execute() {
    executed = true;
}

EagleStreamPtr EagleStream::copy(int line) {
    if (executed) {
        throw RuntimeError(line, "Stream should not be executed repeatedly");
    }
    return std::make_shared<EagleStream>(init_expr, operations);
}

std::string EagleStream::toString() {
    return "<stream at " + std::to_string(reinterpret_cast<size_t>(this)) + ">";
}

std::string EagleStream::GetStreamInfo() {
    return "stream methods:\n"

           "non-final methods:\n"
           "\tmap(func: Function)\n"
           "\t\tusage: for each element of stream, return the result of func(element)\n"

           "\tfilter(func: Function)\n"
           "\t\tusage: return element only if func(element) is true, otherwise drop it\n"

           "\tlimit(n: Number)\n"
           "\t\tusage: return the first n elements of stream\n"

           "final methods:\n"
           "\tto_list()->List\n"
           "\t\tusage: return a list consist of stream's elements\n"

           "\tto_tuple()->Tuple\n"
           "\t\tusage: return a tuple consist of stream's elements\n"

           "\tto_dict()->Dict\n"
           "\t\tusage: if each element of stream is a tuple contains 2 sub_elements(key, value), "
           "return a Dict consist of (key, value) pairs\n"

           "\tfor_each(func: Function)->Null\n"
           "\t\tusage: for each element of stream, execute func(element)\n"

           "\tcount()->Number\n"
           "\t\tusage: return the number of stream's elements";
}

std::vector<std::pair<int, int>> EagleStream::GetProcessSegments() {
    std::vector<std::pair<int, int>> process_segments;
    int start = 0, cur = 0;
    while (cur < operations.size()) {
        auto &func = operations[cur].first;
        if (isManyToManyMethod(func->text) || isFinalMethod(func->text)) {
            process_segments.emplace_back(std::pair<int, int>{start, cur});
            cur++;
            start = cur;
        } else if (isOneToOneMethod(func->text)) {
            cur++;
        } else {
            throw RuntimeError(func->line, "Stream has no operation named " + func->text);
        }
    }
    return std::move(process_segments);
}

bool EagleStream::isFinalMethod(const std::string &method_name) {
    return final_methods.find(method_name) != final_methods.end();
}

bool EagleStream::isOneToOneMethod(const std::string &method_name) {
    return one2one_methods.find(method_name) != one2one_methods.end();
}

bool EagleStream::isManyToManyMethod(const std::string &method_name) {
    return many2many_methods.find(method_name) != many2many_methods.end();
}

ObjectPtr EagleStream::map(const eagle::ObjectPtr &para, const eagle::ObjectPtr &element,
                           int line) {
    if (para == nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'map' should not be null");
    }
    if (!InstanceOf<EagleCallable>(para)) {
        throw RuntimeError(line, "The parameter of stream method 'map' should be callable");
    }
    EagleCallablePtr call = cast<EagleCallable>(para);
    if (call->arity() != 1) {
        throw RuntimeError(
            line, "The arity of inner function parameter of stream method 'map' should be 1.");
    }
    std::vector<ObjectPtr> arguments{element};
    return call->WrapperCall(arguments, line);
}

ObjectPtr EagleStream::filter(const eagle::ObjectPtr &para, const eagle::ObjectPtr &element,
                              int line) {
    if (para == nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'filter' should not be null");
    }
    if (!InstanceOf<EagleCallable>(para)) {
        throw RuntimeError(line, "The parameter of stream method 'filter' should be callable");
    }
    EagleCallablePtr call = cast<EagleCallable>(para);
    if (call->arity() != 1) {
        throw RuntimeError(
            line, "The arity of inner function parameter of stream method 'filter' should be 1.");
    }
    std::vector<ObjectPtr> arguments{element};
    ObjectPtr filter_result = call->WrapperCall(arguments, line);
    return filter_result->WrapIsTruthy() ? element : nullptr;
}

std::vector<ObjectPtr> EagleStream::limit(const eagle::ObjectPtr &para,
                                          const std::vector<ObjectPtr> &elements, int line) {
    if (para == nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'limit' should not be null");
    }
    if (!InstanceOf<Number>(para)) {
        throw RuntimeError(line, "The parameter of stream method 'limit' should be integer");
    }
    BigFloatPtr n = cast<Number>(para);
    if (!n->isInteger()) {
        throw RuntimeError(line,
                           "The parameter of stream method 'limit' should be integer, not float");
    }
    int limit_n = n->ToInt(), ele_size = (int)elements.size();
    std::vector<ObjectPtr> results;
    for (int i = 0; i < limit_n && i < ele_size; i++) {
        results.emplace_back(elements[i]);
    }
    return std::move(results);
}

ObjectPtr EagleStream::to_list(const eagle::ObjectPtr &para, const std::vector<ObjectPtr> &elements,
                               int line) {
    if (para != nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'to_list' should be null");
    }
    return std::make_shared<EagleList>(elements);
}

ObjectPtr EagleStream::to_tuple(const eagle::ObjectPtr &para,
                                const std::vector<ObjectPtr> &elements, int line) {
    if (para != nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'to_tuple' should be null");
    }
    return std::make_shared<EagleTuple>(elements);
}

ObjectPtr EagleStream::to_dict(const eagle::ObjectPtr &para, const std::vector<ObjectPtr> &elements,
                               int line) {
    if (para != nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'to_dict' should be null");
    }
    std::vector<EagleDictEntry> entries;
    for (int i = 0; i < elements.size(); i++) {
        auto &element = elements[i];
        if (!InstanceOf<EagleList>(element) && !InstanceOf<EagleTuple>(element)) {
            throw RuntimeError(line, "Cannot convert element#" + std::to_string(i) +
                                         "(not a sequence) to an dict entry.");
        }
        std::vector<ObjectPtr> sequence = cast<EagleContainer>(element)->iterator();
        if (sequence.size() != 2) {
            throw RuntimeError(line, "Cannot convert element#" + std::to_string(i) +
                                         "(a sequence whose size is not 2) to an dict entry.");
        }
        entries.emplace_back(EagleDictEntry{sequence[0], sequence[1]});
    }
    return std::make_shared<EagleDict>(std::move(entries));
}

ObjectPtr EagleStream::for_each(const eagle::ObjectPtr &para,
                                const std::vector<ObjectPtr> &elements, int line) {
    if (para == nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'for_each' should not be null");
    }
    if (!InstanceOf<EagleCallable>(para)) {
        throw RuntimeError(line, "The parameter of stream method 'for_each' should be callable");
    }
    EagleCallablePtr call = cast<EagleCallable>(para);
    if (call->arity() != 1) {
        throw RuntimeError(
            line, "The arity of inner function parameter of stream method 'for_each' should be 1.");
    }
    for (auto &element : elements) {
        std::vector<ObjectPtr> arguments{element};
        call->WrapperCall(arguments, line);
    }
    return std::make_shared<Null>();
}

ObjectPtr EagleStream::count(const eagle::ObjectPtr &para, const std::vector<ObjectPtr> &elements,
                             int line) {
    if (para != nullptr) {
        throw RuntimeError(line, "The parameter of stream method 'count' should be null");
    }
    return std::make_shared<Number>((int)elements.size());
}

EagleStreamCall::EagleStreamCall(eagle::EagleStreamPtr stream, eagle::TokenPtr func)
    : stream(std::move(stream)), func(std::move(func)) {}

ObjectPtr EagleStreamCall::call(std::vector<ObjectPtr> &arguments, int call_line) {
    if (!arguments.empty() && arguments.size() != 1) {
        throw RuntimeError(call_line, "The number of parameters of Stream call should be 0 or 1.");
    }
    ObjectPtr para = arguments.empty() ? nullptr : arguments[0];
    stream->append(std::pair<TokenPtr, ObjectPtr>{func, para});
    return stream->run(call_line);
}

}  // namespace eagle
