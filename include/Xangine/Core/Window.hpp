#pragma once

#include <string>
#include <memory>
#include <Xangine/Graphics/RendererType.hpp>
#include <Xangine/Core/Colour.hpp>
#include <Xangine/Core/Window.hpp>
#include <Xangine/Platform/GLFW.hpp> 
#include <Xangine/Graphics/Renderer.hpp>

namespace Xangine {

class Renderer;  // Forward declaration

struct WindowConfig {
    std::string title = "Xangine";
    int width = 1280;
    int height = 720;
    bool vsync = true;
    bool resizable = true;
    RendererType renderer = RendererType::Automatic;
};

class Window {
public:
    Window(const WindowConfig& config = WindowConfig());
    ~Window();
    
    // Initialise the window and renderer
    bool initialise();
    void shutdown();
    
    // Main loop helpers
    bool shouldClose() const;
    void pollEvents();
    void update();  // Polls events + swaps buffers
    
    // Rendering helpers (forward to renderer)
    void clear(float r, float g, float b, float a);
    void clear(const Colour& colour);
    
    // Getters
    Renderer* getRenderer() { return m_renderer.get(); }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    void* getNativeHandle() const { return m_window; }
    
private:
    void* m_window;  // GLFWwindow*
    int m_width, m_height;
    WindowConfig m_config;
    std::unique_ptr<Renderer> m_renderer;
    
    bool createWindow();
    bool createRenderer();
};

} // namespace Xangine