// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <filesystem>
#include <fstream>

#include "psf.h"
#include "ui.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::println("Usage: {} [FILE PATH]", argv[0]);
        return EXIT_SUCCESS;
    }
    fs::path filePath = argv[1];
    if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
        std::println(stderr, "File {} does not exist!", filePath.string());
        return EXIT_FAILURE;
    }
    // get file size
    std::size_t fileSize = 0;
    std::ifstream readFile(filePath, std::ifstream::binary);
    if (!readFile.is_open()) {
        std::println(stderr, "Could not open file {}", filePath.string());
        return EXIT_FAILURE;
    }
    readFile.seekg(0, std::ifstream::end);
    fileSize = readFile.tellg();
    readFile.seekg(0, std::ifstream::beg);

    // read file
    char buffer[fileSize];
    readFile.read(buffer, fileSize);

    Psf psf(buffer, fileSize);
    if (!psf.isValid()) {
        std::println(stderr, "Invalid PSF file");
        return EXIT_FAILURE;
    }

    std::pair<Command, unsigned short int> command = {Command::COMMAND_SIZE, 0};
    command = getCommand();
    while (command.first != Command::EXIT) {
        switch (command.first) {
            case Command::SHOW:
                try {
                showGlyph(psf.getGlyph(command.second));
                } catch (std::out_of_range& e) {
                    std::println("Failed to get glyph: {}", e.what());
                }
                break;
            default:
                std::println("Could not handle command {}", static_cast<int>(command.first));
        }
        command = getCommand();
    }

    return EXIT_SUCCESS;
}

