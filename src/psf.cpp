// SPDX-License-Identifier: GPL-3.0-only

#include <stdexcept>
#include <codecvt>
#include <locale>

#include "psf.h"

#define PSF_MAGIC 0x864ab572

Psf::Psf(char* buffer, std::size_t size) {
    m_buffer = new char[size];
    std::copy(buffer, buffer + size, m_buffer);
    m_size = size;
    m_header = (PsfHeader*)(m_buffer);
    for (unsigned short i = 0; i < std::numeric_limits<unsigned short>::max(); i++) {
        m_unicodeTable[i] = i;
    }
    if (m_header->flags != 0) {
        parseUnicodeTable();
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

Glyph Psf::getGlyph(unsigned short int code) {
    if (code >= m_header->numglyph) {
        throw std::out_of_range("Code not included in this font!");
    }
    return Glyph(getGlyphPointer(code), m_header->height, m_header->width);
}

bool Psf::setGlyph(unsigned short int code, const Glyph& glyph) {
    if (code >= m_header->numglyph) {
        return false;
    }
    unsigned char* glyphp = getGlyphPointer(code);
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

bool Psf::addGlyphUnicode(unsigned short int code) {
    if (m_header->flags == 0) {
        return false;
    }
    // TODO
    return true;
}

Psf::PsfHeader Psf::getHeader() {
    return *m_header;
}

void Psf::parseUnicodeTable() {
    uint16_t glyph = 0;
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv_utf8_utf16;
    std::string utf8str;
    for (
        unsigned char* bytePointer = (unsigned char*)m_buffer + m_header->headersize + m_header->numglyph * m_header->bytesperglyph;
        bytePointer < (unsigned char*)m_buffer + m_size;
        bytePointer++
    ) {
        if (*bytePointer == 0xFF) {
            std::u16string utf16str
                = conv_utf8_utf16.from_bytes(utf8str);

            m_unicodeTable[glyph] = utf16str[0];
            glyph++;
            utf8str.clear();
        } else {
            utf8str += *bytePointer;
        }
    }
}

unsigned char* Psf::getGlyphPointer(unsigned short int code) {
    unsigned char* glyph = (unsigned char*)m_buffer + m_header->headersize + m_unicodeTable[code] * m_header->bytesperglyph;
    if ((std::size_t)(glyph - (unsigned char*)m_buffer) + m_header->bytesperglyph > m_size) {
        return nullptr;
    }
    return glyph;
}

