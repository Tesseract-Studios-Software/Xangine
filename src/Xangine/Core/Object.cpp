// Core/Object.cpp
#include <Xangine/Core/Object.hpp>

namespace Xangine {

Object::Object() = default;

Object::Object(const std::string& name) : name(name) {}

Object::Object(Mesh* mesh, const TransformF& transform)
    : mesh(mesh), transform(transform) {}

Object::Object(Mesh* mesh, const Material& material, const TransformF& transform)
    : mesh(mesh), transform(transform) {
    setMaterial(material);
}

Object::~Object() {}

void Object::addChild(Object* child) {
    if (child->m_parent) {
        child->m_parent->removeChild(child);
    }
    child->m_parent = this;
    m_children.push_back(child);
}

void Object::removeChild(Object* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        child->m_parent = nullptr;
        m_children.erase(it);
    }
}

Object* Object::getParent() const {
    return m_parent;
}

const std::vector<Object*>& Object::getChildren() const {
    return m_children;
}

void Object::setMaterial(const Material& mat) {
    m_defaultMaterial = mat;
    material = &m_defaultMaterial;
}

Material* Object::getMaterial() const {
    if (material) {
        return material;
    }
    if (mesh) {
        return const_cast<Material*>(&mesh->getDefaultMaterial());
    }
    return nullptr;
}

} // namespace Xangine