//
// Created by DanGuge on 2022/11/25.
//

#include "eagle_shell.h"

#ifdef _WIN32
#include <conio.h>
#elif (__APPLE__) || (__unix__)
#include <termios.h>
#include <unistd.h>
#define putch(x) putc((x), stdout)
#endif

namespace eagle {

std::string EagleShellHistory::empty_str;
void EagleShellHistory::reset() {
    index = (int)lines.size();
}

std::string& EagleShellHistory::get() {
    if (0 <= index && index < lines.size()) {
        return lines[index];
    }
    return empty_str;
}

void EagleShellHistory::forward() {
    if (index > 0) {
        index--;
    }
}

void EagleShellHistory::backward() {
    if (index < lines.size()) {
        index++;
    }
}

void EagleShellHistory::append(std::string line) {
    lines.emplace_back(std::move(line));
}

#ifdef _WIN32
std::string EagleShell::readLine() {
    std::string line;
    char c;
    c = getch();
    while (c != '\n' && c != '\r') {
        if (c == -32) {
            c = getch();
            if (c == 72) {
                if (!line.empty()) {
                    moveLeftToEndAndClear((int)line.size());
                }
                history.forward();
                line.clear();
                line = history.get();
                printf("%s", line.c_str());
            } else if (c == 80) {
                if (!line.empty()) {
                    moveLeftToEndAndClear((int)line.size());
                }
                history.backward();
                line.clear();
                line = history.get();
                printf("%s", line.c_str());
            }
            c = getch();
            continue;
        }

        if (c == '\b') {
            if (!line.empty()) {
                line.pop_back();
                moveLeftOneAndClear();
            }
            c = getch();
            continue;
        }

        if (c == '\t') {
            for (int i = 0; i < 4; i++) {
                line += ' ';
                putch(' ');
            }
            c = getch();
            continue;
        }

        line += c;
        putch(c);
        // printf("%d\n", c);

        c = getch();
    }
    if (!line.empty()) {
        history.append(line);
    }
    history.reset();
    return std::move(line);
}
#elif (__APPLE__) || (__unix__)
char getch() {
    int ret;
    struct termios term {
    }, term_old{};
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        printf("Error!");
        return 0;
    }
    term_old = term;
    cfmakeraw(&term);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    ret = fgetc(stdin);
    tcsetattr(STDIN_FILENO, TCSANOW, &term_old);
    return (char)ret;
}

std::string EagleShell::readLine() {
    std::string line;
    char c;
    c = getch();
    while (c != '\n' && c != '\r') {
        if (c == 27) {
            c = getch();
            if (c == '[') {
                c = getch();
                if (c == 'A') {
                    if (!line.empty()) {
                        moveLeftToEndAndClear((int)line.size());
                    }
                    history.forward();
                    line.clear();
                    line = history.get();
                    printf("%s", line.c_str());
                } else if (c == 'B') {
                    if (!line.empty()) {
                        moveLeftToEndAndClear((int)line.size());
                    }
                    history.backward();
                    line.clear();
                    line = history.get();
                    printf("%s", line.c_str());
                }
            }
            c = getch();
            continue;
        }

        if (c == 127) {
            if (!line.empty()) {
                line.pop_back();
                moveLeftOneAndClear();
            }
            c = getch();
            continue;
        }

        if (c == '\t') {
            for (int i = 0; i < 4; i++) {
                line += ' ';
                putch(' ');
            }
            c = getch();
            continue;
        }

        line += c;
        putch(c);

        c = getch();
    }

    if (!line.empty()) {
        history.append(line);
    }
    history.reset();
    return std::move(line);
}
#endif
void EagleShell::moveLeftToEndAndClear(int length) {
    for (int i = 0; i < length; i++) {
        moveLeftOneAndClear();
    }
}

void EagleShell::moveLeftOneAndClear() {
    putch('\b');
    putch(' ');
    putch('\b');
}

}  // namespace eagle