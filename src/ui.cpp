// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <sstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#include "ui.h"

void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);

std::pair<UI::Command, unsigned short int> UI::getCommand() {
    rl_attempted_completion_function = UI::menuCompletion;
    while (true) {
        char* line = readline("type h for help> ");
        if (line == nullptr) {
            return {Command::EXIT, 0};
        }
        if (*line) {
            add_history(line);
        }
        std::istringstream lineStream(line);
        delete line;
        std::string command;
        lineStream >> command;
        if (command == "h" || command == "help") {
            for (const auto& [command, helpMessage] : menuCommands) {
                std::println("{}", helpMessage);
            }
        } else if (command == "w" || command == "write") {
            return {Command::SAVE, 0};
        } else if (command == "e" || command == "exit") {
            return {Command::EXIT, 0};
        } else if (command == "s" || command == "m"
                || command == "show" || command == "modify") {
            std::string number;
            lineStream >> number;
            try {
                return {
                    (command == "s" || command == "show") ? Command::SHOW : Command::EDIT,
                    stoi(number)
                };
            } catch (const std::exception& e) {
                std::println(stderr, "Invalid number: {}", number);
            }
        } else if (command == "header") {
            return {Command::HEADER, 0};
        } else {
            std::println("Unknown command: {}", command);
        }
    }
}

void UI::showHeader(Psf::PsfHeader header) {
    std::println("Version: {}", header.version);
    std::println("Header size: {}", header.headersize);
    std::println("flags: {}", header.flags);
    std::println("number of glyphs: {}", header.numglyph);
    std::println("bytes per glyph: {}", header.bytesperglyph);
    std::println("height: {}", header.height);
    std::println("width: {}", header.width);
}

char** UI::menuCompletion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, UI::menuCompletionGenerator);
}

char* UI::menuCompletionGenerator(const char* text, int state) {
    static size_t index = 0;
    static size_t length = 0;
    if (state == 0) {
        index = 0;
        length = strlen(text);
    }
    while (index < UI::menuCommands.size()) {
        std::string completedText = UI::menuCommands.at(index++).first;
        if (strncmp(text, completedText.c_str(), length) == 0) {
            return strdup(completedText.c_str());
        }
    }
    return nullptr;
}

const std::vector<std::pair<std::string, std::string>> UI::menuCommands = {
    {"help", "h, help: show this help"},
    {"write", "w, write: save"},
    {"exit", "e, exit: leave witout saving"},
    {"show", "s, show [code]: show glyph"},
    {"modify", "m, modify [code]: edit glyph interactively"},
    {"header", "header: show font header"},
};

