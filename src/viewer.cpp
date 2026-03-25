// SPDX-License-Identifier: GPL-3.0-only
#include <iostream>

#include "viewer.h"

void Viewer::showGlyph(const Glyph& glyph) {
    std::cout << ' ';
    for (size_t i = 0; i < glyph.getWidth(); i ++) {
        printCoordinate(i);
    }
    std::cout << '\n';
    for (size_t y = 0; y < glyph.getHeight(); y += 2) {
        printCoordinate(y);
        printGlyphLine(glyph, y);
        std::cout << std::endl;
    }
}

void Viewer::showGlyphs(const std::vector<Glyph>& glyphs) {
    if (glyphs.empty()) {
        return;
    }
    for (size_t i = 0; i < glyphs.size(); i++) {
        std::cout << ' ';
        for (size_t i = 0; i < glyphs.begin()->getWidth(); i ++) {
            printCoordinate(i);
        }
    }
    std::cout << '\n';
    for (size_t y = 0; y < glyphs.begin()->getHeight(); y+= 2) {
        for (size_t glyphNumber = 0; glyphNumber < glyphs.size(); glyphNumber++) {
            printCoordinate(y);
            printGlyphLine(glyphs.at(glyphNumber), y);
        }
        std::cout << '#' << std::endl;
    }
}

void Viewer::printCoordinate(size_t coordinate) {
    if (coordinate % 10 == 0) {
        std::cout << "\033[7m";
    }
    std::cout << "\033[32m";
    std::cout << coordinate % 10;
    std::cout << "\033[0m";
}

bool Viewer::printGlyphLine(const Glyph& glyph, size_t lineNumber) {
    if (lineNumber % 2 || lineNumber >= glyph.getHeight()) {
        return false;
    }
    for (size_t x = 0; x < glyph.getWidth(); x++) {
        bool bit = glyph.getBit(x, lineNumber);
        if (bit) {
            std::cout << "\033[7m";
        }
        if (lineNumber + 1 < glyph.getHeight() && glyph.getBit(x, lineNumber + 1) != bit) {
            // bit at y + 1 is different
            std::cout << "▄";
        } else {
            std::cout << ":";
        }
        if (bit) {
            std::cout << "\033[27m";
        }
    }
    return true;
    //for (const bool fg : line) {
    //    if (shrink) {
    //        std::cout << (fg ? "\033[7m:\033[27m" : ":");
    //    } else {
    //        std::cout << (fg ? "\033[7m· \033[27m" : "· ");
    //    }
    //}
    //std::cout << "\033[0m";
}

