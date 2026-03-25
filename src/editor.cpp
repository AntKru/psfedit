// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

#include "editor.h"

Glyph Editor::editGlyph(const Glyph& glyph) {
    rl_attempted_completion_function = Editor::editorCompletion;
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
        delete line;
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

char** Editor::editorCompletion(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, Editor::editorCompletionGenerator);
}

char* Editor::editorCompletionGenerator(const char* text, int state) {
    return nullptr;
}

