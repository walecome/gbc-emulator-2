#include "Window.hh"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLFWwindow* _mainWindow = nullptr;

bool Window::createMainWindow(int width, int height, std::string title) {
    glfwInit();
    const char* glsl_version = "#version 400";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    _mainWindow =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (_mainWindow == nullptr) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        fprintf(stderr, "Failed to Create OpenGL Context");
        return false;
    }

    glfwMakeContextCurrent(_mainWindow);

    ImGui_ImplGlfw_InitForOpenGL(_mainWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

GLFWwindow* Window::mainWindow() { return _mainWindow; }

void Window::imgui() {
    ImGui::NewFrame();
    ImGui::Begin("Hello world!");
    ImGui::Text("This is some text");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
}

void Window::update() {
    glfwPollEvents();

    imgui();

    glfwSwapBuffers(Window::mainWindow());
}

bool Window::shouldRemainOpen() {
    assert(_mainWindow);
    return !glfwWindowShouldClose(_mainWindow);
}

void Window::destroy() { glfwTerminate(); }
