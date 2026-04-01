// SPDX-License-Identifier: GPL-3.0-only

#include "psf.h"

#include <vector>
#include <format>
#include <stdexcept>

#define PSF_MAGIC 0x864ab572

Psf::Psf(char* buffer, std::size_t size) {
    m_buffer = new char[size];
    std::copy(buffer, buffer + size, m_buffer);
    m_size = size;
    m_header = (PsfHeader*)(m_buffer);
    m_unicodeTable.reserve(m_header->numglyph);
    if (m_header->flags != 0) {
        parseUnicodeTable();
    } else {
        for (size_t i = 0; i < std::min(m_header->numglyph, 256u); i++) {
            char* glyphStart = m_buffer + m_header->headersize;
            const unsigned char key[] = {static_cast<unsigned char>(i), 0};
            m_unicodeTable[(char*)key] = glyphStart + i * m_header->bytesperglyph;
        }
    }
}

Psf::~Psf() {
    delete[] m_buffer;
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
    return true;
}

char* Psf::getBuffer() {
    return m_buffer;
}

size_t Psf::getBufferSize() {
    return m_size;
}

std::unordered_map<std::string, char*> Psf::getUnicodeTable() {
    return m_unicodeTable;
}

Glyph Psf::getGlyph(const std::string& code) {
    char* glyphp = getGlyphPointer(code);
    if (!glyphp) {
        throw std::out_of_range(std::format("\"{}\" is not included in this font", code));
    }
    return Glyph(glyphp, m_header->height, m_header->width);
}

bool Psf::setGlyph(const std::string& code, const Glyph& glyph) {
    char* glyphp = getGlyphPointer(code);
    if (!glyphp){
        return false;
    }
    std::copy(glyph.getBitmap(), glyph.getBitmap() + glyph.getHeight() * glyph.getWidth(), glyphp);
    return true;
}

bool Psf::addGlyphNoUnicode() {
    if (m_header->flags != 0) {
        return false;
    }
    const size_t oldBufferSize = m_header->headersize + m_header->bytesperglyph * m_header->numglyph;
    const size_t newBufferSize = oldBufferSize + m_header->bytesperglyph;
    char* newBuffer = new char[newBufferSize];
    std::copy(m_buffer, m_buffer + oldBufferSize, newBuffer);
    delete[] m_buffer;
    m_buffer = newBuffer;
    m_size = newBufferSize;
    m_header = (PsfHeader*)m_buffer;
    m_header->numglyph++;
    return true;
}

bool Psf::addGlyphUnicode(std::string code) {
    if (m_header->flags == 0) {
        return false;
    }
    if (code.size() > 1) {
        code.insert(code.begin(), 1, (char)0xFE);
    }
    code += (char)0xFF;
    const size_t oldSizeWithoutUnicode = m_header->headersize + m_header->numglyph * m_header->bytesperglyph;
    const size_t newSizeWithoutUnicode = oldSizeWithoutUnicode + m_header->bytesperglyph;
    const size_t newBufferSize = m_size + m_header->bytesperglyph + code.size();
    char* newBuffer = new char[newBufferSize];
    std::copy(m_buffer, m_buffer + oldSizeWithoutUnicode, newBuffer);
    std::copy(m_buffer + oldSizeWithoutUnicode, m_buffer + m_size, newBuffer + newSizeWithoutUnicode);
    std::copy(code.c_str(), code.c_str() + code.size(), newBuffer + newBufferSize - code.size());
    delete m_buffer;
    m_buffer = newBuffer;
    m_size = newBufferSize;
    m_header = (PsfHeader*)m_buffer;
    m_header->numglyph++;
    parseUnicodeTable();
    return true;
}

Psf::PsfHeader Psf::getHeader() {
    return *m_header;
}

void Psf::parseUnicodeTable() {
    m_unicodeTable.clear();
    std::vector<std::string> characters;
    char* glyphBufferStart = m_buffer + m_header->headersize;
    char* glyph = glyphBufferStart;
    bool isSeries = false;
    for (
        unsigned char* bytePointer = (unsigned char*)glyphBufferStart + m_header->numglyph * m_header->bytesperglyph;
        bytePointer < (unsigned char*)m_buffer + m_size;
        bytePointer++
    ) {
        if (*bytePointer == 0xFE) {
            characters.push_back("");
            isSeries = true;
        } else if (*bytePointer == 0xFF) {
            for (const std::string& character : characters) {
                m_unicodeTable[character] = glyph;
            }
            characters.clear();
            isSeries = false;
            glyph += m_header->bytesperglyph;
        } else if (!isSeries) {
            if (characters.empty()) {
                characters.push_back("");
            }
            characters.back() = *bytePointer;
            characters.push_back("");
        } else {
            characters.back() += *bytePointer;
        }
    }
}

char* Psf::getGlyphPointer(const std::string& code) {
    auto entry = m_unicodeTable.find(code);
    if (entry == m_unicodeTable.end()) {
        return nullptr;
    }
    if (entry->second + m_header->bytesperglyph > m_buffer + m_size || entry->second < m_buffer + m_header->headersize) {
        return nullptr;
    }
    return entry->second;
}

