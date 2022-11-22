#include "eagle.h"
#include "util/error_reporter.h"

using namespace eagle;

int main() {
    Eagle eagle;
    std::string source = "class A {\n"
                         "  def init() {\n"
                         "      this.b = 1;\n"
                         "  }\n"
                         "  def printB() {\n"
                         "      print this.b;\n"
                         "  }\n"
                         "}\n"
                         "A().printB();\n"
                         "class B extends A {"
                         "  var b = 2;\n"
                         "};\n"
                         "B().printB();\n";  // test source
    eagle.run(source);
    ErrorReporter::getInstance().printErrors();
    return 0;
}