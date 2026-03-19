// SPDX-License-Identifier: GPL-3.0-only

#include <print>
#include <iostream>

#include "ui.h"

std::pair<Command, unsigned short int> getCommand() {
    while (true) {
        std::print("type h for help> ");
        std::string command;
        std::cin >> command;
        if (command == "h") {
            std::println("Available commands:");
            std::println("h: show this help");
            std::println("e: exit");
            std::println("s [character code]: show character");
        } else if (command == "e") {
            return {Command::EXIT, 0};
        } else if (command == "s") {
            std::string number;
            std::cin >> number;
            try {
                return {Command::SHOW, stoi(number)};
            } catch (const std::exception& e) {
                std::println(stderr, "Invalid number: {}", number);
            }
        }
    }
}

void showGlyph(const std::vector<std::vector<bool>>& glyph) {
    for (const std::vector<bool>& line : glyph) {
        for (const bool fg : line) {
            std::cout << (fg ? "\033[7m· \033[0m" : "· ");
        }
        std::cout << std::endl;
    }
}

