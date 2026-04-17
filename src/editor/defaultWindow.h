// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <optional>
#include <panel.h>

#include "glyph.h"
#include "window.h"

class DefaultWindow : public Window {
    public:
        DefaultWindow() = default;
        ~DefaultWindow() = default;

        void update() override;
        void handleKey(int key) override;

        void setGlyph(const Glyph& glyph);

    private:
        bool m_highlight = false;
        size_t m_cursorY = 0, m_cursorX = 0;
        size_t m_winRootY = 0, m_winRootX = 0;
        std::optional<Glyph> m_glyph = {};
};

