// SPDX-License-Identifier: GPL-3.0-only

#include "psf.h"

#include <vector>
#include <format>
#include <stdexcept>
#include <fstream>

#define PSF_MAGIC 0x864ab572

namespace fs = std::filesystem;

Psf::Psf(char* buffer, std::size_t size) {
    m_buffer = new char[size];
    std::copy(buffer, buffer + size, m_buffer);
    m_size = size;
    m_header = (PsfHeader*)(m_buffer);
    m_unicodeTable.reserve(m_header->numglyph);
    if (m_header->flags != 0) {
        createMapFromTable();
    } else {
        createMap();
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
    createMap();
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
    createMapFromTable();
    return true;
}

Psf::PsfHeader Psf::getHeader() {
    return *m_header;
}

std::unique_ptr<Psf> Psf::loadFromFile(const std::filesystem::path& filePath) {
    if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
        return nullptr;
    }
    // get file size
    std::size_t fileSize = 0;
    std::ifstream readFile(filePath, std::ifstream::binary);
    if (!readFile.is_open()) {
        return nullptr;
    }
    readFile.seekg(0, std::ifstream::end);
    fileSize = readFile.tellg();
    readFile.seekg(0, std::ifstream::beg);

    // read file
    char buffer[fileSize];
    readFile.read(buffer, fileSize);
    readFile.close();

    return std::make_unique<Psf>((char*)buffer, fileSize);
}

std::unique_ptr<Psf> Psf::createNew(uint32_t height, uint32_t width, bool hasUnicodeTable) {
    if (width % 8) {
        return nullptr;
    }
    const uint32_t bytesperglyph = width / 8 * height;
    PsfHeader* header = new PsfHeader;
    header->magic = PSF_MAGIC;
    header->version = 0;
    header->headersize = sizeof(PsfHeader);
    header->flags = hasUnicodeTable;
    header->numglyph = 0;
    header->bytesperglyph = bytesperglyph;
    header->height = height;
    header->width = width;
    return std::make_unique<Psf>((char*)header, sizeof(PsfHeader));
}

void Psf::createMap() {
    m_unicodeTable.clear();
    for (size_t i = 0; i < std::min(m_header->numglyph, 256u); i++) {
        char* glyphStart = m_buffer + m_header->headersize;
        const unsigned char key[] = {static_cast<unsigned char>(i), 0};
        m_unicodeTable[(char*)key] = glyphStart + i * m_header->bytesperglyph;
    }
}

void Psf::createMapFromTable() {
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

