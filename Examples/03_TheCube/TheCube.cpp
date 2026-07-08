// TheCube.cpp
#include <Xangine/Xangine.hpp>
#include <iostream>

using namespace std;

int main() {
    using namespace Xangine;
    
    cout << Xangine::Version::getFull() << " (" << Xangine::Version::getCode() << ")" << endl;
    cout << "Welcome to Xangine!" << endl;
    
    WindowConfig config;
    config.title = "Xangine - The Cube";
    config.width = 800;
    config.height = 600;
    config.renderer = RendererType::OpenGL;
    
    Window window(config);
    
    if (!window.initialise()) {
        cerr << "Failed to initialise Xangine!" << endl;
        return -1;
    }
    
    Renderer* renderer = window.getRenderer();
    Scene scene(renderer);
    
    Material metal = Material::fromAlbedo(Vec3(1.0f, 1.0f, 1.0f), ShadingModel::Flat);
    metal.metallic = 1.0f;
    metal.roughness = 0.0f;
    
    Mesh cubeMesh = Mesh::createCube(1.0f);
    cubeMesh.materials.push_back(metal);
    cout << "Cube has " << cubeMesh.vertices.size() << " vertices and " 
            << cubeMesh.indices.size() << " indices" << endl;
    
    Object cube("Cube");
    cube.mesh = &cubeMesh;
    cube.material = &metal;
    cube.transform.setPosition(Vec3(0.0f, 0.5f, 0.0f));
    
    Material floorMaterial = Material::fromAlbedo(Vec3(1.0f, 1.0f, 1.0f), ShadingModel::Flat);
    floorMaterial.metallic = 0.5f;
    floorMaterial.roughness = 1.0f;
    
    Mesh floorMesh = Mesh::createPlane(5.0f, 5.0f);
    floorMesh.materials.push_back(floorMaterial);
    
    Object floor("Floor");
    floor.mesh = &floorMesh;
    floor.material = &floorMaterial;
    floor.transform.setPosition(Vec3(0.0f, -0.5f, 0.0f));
    floor.transform.setRotation(Quat::fromEuler(Vec3(-3.1416f, 0.0f, 0.0f)));
    
    scene.addObject(&cube);
    scene.addObject(&floor);
    
    Camera& camera = scene.getCamera();
    camera.transform.setPosition(Vec3(0.0f, 0.0f, 3.0f));
    camera.transform.setRotation(Quat::identity());
    camera.transform.setGimbalLock(true);
    
    float cameraMoveSpeed = 0.05f;
    float cameraTurnSpeed = 0.03f;
    float rotationSpeed = 0.01f;
    float angle = 0.0f;
    bool rotateCube = true;
    
    window.bindKey(Key::Space, [&]() {
        rotateCube = !rotateCube;
        cout << "Rotation " << (rotateCube ? "enabled" : "disabled") << endl;
    });
    
    window.bindKey(Key::R, [&]() {
        camera.transform.setPosition(Vec3(0.0f, 0.0f, 3.0f));
        camera.transform.setYawPitchRoll(0.0f, 0.0f, 0.0f);
        cout << "Camera reset" << endl;
    });
    
    cout << "\nControls:" << endl;
    cout << "WASD - Move camera" << endl;
    cout << "QE - Move up/down" << endl;
    cout << "Arrow Keys - Rotate view" << endl;
    cout << "Space - Toggle cube rotation" << endl;
    cout << "R - Reset camera\n" << endl;
    
    while (!window.shouldClose()) {
        if (window.isKeyDown(Key::W)) {
            camera.transform.moveForward(cameraMoveSpeed);
        }
        if (window.isKeyDown(Key::S)) {
            camera.transform.moveBackward(cameraMoveSpeed);
        }
        if (window.isKeyDown(Key::A)) {
            camera.transform.moveLeft(cameraMoveSpeed);
        }
        if (window.isKeyDown(Key::D)) {
            camera.transform.moveRight(cameraMoveSpeed);
        }
        if (window.isKeyDown(Key::E)) {
            camera.transform.moveUp(cameraMoveSpeed);
        }
        if (window.isKeyDown(Key::Q)) {
            camera.transform.moveDown(cameraMoveSpeed);
        }
        
        if (window.isKeyDown(Key::Up)) {
            camera.transform.rotatePitch(cameraTurnSpeed);
        }
        if (window.isKeyDown(Key::Down)) {
            camera.transform.rotatePitch(-cameraTurnSpeed);
        }
        if (window.isKeyDown(Key::Left)) {
            camera.transform.rotateYaw(cameraTurnSpeed);
        }
        if (window.isKeyDown(Key::Right)) {
            camera.transform.rotateYaw(-cameraTurnSpeed);
        }
        
        if (rotateCube) {
            angle += rotationSpeed;
            cube.transform.setRotation(Quat::fromEuler(Vec3(0.0f, angle, 0.0f)));
        }
        
        window.clear(0.8f, 0.8f, 0.9f, 1.0f);
        scene.render();
        window.update();
    }
    
    cout << "Cube demo closed." << endl;
    return 0;
}