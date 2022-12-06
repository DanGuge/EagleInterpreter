//
// Created by Akman on 2022/11/23.
//

#include <algorithm>

#include "eagle_container.h"
#include "Number.h"

namespace eagle {

const std::unordered_map<std::string, BuiltInClassMethodInfo> EagleDict::built_in_methods = {
    {"size", {size, 0}},
    {"clear", {clear, 0}},
    {"get", {get_value, 1}},
    {"put", {put, 2}},
    {"contains_key", {contains_key, 1}},
    {"contains_value", {contains_value, 1}},
    {"remove", {remove, 1}},
    {"keys", {keys, 0}},
    {"values", {values, 0}},
    {"items", {items, 0}},
};

EagleDict::EagleDict(const std::vector<EagleDictEntry> &elements) {
    element_cnt = 0;
    for (auto &insert_entry : elements) {
        size_t h = insert_entry.first->hashcode();
        InsertDictEntry(h, insert_entry.first, insert_entry.second);
    }
}

ObjectPtr EagleDict::get(const eagle::ObjectPtr &key, int line) {
    size_t h = key->hashcode();
    auto found = elements.find(h);
    if (found == elements.end()) {
        throw RuntimeError(line, "Dict does not have key: " + key->toString());
    }
    for (auto &entry : found->second) {
        if (key->equals(entry.first)) {
            return entry.second;
        }
    }
    throw RuntimeError(line, "Dict does not have key: " + key->toString());
}

void EagleDict::set(const eagle::ObjectPtr &key, eagle::ObjectPtr value, int line) {
    if (key.get() == this || value.get() == this) {
        throw RuntimeError(line, "Dict can not contain itself.");
    }
    InsertDictEntry(key->hashcode(), key, std::move(value));
}

ObjectPtr EagleDict::size() {
    return std::make_shared<Number>(this->element_cnt);
}

std::vector<ObjectPtr> EagleDict::iterator() {
    std::vector<ObjectPtr> keys;
    for (auto &node : elements) {
        for (auto &entry : node.second) {
            keys.emplace_back(entry.first);
        }
    }
    return std::move(keys);
}

BuiltInClassMethodInfo EagleDict::GetMethod(const eagle::TokenPtr &method_name) {
    auto method_found = built_in_methods.find(method_name->text);
    if (method_found != built_in_methods.end()) {
        return method_found->second;
    } else {
        throw RuntimeError(method_name->line, "Dict has no method named " + method_name->text);
    }
}

std::string EagleDict::GetBuiltInClassInfo() {
    return "built-in class dict methods:\n"

           "size()->Number\n"
           "\tusage: return the size of dict\n"

           "clear()->Null\n"
           "\tusage: clear dict\n"

           "get_value(key: Object)->Object\n"
           "\tusage: return the value for key if key is in the dict, else Null\n"

           "put(key: Object, new_value: Object)->Null\n"
           "\tusage: update the value for key to new_value if key is in the dict, else insert "
           "(key, new_value) in dict\n"

           "contains_key(key: Object)->Boolean\n"
           "\tusage: return whether key is in dict\n"

           "contains_value(value: Object)->Boolean\n"
           "\tusage: return whether value has appeared in dict\n"

           "remove(key: Object)->Null\n"
           "\tusage: remove the (key, value) pair if key is in dict, else do nothing\n"

           "keys()->Tuple\n"
           "\tusage: return a Tuple object providing a view on keys of dict\n"

           "values()->Tuple\n"
           "\tusage: return a Tuple object providing a view on values of dict\n"

           "items()->Tuple\n"
           "\tusage: return a Tuple object providing a view on items of dict, and each item "
           "is a Tuple object consisting of (key, value) pair";
}

std::string EagleDict::toString() {
    std::string str = "{";
    bool first_entry = true;
    for (auto &node : elements) {
        for (auto &entry : node.second) {
            if (first_entry) {
                first_entry = false;
            } else {
                str += ", ";
            }
            str += (entry.first->toString() + ": " + entry.second->toString());
        }
    }
    str += "}";
    return std::move(str);
}

bool EagleDict::equals(eagle::ObjectPtr other) {
    if (other.get() == this) {
        return true;
    }
    if (!InstanceOf<EagleDict>(other)) {
        return false;
    }
    EagleDictPtr another = cast<EagleDict>(other);
    if (element_cnt != another->element_cnt) {
        return false;
    }
    for (auto &node : elements) {
        for (auto &entry : node.second) {
            auto &key = entry.first;
            auto &value = entry.second;
            if (!another->contains(key)) {
                return false;
            }
            if (!value->equals(another->get(key, 0))) {
                return false;
            }
        }
    }
    return true;
}

size_t EagleDict::hashcode() {
    size_t h = 0;
    for (auto &node : elements) {
        for (auto &entry : node.second) {
            auto &key = entry.first;
            auto &value = entry.second;
            h += (key->hashcode() ^ value->hashcode());
        }
    }
    return h;
}

bool EagleDict::isTruthy() {
    return element_cnt > 0;
}

void EagleDict::InsertDictEntry(size_t h, const eagle::ObjectPtr &key, eagle::ObjectPtr value) {
    if (elements.find(h) == elements.end()) {
        elements[h] = std::vector<EagleDictEntry>{};
    }
    for (auto &entry : elements[h]) {
        if (key->equals(entry.first)) {
            entry.second = std::move(value);
            return;
        }
    }
    elements[h].emplace_back(EagleDictEntry{key, std::move(value)});
    element_cnt++;
}

bool EagleDict::contains(const eagle::ObjectPtr &key) {
    size_t h = key->hashcode();
    auto found = elements.find(h);
    if (found == elements.end()) {
        return false;
    }
    return std::any_of(found->second.begin(), found->second.end(),
                       [&](const EagleDictEntry &entry) { return key->equals(entry.first); });
}

ObjectPtr EagleDict::size(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "size");
    return std::make_shared<Number>(dict->element_cnt);
}

ObjectPtr EagleDict::clear(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "clear");
    dict->elements.clear();
    dict->element_cnt = 0;
    return std::make_shared<Null>();
}

ObjectPtr EagleDict::get_value(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                               int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "get");
    try {
        ObjectPtr value = dict->get(args[0], line);
        return value;
    } catch (EagleRuntimeError &error) {
        return std::make_shared<Null>();
    }
}

ObjectPtr EagleDict::put(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "put");
    dict->set(args[0], args[1], line);
    return std::make_shared<Null>();
}

ObjectPtr EagleDict::contains_key(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                                  int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "contains_key");
    return std::make_shared<Boolean>(dict->contains(args[0]));
}

ObjectPtr EagleDict::contains_value(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args,
                                    int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "contains_value");
    for (auto &node : dict->elements) {
        for (auto &entry : node.second) {
            if (entry.second->equals(args[0])) {
                return std::make_shared<Boolean>(true);
            }
        }
    }
    return std::make_shared<Boolean>(false);
}

ObjectPtr EagleDict::remove(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "remove");
    ObjectPtr &key = args[0];
    size_t h = key->hashcode();
    if (dict->elements.find(h) != dict->elements.end()) {
        auto &list = dict->elements[h];
        for (auto it = list.begin(); it != list.end(); it++) {
            if (key->equals(it->first)) {
                list.erase(it);
                dict->element_cnt--;
                return std::make_shared<Null>();
            }
        }
    }
    return std::make_shared<Null>();
}

ObjectPtr EagleDict::keys(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "keys");
    std::vector<ObjectPtr> ele_keys;
    for (auto &node : dict->elements) {
        for (auto &entry : node.second) {
            ele_keys.emplace_back(entry.first);
        }
    }
    return std::make_shared<EagleTuple>(std::move(ele_keys));
}

ObjectPtr EagleDict::values(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "values");
    std::vector<ObjectPtr> ele_values;
    for (auto &node : dict->elements) {
        for (auto &entry : node.second) {
            ele_values.emplace_back(entry.second);
        }
    }
    return std::make_shared<EagleTuple>(std::move(ele_values));
}

ObjectPtr EagleDict::items(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "items");
    std::vector<ObjectPtr> ele_items;
    for (auto &node : dict->elements) {
        for (auto &entry : node.second) {
            ele_items.emplace_back(
                std::make_shared<EagleTuple>(std::vector<ObjectPtr>{entry.first, entry.second}));
        }
    }
    return std::make_shared<EagleTuple>(std::move(ele_items));
}

EagleDictPtr EagleDict::CheckBuiltInClassType(const BuiltInClassPtr &instance, int line,
                                              const std::string &method_name) {
    if (!InstanceOf<EagleDict>(instance)) {
        throw RuntimeError(line, "Expect callee type: Dict, for method: " + method_name + ".");
    }
    return cast<EagleDict>(instance);
}

}  // namespace eagle
