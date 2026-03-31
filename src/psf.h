// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "glyph.h"

class Psf {
    public:
        Psf(char* buffer, std::size_t size);
        ~Psf();
        bool isValid();
        char* getBuffer();
        size_t getBufferSize();
        std::unordered_map<std::string, char*> getUnicodeTable();
        Glyph getGlyph(const std::string& code);
        bool setGlyph(const std::string& code, const Glyph& glyph);
        bool addGlyphNoUnicode();
        bool addGlyphUnicode(const std::string& code);

        struct PsfHeader {
            uint32_t magic;
            uint32_t version;
            uint32_t headersize;
            uint32_t flags;
            uint32_t numglyph;
            uint32_t bytesperglyph;
            uint32_t height;
            uint32_t width;
        };

        PsfHeader getHeader();

    private:
        char* m_buffer;
        std::size_t m_size;
        std::unordered_map<std::string, char*> m_unicodeTable;

        PsfHeader* m_header;

        void parseUnicodeTable();
        char* getGlyphPointer(const std::string& code);
};

