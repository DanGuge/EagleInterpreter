//
// Created by DanGuge on 2022/11/26.
//

#include "file_io.h"

#include <fstream>

#include "modules/eagle_string.h"

namespace eagle::built_in_functions {
int ReadFromFile::arity() {
    return 1;
}

ObjectPtr ReadFromFile::call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments,
                             int call_line) {
    if (InstanceOf<String>(arguments[0])) {
        std::ifstream if_stream;
        if_stream.open(cast<String>(arguments[0])->str);
        std::ostringstream tmp;
        tmp << if_stream.rdbuf();
        std::string content = tmp.str();
        if_stream.close();
        if (content.empty()) {
            throw RuntimeError(call_line, "read_from_file(): file does not exist or file is empty");
        } else {
            return std::make_shared<String>(tmp.str());
        }
    } else {
        throw RuntimeError(call_line, "read_from_file() need one 'String' argument as filename");
    }
}

std::string ReadFromFile::toString() {
    return "<built in function read_from_file>:\n"
           "usage: read_from_file(filename: String)->String";
}

int WriteToFile::arity() {
    return 2;
}

ObjectPtr WriteToFile::call(Interpreter& interpreter, std::vector<ObjectPtr>& arguments,
                            int call_line) {
    if (InstanceOf<String>(arguments[0])) {
        std::ofstream of_stream(cast<String>(arguments[0])->str);
        of_stream << arguments[1]->toString();
        of_stream.close();
        return std::make_shared<Null>();
    } else {
        throw RuntimeError(call_line, "write_to_file() need the first 'String' argument to be a "
                                      "file name and the second argument as file content");
    }
}

std::string WriteToFile::toString() {
    return "<built in function write_to_file>\n"
           "write_to_file(filename: String, content: Object)->Nil";
}

}  // namespace eagle::built_in_functions