// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <string>
#include <map>
#include <unordered_map>

class Theme {
    public:
        static bool setTheme(std::string theme);

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

        using Colorscheme = std::unordered_map<ColorPairs, std::pair<short, short>>;

        static std::map<std::string, Colorscheme> colorschemes;
};

