// SPDX-License-Identifier: GPL-3.0-only
#pragma once

#include <panel.h>

#include "window.h"

class DefaultWindow : public Window {
    public:
        DefaultWindow() = default;
        ~DefaultWindow() = default;

        void update() override;
};

