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
                         "B().printB();\n"
                         "def getA() {\n"
                         "      var b = B();\n"
                         "      print b.b;\n"
                         "      b.c = 10;\n"
                         "      print b.c;\n"
                         "      b.printB();"
                         "      print b;\n"
                         "      return b;\n"
                         "}\n"
                         "var c = getA;\n"
                         "print c().c;\n"
                         "c();\n"
                         "print A;\n"
                         "print getA;";  // test source
    eagle.run(source);
    ErrorReporter::getInstance().printErrors();
    return 0;
}