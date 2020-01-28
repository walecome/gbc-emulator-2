#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include "imgui.h"

class Processor;
class InstructionDecoder;

/**
 * Window utilites.
 */
class Window {
   public:
    Window(const Processor& cpu, InstructionDecoder& instruction_decoder)
        : m_cpu { cpu }, m_instruction_decoder { instruction_decoder } {}

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

    bool shouldRemainOpen();

    void finish_render();

    void draw_hello_box();
    void draw_control_box();
    void draw_cpu_box();

    /**
     * Destroy.
     */
    void destroy();

   private:
    const Processor& m_cpu;
    InstructionDecoder& m_instruction_decoder;

    GLFWwindow* m_window { nullptr };

    bool m_show_window { true };
    bool m_show_another_window { true };

    ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};