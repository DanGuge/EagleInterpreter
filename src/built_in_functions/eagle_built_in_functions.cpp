//
// Created by DanGuge on 2022/11/26.
//

#include "eagle_built_in_functions.h"

#include <fstream>

#include "modules/eagle_class.h"
#include "modules/eagle_stream.h"
#include "modules/eagle_string.h"
#include "pretty_print/pretty_print.h"

namespace eagle::built_in_functions {

int ReadFromFile::arity() {
    return 1;
}

ObjectPtr ReadFromFile::call(std::vector<ObjectPtr>& arguments, int call_line) {
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
    return "<built-in function read_from_file>";
}

std::string ReadFromFile::GetBuiltInFunctionInfo() {
    return "function: read file content from file\n"
           "usage: read_from_file(filename: String)->String";
}

int WriteToFile::arity() {
    return 2;
}

ObjectPtr WriteToFile::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<String>(arguments[0])) {
        std::ofstream of_stream(cast<String>(arguments[0])->str);
        if (InstanceOf<String>(arguments[1])) {
            of_stream << cast<String>(arguments[1])->str;
        } else {
            of_stream << arguments[1]->toString();
        }
        of_stream.close();
        return std::make_shared<Null>();
    } else {
        throw RuntimeError(call_line, "write_to_file() need the first 'String' argument to be a "
                                      "file name and the second argument as file content");
    }
}

std::string WriteToFile::toString() {
    return "<built-in function write_to_file>";
}

std::string WriteToFile::GetBuiltInFunctionInfo() {
    return "function: write content to file\n"
           "write_to_file(filename: String, content: Object)->Null";
}

int Input::arity() {
    return 0;
}

ObjectPtr Input::call(std::vector<ObjectPtr>& arguments, int call_line) {
    std::string input;
    getline(std::cin, input);
    return std::make_shared<String>(input);
}

std::string Input::toString() {
    return "<built-in function input>";
}

std::string Input::GetBuiltInFunctionInfo() {
    return "function: read a line from console\n"
           "usage: input()->String";
}

int Str::arity() {
    return 1;
}

ObjectPtr Str::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<String>(arguments[0])) {
        return arguments[0];
    } else if (InstanceOf<Object>(arguments[0])) {
        return std::make_shared<String>(arguments[0]->toString());
    } else {
        throw RuntimeError(call_line, "str() need one 'Object' argument");
    }
}

std::string Str::toString() {
    return "<built-in function str>";
}

std::string Str::GetBuiltInFunctionInfo() {
    return "function: cast input into String\n"
           "usage: str(variable: Object)->String";
}

int Num::arity() {
    return 1;
}

ObjectPtr Num::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<Number>(arguments[0])) {
        return arguments[0];
    } else if (InstanceOf<String>(arguments[0])) {
        return std::make_shared<Number>(cast<String>(arguments[0])->str);
    } else {
        throw RuntimeError(call_line, "num() need one 'Number' or 'String' argument");
    }
}

std::string Num::toString() {
    return "<built-in function num>";
}

std::string Num::GetBuiltInFunctionInfo() {
    return "function: cast input into Number\n"
           "usage: num(variable: Number/String)->Number";
}

int Help::arity() {
    return 1;
}

ObjectPtr Help::call(std::vector<ObjectPtr>& arguments, int call_line) {
    std::string info;
    if (InstanceOf<BuiltInClass>(arguments[0])) {
        info = cast<BuiltInClass>(arguments[0])->GetBuiltInClassInfo();
    } else if (InstanceOf<EagleStream>(arguments[0])) {
        info = cast<EagleStream>(arguments[0])->GetStreamInfo();
    } else if (InstanceOf<BuiltInFunction>(arguments[0])) {
        info = cast<BuiltInFunction>(arguments[0])->GetBuiltInFunctionInfo();
    } else {
        throw RuntimeError(call_line, "help() need one argument and can only provide information "
                                      "about built-in class & built-in function & stream");
    }
    pretty_print::PrettyPrint::print(pretty_print::Front::CYAN, info);
    return std::make_shared<Null>();
}

std::string Help::toString() {
    return "<built-in function help>";
}

std::string Help::GetBuiltInFunctionInfo() {
    return "function: print the function and usage of built-in class & built-in function && stream\n"
           "usage: help(input: BuiltInClass/BuiltInFunction/Stream)->Null";
}

int Bool::arity() {
    return 1;
}

ObjectPtr Bool::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<Boolean>(arguments[0])) {
        return arguments[0];
    } else {
        return std::make_shared<Boolean>(arguments[0]->isTruthy());
    }
}

std::string Bool::toString() {
    return "<built-in function bool>";
}

std::string Bool::GetBuiltInFunctionInfo() {
    return "function: check input object\n"
           "usage: bool(input: Object)->Boolean";
}

int ClassMethod::arity() {
    return 1;
}

ObjectPtr ClassMethod::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<EagleClass>(arguments[0])) {
        pretty_print::PrettyPrint::print(pretty_print::Front::CYAN,
                                         cast<EagleClass>(arguments[0])->getLocalMethodInfo());
        return std::make_shared<Null>();
    } else {
        throw RuntimeError(call_line, "class_method() need one 'Class' argument and can only "
                                      "return the methods of user-defined class");
    }
}

std::string ClassMethod::toString() {
    return "<built-in function class_method>";
}

std::string ClassMethod::GetBuiltInFunctionInfo() {
    return "function: print user-defined class method info\n"
           "usage: class_method(input: Class)->Null";
}

int Globals::arity() {
    return 0;
}

ObjectPtr Globals::call(std::vector<ObjectPtr>& arguments, int call_line) {
    return Interpreter::getInstance().getGlobals();
}

std::string Globals::toString() {
    return "<built-in function globals>";
}

std::string Globals::GetBuiltInFunctionInfo() {
    return "function: return a dict of global environment definitions\n"
           "usage: globals()->Dict";
}

int Id::arity() {
    return 1;
}

ObjectPtr Id::call(std::vector<ObjectPtr>& arguments, int call_line) {
    return std::make_shared<Number>(std::to_string(reinterpret_cast<size_t>(arguments[0].get())));
}

std::string Id::toString() {
    return "<built-in function id>";
}

std::string Id::GetBuiltInFunctionInfo() {
    return "function: get the address of object\n"
           "usage: id(object: Object)->Number";
}

int Len::arity() {
    return 1;
}

ObjectPtr Len::call(std::vector<ObjectPtr>& arguments, int call_line) {
    if (InstanceOf<EagleContainer>(arguments[0])) {
        return cast<EagleContainer>(arguments[0])->size();
    } else if (InstanceOf<String>(arguments[0])) {
        return std::make_shared<Number>(std::to_string(cast<String>(arguments[0])->str.size()));
    } else {
        throw RuntimeError(call_line, "len() need 'Container' or 'String' argument");
    }
}

std::string Len::toString() {
    return "<built-in function len>";
}

std::string Len::GetBuiltInFunctionInfo() {
    return "function: get the length of Container or String\n"
           "usage: len(object: Container/String)->Number";
}

}  // namespace eagle::built_in_functions
