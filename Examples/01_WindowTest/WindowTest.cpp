#include <Xangine/Core/Window.hpp>
#include <iostream>

int main() {
    using namespace Xangine;
    
    std::cout << "Xangine Engine Starting..." << std::endl;
    
    WindowConfig config;
    config.title = "Xangine - Window Test";
    config.width = 800;
    config.height = 600;
    
    Window window(config);
    
    if (!window.initialise()) {
        std::cerr << "Failed to initialise Xangine!" << std::endl;
        return -1;
    }
    
    std::cout << "Window created! Running main loop..." << std::endl;
    
    while (!window.shouldClose()) {
        window.clear(0.2f, 0.3f, 0.3f, 1.0f);
        window.update();
    }
    
    std::cout << "Engine shutdown." << std::endl;
    return 0;
}