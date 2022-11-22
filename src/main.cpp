#include "eagle.h"
#include "util/error_reporter.h"

using namespace eagle;

int main() {
    Eagle eagle;
    std::string source = "print \"sssss\";";  // test source
    eagle.run(source);
    ErrorReporter::getInstance().printErrors();
    return 0;
}