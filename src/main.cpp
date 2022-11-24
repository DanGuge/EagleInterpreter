#include <fstream>

#include "eagle.h"
#include "util/error_reporter.h"

using namespace eagle;

int main() {
    Eagle eagle;
    std::ifstream if_stream;
    if_stream.open("../test.eagle");
    std::ostringstream tmp;
    tmp << if_stream.rdbuf();
    std::string source = tmp.str();  // test source

    eagle.run(source);
    ErrorReporter::getInstance().printErrors();

    if_stream.close();
    return 0;
}