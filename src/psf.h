// SPDX-License-Identifier: GPL-3.0-only

#include <cstdint>
#include <vector>

class Psf {
    public:
        Psf(char* buffer, std::size_t size);
        bool isValid();
        std::vector<std::vector<bool>> getGlyph(unsigned short int code);

    private:
        char* m_buffer;
        std::size_t m_size;

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
};

