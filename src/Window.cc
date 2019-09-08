#include "Window.hh"

GLFWwindow* _mainWindow = nullptr;

bool Window::createMainWindow(int width, int height, std::string title) {
    glfwInit();
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

    return true;
}

GLFWwindow* Window::mainWindow() { return _mainWindow; }

void Window::update() {
    glfwSwapBuffers(Window::mainWindow());
    glfwPollEvents();
}

bool Window::shouldRemainOpen() {
    assert(_mainWindow);
    return !glfwWindowShouldClose(_mainWindow);
}

void Window::destroy() { glfwTerminate(); }
