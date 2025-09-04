/**
 * @class C32GCode_WindowManager
 * @brief Определяет все необходимые для Mental Window Manager функции-обертки над GLFW.
 *
 * Класс соответсвует всем принципам RAII и C++17-23.
 * Мы не допускаем его копирование и перемещение.
 *
 * @date 04.09.2025
 * @author Daniil [OneFaced] Ermolaev.
 */
#ifndef C32GCODE_WINDOW_MANAGER_H
#define C32GCODE_WINDOW_MANAGER_H

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../ui/ui.h"

class C32GCode_WindowManager {
    GLFWwindow* pWindow = nullptr;
    static void initialize_glfw_library() noexcept(false);
    static void initialize_glew_library() noexcept(false);
    void create_glfw_window() noexcept(false);

    std::unique_ptr<C32GCodeUI> ui = nullptr;
public:
    void render() const;

    C32GCode_WindowManager();
    C32GCode_WindowManager operator=(C32GCode_WindowManager const&) = delete;
    C32GCode_WindowManager(C32GCode_WindowManager const&) = delete;
    ~C32GCode_WindowManager();
};

#endif //C32GCODE_WINDOW_MANAGER_H