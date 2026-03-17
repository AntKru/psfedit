// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <filesystem>
#include <fstream>

#include "psf.h"

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
    std::ifstream readFile(filePath, std::ifstream::ate | std::ifstream::binary);
    if (!readFile.is_open()) {
        std::println(stderr, "Could not open file {}", filePath.string());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

