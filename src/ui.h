// SPDX-License-Identifier: GPL-3.0-only

#include <utility>
#include <vector>

enum class Command {
    SHOW,
    EDIT,
    EXIT,
    COMMAND_SIZE,
};

std::pair<Command, unsigned short int> getCommand();
void showGlyph(const std::vector<std::vector<bool>>& glyph);

