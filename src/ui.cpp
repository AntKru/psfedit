// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <iostream>
#include <sstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#include "ui.h"

void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);

std::pair<Command, unsigned short int> getCommand() {
    while (true) {
        char* line = readline("type h for help> ");
        if (line == nullptr) {
            return {Command::EXIT, 0};
        }
        if (*line) {
            add_history(line);
        }
        std::istringstream lineStream(line);
        std::string command;
        lineStream >> command;
        if (command == "h") {
            std::println("Available commands:");
            std::println("h: show this help");
            std::println("w: save");
            std::println("e: exit (Does not save automatically!)");
            std::println("s [character code]: show character");
            std::println("m [character code]: modify character");
        } else if (command == "w") {
            return {Command::SAVE, 0};
        } else if (command == "e") {
            return {Command::EXIT, 0};
        } else if (command == "s" || command == "m") {
            std::string number;
            lineStream >> number;
            try {
                return {
                    command == "s" ? Command::SHOW : Command::EDIT,
                    stoi(number)
                };
            } catch (const std::exception& e) {
                std::println(stderr, "Invalid number: {}", number);
            }
        }
    }
}

void showGlyph(const Glyph& glyph) {
    const bool shrink = glyph.getWidth() > 32;
    for (size_t y = 0; y < glyph.getHeight(); y += 1 + shrink) {
        std::vector<bool> line;
        for (size_t x = 0; x < glyph.getWidth(); x++) {
            line.push_back(glyph.getBit(x, y));
        }
        printGlyphLine(line, false, shrink);
        std::cout << std::endl;
    }
}

Glyph editGlyph(const Glyph& glyph) {
    static bool showHelp = true;
    if (showHelp) {
        std::print(
            "The left preview shows the old character, the right one the new character.\n"
            "Enter a string of characters, as long as the equal signs show you.\n"
            "This is the meaning of the characters you enter:\n"
            "s: skip this line\n"
            "e: exit editing mode, without saving changes\n"
            "x: set this pixel\n"
            "everything else: unset this pixel\n"
            "The line marked in green is the one you are currently editing.\n"
        );
        showHelp = false;
    }
    auto newGlyph = glyph;
    const bool shrink = glyph.getWidth() > 32;
    if (shrink) {
        std::println("Warning: this view has a lower resolution than the original, but you can still edit the full resolution.");
        std::println("Please rely on the equal signs.");
    }
    for (size_t i = 0; i < glyph.getHeight(); i++) {
        // Show new and old character
        for (size_t j = 0; j < glyph.getHeight(); j += 1 + shrink) {
            std::vector<bool> oldLine;
            std::vector<bool> newLine;
            for (size_t x = 0; x < glyph.getWidth(); x++) {
                oldLine.push_back(glyph.getBit(x, j));
                newLine.push_back(newGlyph.getBit(x, j));
            }
            if (!shrink) {
                std::cout << (j == i ? "*" : " ");
                printGlyphLine(oldLine, j == i);
                std::cout << ' ';
                std::cout << (j == i ? "*" : " ");
                printGlyphLine(newLine, j == i);
            } else {
                std::cout << (j == i || j + 1 == i ? "*" : " ");
                printGlyphLine(newLine, j == i || j + 1 == i, shrink);
            }
            std::cout << '\n';
        }

        const std::string prompt = "Editing mode> ";
        for (size_t i = 0; i < prompt.size(); i++) {
            std::cout << ' ';
        }
        // Show preview with (=)
        for (size_t j = 0; j < newGlyph.getWidth(); j++) {
            if (newGlyph.getBit(j, i)) {
                std::cout << "\033[7m";
            }
            std::cout << '=';
            std::cout << "\033[0m";
        }
        std::cout << std::endl;
        char* line = readline(prompt.c_str());
        if (line == nullptr) {
            // do not save changes
            return glyph;
        }
        if (*line) {
            add_history(line);
        }
        std::string newLine(line);
        for (size_t j = 0; j < newGlyph.getWidth(); j++) {
            if (j == newLine.size() || newLine.at(j) == 's') {
                std::println("Skipping");
                break;
            }
            if (newLine.at(j) == 'e') {
                // do not save changes
                return glyph;
            }
            newGlyph.setBit(j, i, newLine.at(j) == 'x');
        }
    }
    return newGlyph;
}

void printGlyphLine(const std::vector<bool>& line, bool highlight, bool shrink) {
    if (highlight) {
        std::cout << "\033[32m";
    }
    for (const bool fg : line) {
        if (shrink) {
            std::cout << (fg ? "\033[7m:\033[27m" : ":");
        } else {
            std::cout << (fg ? "\033[7m· \033[27m" : "· ");
        }
    }
    std::cout << "\033[0m";
}

