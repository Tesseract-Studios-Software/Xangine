// src/Xangine/Core/Window.cpp
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
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return false;
    }
    
#ifdef XANGINE_PLATFORM_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    glfwWindowHint(GLFW_RESIZABLE, m_config.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    m_window = glfwCreateWindow(m_config.width, m_config.height, 
                                 m_config.title.c_str(), nullptr, nullptr);
    
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent((GLFWwindow*)m_window);

#ifdef XANGINE_PLATFORM_MACOS
    glfwSwapInterval(m_config.vsync ? 1 : 0);
#endif

    glfwSetWindowUserPointer((GLFWwindow*)m_window, this);
    glfwSetKeyCallback((GLFWwindow*)m_window, keyCallback);
    
    return true;
}

bool Window::createRenderer() {
    RendererType type = m_config.renderer;
    
    if (type == RendererType::Automatic) {
#ifdef XANGINE_PLATFORM_MACOS
        type = RendererType::OpenGL;
#elif defined(XANGINE_PLATFORM_WINDOWS)
        type = RendererType::OpenGL;
#else
        type = RendererType::OpenGL;
#endif
    }
    
    switch (type) {
        case RendererType::OpenGL:
            m_renderer = std::make_unique<OpenGLRenderer>();
            break;
            
        case RendererType::Vulkan:
            std::cerr << "Vulkan renderer not yet implemented" << std::endl;
            return false;
            
        case RendererType::Metal:
            std::cerr << "Metal renderer not yet implemented" << std::endl;
            return false;
            
        default:
            m_renderer = std::make_unique<OpenGLRenderer>();
            break;
    }
    
    return m_renderer != nullptr;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* app = (Window*)glfwGetWindowUserPointer(window);
    if (app) {
        app->m_width = width;
        app->m_height = height;
        if (app->m_resizeCallback) {
            app->m_resizeCallback(width, height);
        }
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* app = (Window*)glfwGetWindowUserPointer(window);
    if (!app) {
        return;
    }

    if (key < 0) {
        return;
    }

    app->m_input.handleKeyEvent(key, action);
}

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    m_resizeCallback = callback;
    glfwSetFramebufferSizeCallback((GLFWwindow*)m_window, framebufferSizeCallback);
}

void Window::bindKey(Key key, KeyCallback callback, KeyAction action) {
    m_input.bindKey(key, std::move(callback), action);
}

bool Window::isKeyDown(Key key) const {
    return m_input.isKeyDown(key);
}

bool Window::isKeyPressed(Key key) const {
    return m_input.isKeyPressed(key);
}

bool Window::isKeyReleased(Key key) const {
    return m_input.isKeyReleased(key);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose((GLFWwindow*)m_window);
}

void Window::pollEvents() {
    m_input.resetFrameState();
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