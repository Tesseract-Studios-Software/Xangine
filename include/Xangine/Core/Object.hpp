// Core/Object.hpp
#pragma once

#include <Xangine/Core/Mesh.hpp>
#include <Xangine/Graphics/Material.hpp>
#include <Xangine/Math/Transform.hpp>
#include <string>
#include <vector>

namespace Xangine {

class Object {
public:
    Object();
    Object(const std::string& name);
    Object(Mesh* mesh, const TransformF& transform = TransformF());
    Object(Mesh* mesh, const Material& material, const TransformF& transform = TransformF());
    ~Object();

    std::string name = "Object";
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    TransformF transform;
    bool visible = true;
    bool active = true;

    void addChild(Object* child);
    void removeChild(Object* child);
    Object* getParent() const;
    const std::vector<Object*>& getChildren() const;

    void setMaterial(const Material& mat);
    Material* getMaterial() const;

private:
    Object* m_parent = nullptr;
    std::vector<Object*> m_children;
    Material m_defaultMaterial;
};

} // namespace Xangine