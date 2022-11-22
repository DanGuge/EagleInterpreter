//
// Created by Akman on 2022/11/21.
//

#include "eagle_string.h"

namespace eagle {

const std::unordered_map<std::string, BuiltInClassMethodInfo> String::built_in_methods = {
    {"size", {size, 0}},   {"empty", {empty, 0}}, {"char_at", {char_at, 1}},
    {"count", {find, 1}},  {"find", {find, 1}},   {"upper", {upper, 0}},
    {"lower", {lower, 0}}, {"split", {split, 1}}, {"replace", {replace, 2}}};

BuiltInClassMethodInfo String::GetMethod(const std::string& method_name) {
    auto method_found = built_in_methods.find(method_name);
    if (method_found != built_in_methods.end()) {
        return method_found->second;
    } else {
        throw EagleRuntimeError{"String has no method named " + method_name};
    }
}

ObjectPtr String::size(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    return std::make_shared<BigFloat>((int)str->str.size());
}

ObjectPtr String::empty(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    return std::make_shared<Boolean>(str->str.empty());
}

ObjectPtr String::char_at(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    if (!InstanceOf<BigFloat>(arguments[0])) {
        throw EagleRuntimeError{"parameter of method char_at must be integer"};
    }
    std::shared_ptr<BigFloat> index = cast<BigFloat>(arguments[0]);
    if (index->Decimals() != 0) {
        throw EagleRuntimeError{"parameter of method char_at must be integer, not float"};
    }
    int index_ = index->ToInt(), str_size = (int)str->str.size();
    if (index_ < -str_size || index_ >= str_size) {
        throw EagleRuntimeError{"string index out of range"};
    }
    if (index_ < 0) {
        index_ += str_size;
    }
    return std::make_shared<String>(str->str[index_]);
}

ObjectPtr String::count(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    if (!InstanceOf<String>(arguments[0])) {
        throw EagleRuntimeError{"parameter of method count must be string"};
    }
    StringPtr substr = cast<String>(arguments[0]);
    size_t pos = 0;
    int found_count = 0;
    auto found = str->str.find(substr->str, pos);
    while (found != std::string::npos) {
        found_count++;
        pos = found + substr->str.size();
        found = str->str.find(substr->str, pos);
    }
    return std::make_shared<BigFloat>(found_count);
}

ObjectPtr String::find(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    if (!InstanceOf<String>(arguments[0])) {
        throw EagleRuntimeError{"parameter of method find must be string"};
    }
    StringPtr substr = cast<String>(arguments[0]);
    auto found = str->str.find(substr->str);
    int position = found == std::string::npos ? -1 : static_cast<int>(found);
    return std::make_shared<BigFloat>(position);
}

ObjectPtr String::upper(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    std::string new_str = str->str;
    for (char& ch : new_str) {
        ch = (char)std::toupper(ch);
    }
    return std::make_shared<String>(new_str);
}

ObjectPtr String::lower(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    std::string new_str = str->str;
    for (char& ch : new_str) {
        ch = (char)std::tolower(ch);
    }
    return std::make_shared<String>(new_str);
}

ObjectPtr String::split(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    // TODO: Need to implement list first
    return nullptr;
}

ObjectPtr String::replace(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments) {
    StringPtr str = CheckBuiltInClassType(instance);
    if (!InstanceOf<String>(arguments[0]) || !InstanceOf<String>(arguments[1])) {
        throw EagleRuntimeError{"parameters of method replace must be string"};
    }
    StringPtr target_str = cast<String>(arguments[0]);
    StringPtr new_str = cast<String>(arguments[1]);

    std::string result_str = str->str;
    size_t pos = 0;
    auto found = result_str.find(target_str->str, pos);
    while (found != std::string::npos) {
        result_str.replace(found, target_str->str.size(), new_str->str);
        pos = found + new_str->str.size();
        found = result_str.find(target_str->str, pos);
    }
    return std::make_shared<String>(result_str);
}

StringPtr String::CheckBuiltInClassType(BuiltInClassPtr instance) {
    if (!InstanceOf<String>(instance)) {
        throw EagleRuntimeError{"expected callee type String for method size."};
    }
    return cast<String>(instance);
}

std::string String::toString() {
    return str;
}

bool String::equals(ObjectPtr other) {
    if (!InstanceOf<String>(other)) {
        return false;
    }
    return str == cast<String>(other)->str;
}

size_t String::hashcode() {
    return std::hash<std::string>()(str);
}

bool String::isTruthy() {
    return !str.empty();
}

std::ostream& operator<<(std::ostream& out, String& s) {
    out << s.str;
    return out;
}

}  // namespace eagle
