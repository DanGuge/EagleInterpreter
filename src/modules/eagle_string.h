//
// Created by Akman on 2022/11/21.
//

#pragma once
#include <string>
#include <unordered_map>

#include "BigFloat.h"
#include "eagle_built_in_class.h"
#include "eagle_exceptions.h"
#include "object.h"

namespace eagle {

class String;
using StringPtr = std::shared_ptr<String>;

class String : public BuiltInClass {
public:
    static ObjectPtr size(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr empty(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr char_at(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr count(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr find(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr upper(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr lower(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr split(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

    static ObjectPtr replace(BuiltInClassPtr instance, std::vector<ObjectPtr>& arguments);

private:
    static StringPtr CheckBuiltInClassType(BuiltInClassPtr instance);

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

    BuiltInClassMethodInfo GetMethod(const std::string& method_name) override;

    friend std::ostream& operator<<(std::ostream& out, String& s);
};

}  // namespace eagle
