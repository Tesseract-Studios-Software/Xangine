#include <Xangine/Xangine.hpp>
#include <iostream>

int main() {
    using namespace Xangine;
    
    cout << Xangine::Version::getFull() << " (" << Xangine::Version::getCode() << ")" << endl;
    cout << "Welcome to Xangine!" << endl;
    
    // Create window
    WindowConfig config;
    config.title = "Xangine - The Cube";
    config.width = 800;
    config.height = 600;
    config.renderer = RendererType::OpenGL;
    
    Window window(config);
    
    if (!window.initialise()) {
        std::cerr << "Failed to initialise Xangine!" << std::endl;
        return -1;
    }
    
    // Create a cube mesh
    Mesh cube = Mesh::createCube(1.0f);
    cout << "Cube has " << cube.vertices.size() << " vertices and " 
            << cube.indices.size() << " indices" << std::endl;
    
    // Upload to GPU
    Renderer* renderer = window.getRenderer();
    renderer->createVertexBuffer(cube);
    
    // Setup camera
    Camera camera;
    camera.transform.position = Vec3(0.0f, 0.0f, 3.0f);
    
    // Cube transform
    Transform cubeTransform;
    float rotationSpeed = 0.01f;
    float angle = 0.0f;
    
    std::cout << "Spinning cube! Close window to exit." << std::endl;
    
    // Main loop
    while (!window.shouldClose()) {
        // Update rotation
        angle += rotationSpeed;
        cubeTransform.rotation = Quat::fromEuler(Vec3(0.0f, angle, 0.0f));
        
        // Clear screen
        window.clear(0.1f, 0.1f, 0.15f, 1.0f);
        
        // Draw cube
        renderer->drawMesh(cube, cubeTransform, camera);
        
        // Update window
        window.update();
    }
    
    // Cleanup
    renderer->destroyVertexBuffer(cube);
    
    std::cout << "Cube demo closed." << std::endl;
    return 0;
}