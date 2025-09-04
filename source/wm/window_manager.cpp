//
// Created by Даниил Ермолаев on 04.09.2025.
//

#include "./window_manager.h"

#include "spdlog/spdlog.h"

void C32GCode_WindowManager::initialize_glew_library() {
    glewExperimental = GL_TRUE;
    const GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        spdlog::error("Failed to initialize GLEW library");
        throw std::runtime_error("Failed to initialize GLEW library.");
    }
}

void C32GCode_WindowManager::initialize_glfw_library() {
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    spdlog::info("Window Manager initialized");
}

void C32GCode_WindowManager::create_glfw_window() {
    spdlog::info("Creating GLFW window");
    this->pWindow = glfwCreateWindow(640, 480, "c32gcode (ogl)", nullptr, nullptr);
    if (!this->pWindow) {
        spdlog::error("Failed to create GLFW window.");
        throw std::runtime_error("Failed to create GLFW window.");
    }
}

void C32GCode_WindowManager::render() const {
    while (!glfwWindowShouldClose(this->pWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        this->ui->Render();
        glfwPollEvents();
        glfwSwapBuffers(this->pWindow);
    }
}

C32GCode_WindowManager::C32GCode_WindowManager() {
    C32GCode_WindowManager::initialize_glfw_library();
    this->create_glfw_window();
    glfwMakeContextCurrent(this->pWindow);
    C32GCode_WindowManager::initialize_glew_library();
    glfwSwapInterval(1);
    this->ui = std::make_unique<C32GCodeUI>(this->pWindow);
}

C32GCode_WindowManager::~C32GCode_WindowManager() {
    spdlog::info("Destroying Window Manager.");
    if (this->pWindow) {glfwDestroyWindow(this->pWindow);}
    glfwTerminate();
}
