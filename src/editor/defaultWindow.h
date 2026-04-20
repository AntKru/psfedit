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
        std::optional<Glyph> getGlyph();
        bool changed();

    private:
        mmask_t m_oldmask;
        bool m_borders = false;
        bool m_eraser = false;
        int m_vcursorY = 0, m_vcursorX = 0;
        int m_winRootY = 0, m_winRootX = 0;
        int m_marker1Y = 0, m_marker1X = 0;
        int m_marker2Y = 0, m_marker2X = 0;
        std::optional<Glyph> m_glyph = {};
        std::vector<Glyph> m_history;
        std::vector<Glyph> m_redo_history;

        void handleMouse();
        bool areMarkersSet();
        void updateVCursor(int y, int x);
        void updateHistory();
        void undo();
        void redo();
};

