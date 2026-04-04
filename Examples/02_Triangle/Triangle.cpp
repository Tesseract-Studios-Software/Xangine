#include <Xangine/Xangine.hpp>
#include <iostream>

using namespace std;

int main() {
    using namespace Xangine;

    cout << Xangine::Version::getFull() << " (" << Xangine::Version::getCode() << ")" << endl;
    cout << "Welcome to Xangine!" << endl;
    
    // Create window
    WindowConfig config;
    config.title = "Xangine - First Triangle";
    config.width = 800;
    config.height = 600;
    config.renderer = RendererType::OpenGL;
    
    Window window(config);
    
    if (!window.initialise()) {
        cerr << "Failed to initialise Xangine!" << endl;
        return -1;
    }
    
    // Create triangle mesh
    Mesh triangle;
    
    // Three vertices (x, y, z)
    triangle.vertices = {
        {{ 0.0f,  0.5f, 0.0f}},  // Top
        {{-0.5f, -0.5f, 0.0f}},  // Bottom left
        {{ 0.5f, -0.5f, 0.0f}}   // Bottom right
    };
    
    // Three indices (one triangle)
    triangle.indices = {0, 1, 2};
    
    // Calculate bounding box (optional but good practice)
    triangle.calculateBounds();
    
    // Get renderer and upload mesh to GPU
    Renderer* renderer = window.getRenderer();
    renderer->createVertexBuffer(triangle);
    
    // Simple camera to look at the triangle
    Camera camera;
    camera.transform.position = Vec3(0.0f, 0.0f, 2.0f);
    camera.transform.rotation = Quat::identity();
    
    // Identity transform for the triangle
    Transform triangleTransform;
    
    cout << "Rendering triangle... Close window to exit." << endl;
    
    // Main loop
    while (!window.shouldClose()) {
        // Clear screen (dark grey background)
        window.clear(0.1f, 0.1f, 0.15f, 1.0f);
        
        // Draw the triangle
        renderer->drawMesh(triangle, triangleTransform, camera);
        
        // Update window (swap buffers, poll events)
        window.update();
    }
    
    // Cleanup
    renderer->destroyVertexBuffer(triangle);
    
    cout << "Triangle demo closed." << endl;
    return 0;
}