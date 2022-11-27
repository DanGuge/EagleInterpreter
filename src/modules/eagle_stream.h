//
// Created by Akman on 2022/11/25.
//

#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "eagle_callable.h"
#include "eagle_container.h"
#include "interpreter/interpreter.h"
#include "object.h"

namespace eagle {

class EagleStream;
using EagleStreamPtr = std::shared_ptr<EagleStream>;
using StreamOneToOneMethod = ObjectPtr (*)(const ObjectPtr& para, const eagle::ObjectPtr& element,
                                           int line);
using StreamManyToManyMethod = std::vector<ObjectPtr> (*)(const ObjectPtr& para,
                                                          const std::vector<ObjectPtr>& elements,
                                                          int line);
using StreamFinalMethod = ObjectPtr (*)(const ObjectPtr& para,
                                        const std::vector<ObjectPtr>& elements, int line);

class EagleStream : public Object, public std::enable_shared_from_this<EagleStream> {
public:
    EagleStream(EagleContainerPtr init_expr,
                std::vector<std::pair<TokenPtr, ObjectPtr>> operations);

    ObjectPtr run(int line);

    void append(std::pair<TokenPtr, ObjectPtr> operation);

    void execute();

    EagleStreamPtr copy(int line);

    std::string toString() override;

    static std::string GetStreamInfo();

private:
    std::vector<std::pair<int, int>> GetProcessSegments();

private:
    EagleContainerPtr init_expr;
    std::vector<std::pair<TokenPtr, ObjectPtr>> operations;
    bool executed;

public:
    static bool isFinalMethod(const std::string& method_name);
    static bool isOneToOneMethod(const std::string& method_name);
    static bool isManyToManyMethod(const std::string& method_name);

private:
    // non-final methods
    static ObjectPtr map(const ObjectPtr& para, const ObjectPtr& element, int line);
    static ObjectPtr filter(const ObjectPtr& para, const ObjectPtr& element, int line);
    static std::vector<ObjectPtr> limit(const ObjectPtr& para,
                                        const std::vector<ObjectPtr>& elements, int line);

    // final methods
    static ObjectPtr to_list(const ObjectPtr& para, const std::vector<ObjectPtr>& elements,
                             int line);
    static ObjectPtr to_tuple(const ObjectPtr& para, const std::vector<ObjectPtr>& elements,
                              int line);
    static ObjectPtr to_dict(const ObjectPtr& para, const std::vector<ObjectPtr>& elements,
                             int line);
    static ObjectPtr for_each(const ObjectPtr& para, const std::vector<ObjectPtr>& elements,
                              int line);
    static ObjectPtr count(const ObjectPtr& para, const std::vector<ObjectPtr>& elements, int line);

private:
    // non-final methods
    static const std::unordered_map<std::string, StreamOneToOneMethod> one2one_methods;
    static const std::unordered_map<std::string, StreamManyToManyMethod> many2many_methods;
    // final methods
    static const std::unordered_map<std::string, StreamFinalMethod> final_methods;
};

class EagleStreamCall : public EagleCallable {
public:
    EagleStreamCall(EagleStreamPtr stream, TokenPtr func);

    ObjectPtr call(std::vector<ObjectPtr>& arguments, int call_line) override;

    int arity() override {
        return -1;
    }

private:
    EagleStreamPtr stream;
    TokenPtr func;
};

}  // namespace eagle
