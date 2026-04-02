#pragma once

#include <string>

namespace Xangine {

struct WindowConfig {
    std::string title = "Xangine";
    int width = 1280;
    int height = 720;
    bool vsync = true;
    bool resizable = true;
};

class Window {
public:
    Window(const WindowConfig& config);
    ~Window();
    
    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    
    void* getNativeHandle() const;  // For GLFW/Vulkan
    
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
private:
    void* m_window;  // GLFWwindow*
    int m_width, m_height;
};

} // namespace Xangine