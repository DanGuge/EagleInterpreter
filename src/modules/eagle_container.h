//
// Created by Akman on 2022/11/23.
//

#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "eagle_built_in_class.h"

namespace eagle {

class EagleContainer;
class EagleList;
class EagleTuple;
class EagleDict;

using EagleContainerPtr = std::shared_ptr<EagleContainer>;
using EagleListPtr = std::shared_ptr<EagleList>;
using EagleTuplePtr = std::shared_ptr<EagleTuple>;
using EagleDictPtr = std::shared_ptr<EagleDict>;
using EagleDictEntry = std::pair<ObjectPtr, ObjectPtr>;

class EagleContainer : public BuiltInClass {
public:
    virtual ObjectPtr get(const ObjectPtr& subscript, int line) = 0;
    virtual void set(const ObjectPtr& subscript, ObjectPtr value, int line) = 0;
    virtual std::vector<ObjectPtr> iterator() = 0;
};

class EagleList : public EagleContainer {
public:
    explicit EagleList(std::vector<ObjectPtr> elements);

    ObjectPtr get(const eagle::ObjectPtr& subscript, int line) override;
    void set(const eagle::ObjectPtr& subscript, eagle::ObjectPtr value, int line) override;
    std::vector<ObjectPtr> iterator() override;

    BuiltInClassMethodInfo GetMethod(const TokenPtr& method_name) override;

    std::string toString() override;
    bool equals(ObjectPtr other) override;
    size_t hashcode() override;
    bool isTruthy() override;

private:
    int CheckSubscript(const ObjectPtr& subscript, int line);

private:
    std::vector<ObjectPtr> elements;

public:
    static ObjectPtr size(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr empty(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr append(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr clear(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr contains(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args,
                              int line);
    static ObjectPtr count(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr insert(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr remove(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr pop(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr reverse(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);

private:
    static EagleListPtr CheckBuiltInClassType(const BuiltInClassPtr& instance, int line,
                                              const std::string& method_name);

private:
    static const std::unordered_map<std::string, BuiltInClassMethodInfo> built_in_methods;
};

class EagleTuple : public EagleContainer {
public:
    explicit EagleTuple(std::vector<ObjectPtr> elements);

    ObjectPtr get(const eagle::ObjectPtr& subscript, int line) override;
    void set(const eagle::ObjectPtr& subscript, eagle::ObjectPtr value, int line) override;
    std::vector<ObjectPtr> iterator() override;

    BuiltInClassMethodInfo GetMethod(const TokenPtr& method_name) override;

    std::string toString() override;
    bool equals(ObjectPtr other) override;
    size_t hashcode() override;
    bool isTruthy() override;

private:
    int CheckSubscript(const ObjectPtr& subscript, int line);

private:
    std::vector<ObjectPtr> elements;

public:
    static ObjectPtr size(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr empty(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr count(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr contains(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args,
                              int line);

private:
    static EagleTuplePtr CheckBuiltInClassType(const BuiltInClassPtr& instance, int line,
                                               const std::string& method_name);

private:
    static const std::unordered_map<std::string, BuiltInClassMethodInfo> built_in_methods;
};

class EagleDict : public EagleContainer {
public:
    explicit EagleDict(const std::vector<EagleDictEntry>& elements);

    ObjectPtr get(const eagle::ObjectPtr& key, int line) override;
    void set(const eagle::ObjectPtr& key, eagle::ObjectPtr value, int line) override;
    std::vector<ObjectPtr> iterator() override;

    BuiltInClassMethodInfo GetMethod(const TokenPtr& method_name) override;

    std::string toString() override;
    bool equals(ObjectPtr other) override;
    size_t hashcode() override;
    bool isTruthy() override;

private:
    void InsertDictEntry(size_t h, const ObjectPtr& key, ObjectPtr value);

    bool contains(const ObjectPtr& key);

private:
    std::unordered_map<size_t, std::vector<EagleDictEntry>> elements;
    int element_cnt;

public:
    static ObjectPtr size(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr clear(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr get_value(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args,
                               int line);
    static ObjectPtr put(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr contains_key(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args,
                                  int line);
    static ObjectPtr contains_value(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args,
                                    int line);
    static ObjectPtr remove(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr keys(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr values(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);
    static ObjectPtr items(const BuiltInClassPtr& object, std::vector<ObjectPtr>& args, int line);

private:
    static EagleDictPtr CheckBuiltInClassType(const BuiltInClassPtr& instance, int line,
                                              const std::string& method_name);

private:
    static const std::unordered_map<std::string, BuiltInClassMethodInfo> built_in_methods;
};

}  // namespace eagle
