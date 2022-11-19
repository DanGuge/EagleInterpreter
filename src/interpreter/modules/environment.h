//
// Created by DanGuge on 2022/11/16.
//

#pragma once

#include <unordered_map>

#include "object.h"
#include "parser/token.h"

namespace eagle {

class Environment;
using EnvironmentPtr = std::shared_ptr<Environment>;

class Environment : public std::enable_shared_from_this<Environment> {
public:
    explicit Environment(EnvironmentPtr enclosing);
    void define(const std::string& name, const ObjectPtr& object);
    ObjectPtr getAt(int distance, const TokenPtr& name);
    ObjectPtr get(const TokenPtr& name);
    void assignAt(int distance, const TokenPtr& name, const ObjectPtr& object);
    void assign(const TokenPtr& name, const ObjectPtr& object);

private:
    EnvironmentPtr ancestor(int distance);
    static void environmentError(const TokenPtr& name, std::string message);

public:
    // the environment encloses this one
    EnvironmentPtr enclosing;

private:
    std::unordered_map<std::string, ObjectPtr> name_object_map;
};

}  // namespace eagle