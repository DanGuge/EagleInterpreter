//
// Created by Akman on 2022/11/10.
//

#pragma once
#include <iostream>
#include <memory>

namespace eagle {

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
public:
    virtual void ObjectEmptyFunction() {}

    virtual size_t hashcode() {
        return reinterpret_cast<size_t>(this);
    }
};

template <class V>
V* cast(Object* o) {
    V* v = dynamic_cast<V*>(o);
    return v;
}

template <class V>
std::shared_ptr<V> cast(const ObjectPtr& o) {
    std::shared_ptr<V> v = std::dynamic_pointer_cast<V>(o);
    return v;
}

template <class V>
bool InstanceOf(Object* o) {
    V* v = dynamic_cast<V*>(o);
    return (v != nullptr);
}

template <class V>
bool InstanceOf(const ObjectPtr& o) {
    std::shared_ptr<V> v = std::dynamic_pointer_cast<V>(o);
    return (v != nullptr);
}

class Null : public Object {
public:
    Null() = default;

    friend std::ostream& operator<<(std::ostream& out, Null& n);

    std::string ToString() const {
        return "Nil";
    }
};

class Boolean : public Object {
public:
    bool value;
    Boolean() : value(false) {}

    Boolean(bool value) : value(value) {}

    explicit operator bool() const {
        return value;
    }

    std::string ToString() const {
        return value ? "true" : "false";
    }

    friend std::ostream& operator<<(std::ostream& out, Boolean& b);
};

}  // namespace eagle
