#pragma once

#include <Xangine/Core/Mesh.hpp>

namespace Xangine {

class Primitive {
public:
    // Create a cube with position, color, and UVs
    static Mesh createCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    
    // Create a sphere with UV mapping
    static Mesh createSphere(float radius = 0.5f, int segments = 32);
    
    // Create a cylinder
    static Mesh createCylinder(float radius = 0.5f, float height = 1.0f, int segments = 32);
    
    // Create a cone/pyramid
    static Mesh createCone(float radius = 0.5f, float height = 1.0f, int segments = 32);
    
    // Create a torus (donut shape)
    static Mesh createTorus(float radius = 0.5f, float tubeRadius = 0.1f, int radialSegments = 32, int tubularSegments = 32);
    
    // Create a plane (flat ground)
    static Mesh createPlane(float width = 1.0f, float depth = 1.0f, int segmentsX = 1, int segmentsZ = 1);
    
    // Create a grid (wireframe-like, for debugging)
    static Mesh createGrid(int size = 10, float spacing = 1.0f);
    
    // Create a UV sphere (better for texturing)
    static Mesh createUVSphere(float radius = 0.5f, int rings = 32, int sectors = 32);
    
    // Create a icosphere (better for subdivision)
    static Mesh createIcosphere(float radius = 0.5f, int subdivisions = 2);
    
private:
    // Helper functions
    static void addVertex(std::vector<Vertex>& vertices, const Vec3& pos, const Vec3& normal, const Vec2& uv);
    static void addIndex(std::vector<uint32_t>& indices, uint32_t i1, uint32_t i2, uint32_t i3);
};

} // namespace Xangine