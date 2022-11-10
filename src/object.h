//
// Created by Akman on 2022/11/10.
//

#pragma once
#include <memory>

namespace eagle {

class Object {
public:
    virtual void ObjectEmptyFunction() {}
};

template <class V>
V* cast(Object* o) {
    V* v = dynamic_cast<V*>(o);
    return v;
}

template <class V>
std::shared_ptr<V> cast(const std::shared_ptr<Object>& o) {
    std::shared_ptr<V> v = std::dynamic_pointer_cast<V>(o);
    return v;
}

template <class V>
bool InstanceOf(Object* o) {
    V* v = dynamic_cast<V*>(o);
    return (v != nullptr);
}

template <class V>
bool InstanceOf(const std::shared_ptr<Object>& o) {
    std::shared_ptr<V> v = std::dynamic_pointer_cast<V>(o);
    return (v != nullptr);
}

class String : public Object {
public:
    String() : str() {}

    String(std::string str) : str(std::move(str)) {}

    String(const char *str) : str(str) {}

    std::string str;

    std::string* operator -> () {
        return &str;
    }
};

}  // namespace eagle
