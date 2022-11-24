//
// Created by Akman on 2022/11/23.
//

#include <algorithm>

#include "BigFloat.h"
#include "eagle_container.h"

namespace eagle {

const std::unordered_map<std::string, BuiltInClassMethodInfo> EagleTuple::built_in_methods = {
    {"size", {size, 0}},
    {"empty", {empty, 0}},
    {"count", {count, 1}},
    {"contains", {contains, 1}},
};

EagleTuple::EagleTuple(std::vector<ObjectPtr> elements) : elements(std::move(elements)) {}

ObjectPtr EagleTuple::get(const eagle::ObjectPtr &subscript, int line) {
    int index_num = CheckSubscript(subscript, line);
    return elements[index_num];
}

void EagleTuple::set(const eagle::ObjectPtr &subscript, eagle::ObjectPtr value, int line) {
    throw RuntimeError(line, "'Tuple does not support item assignment");
}

BuiltInClassMethodInfo EagleTuple::GetMethod(const eagle::TokenPtr &method_name) {
    auto method_found = built_in_methods.find(method_name->text);
    if (method_found != built_in_methods.end()) {
        return method_found->second;
    } else {
        throw RuntimeError(method_name->line, "Tuple has no method named " + method_name->text);
    }
}

std::string EagleTuple::toString() {
    std::string str = "(";
    for (int i = 0; i < elements.size(); i++) {
        str += elements[i]->toString();
        str += (elements.size() == 1) ? "," : (i == elements.size() - 1) ? "" : ", ";
    }
    str += ")";
    return std::move(str);
}

bool EagleTuple::equals(eagle::ObjectPtr other) {
    if (!InstanceOf<EagleTuple>(other)) {
        return false;
    }
    auto another = cast<EagleTuple>(other);
    if (elements.size() != another->elements.size()) {
        return false;
    }
    for (int i = 0; i < elements.size(); i++) {
        if (!elements[i]->equals(another->elements[i])) {
            return false;
        }
    }
    return true;
}

size_t EagleTuple::hashcode() {
    // TODO: 参考的java的数组hash算法，待商榷
    size_t h = 0;
    for (auto &element : elements) {
        h = 31 * h + element->hashcode();
    }
    return h;
}

bool EagleTuple::isTruthy() {
    return !elements.empty();
}

int EagleTuple::CheckSubscript(const eagle::ObjectPtr &subscript, int line) {
    if (!InstanceOf<BigFloat>(subscript)) {
        throw RuntimeError(line, "Tuple subscript must be integer.");
    }
    auto index = cast<BigFloat>(subscript);
    if (!index->isInteger()) {
        throw RuntimeError(line, "Tuple subscript must be integer, not float.");
    }
    int index_num = index->ToInt(), list_size = (int)elements.size();
    if (index_num < -list_size || index_num >= list_size) {
        throw RuntimeError(line, "Tuple subscript out of range.");
    }
    if (index_num < 0) {
        index_num += list_size;
    }
    return index_num;
}

ObjectPtr EagleTuple::size(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                           int line) {
    EagleTuplePtr tuple = CheckBuiltInClassType(object, line, "size");
    return std::make_shared<BigFloat>((int)tuple->elements.size());
}

ObjectPtr EagleTuple::empty(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                            int line) {
    EagleTuplePtr tuple = CheckBuiltInClassType(object, line, "empty");
    return std::make_shared<Boolean>(tuple->elements.empty());
}

ObjectPtr EagleTuple::count(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                            int line) {
    EagleTuplePtr tuple = CheckBuiltInClassType(object, line, "count");
    int cnt = (int)std::count_if(tuple->elements.begin(), tuple->elements.end(),
                                 [&](const ObjectPtr &e) { return e->equals(args[0]); });
    return std::make_shared<BigFloat>(cnt);
}

ObjectPtr EagleTuple::contains(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                               int line) {
    EagleTuplePtr tuple = CheckBuiltInClassType(object, line, "contains");
    bool contain = std::any_of(tuple->elements.begin(), tuple->elements.end(),
                               [&](const ObjectPtr &e) { return e->equals(args[0]); });
    return std::make_shared<Boolean>(contain);
}

EagleTuplePtr EagleTuple::CheckBuiltInClassType(eagle::BuiltInClassPtr instance, int line,
                                                const std::string &method_name) {
    if (!InstanceOf<EagleTuple>(instance)) {
        throw RuntimeError(line, "Expect callee type: Tuple, for method: " + method_name + ".");
    }
    return cast<EagleTuple>(instance);
}

}  // namespace eagle
