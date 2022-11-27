//
// Created by Akman on 2022/11/23.
//

#include <algorithm>

#include "BigFloat.h"
#include "eagle_container.h"
#include "eagle_exceptions.h"

namespace eagle {

const std::unordered_map<std::string, BuiltInClassMethodInfo> EagleList::built_in_methods = {
    {"size", {size, 0}},      {"empty", {empty, 0}},       {"append", {append, 1}},
    {"clear", {clear, 0}},    {"contains", {contains, 1}}, {"count", {count, 1}},
    {"insert", {insert, 2}},  {"remove", {remove, 1}},     {"pop", {pop, 0}},
    {"reverse", {reverse, 0}}};

EagleList::EagleList(std::vector<ObjectPtr> elements) : elements(std::move(elements)) {}

ObjectPtr EagleList::get(const eagle::ObjectPtr &subscript, int line) {
    int index_num = CheckSubscript(subscript, line);
    return elements[index_num];
}

void EagleList::set(const eagle::ObjectPtr &subscript, eagle::ObjectPtr value, int line) {
    int index_num = CheckSubscript(subscript, line);
    if (value.get() == this) {
        throw RuntimeError(line, "List can not contain itself.");
    }
    elements[index_num] = std::move(value);
}

ObjectPtr EagleList::size() {
    return std::make_shared<BigFloat>((int)elements.size());
}

std::vector<ObjectPtr> EagleList::iterator() {
    return elements;
}

BuiltInClassMethodInfo EagleList::GetMethod(const eagle::TokenPtr &method_name) {
    auto method_found = built_in_methods.find(method_name->text);
    if (method_found != built_in_methods.end()) {
        return method_found->second;
    } else {
        throw RuntimeError(method_name->line, "List has no method named " + method_name->text);
    }
}

std::string EagleList::GetBuiltInClassInfo() {
    return "built-in class list methods:\n"

           "size()->Number\n"
           "\tusage: return the size of list\n"

           "empty()->Boolean\n"
           "\tusage: return whether list is empty\n"

           "append(element: Object)->Null\n"
           "\tusage: append element to the end of list\n"

           "clear()->Null\n"
           "\tusage: clear list\n"

           "contains(element: Object)->Boolean\n"
           "\tusage: return whether list contains element\n"

           "count(element: Object)->Number\n"
           "\tusage: return the number of times that element appears in list, return 0 if list "
           "does not contain element\n"

           "insert(index: Number, element: Object)->Null\n"
           "\tusage: insert element before index\n"

           "remove(value: Object)->Null\n"
           "\tusage: remove first occurrence of value, do nothing if list does not contain value\n"

           "pop()->Null\n"
           "\tusage: remove the last element of list, do nothing if list is empty\n"

           "reverse()->Null\n"
           "\tusage: reverse list";
}

std::string EagleList::toString() {
    std::string str = "[";
    for (int i = 0; i < elements.size(); i++) {
        str += elements[i]->toString();
        str += (i == elements.size() - 1) ? "" : ", ";
    }
    str += "]";
    return std::move(str);
}

bool EagleList::equals(eagle::ObjectPtr other) {
    if (other.get() == this) {
        return true;
    }
    if (!InstanceOf<EagleList>(other)) {
        return false;
    }
    auto another = cast<EagleList>(other);
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

size_t EagleList::hashcode() {
    size_t h = 0;
    for (auto &element : elements) {
        h = 31 * h + element->hashcode();
    }
    return h;
}

bool EagleList::isTruthy() {
    return !elements.empty();
}

int EagleList::CheckSubscript(const eagle::ObjectPtr &subscript, int line) {
    if (!InstanceOf<BigFloat>(subscript)) {
        throw RuntimeError(line, "List subscript must be integer.");
    }
    auto index = cast<BigFloat>(subscript);
    if (!index->isInteger()) {
        throw RuntimeError(line, "List subscript must be integer, not float.");
    }
    int index_num = index->ToInt(), list_size = (int)elements.size();
    if (index_num < -list_size || index_num >= list_size) {
        throw RuntimeError(line, "List subscript out of range.");
    }
    if (index_num < 0) {
        index_num += list_size;
    }
    return index_num;
}

ObjectPtr EagleList::size(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                          int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "size");
    return std::make_shared<BigFloat>((int)list->elements.size());
}

ObjectPtr EagleList::empty(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                           int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "empty");
    return std::make_shared<Boolean>(list->elements.empty());
}

ObjectPtr EagleList::append(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                            int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "append");
    if (list.get() == args[0].get()) {
        throw RuntimeError(line, "List can not contain itself.");
    }
    list->elements.emplace_back(args[0]);
    return std::make_shared<Null>();
}

ObjectPtr EagleList::clear(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                           int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "clear");
    list->elements.clear();
    return std::make_shared<Null>();
}

ObjectPtr EagleList::contains(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                              int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "contains");
    bool contain = std::any_of(list->elements.begin(), list->elements.end(),
                               [&](const ObjectPtr &e) { return e->equals(args[0]); });
    return std::make_shared<Boolean>(contain);
}

ObjectPtr EagleList::count(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                           int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "count");
    int cnt = (int)std::count_if(list->elements.begin(), list->elements.end(),
                                 [&](const ObjectPtr &e) { return e->equals(args[0]); });
    return std::make_shared<BigFloat>(cnt);
}

ObjectPtr EagleList::insert(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                            int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "insert");
    if (!InstanceOf<BigFloat>(args[0])) {
        throw RuntimeError(line, "List subscript must be integer.");
    }
    auto position = cast<BigFloat>(args[0]);
    if (!position->isInteger()) {
        throw RuntimeError(line, "List subscript must be integer, not float.");
    }
    if (list.get() == args[1].get()) {
        throw RuntimeError(line, "List can not contain itself.");
    }
    int pos_num = position->ToInt(), list_size = (int)list->elements.size();
    if (pos_num < -list_size) {
        pos_num = 0;
    } else if (pos_num >= list_size) {
        pos_num = list_size;
    } else if (pos_num < 0) {
        pos_num += list_size;
    }
    list->elements.insert(list->elements.begin() + pos_num, args[1]);
    return std::make_shared<Null>();
}

ObjectPtr EagleList::remove(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                            int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "remove");
    for (auto it = list->elements.begin(); it != list->elements.end(); it++) {
        if ((*it)->equals(args[0])) {
            list->elements.erase(it);
            return std::make_shared<Null>();
        }
    }
    return std::make_shared<Null>();
}

ObjectPtr EagleList::pop(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                         int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "pop");
    if (!list->elements.empty()) {
        list->elements.pop_back();
    }
    return std::make_shared<Null>();
}

ObjectPtr EagleList::reverse(const eagle::BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                             int line) {
    EagleListPtr list = CheckBuiltInClassType(object, line, "reverse");
    std::reverse(list->elements.begin(), list->elements.end());
    return std::make_shared<Null>();
}

EagleListPtr EagleList::CheckBuiltInClassType(const eagle::BuiltInClassPtr &instance, int line,
                                              const std::string &method_name) {
    if (!InstanceOf<EagleList>(instance)) {
        throw RuntimeError(line, "Expect callee type: List, for method: " + method_name + ".");
    }
    return cast<EagleList>(instance);
}

}  // namespace eagle
