#include <Xangine/Core/Window.hpp>
#include <Xangine/Platform/OpenGL.hpp>
#include <iostream>

namespace Xangine {

Window::Window(const WindowConfig& config) 
    : m_width(config.width), m_height(config.height) {
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        m_window = nullptr;
        return;
    }
    
    // Configure OpenGL version (macOS needs this)
    #ifdef XANGINE_PLATFORM_MACOS
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    // Create window
    m_window = glfwCreateWindow(config.width, config.height, 
                                 config.title.c_str(), nullptr, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
}

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow((GLFWwindow*)m_window);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose((GLFWwindow*)m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers((GLFWwindow*)m_window);
}

} // namespace Xanginejj