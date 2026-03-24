// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <cstddef>
#include <cstdint>

class Glyph {
    public:
        Glyph(unsigned char* bitmap, const uint32_t height, const uint32_t width);
        Glyph(const Glyph& glyph);
        ~Glyph();
        bool getBit(const size_t x, const size_t y) const;
        void setBit(const size_t x, const size_t y, const bool bit);
        uint32_t getHeight() const;
        uint32_t getWidth() const;
        const unsigned char* getBitmap() const;
    private:
        unsigned char* m_bitmap;
        const uint32_t m_height, m_width;
};

