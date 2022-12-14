//
// Created by Akman on 2022/11/21.
//

#pragma once
#include <string>
#include <unordered_map>

#include "eagle_built_in_class.h"
#include "eagle_exceptions.h"
#include "Number.h"
#include "object.h"

namespace eagle {

class String;
using StringPtr = std::shared_ptr<String>;

class String : public BuiltInClass {
public:
    static ObjectPtr size(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                          int line);

    static ObjectPtr empty(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                           int line);

    static ObjectPtr char_at(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                             int line);

    static ObjectPtr count(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                           int line);

    static ObjectPtr find(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                          int line);

    static ObjectPtr upper(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                           int line);

    static ObjectPtr lower(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                           int line);

    static ObjectPtr split(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                           int line);

    static ObjectPtr replace(const BuiltInClassPtr& instance, std::vector<ObjectPtr>& arguments,
                             int line);

private:
    static StringPtr CheckBuiltInClassType(const BuiltInClassPtr& instance, int line,
                                           const std::string& method_name);

private:
    static const std::unordered_map<std::string, BuiltInClassMethodInfo> built_in_methods;

public:
    String() : str() {}

    String(std::string str) : str(std::move(str)) {}

    String(const char* str) : str(str) {}

    String(char c) : str() {
        str += c;
    }

    std::string str;

    std::string* operator->() {
        return &str;
    }

    BuiltInClassMethodInfo GetMethod(const TokenPtr& method_name) override;

    std::string GetBuiltInClassInfo() override;

    std::string toString() override;

    bool equals(ObjectPtr other) override;

    size_t hashcode() override;

    bool isTruthy() override;

    friend std::ostream& operator<<(std::ostream& out, String& s);
};

}  // namespace eagle
