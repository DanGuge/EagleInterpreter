#include <fstream>
#include <stack>

#include "eagle.h"
#include "pretty_print/pretty_print.h"
#include "shell/eagle_shell.h"
#include "util/error_reporter.h"
using namespace eagle;

bool checkParen(const std::string& input) {
    std::stack<char> paren_stack;
    for (const auto& c : input) {
        if (c == '(' || c == '[' || c == '{') {
            paren_stack.push(c);
        } else if (!paren_stack.empty()) {
            bool paren_paired = (c == ')' && paren_stack.top() == '(') ||
                                (c == ']' && paren_stack.top() == '[') ||
                                (c == '}' && paren_stack.top() == '{');
            if (paren_paired)
                paren_stack.pop();
        }
    }
    return !paren_stack.empty();
}

std::string getLine(EagleShell& eagle_shell) {
    pretty_print::PrettyPrint::print(pretty_print::Front::Color::YELLOW, ">>> ", false);
    std::string input;
    std::string line = eagle_shell.readLine();
    printf("\n");
    input.append(line);
    if (checkParen(input)) {
        while (true) {
            pretty_print::PrettyPrint::print(pretty_print::Front::Color::BLUE, "... ", false);
            line = eagle_shell.readLine();
            if (line.empty()) {
                printf("\n");
                break;
            }
            input.append("\n" + line);
            printf("\n");
        }
    }
    return std::move(input);
}

int main(int argc, char* argv[]) {
    Eagle eagle;
    if (argc == 1) {
        EagleShell eagle_shell;
        std::string input = getLine(eagle_shell);
        while (input != "quit") {
            eagle.run(input);
            if (ErrorReporter::getInstance().hasError()) {
                ErrorReporter::getInstance().printErrors();
                ErrorReporter::getInstance().reset();
            }
            input = getLine(eagle_shell);
        }
#if (__APPLE__) || (__unix__)
        printf("\n");
#endif
    } else if (argc == 2) {
        std::ifstream if_stream;
        if_stream.open(argv[1]);
        std::ostringstream tmp;
        tmp << if_stream.rdbuf();
        std::string source = tmp.str();  // test source

        eagle.run(source);
        ErrorReporter::getInstance().printErrors();

        if_stream.close();
    } else {
        pretty_print::PrettyPrint::print(
            pretty_print::Front::Color::RED,
            std::vector<pretty_print::DisplayStyle>{pretty_print::BOLD, pretty_print::ITALIC,
                                                    pretty_print::UNDERLINE},
            "WRONG USAGE OF EAGLE");
        pretty_print::PrettyPrint::print(
            pretty_print::Front::Color::GREEN,
            std::vector<pretty_print::DisplayStyle>{pretty_print::BOLD, pretty_print::ITALIC},
            "usage: eagle [eagle_file_name]\n"
            "Help: You Should Run Eagle with 0 or 1 parameter\n"
            "0 parameter: Run EagleShell\n"
            "\texample: ./EagleInterpreter\n"
            "1 parameter: Interpret EagleFile\n"
            "\texample: ./EagleInterpreter ./test.eagle");
    }
    return 0;
}