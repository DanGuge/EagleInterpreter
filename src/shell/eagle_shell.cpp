//
// Created by DanGuge on 2022/11/25.
//

#include "eagle_shell.h"

#ifdef _WIN32
#include <conio.h>

#define PREFIX -32
#define UP     72
#define LEFT   75
#define RIGHT  77
#define DOWN   80

#elif (__APPLE__) || (__unix__)
#include <termios.h>
#include <unistd.h>
#define putch(x) putc((x), stdout)

#define PREFIX      27
#define NEXT_PREFIX '['
#define UP          'A'
#define DOWN        'B'
#define RIGHT       'C'
#define LEFT        'D'

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

bool EagleShellHistory::forward_end() {
    return index == 0;
}

bool EagleShellHistory::backward_end() {
    return index == lines.size();
}

#ifdef _WIN32
std::string EagleShell::readLine() {
    std::string line;
    int pos = 0;
    char c;
    c = getch();
    while (c != '\n' && c != '\r') {
        if (c == PREFIX) {
            c = getch();
            if (c == UP && !history.forward_end()) {
                if (!line.empty()) {
                    moveLeftToEndAndClear((int)line.size());
                }
                history.forward();
                line.clear();
                line = history.get();
                printf("%s", line.c_str());
                pos = (int)line.size();
            } else if (c == DOWN && !history.backward_end()) {
                if (!line.empty()) {
                    moveLeftToEndAndClear((int)line.size());
                }
                history.backward();
                line.clear();
                line = history.get();
                printf("%s", line.c_str());
                pos = (int)line.size();
            } else if (c == LEFT && pos > 0) {
                putch('\b');
                pos--;
            } else if (c == RIGHT && pos < line.size()) {
                putch(line[pos]);
                pos++;
            }
            c = getch();
            continue;
        }

        if (c == '\b') {
            if (!line.empty() && pos > 0) {
                // process variables
                pos--;
                line.erase(pos, 1);
                // process console output
                putch('\b');
                for (int i = pos; i < line.size(); i++) {
                    putch(line[i]);
                }
                putch(' ');
                for (int i = 0; i < line.size() - pos + 1; i++) {
                    putch('\b');
                }
            }
            c = getch();
            continue;
        }
        if (c == '\t') {
            // process variables
            line.insert(pos, 4, ' ');
            pos += 4;
            // process console output
            for (int i = 0; i < 4; i++) {
                putch(' ');
            }
            for (int i = pos; i < line.size(); i++) {
                putch(line[i]);
            }
            for (int i = 0; i < line.size() - pos; i++) {
                putch('\b');
            }

            c = getch();
            continue;
        }

        // process variables
        line.insert(pos, 1, c);
        pos++;
        // process console output
        putch(c);
        for (int i = pos; i < line.size(); i++) {
            putch(line[i]);
        }
        for (int i = 0; i < line.size() - pos; i++) {
            putch('\b');
        }
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
    int pos = 0;
    char c;
    c = getch();
    while (c != '\n' && c != '\r') {
        if (c == PREFIX) {
            c = getch();
            if (c == NEXT_PREFIX) {
                c = getch();
                if (c == UP && !history.forward_end()) {
                    if (!line.empty()) {
                        moveLeftToEndAndClear((int)line.size());
                    }
                    history.forward();
                    line.clear();
                    line = history.get();
                    printf("%s", line.c_str());
                    pos = line.size();
                } else if (c == DOWN && !history.backward_end()) {
                    if (!line.empty()) {
                        moveLeftToEndAndClear((int)line.size());
                    }
                    history.backward();
                    line.clear();
                    line = history.get();
                    printf("%s", line.c_str());
                    pos = line.size();
                } else if (c == LEFT && pos > 0) {
                    putch('\b');
                    pos--;
                } else if (c == RIGHT && pos < line.size()) {
                    putch(line[pos]);
                    pos++;
                }
            }
            c = getch();
            continue;
        }

        if (c == 127) {
            if (!line.empty() && pos > 0) {
                // process variables
                pos--;
                line.erase(pos, 1);
                // process console output
                putch('\b');
                for (int i = pos; i < line.size(); i++) {
                    putch(line[i]);
                }
                putch(' ');
                for (int i = 0; i < line.size() - pos + 1; i++) {
                    putch('\b');
                }
            }
            c = getch();
            continue;
        }

        if (c == '\t') {
            // process variables
            line.insert(pos, 4, ' ');
            pos += 4;
            // process console output
            for (int i = 0; i < 4; i++) {
                putch(' ');
            }
            for (int i = pos; i < line.size(); i++) {
                putch(line[i]);
            }
            for (int i = 0; i < line.size() - pos; i++) {
                putch('\b');
            }

            c = getch();
            continue;
        }

        // process variables
        line.insert(pos, 1, c);
        pos++;
        // process console output
        putch(c);
        for (int i = pos; i < line.size(); i++) {
            putch(line[i]);
        }
        for (int i = 0; i < line.size() - pos; i++) {
            putch('\b');
        }
        // printf("%d\n", c);

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