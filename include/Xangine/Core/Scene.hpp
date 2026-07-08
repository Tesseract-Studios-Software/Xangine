// Core/Scene.hpp
#pragma once

#include <Xangine/Core/Camera.hpp>
#include <Xangine/Core/Object.hpp>
#include <Xangine/Graphics/Renderer.hpp>
#include <vector>
#include <memory>

namespace Xangine {

class Scene {
public:
    Scene(Renderer* renderer = nullptr);
    ~Scene();

    void addObject(Object* object);
    void removeObject(Object* object);
    void clear();

    void setCamera(const Camera& camera);
    Camera& getCamera();
    const Camera& getCamera() const;

    void setRenderer(Renderer* renderer);
    Renderer* getRenderer() const;

    void render();

    Object* findObject(const std::string& name);
    const std::vector<Object*>& getObjects() const { return m_objects; }

private:
    void renderObject(Object* object);

    Renderer* m_renderer;
    Camera m_defaultCamera;
    std::vector<Object*> m_objects;
};

} // namespace Xangine