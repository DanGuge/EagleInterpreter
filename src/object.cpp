//
// Created by Akman on 2022/11/11.
//

#include "object.h"

namespace eagle {

std::ostream& operator<<(std::ostream& out, Null& n) {
    out << "Nil";
    return out;
}

std::ostream& operator<<(std::ostream& out, Boolean& b) {
    out << (b ? "true" : "false");
    return out;
}

}  // namespace eagle
