// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <iostream>

#include "ui.h"

void printGlyphLine(const std::vector<bool>& line, bool highlight = false, bool shrink = false);

std::pair<Command, unsigned short int> getCommand() {
    while (true) {
        std::print("type h for help> ");
        std::string command;
        std::cin >> command;
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
            std::cin >> number;
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

void showGlyph(const std::vector<std::vector<bool>>& glyph) {
    const bool shrink = glyph.at(0).size() > 32;
    for (size_t i = 0; i < glyph.size(); i += 1 + shrink) {
        printGlyphLine(glyph.at(i), false, shrink);
        std::cout << std::endl;
    }
}

std::vector<std::vector<bool>> editGlyph(const std::vector<std::vector<bool>>& glyph) {
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
    const bool shrink = glyph.at(0).size() > 32;
    if (shrink) {
        std::println("Warning: this view has a lower resolution than the original, but you can still edit the full resolution.");
        std::println("Please rely on the equal signs.");
    }
    for (size_t i = 0; i < glyph.size(); i++) {
        // Show new and old character
        for (size_t j = 0; j < glyph.size(); j += 1 + shrink) {
            if (!shrink) {
                std::cout << (j == i ? "*" : " ");
                printGlyphLine(glyph.at(j), j == i);
                std::cout << ' ';
                std::cout << (j == i ? "*" : " ");
                printGlyphLine(newGlyph.at(j), j == i);
            } else {
                std::cout << (j == i || j + 1 == i ? "*" : " ");
                printGlyphLine(newGlyph.at(j), j == i || j + 1 == i, shrink);
            }
            std::cout << '\n';
        }

        std::cout << ' ';
        for (size_t j = 0; j < newGlyph.at(i).size(); j++) {
            if (newGlyph.at(i).at(j)) {
                std::cout << "\033[7m";
            }
            std::cout << '=';
            std::cout << "\033[0m";
        }
        std::cout << std::endl;
        std::print(">");
        std::string newLine;
        std::getline(std::cin >> std::ws, newLine);
        for (size_t j = 0; j < newGlyph.at(i).size(); j++) {
            if (j == newLine.size() || newLine.at(j) == 's') {
                std::println("Skipping");
                break;
            }
            if (newLine.at(j) == 'e') {
                return glyph;
            }
            newGlyph.at(i).at(j) = newLine.at(j) == 'x';
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

