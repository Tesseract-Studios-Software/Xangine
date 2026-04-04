#pragma once

#include <Xangine/Math/Vector2.hpp>
#include <Xangine/Core/Camera.hpp>
#include <Xangine/Graphics/Texture.hpp>

namespace Xangine {

enum class ViewportType {
    _2D,      // UI surface, textures, sprites
    _3D       // 3D rendering with camera
};

class Viewport {
public:
    Viewport(int x, int y, int width, int height);
    
    // Position and size
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void setBounds(int x, int y, int width, int height);
    
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
    // Type
    void setType(ViewportType type);
    ViewportType getType() const { return m_type; }
    
    // For 3D viewports
    void setCamera(Camera* camera);
    Camera* getCamera() const { return m_camera; }
    
    // For 2D viewports
    void setTexture(Texture* texture);
    Texture* getTexture() const { return m_texture; }
    
    // Behaviour
    bool autoResize = true;  // Automatically resize with window
    bool autoAspect = true;   // Automatically update camera aspect ratio
    
    // Apply to OpenGL
    void applyViewport() const;
    void updateCameraAspect();
    
private:
    int m_x, m_y;
    int m_width, m_height;
    ViewportType m_type = ViewportType::_3D;
    Camera* m_camera = nullptr;
    Texture* m_texture = nullptr;
};

} // namespace Xangine