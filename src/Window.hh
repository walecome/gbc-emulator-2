#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/glad.h>
#include <iostream>
#include "imgui.h"

/**
 * Window utilites.
 */
class Window {
   public:
    /**
     * Create main game window.
     */
    bool createMainWindow(int width, int height, std::string title);

    /**
     * Update main window.
     *
     * To be ran inside of game loop.
     */
    void update();

    void imgui();

    /**
     * Return true if window is not set for closing.
     */
    bool shouldRemainOpen();

    void finish_render();

    /**
     * Destroy.
     */
    void destroy();

   private:
    GLFWwindow* m_window { nullptr };

    bool m_show_window { true };
    bool m_show_another_window { true };

    ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};