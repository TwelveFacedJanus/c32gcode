//
// Created by Даниил Ермолаев on 04.09.2025.
//

#include <iostream>
#include <spdlog/spdlog.h>
#include <memory>
#include "wm/window_manager.h"

int main(int argc, const char *argv[])
{
    spdlog::info("Welcome to c32gcode");
    const auto pWindowManager = std::make_unique<C32GCode_WindowManager>();
    pWindowManager->render();
    return 0;
}
