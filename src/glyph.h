// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <cstddef>
#include <cstdint>

class Glyph {
    public:
        Glyph(char* bitmap, const uint32_t height, const uint32_t width);
        Glyph(const Glyph& glyph);
        Glyph& operator=(const Glyph& glyph);
        ~Glyph();
        uint32_t getHeight() const;
        uint32_t getWidth() const;
        const unsigned char* getBitmap() const;
        bool getBit(const size_t x, const size_t y) const;

        // Editing
        void setBit(const size_t x, const size_t y, const bool bit);
        void fill(size_t x1, size_t y1, size_t x2, size_t y2, bool bit);
        void clear(bool bit);
        void drawLine(size_t x1, size_t y1, size_t x2, size_t y2, bool bit);

    private:
        unsigned char* m_bitmap;
        const uint32_t m_height, m_width;

        void plotLine(size_t x1, size_t y1, size_t x2, size_t y2, bool high, bool bit);
};

