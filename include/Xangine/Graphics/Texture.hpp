#pragma once

#include <Xangine/Core/Colour.hpp>
#include <string>

namespace Xangine {

enum class TextureFormat {
    RGB,
    RGBA,
    Red,
    Green,
    Blue,
    Depth,
    Stencil
};

enum class TextureFilter {
    Nearest,   // Pixelated (retro style)
    Linear,    // Smooth
    Trilinear  // Mipmapped smooth
};

enum class TextureWrap {
    Repeat,    // Tile the texture
    Clamp,     // Clamp to edge
    Mirror     // Mirror the texture
};

class Texture {
public:
    Texture();
    ~Texture();
    
    // Create empty texture
    bool create(int width, int height, TextureFormat format);
    
    // Load from file
    bool loadFromFile(const std::string& filename);
    
    // Load from memory
    bool loadFromMemory(const unsigned char* data, int width, int height, TextureFormat format);
    
    // Set texture data
    void setData(const unsigned char* data);
    void setData(const Colour* data, int width, int height);
    
    // Settings
    void setFilter(TextureFilter filter);
    void setWrap(TextureWrap wrap);
    void generateMipmaps();
    
    // Bind to shader
    void bind(int unit = 0) const;
    void unbind() const;
    
    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    unsigned int getHandle() const { return m_textureID; }
    bool isValid() const { return m_textureID != 0; }
    
    // Static helpers
    static Texture* createWhiteTexture();
    static Texture* createBlackTexture();
    static Texture* createCheckerboard(int size = 64);
    
private:
    unsigned int m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
    TextureFormat m_format = TextureFormat::RGBA;
    TextureFilter m_filter = TextureFilter::Linear;
    TextureWrap m_wrap = TextureWrap::Repeat;
    
    void applySettings();
    int getGLFormat(TextureFormat format) const;
    int getGLFilter(TextureFilter filter) const;
    int getGLWrap(TextureWrap wrap) const;
};

} // namespace Xangine