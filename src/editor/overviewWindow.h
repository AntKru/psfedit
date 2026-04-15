// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include "window.h"

class OverviewWindow : public Window {
    public:
        OverviewWindow() = default;
        ~OverviewWindow() = default;

        void update() override;
        void handleKey(int key) override;
};

