//
// Created by Даниил Ермолаев on 04.09.2025.
//

#ifndef C32GCODE_UI_H
#define C32GCODE_UI_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

class C32GCodeUI {
    GLFWwindow* pWindow = nullptr;
    ImGuiViewport * pViewport{};
    void initialize_imgui_library() const;
public:

    void Dockspace();
    void MainMenu() const;
    static void NewFrame();
    static void DrawData();
    void Render();

    C32GCodeUI operator=(C32GCodeUI const&) = delete;
    C32GCodeUI(C32GCodeUI const&) = delete;
    explicit C32GCodeUI(GLFWwindow *pWindow);
    ~C32GCodeUI();
};

#endif //C32GCODE_UI_H