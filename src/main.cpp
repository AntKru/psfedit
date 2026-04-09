// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <filesystem>
#include <fstream>

#include "psf.h"
#include "ui.h"
#include "viewer.h"
#include "editor/editor.h"
#include "config.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::println("Usage: {} [OPTIONS] [FILE PATH]", argv[0]);
        return EXIT_SUCCESS;
    }
    std::filesystem::path filePath = argv[argc - 1];
    std::unique_ptr<Psf> psf;
    bool newFile = false;
    bool newFileUnicode = false;
    uint32_t newFileHeight = 12;
    uint32_t newFileWidth = 8;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::println("Usage: {} [OPTIONS] [FILE PATH]", argv[0]);
            std::println("psfedit will put you in an interactive mode, where you can use the \"h\" command to learn more");
            std::println();
            std::println("Options:");
            std::println("-h, --help: show this help message");
            std::println("-v, --version: show psfedit version");
            std::println("-c, --create [height] [width]: create a new file");
            std::println("-u, --create-unicode [height] [width]: like --create, but with unicode table");
            return EXIT_SUCCESS;
        } else if (arg == "-v" || arg == "--version") {
            std::println("psfedit {}", VERSION_STR);
            return EXIT_SUCCESS;
        } else if (arg == "-c" || arg == "--create"
                || arg == "-u" || arg == "--create-unicode") {
            if (i + 2 > argc - 2) {
                std::println(stderr, "No width, height and/or file specified!");
                return EXIT_FAILURE;
            }
            try {
                newFileHeight = std::stoi(argv[i + 1]);
                newFileHeight = std::stoi(argv[i + 2]);
            } catch (const std::out_of_range& e) {
                std::println(stderr, "Invalid width and/or height!");
            }
            i += 2;
            newFile = true;
            newFileUnicode = (arg == "-u" || arg == "--create-unicode");
        } else if (i != argc - 1) {
            std::println("Unknown argument: {}", arg);
            return EXIT_FAILURE;
        }
    }
    bool saved = true;
    if (newFile) {
        if (std::filesystem::exists(filePath)) {
            std::println(stderr, "{} already exists!", filePath.string());
            return EXIT_FAILURE;
        }
        psf = Psf::createNew(newFileHeight, newFileWidth, newFileUnicode);
        if (!psf) {
            std::println(stderr, "Failed to create new font");
            return EXIT_FAILURE;
        }
        saved = false;
    } else {
        psf = Psf::loadFromFile(filePath);
        if (!psf) {
            std::println(stderr, "Failed to open {}, please check if the file exists and you have access to it",
                filePath.string());
            return EXIT_FAILURE;
        }
    }

    if (!psf->isValid()) {
        std::println(stderr, "{} is not a valid PSF file!", filePath.string());
        return EXIT_FAILURE;
    }

    std::pair<UI::Command, std::string> command = {UI::Command::COMMAND_SIZE, ""};
    while ((command = UI::getCommand(saved)).first != UI::Command::EXIT) {
        switch (command.first) {
            case UI::Command::SHOW:
                try {
                    Viewer::showGlyph(psf->getGlyph(command.second));
                } catch (std::out_of_range& e) {
                    std::println("Failed to get glyph: {}", e.what());
                }
                break;
            case UI::Command::EDIT:
                try {
                    Editor editor;
                    std::optional<Glyph> glyph = editor.editGlyph(psf->getGlyph(command.second));
                    if (glyph) {
                        psf->setGlyph(command.second, *glyph);
                        saved = false;
                    }
                } catch (std::out_of_range& e) {
                    std::println("Failed to get glyph: {}", e.what());
                }
                break;
            case UI::Command::SAVE:
                {
                    std::ofstream writeFile(filePath);
                    writeFile.write(psf->getBuffer(), psf->getBufferSize());
                }
                saved = true;
                break;
            case UI::Command::HEADER:
                UI::showHeader(psf->getHeader());
                break;
            case UI::Command::ADD_GLYPH_UNICODE:
                if (!psf->addGlyphUnicode(command.second)) {
                    std::println("Could not add glyph {}", command.second);
                } else {
                    saved = false;
                }
                break;
            case UI::Command::ADD_GLYPH_NO_UNICODE:
                if (!psf->addGlyphNoUnicode()) {
                    std::println("Could not add glyph");
                } else {
                    saved = false;
                }
                break;
            case UI::Command::LIST:
                UI::showList(psf->getUnicodeTable());
                break;
            case UI::Command::LLIST:
                UI::showList(psf->getUnicodeTable(), true);
                break;
            default:
                std::println("Could not handle command {}", static_cast<int>(command.first));
        }
    }

    return EXIT_SUCCESS;
}

