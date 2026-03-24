// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <utility>

#include "glyph.h"

enum class Command {
    SHOW,
    EDIT,
    SAVE,
    EXIT,
    COMMAND_SIZE,
};

std::pair<Command, unsigned short int> getCommand();
void showGlyph(const Glyph& glyph);
Glyph editGlyph(const Glyph& glyph);

