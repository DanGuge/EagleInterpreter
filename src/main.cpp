#include "eagle.h"
#include "util/error_reporter.h"

using namespace eagle;

int main() {
    Eagle eagle;
    std::string source = "var a = 1;";  // test source
    eagle.run(source);
    ErrorReporter::getInstance().printErrors();
    eagle.run(source);
    return 0;
}