// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

#include "editor.h"

Glyph Editor::editGlyph(Glyph glyph) {
    rl_attempted_completion_function = Editor::editorCompletion;
    Glyph newGlyph(glyph);
    std::vector<Glyph> editorHistory;
    bool preview = true;
    bool modified = false;
    while (true) {
        if (preview) {
            Viewer::showGlyphs({glyph, newGlyph});
        }
        preview = true;
        char* line = readline(std::format("\033[36mEditing mode>{}\033[0m ", modified ? "\033[31m*" : "").c_str());
        if (line == nullptr) {
            // do not save changes
            return glyph;
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
        } else if (command == "e" || command == "exit") {
            return glyph;
        } else if (command == "s" || command == "set"
            || command == "us" || command == "unset") {
            editorHistory.push_back(newGlyph);
            modified = true;
            std::string xs, ys;
            lineStream >> xs >> ys;
            size_t x, y;
            try {
                x = stoi(xs);
                y = stoi(ys);
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
        } else if (command == "f" || command == "fill") {
            std::string x1s, y1s, x2s, y2s;
            lineStream >> x1s >> y1s >> x2s >> y2s;
            size_t x1, y1, x2, y2;
            try {
                x1 = stoi(x1s);
                y1 = stoi(y1s);
                x2 = stoi(x2s);
                y2 = stoi(y2s);
                modified = true;
                fillGlyph(newGlyph, x1, y1, x2, y2);
            } catch (const std::exception& e) {
                std::println("Invalid coordinates: {}, {}, {}, {}", x1s, y1s, x2s, y2s);
                preview = false;
            }
        } else if (command == "c" || command == "clear") {
            modified = true;
            clearGlyph(newGlyph);
        } else if (command == "u" || command == "undo") {
            modified = true;
            if (editorHistory.empty()) {
                newGlyph = glyph;
                std::println("Oldest change");
            } else {
                newGlyph = editorHistory.back();
                editorHistory.pop_back();
            }
        } else {
            std::println("Unknown editor command: {}", command);
            preview = false;
        }
    }
}

void Editor::fillGlyph(Glyph& glyph, size_t x1, size_t y1, size_t x2, size_t y2) {
    for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
        for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            glyph.setBit(x, y, true);
        }
    }
}

void Editor::clearGlyph(Glyph& glyph) {
    for (size_t y = 0; y < glyph.getHeight(); y++) {
        for (size_t x = 0; x < glyph.getWidth(); x++) {
            glyph.setBit(x, y, false);
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
    {"exit", "e, exit: leave editing mode witout saving"},
    {"set", "s, set [x] [y]: set pixel"},
    {"unset", "us, unset [x] [y]: unset pixel"},
    {"fill", "f, fill [x1] [y1] [x2] [y2]: fill area"},
    {"clear", "c, clear: clear glyph"},
    {"undo", "u, undo: undo last change"},
};

