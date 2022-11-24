//
// Created by Akman on 2022/11/23.
//

#include <algorithm>

#include "BigFloat.h"
#include "eagle_container.h"

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
    InsertDictEntry(key->hashcode(), key, std::move(value));
}

BuiltInClassMethodInfo EagleDict::GetMethod(const eagle::TokenPtr &method_name) {
    auto method_found = built_in_methods.find(method_name->text);
    if (method_found != built_in_methods.end()) {
        return method_found->second;
    } else {
        throw RuntimeError(method_name->line, "Dict has no method named " + method_name->text);
    }
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
    // TODO: 参考的java的数组hash算法，待商榷
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
    return std::make_shared<BigFloat>(dict->element_cnt);
}

ObjectPtr EagleDict::clear(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "clear");
    dict->elements.clear();
    return nullptr;
}

ObjectPtr EagleDict::get_value(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "get");
    try {
        ObjectPtr value = dict->get(args[0], line);
        return value;
    } catch (EagleRuntimeError &error) { return std::make_shared<Null>(); }
}

ObjectPtr EagleDict::put(const BuiltInClassPtr &object, std::vector<ObjectPtr> &args, int line) {
    EagleDictPtr dict = CheckBuiltInClassType(object, line, "put");
    dict->set(args[0], args[1], line);
    return nullptr;
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
                return nullptr;
            }
        }
    }
    return nullptr;
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

EagleDictPtr EagleDict::CheckBuiltInClassType(BuiltInClassPtr instance, int line,
                                              const std::string &method_name) {
    if (!InstanceOf<EagleDict>(instance)) {
        throw RuntimeError(line, "Expect callee type: Dict, for method: " + method_name + ".");
    }
    return cast<EagleDict>(instance);
}

}  // namespace eagle
