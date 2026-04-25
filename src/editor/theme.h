// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <string>
#include <unordered_map>

class Theme {
    public:
        Theme() = default;
        Theme(std::string name);
        static bool setTheme(const Theme& theme);
        static void resetTheme();

        enum ColorPairs {
            C_RESERVED,
            C_PRIMARY,
            C_SECONDARY,
            C_SECONDARY_IMPORTANT,
            C_TERTIARY,
            C_TERTIARY_CURSOR,
            C_TERTIARY_MARKER,
            ColorPairs_SIZE,
        };

    private:
        static Theme m_defaultTheme;
        static short currentColorIndex;
        std::unordered_map<ColorPairs, std::pair<short, short>> colors;
};

