// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <cstdint>
#include <limits>

#include "glyph.h"

class Psf {
    public:
        Psf(char* buffer, std::size_t size);
        ~Psf();
        bool isValid();
        char* getBuffer();
        size_t getBufferSize();
        Glyph getGlyph(unsigned short int code);
        bool setGlyph(unsigned short int code, const Glyph& glyph);
        bool addGlyphNoUnicode();
        bool addGlyphUnicode(unsigned short int code);

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
        uint16_t m_unicodeTable[std::numeric_limits<unsigned short>::max()];

        PsfHeader* m_header;

        void parseUnicodeTable();
        unsigned char* getGlyphPointer(unsigned short int code);
};

