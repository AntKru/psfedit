// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

#include "editor.h"

std::optional<Glyph> Editor::editGlyph(Glyph glyph) {
    static bool firstRun = true;
    rl_attempted_completion_function = Editor::editorCompletion;
    Glyph newGlyph(glyph);
    std::vector<Glyph> editorHistory;
    bool preview = true;
    bool modified = false;
    bool erase = false;
    while (true) {
        if (preview) {
            Viewer::showGlyphs({glyph, newGlyph});
        }
        preview = true;
        if (firstRun) {
            std::println("Hint: Since you are in editing mode, the list of available commands has changed. You can use \"help\" to see the now available commands.");
            firstRun = false;
        }
        char* line = readline(std::format("\033[34m(EDITING MODE{}){}\033[0m ",
                    erase ? " [e]" : "",
                    modified ? "\033[31m*" : "").c_str());
        if (line == nullptr) {
            // do not save changes
            return {};
        }
        if (*line) {
            add_history(line);
        }
        std::istringstream lineStream(line);
        delete line;
        std::string command;
        lineStream >> command;
        if (command == "h" || command == "help") {
            for (const auto& [command, helpMessage] : editorCommands) {
                std::println("{}", helpMessage);
            }
            preview = false;
        } else if (command == "w" || command == "write") {
            glyph = newGlyph;
            modified = false;
        } else if (command == "q" || command == "quit") {
            if (editorHistory.empty()) {
                return {};
            } else if (modified) {
                std::println("You have unsaved changes, use \"{}!\" to quit anyway", command);
                preview = false;
            } else {
                return glyph;
            }
        } else if (command == "q!" || command == "quit!") {
            return {};
        } else if (command == "wq") {
            return newGlyph;
        } else if (command == "s" || command == "set"
            || command == "us" || command == "unset") {
            std::string xs, ys;
            lineStream >> xs >> ys;
            size_t x, y;
            try {
                x = stoi(xs);
                y = stoi(ys);
                editorHistory.push_back(newGlyph);
                modified = true;
                try {
                    newGlyph.setBit(x, y, (command == "s" || command == "set"));
                } catch (const std::out_of_range& e) {
                    std::println("Could not set pixel: {}", e.what());
                    preview = false;
                }
            } catch (const std::exception& e) {
                std::println("Invalid coordinates: {}, {}", xs, ys);
                preview = false;
            }
        } else if (command == "f" || command == "fill"
            || command == "l" || command == "line") {
            std::string x1s, y1s, x2s, y2s;
            lineStream >> x1s >> y1s >> x2s >> y2s;
            size_t x1, y1, x2, y2;
            try {
                x1 = stoi(x1s);
                y1 = stoi(y1s);
                x2 = stoi(x2s);
                y2 = stoi(y2s);
                editorHistory.push_back(newGlyph);
                modified = true;
                if (command == "f" || command == "fill") {
                    fillGlyph(newGlyph, x1, y1, x2, y2, !erase);
                } else {
                    drawLine(newGlyph, x1, y1, x2, y2, !erase);
                }
            } catch (const std::exception& e) {
                std::println("Invalid coordinates: {}, {}, {}, {}", x1s, y1s, x2s, y2s);
                preview = false;
            }
        } else if (command == "c" || command == "clear") {
            modified = true;
            editorHistory.push_back(newGlyph);
            clearGlyph(newGlyph, erase);
        } else if (command == "u" || command == "undo") {
            modified = true;
            if (editorHistory.empty()) {
                newGlyph = glyph;
                std::println("Oldest change");
            } else {
                newGlyph = editorHistory.back();
                editorHistory.pop_back();
            }
        } else if (command == "er" || command == "erase") {
            erase = !erase;
            std::println("Toggled eraser");
            preview = false;
        } else {
            std::println("Unknown editor command: {}", command);
            preview = false;
        }
    }
}

void Editor::fillGlyph(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2, bool bit) {
    for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
        for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            glyph.setBit(x, y, bit);
        }
    }
}

void Editor::clearGlyph(Glyph& glyph, bool bit) {
    for (size_t y = 0; y < glyph.getHeight(); y++) {
        for (size_t x = 0; x < glyph.getWidth(); x++) {
            glyph.setBit(x, y, bit);
        }
    }
}

void Editor::drawLine(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2, bool bit) {
    if (std::labs(static_cast<long>(y2) - static_cast<long>(y1)) < std::labs(static_cast<long>(x2) - static_cast<long>(x1))) {
        if (x1 > x2) {
            plotLine(glyph, x2, y2, x1, y1, false, bit);
        } else {
            plotLine(glyph, x1, y1, x2, y2, false, bit);
        }
    } else {
        if (y1 > y2) {
            plotLine(glyph, x2, y2, x1, y1, true, bit);
        } else {
            plotLine(glyph, x1, y1, x2, y2, true, bit);
        }
    }
}

void Editor::plotLine(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2, bool high, bool bit) {
    long d1 = high ? y2 - y1 : x2 - x1;
    long d2 = high ? x2 - x1 : y2 - y1;
    short i = 1;
    if (d2 < 0) {
        i = -1;
        d2 = -d2;
    }
    long D = 2 * d2 - d1;
    size_t val1 = high ? x1 : y1;
    for (size_t val2 = high ? y1 : x1; val2 <= (high ? y2 : x2); val2++) {
        if (high) {
            glyph.setBit(val1, val2, bit);
        } else {
            glyph.setBit(val2, val1, bit);
        }
        if (D > 0) {
            val1 += i;
            D += 2 * (d2 - d1);
        } else {
            D += 2 * d2;
        }
    }
}

char** Editor::editorCompletion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, Editor::editorCompletionGenerator);
}

char* Editor::editorCompletionGenerator(const char* text, int state) {
    static size_t index = 0;
    static size_t length = 0;
    if (state == 0) {
        index = 0;
        length = strlen(text);
    }
    while (index < Editor::editorCommands.size()) {
        std::string completedText = Editor::editorCommands.at(index++).first;
        if (strncmp(text, completedText.c_str(), length) == 0) {
            return strdup(completedText.c_str());
        }
    }
    return nullptr;
}

const std::vector<std::pair<std::string, std::string>> Editor::editorCommands = {
    {"help", "h, help: show this help"},
    {"write", "w, write: save this glyph"},
    {"quit", "q, quit: leave editing mode witout saving"},
    {"wq", "wq: quit and save"},
    {"set", "s, set [x] [y]: set pixel"},
    {"unset", "us, unset [x] [y]: unset pixel"},
    {"line", "l, line [x1] [y1] [x2] [y2]: draw a line"},
    {"fill", "f, fill [x1] [y1] [x2] [y2]: fill area"},
    {"clear", "c, clear: clear glyph"},
    {"undo", "u, undo: undo last change"},
    {"erase", "er, erase: toggle eraser (inverts line, fill and clear commands)"},
};

