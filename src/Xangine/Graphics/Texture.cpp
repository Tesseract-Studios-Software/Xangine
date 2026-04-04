#include <Xangine/Graphics/Texture.hpp>
#include <Xangine/Platform/OpenGL.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Xangine {

Texture::Texture() = default;

Texture::~Texture() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::create(int width, int height, TextureFormat format) {
    m_width = width;
    m_height = height;
    m_format = format;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Create empty texture
    glTexImage2D(GL_TEXTURE_2D, 0, getGLFormat(format), width, height, 0, 
                 getGLFormat(format), GL_UNSIGNED_BYTE, nullptr);
    
    applySettings();
    
    return true;
}

bool Texture::loadFromFile(const std::string& filename) {
    int width, height, channels;
    
    // Load image using stb_image
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }
    
    // Determine format
    TextureFormat format;
    if (channels == 1) format = TextureFormat::Red;
    else if (channels == 3) format = TextureFormat::RGB;
    else if (channels == 4) format = TextureFormat::RGBA;
    else format = TextureFormat::RGBA;
    
    // Create texture
    bool result = loadFromMemory(data, width, height, format);
    
    stbi_image_free(data);
    return result;
}

bool Texture::loadFromMemory(const unsigned char* data, int width, int height, TextureFormat format) {
    m_width = width;
    m_height = height;
    m_format = format;
    
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, getGLFormat(format), width, height, 0,
                 getGLFormat(format), GL_UNSIGNED_BYTE, data);
    
    applySettings();
    generateMipmaps();
    
    return true;
}

void Texture::setData(const unsigned char* data) {
    if (!m_textureID) return;
    
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLFormat(m_format), m_width, m_height, 0,
                 getGLFormat(m_format), GL_UNSIGNED_BYTE, data);
}

void Texture::setData(const Colour* data, int width, int height) {
    m_width = width;
    m_height = height;
    m_format = TextureFormat::RGBA;
    
    // Convert Colour to unsigned char array
    std::vector<unsigned char> rawData(width * height * 4);
    for (int i = 0; i < width * height; i++) {
        rawData[i * 4 + 0] = (unsigned char)(data[i].r * 255);
        rawData[i * 4 + 1] = (unsigned char)(data[i].g * 255);
        rawData[i * 4 + 2] = (unsigned char)(data[i].b * 255);
        rawData[i * 4 + 3] = (unsigned char)(data[i].a * 255);
    }
    
    setData(rawData.data());
}

void Texture::setFilter(TextureFilter filter) {
    m_filter = filter;
    if (m_textureID) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLFilter(filter));
    }
}

void Texture::setWrap(TextureWrap wrap) {
    m_wrap = wrap;
    if (m_textureID) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLWrap(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLWrap(wrap));
    }
}

void Texture::generateMipmaps() {
    if (m_textureID) {
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::bind(int unit) const {
    if (m_textureID) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::applySettings() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getGLFilter(m_filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getGLFilter(m_filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getGLWrap(m_wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getGLWrap(m_wrap));
}

int Texture::getGLFormat(TextureFormat format) const {
    switch (format) {
        case TextureFormat::RGB:    return GL_RGB;
        case TextureFormat::RGBA:   return GL_RGBA;
        case TextureFormat::Red:    return GL_RED;
        case TextureFormat::Green:  return GL_GREEN;
        case TextureFormat::Blue:   return GL_BLUE;
        case TextureFormat::Depth:  return GL_DEPTH_COMPONENT;
        case TextureFormat::Stencil:return GL_STENCIL_INDEX;
        default:                    return GL_RGBA;
    }
}

int Texture::getGLFilter(TextureFilter filter) const {
    switch (filter) {
        case TextureFilter::Nearest:    return GL_NEAREST;
        case TextureFilter::Linear:     return GL_LINEAR;
        case TextureFilter::Trilinear:  return GL_LINEAR_MIPMAP_LINEAR;
        default:                        return GL_LINEAR;
    }
}

int Texture::getGLWrap(TextureWrap wrap) const {
    switch (wrap) {
        case TextureWrap::Repeat:   return GL_REPEAT;
        case TextureWrap::Clamp:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::Mirror:   return GL_MIRRORED_REPEAT;
        default:                    return GL_REPEAT;
    }
}

// Static helper textures
Texture* Texture::createWhiteTexture() {
    static Colour whitePixel(1.0f, 1.0f, 1.0f, 1.0f);
    static Texture whiteTexture;
    if (!whiteTexture.isValid()) {
        whiteTexture.create(1, 1, TextureFormat::RGBA);
        whiteTexture.setData(reinterpret_cast<const unsigned char*>(&whitePixel));
    }
    return &whiteTexture;
}

Texture* Texture::createBlackTexture() {
    static Colour blackPixel(0.0f, 0.0f, 0.0f, 1.0f);
    static Texture blackTexture;
    if (!blackTexture.isValid()) {
        blackTexture.create(1, 1, TextureFormat::RGBA);
        blackTexture.setData(reinterpret_cast<const unsigned char*>(&blackPixel));
    }
    return &blackTexture;
}

Texture* Texture::createCheckerboard(int size) {
    static Texture checkerTexture;
    if (!checkerTexture.isValid()) {
        std::vector<Colour> pixels(size * size);
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                bool isBlack = ((x / (size/8)) + (y / (size/8))) % 2 == 0;
                pixels[y * size + x] = isBlack ? Colour::Black() : Colour::White();
            }
        }
        checkerTexture.create(size, size, TextureFormat::RGBA);
        checkerTexture.setData(pixels.data(), size, size);
        checkerTexture.setFilter(TextureFilter::Nearest);
    }
    return &checkerTexture;
}

} // namespace Xangine