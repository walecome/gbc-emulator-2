#include <math.h>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "InstructionDecoder.hh"
#include "Processor.hh"
#include "Window.hh"

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

bool Window::createMainWindow(int width, int height, std::string title) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    const char* glsl_version = "#version 400";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (m_window == nullptr) {
        std::cerr << "Failed to create OpenGL context" << std::endl;
        fprintf(stderr, "Failed to Create OpenGL Context");
        return false;
    }

    glfwMakeContextCurrent(m_window);
    // Initialize OpenGL loader
    bool err = gladLoadGL() == 0;

    if (err) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void Window::draw_cpu_box() {
    ImGui::Begin("CPU info");

    auto cpu_info = m_cpu.getCPUInfo();

    for (auto& x : cpu_info.PM) {
        if (x.address == cpu_info.PC->getValue()) {
            ImGui::Text("---> %s", x.str().c_str());
        } else {
            ImGui::Text("     %s", x.str().c_str());
        }
    }

    ImGui::End();
}

void Window::draw_hello_box() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");

    ImGui::Text("This is some useful text.");  // Display some text (you can
                                               // use a format strings too)
    ImGui::Checkbox("Demo Window",
                    &m_show_window);  // Edit bools storing our window
                                      // open/close state
    ImGui::Checkbox("Another Window", &m_show_another_window);

    for (int i = 0; i < 5; ++i) {
        std::ostringstream oss {};
        oss << "var=" << counter + i;
        ImGui::Text("%s", oss.str().c_str());
    }

    ImGui::SliderFloat("float", &f, 0.0f,
                       1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3(
        "clear color",
        (float*)&m_clear_color);  // Edit 3 floats representing a color

    if (ImGui::Button("Button"))  // Buttons return true when clicked (most
                                  // widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Window::draw_control_box() {
    ImGui::Begin("CPU controls");

    ImGui::Text("Controls for the CPU");

    if (ImGui::Button("Step")) {
        m_instruction_decoder.step();
    }

    ImGui::End();
}

void Window::imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    draw_hello_box();
    draw_cpu_box();
    draw_control_box();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::update() {
    glfwPollEvents();

    imgui();

    finish_render();
}

bool Window::shouldRemainOpen() {
    assert(m_window);
    return !glfwWindowShouldClose(m_window);
}

void Window::finish_render() {
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z,
                 m_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void Window::destroy() { glfwTerminate(); }
