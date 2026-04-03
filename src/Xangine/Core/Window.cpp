#include <Xangine/Core/Window.hpp>
#include <Xangine/Platform/OpenGL.hpp>
#include <Xangine/Graphics/Renderer.hpp>
#include "../Graphics/Backends/OpenGL/OpenGLRenderer.hpp"
#include <iostream>

namespace Xangine {

Window::Window(const WindowConfig& config) 
    : m_config(config), m_width(config.width), m_height(config.height), m_window(nullptr) {}

Window::~Window() {
    shutdown();
}

bool Window::initialise() {
    if (!createWindow()) {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }
    
    if (!createRenderer()) {
        std::cerr << "Failed to create renderer!" << std::endl;
        return false;
    }
    
    if (!m_renderer->initialise(this)) {
        std::cerr << "Failed to initialise renderer!" << std::endl;
        return false;
    }
    
    std::cout << "Xangine window initialised successfully!" << std::endl;
    return true;
}

void Window::shutdown() {
    if (m_renderer) {
        m_renderer->shutdown();
        m_renderer.reset();
    }
    
    if (m_window) {
        glfwDestroyWindow((GLFWwindow*)m_window);
        m_window = nullptr;
    }
    
    glfwTerminate();
}

bool Window::createWindow() {
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return false;
    }
    
    // Configure OpenGL version (macOS needs this)
    #ifdef XANGINE_PLATFORM_MACOS
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    glfwWindowHint(GLFW_RESIZABLE, m_config.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    // Create window
    m_window = glfwCreateWindow(m_config.width, m_config.height, 
                                 m_config.title.c_str(), nullptr, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    return true;
}

bool Window::createRenderer() {
    // Choose renderer based on config
    RendererType type = m_config.renderer;
    
    if (type == RendererType::Automatic) {
        #ifdef XANGINE_PLATFORM_MACOS
            type = RendererType::Metal;
        #elif defined(XANGINE_PLATFORM_WINDOWS)
            type = RendererType::OpenGL;  // For now, default to OpenGL
        #else
            type = RendererType::OpenGL;
        #endif
    }
    
    // Create the appropriate renderer
    switch (type) {
        case RendererType::OpenGL:
            m_renderer = std::make_unique<OpenGLRenderer>();
            break;
            
        case RendererType::Vulkan:
            // m_renderer = std::make_unique<VulkanRenderer>();
            std::cerr << "Vulkan renderer not yet implemented" << std::endl;
            return false;
            
        case RendererType::Metal:
            // m_renderer = std::make_unique<MetalRenderer>();
            std::cerr << "Metal renderer not yet implemented, falling back to OpenGL" << std::endl;
            m_renderer = std::make_unique<OpenGLRenderer>();
            break;
            
        default:
            m_renderer = std::make_unique<OpenGLRenderer>();
            break;
    }
    
    return m_renderer != nullptr;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose((GLFWwindow*)m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::update() {
    pollEvents();
    
    if (m_renderer) {
        m_renderer->swapBuffers();
    }
}

void Window::clear(float r, float g, float b, float a) {
    if (m_renderer) {
        m_renderer->clear(r, g, b, a);
    }
}

void Window::clear(const Colour& colour) {
    clear(colour.r, colour.g, colour.b, colour.a);
}

} // namespace Xangine