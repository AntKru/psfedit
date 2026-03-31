// SPDX-License-Identifier: GPL-3.0-only

#include "glyph.h"
#include <format>
#include <stdexcept>

Glyph::Glyph(char* bitmap, const uint32_t height, const uint32_t width)
    : m_height(height), m_width(width) {
    m_bitmap = new unsigned char[width * height];
    std::copy(bitmap, bitmap + width * height, m_bitmap);
}

Glyph::Glyph(const Glyph& glyph)
    : m_height(glyph.getHeight()), m_width(glyph.getWidth()) {
    m_bitmap = new unsigned char[m_width * m_height];
    std::copy(glyph.getBitmap(), glyph.getBitmap() + m_width * m_height, m_bitmap);
}

Glyph& Glyph::operator=(const Glyph& glyph) {
    m_bitmap = new unsigned char[m_width * m_height];
    std::copy(glyph.getBitmap(), glyph.getBitmap() + m_width * m_height, m_bitmap);
    return *this;
}

Glyph::~Glyph() {
    delete[] m_bitmap;
}

bool Glyph::getBit(const size_t x, const size_t y) const {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range(std::format("{} {} is not in the bitmap of size {} {}", x, y, m_width, m_height));
    }
    const unsigned int bytesPerGlyphLine = (m_width + 7) / 8;
    // If the font is wider than one byte
    const size_t byteNum = x / 8;
    uint8_t mask = 128 >> (x % 8);
    return m_bitmap[y * bytesPerGlyphLine + byteNum] & mask ;
}

void Glyph::setBit(const size_t x, const size_t y, const bool bit) {
    if (x >= m_width || y >= m_height) {
        throw std::out_of_range(std::format("{} {} is not in the bitmap of size {} {}", x, y, m_width, m_height));
    }
    const size_t byteNum = x / 8;
    uint8_t mask = 128 >> (x % 8);
    if (bit) {
        m_bitmap[y + byteNum] |= mask;
    } else {
        m_bitmap[y + byteNum] &= ~mask;
    }
}

uint32_t Glyph::getHeight() const {
    return m_height;
}

uint32_t Glyph::getWidth() const {
    return m_width;
}

const unsigned char* Glyph::getBitmap() const {
    return m_bitmap;
}

