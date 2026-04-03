// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <sstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#include "ui.h"

void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);

std::pair<UI::Command, std::string> UI::getCommand(bool saved) {
    rl_attempted_completion_function = UI::menuCompletion;
    static std::vector<std::pair<UI::Command, std::string>> commandQueue;
    if (!commandQueue.empty()) {
        std::pair<UI::Command, std::string> command = commandQueue.front();
        commandQueue.erase(commandQueue.begin());
        return command;
    }
    while (true) {
        char* line = readline(
            std::format("\033[36mtype h for help>{}\033[0m ", saved ? "" : "\033[31m*").c_str());
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
        } else if (command == "q" || command == "quit") {
            if (saved) {
                return {Command::EXIT, rest};
            } else {
                std::println("You have unsaved changes, use \"{}!\" to quit anyway", command);
            }
        } else if (command == "q!" || command == "quit!") {
            return {Command::EXIT, rest};
        } else if (command == "wq") {
            commandQueue.push_back({Command::EXIT, rest});
            return {Command::SAVE, rest};
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
        } else if (command == "ll" || command == "llist") {
            return {Command::LLIST, rest};
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

void UI::showList(const std::unordered_map<std::string, char*> unicodeTable, bool longList) {
    std::vector<std::pair<std::string, char*>> sortedUnicodeTable(unicodeTable.begin(), unicodeTable.end());
    std::sort(sortedUnicodeTable.begin(), sortedUnicodeTable.end(),
        [](std::pair<std::string, char*> a, std::pair<std::string, char*> b) -> bool {
            return a.second < b.second;
        });
    if (longList) {
        for (const auto& [key, value] : sortedUnicodeTable) {
            std::string code;
            for (const unsigned char byte : key) {
                code += std::to_string(byte);
                code += ' ';
            }
            std::println("Glyph: \"{}\", code: {}", key, code);
        }
    } else {
        for (const auto& [key, value] : sortedUnicodeTable) {
            std::print("\"{}\" ", key);
        }
        std::println();
    }
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
    {"quit", "q, quit: leave witout saving"},
    {"wq", "wq: quit and save"},
    {"show", "s, show [character]: show glyph"},
    {"modify", "m, modify [character]: edit glyph interactively"},
    {"header", "header: show font header"},
    {"add", "a, add: add new glyph (no unicode table)"},
    {"addu", "au, addu [character]: add new glyph (font with unicode table)"},
    {"list", "ls, list: list all characters included in this font"},
    {"llist", "ll, llist: ls but with more details"},
};

