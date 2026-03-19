// SPDX-License-Identifier: GPL-3.0-only

#include <stdexcept>

#include "psf.h"

#define PSF_MAGIC 0x864ab572

Psf::Psf(char* buffer, std::size_t size) {
    m_buffer = buffer;
    m_size = size;
    m_header = (PsfHeader*)(buffer);
}

bool Psf::isValid() {
    if (sizeof(PsfHeader) > m_size) {
        return false;
    }
    if (m_header->magic != PSF_MAGIC) {
        return false;
    }
    if (m_header->version != 0) {
        return false;
    }
    if (m_header->flags != 0) {
        // Fonts with unicode table are not supported
        return false;
    }
    return true;
}

std::vector<std::vector<bool>> Psf::getGlyph(unsigned short int code) {
    if (code >= m_header->numglyph) {
        throw std::out_of_range("Code not included in this font!");
    }
    std::vector<std::vector<bool>> glyphVec;
    // calculate glyph pointer
    const unsigned int bytesPerGlyphLine = (m_header->width + 7) / 8;
    unsigned char* glyph = getGlyphPointer(code);
    if (!glyph) {
        throw std::out_of_range("Code outside of this file!");
    }
    for (std::size_t y = 0; y < m_header->height; y++) {
        std::vector<bool> line;
        unsigned char* currentByte = glyph + (y * bytesPerGlyphLine);
        std::uint8_t mask = 1 << 7;
        for (std::size_t x = 0; x < m_header->width; x++) {
            line.push_back(*currentByte & mask);
            mask >>= 1;
            if (mask == 0) {
                mask = 1 << 7;
                currentByte++;
            }
        }
        glyphVec.push_back(line);
    }
    return glyphVec;
}

bool Psf::setGlyph(unsigned short int code, const std::vector<std::vector<bool>>& glyphVec) {
    if (code >= m_header->numglyph) {
        return false;
    }
    unsigned char* glyph = getGlyphPointer(code);
    if (!glyph){
        return false;
    }
    size_t i = 0;
    std::uint8_t mask = 1 << 7;
    for (std::vector<bool> byteVec : glyphVec) {
        for (bool bit : byteVec) {
            if (i < m_header->bytesperglyph) {
                if (bit) {
                    *(glyph + i) |= mask;
                } else {
                    *(glyph + i) &= ~mask;
                }
                mask >>= 1;
                if (mask == 0) {
                    mask = 1 << 7;
                    i++;
                }
            } else {
                break;
            }
        }
    }
    return true;
}

unsigned char* Psf::getGlyphPointer(unsigned short int code) {
    unsigned char* glyph = (unsigned char*)m_buffer + m_header->headersize + code * m_header->bytesperglyph;
    if ((std::size_t)(glyph - (unsigned char*)m_buffer) + m_header->bytesperglyph > m_size) {
        return nullptr;
    }
    return glyph;
}

