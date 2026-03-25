// SPDX-License-Identifier: GPL-3.0-only
#include <iostream>

#include "viewer.h"

void Viewer::showGlyph(const Glyph& glyph) {
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

void Viewer::showGlyphs(const std::vector<Glyph>& glyphs) {
    if (glyphs.empty()) {
        return;
    }
    const bool shrink = glyphs.begin()->getWidth() > 32;
    for (size_t y = 0; y < glyphs.begin()->getHeight(); y+= 1 + shrink) {
        for (const auto& glyph : glyphs) {
            std::vector<bool> line;
            for (size_t x = 0; x < glyph.getWidth(); x++) {
                line.push_back(glyph.getBit(x, y));
            }
            printGlyphLine(line, false, shrink);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Viewer::printGlyphLine(const std::vector<bool>& line, bool highlight, bool shrink) {
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

