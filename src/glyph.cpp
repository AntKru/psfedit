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

uint32_t Glyph::getHeight() const {
    return m_height;
}

uint32_t Glyph::getWidth() const {
    return m_width;
}

const unsigned char* Glyph::getBitmap() const {
    return m_bitmap;
}

bool Glyph::getBit(const size_t x, const size_t y, bool fail) const {
    if (x >= m_width || y >= m_height) {
        if (fail) {
            throw std::out_of_range(std::format("{} {} is not in the bitmap of size {} {}", x, y, m_width, m_height));
        }
        return false;
    }
    const unsigned int bytesPerGlyphLine = (m_width + 7) / 8;
    const size_t byteNum = x / 8;
    uint8_t mask = 128 >> (x % 8);
    return m_bitmap[y * bytesPerGlyphLine + byteNum] & mask ;
}

void Glyph::setBit(const size_t x, const size_t y, const bool bit, bool fail) {
    if (x >= m_width || y >= m_height) {
        if (fail) {
            throw std::out_of_range(std::format("{} {} is not in the bitmap of size {} {}", x, y, m_width, m_height));
        }
        return;
    }
    const unsigned int bytesPerGlyphLine = (m_width + 7) / 8;
    const size_t byteNum = x / 8;
    uint8_t mask = 128 >> (x % 8);
    if (bit) {
        m_bitmap[y * bytesPerGlyphLine + byteNum] |= mask;
    } else {
        m_bitmap[y * bytesPerGlyphLine + byteNum] &= ~mask;
    }
}

void Glyph::fill(size_t x1, size_t y1, size_t x2, size_t y2, bool bit) {
    for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
        for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            setBit(x, y, bit);
        }
    }
}

void Glyph::clear(bool bit) {
    for (size_t y = 0; y < getHeight(); y++) {
        for (size_t x = 0; x < getWidth(); x++) {
            setBit(x, y, bit);
        }
    }
}

void Glyph::drawLine(size_t x1, size_t y1, size_t x2, size_t y2, bool bit) {
    if (std::labs(static_cast<long>(y2) - static_cast<long>(y1)) < std::labs(static_cast<long>(x2) - static_cast<long>(x1))) {
        if (x1 > x2) {
            plotLine(x2, y2, x1, y1, false, bit);
        } else {
            plotLine(x1, y1, x2, y2, false, bit);
        }
    } else {
        if (y1 > y2) {
            plotLine(x2, y2, x1, y1, true, bit);
        } else {
            plotLine(x1, y1, x2, y2, true, bit);
        }
    }
}

void Glyph::drawCircle(size_t x, size_t y, size_t radius, bool bit) {
    int t1 = radius / 16;
    int pointX = radius;
    int pointY = 0;
    while (pointY <= pointX) {
        setBit(x + pointX, y + pointY, true, false);
        setBit(x - pointX, y - pointY, true, false);
        setBit(x - pointX, y + pointY, true, false);
        setBit(x + pointX, y - pointY, true, false);
        setBit(x + pointY, y + pointX, true, false);
        setBit(x - pointY, y - pointX, true, false);
        setBit(x - pointY, y + pointX, true, false);
        setBit(x + pointY, y - pointX, true, false);
        pointY++;
        t1 += pointY;
        int t2 = t1 - pointX;
        if (t2 >= 0) {
            t1 = t2;
            pointX--;
        }
    }
}

void Glyph::plotLine(size_t x1, size_t y1, size_t x2, size_t y2, bool high, bool bit) {
    long d1 = high ? y2 - y1 : x2 - x1;
    long d2 = high ? x2 - x1 : y2 - y1;
    short i = 1;
    if (d2 < 0) {
        i = -1;
        d2 = -d2;
    }
    long D = 2 * d2 - d1;
    size_t val1 = high ? x1 : y1;
    for (size_t val2 = high ? y1 : x1; val2 <= (high ? y2 : x2); val2++) {
        if (high) {
            setBit(val1, val2, bit);
        } else {
            setBit(val2, val1, bit);
        }
        if (D > 0) {
            val1 += i;
            D += 2 * (d2 - d1);
        } else {
            D += 2 * d2;
        }
    }
}

