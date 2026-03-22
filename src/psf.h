// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <vector>
#include <limits>

class Psf {
    public:
        Psf(char* buffer, std::size_t size);
        bool isValid();
        std::vector<std::vector<bool>> getGlyph(unsigned short int code);
        bool setGlyph(unsigned short int code, const std::vector<std::vector<bool>>& glyphVec);

    private:
        char* m_buffer;
        std::size_t m_size;
        uint16_t m_unicodeTable[std::numeric_limits<unsigned short>::max()];

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

        PsfHeader* m_header;

        void parseUnicodeTable();
        unsigned char* getGlyphPointer(unsigned short int code);
};

