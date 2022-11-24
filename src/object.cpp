//
// Created by Akman on 2022/11/11.
//

#include "object.h"

namespace eagle {

std::string Object::toString() {
    return "<object at " + std::to_string(reinterpret_cast<size_t>(this)) + ">";
}

bool Object::equals(ObjectPtr other) {
    return this == other.get();
}

size_t Object::hashcode() {
    return reinterpret_cast<size_t>(this);
}

bool Object::isTruthy() {
    return true;
}

std::string Null::toString() {
    return "Nil";
}

bool Null::equals(ObjectPtr other) {
    return InstanceOf<Null>(other);
}

size_t Null::hashcode() {
    return 0;
}

bool Null::isTruthy() {
    return false;
}

std::ostream& operator<<(std::ostream& out, Null& n) {
    out << "Nil";
    return out;
}

std::string Boolean::toString() {
    return value ? "true" : "false";
}

bool Boolean::equals(ObjectPtr other) {
    if (!InstanceOf<Boolean>(other)) {
        return false;
    }
    return value == cast<Boolean>(other)->value;
}

size_t Boolean::hashcode() {
    return std::hash<bool>()(value);
}

bool Boolean::isTruthy() {
    return value;
}

std::ostream& operator<<(std::ostream& out, Boolean& b) {
    out << (b ? "true" : "false");
    return out;
}

}  // namespace eagle
