// Core/Scene.cpp
#include <Xangine/Core/Scene.hpp>

namespace Xangine {

Scene::Scene(Renderer* renderer)
    : m_renderer(renderer) {
    m_defaultCamera.transform.setPosition(Vec3(0.0f, 0.0f, 5.0f));
    m_defaultCamera.transform.lookAt(Vec3(0.0f, 0.0f, 0.0f));
}

Scene::~Scene() {
    clear();
}

void Scene::addObject(Object* object) {
    if (!object) return;
    
    if (m_renderer && object->mesh && object->mesh->isValid()) {
        m_renderer->createVertexBuffer(*object->mesh);
    }
    
    m_objects.push_back(object);
}

void Scene::removeObject(Object* object) {
    auto it = std::find(m_objects.begin(), m_objects.end(), object);
    if (it != m_objects.end()) {
        if (m_renderer && (*it)->mesh) {
            m_renderer->destroyVertexBuffer(*(*it)->mesh);
        }
        m_objects.erase(it);
    }
}

void Scene::clear() {
    for (auto* object : m_objects) {
        if (m_renderer && object->mesh) {
            m_renderer->destroyVertexBuffer(*object->mesh);
        }
    }
    m_objects.clear();
}

void Scene::setCamera(const Camera& camera) {
    m_defaultCamera = camera;
}

Camera& Scene::getCamera() {
    return m_defaultCamera;
}

const Camera& Scene::getCamera() const {
    return m_defaultCamera;
}

void Scene::setRenderer(Renderer* renderer) {
    m_renderer = renderer;
}

Renderer* Scene::getRenderer() const {
    return m_renderer;
}

void Scene::render() {
    if (!m_renderer) return;
    
    m_renderer->beginFrame();
    
    for (auto* object : m_objects) {
        renderObject(object);
    }
    
    m_renderer->endFrame();
}

void Scene::renderObject(Object* object) {
    if (!object || !object->active || !object->visible) return;
    
    Mesh* mesh = object->mesh;
    if (!mesh || !mesh->isValid()) return;
    
    Material* material = object->getMaterial();
    TransformF& transform = object->transform;
    
    if (material) {
        m_renderer->drawMesh(*mesh, transform, m_defaultCamera, *material);
    } else {
        m_renderer->drawMesh(*mesh, transform, m_defaultCamera);
    }
    
    for (auto* child : object->getChildren()) {
        renderObject(child);
    }
}

Object* Scene::findObject(const std::string& name) {
    for (auto* object : m_objects) {
        if (object->name == name) {
            return object;
        }
    }
    return nullptr;
}

} // namespace Xangine