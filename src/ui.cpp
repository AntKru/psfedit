// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <sstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#include "ui.h"

void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);

std::pair<UI::Command, std::string> UI::getCommand() {
    rl_attempted_completion_function = UI::menuCompletion;
    while (true) {
        char* line = readline("\033[36mtype h for help>\033[0m ");
        if (line == nullptr) {
            return {Command::EXIT, ""};
        }
        if (*line) {
            add_history(line);
        }
        std::istringstream lineStream(line);
        delete line;
        std::string command;
        std::string rest;
        lineStream >> command;
        getline(lineStream >> std::ws, rest);
        if (command == "h" || command == "help") {
            for (const auto& [command, helpMessage] : menuCommands) {
                std::println("{}", helpMessage);
            }
        } else if (command == "w" || command == "write") {
            return {Command::SAVE, rest};
        } else if (command == "e" || command == "exit") {
            return {Command::EXIT, rest};
        } else if (command == "header") {
            return {Command::HEADER, rest};
        } else if (command == "a" || command == "add") {
            return {Command::ADD_GLYPH_NO_UNICODE, rest};
        } else if (command == "s" || command == "show") {
            return {Command::SHOW, rest};
        } else if (command == "m" || command == "modify") {
            return {Command::EDIT, rest};
        } else if (command == "au" || command == "addu") {
            return {Command::ADD_GLYPH_UNICODE, rest};
        } else if (command == "ls" || command == "list") {
            return {Command::LIST, rest};
        } else {
            std::println("Unknown command: {}", command);
        }
    }
}

void UI::showHeader(const Psf::PsfHeader& header) {
    std::println("Version: {}", header.version);
    std::println("Header size: {}", header.headersize);
    std::println("flags: {} ({})", header.flags, header.flags ? "has unicode table" : "no unicode table");
    std::println("number of glyphs: {}", header.numglyph);
    std::println("bytes per glyph: {}", header.bytesperglyph);
    std::println("height: {}", header.height);
    std::println("width: {}", header.width);
}

void UI::showList(const std::unordered_map<std::string, char*> unicodeTable) {
    for (const auto& [key, value] : unicodeTable) {
        std::print("\"{}\" ", key);
    }
    std::println();
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
    {"show", "s, show [character]: show glyph"},
    {"modify", "m, modify [character]: edit glyph interactively"},
    {"header", "header: show font header"},
    {"add", "a, add: add new glyph (no unicode table)"},
    {"addu", "au, addu [character]: add new glyph (font with unicode table)"},
    {"list", "ls, list: List all characters included in this font"},
};

