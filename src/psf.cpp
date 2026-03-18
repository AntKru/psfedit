// SPDX-License-Identifier: GPL-3.0-only

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
    std::vector<std::vector<bool>> glyphVec;
    // calculate glyph pointer
    unsigned char* glyph = (unsigned char*)m_buffer + m_header->headersize + code * m_header->bytesperglyph;
    unsigned int bytesPerGlyphLine = (m_header->width + 7) / 8;
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

