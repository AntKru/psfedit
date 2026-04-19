// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "glyph.h"
#include "window.h"

#include <optional>

class OverviewWindow : public Window {
    public:
        OverviewWindow() = default;
        ~OverviewWindow() = default;

        void update() override;
        void handleKey(int key) override;

        void handleMouse();
        void setGlyph(const Glyph& glyph);

    private:
        size_t m_cursorX = 0, m_cursorY = 0;
        std::optional<Glyph> m_glyph = {};

        size_t m_zoom = 2;
};

