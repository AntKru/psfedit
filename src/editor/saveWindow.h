// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "window.h"

#include <string>

class SaveWindow : public Window {
    public:
        SaveWindow() = default;
        ~SaveWindow() = default;

        void update() override;
        void handleKey(int key) override;

        enum class Action {
            PENDING,
            FORCE_QUIT,
            SAVE_QUIT,
        };

        Action getAction();
        void setAction(Action action);

    private:
        Action m_action = Action::PENDING;
        const std::vector<std::string> m_message = {
            "Do you want to save?",
            "(y/n)",
            "________________________________________________",
            "Hint: You still have to save in interactive mode",
        };
        const size_t m_messageWidth = [this]() -> size_t {
            size_t s = 0;
            for (const std::string& str : m_message) {
                if (str.size() > s) {
                    s = str.size();
                }
            }
            return s;
        } ();
};

