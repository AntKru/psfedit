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

