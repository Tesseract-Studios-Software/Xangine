// Graphics/Primitive.cpp
#include <Xangine/Core/Primitive.hpp>
#include <Xangine/Math/Math.hpp>
#include <Xangine/Core/Mesh.hpp>
#include <cmath>

namespace Xangine {

Mesh Primitive::createCube(float width, float height, float depth) {
    Mesh mesh;
    
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    float halfD = depth * 0.5f;
    
    Vec3 vertices[8] = {
        Vec3(-halfW, -halfH, -halfD),
        Vec3( halfW, -halfH, -halfD),
        Vec3( halfW,  halfH, -halfD),
        Vec3(-halfW,  halfH, -halfD),
        Vec3(-halfW, -halfH,  halfD),
        Vec3( halfW, -halfH,  halfD),
        Vec3( halfW,  halfH,  halfD),
        Vec3(-halfW,  halfH,  halfD)
    };
    
    Vec3 normals[6] = {
        Vec3(0, 0, -1),
        Vec3(0, 0,  1),
        Vec3(0, -1, 0),
        Vec3(0,  1, 0),
        Vec3(-1, 0, 0),
        Vec3( 1, 0, 0)
    };
    
    Vec2 uvs[4] = {
        Vec2(0, 0),
        Vec2(1, 0),
        Vec2(1, 1),
        Vec2(0, 1)
    };
    
    int faceIndices[6][4] = {
        {0, 1, 2, 3},
        {5, 4, 7, 6},
        {4, 5, 1, 0},
        {3, 2, 6, 7},
        {4, 0, 3, 7},
        {1, 5, 6, 2}
    };
    
    for (int f = 0; f < 6; f++) {
        Vec3 v[4];
        for (int i = 0; i < 4; i++) {
            v[i] = vertices[faceIndices[f][i]];
        }
        
        // Triangle 1: v0, v1, v2 (counter-clockwise)
        mesh.vertices.push_back({v[0], normals[f], uvs[0]});
        mesh.vertices.push_back({v[1], normals[f], uvs[1]});
        mesh.vertices.push_back({v[2], normals[f], uvs[2]});
        
        // Triangle 2: v0, v2, v3 (counter-clockwise)
        mesh.vertices.push_back({v[0], normals[f], uvs[0]});
        mesh.vertices.push_back({v[2], normals[f], uvs[2]});
        mesh.vertices.push_back({v[3], normals[f], uvs[3]});
    }
    
    for (uint32_t i = 0; i < mesh.vertices.size(); i++) {
        mesh.indices.push_back(i);
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Primitive;
    return mesh;
}

Mesh Primitive::createSphere(float radius, int segments) {
    if (segments < 3) segments = 3;
    
    Mesh mesh;
    
    for (int y = 0; y <= segments; y++) {
        float theta = y * Math::PI / segments;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);
        
        for (int x = 0; x <= segments; x++) {
            float phi = x * 2.0f * Math::PI / segments;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            
            Vec3 position(
                radius * sinTheta * sinPhi,
                radius * cosTheta,
                radius * sinTheta * cosPhi
            );
            
            Vec3 normal = position.normalized();
            Vec2 uv((float)x / segments, (float)y / segments);
            
            mesh.vertices.push_back({position, normal, uv});
        }
    }
    
    for (int y = 0; y < segments; y++) {
        for (int x = 0; x < segments; x++) {
            int current = y * (segments + 1) + x;
            int next = current + segments + 1;
            
            // Counter-clockwise winding
            mesh.indices.push_back(current);
            mesh.indices.push_back(next);
            mesh.indices.push_back(current + 1);
            
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next);
            mesh.indices.push_back(next + 1);
        }
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Organic;
    return mesh;
}

Mesh Primitive::createCylinder(float radius, float height, int segments) {
    if (segments < 3) segments = 3;
    
    Mesh mesh;
    float halfHeight = height * 0.5f;
    
    for (int i = 0; i <= segments; i++) {
        float theta = i * 2.0f * Math::PI / segments;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);
        
        // Normal points outward from the cylinder surface
        Vec3 normal(cosTheta, 0, sinTheta);
        
        mesh.vertices.push_back({
            Vec3(radius * cosTheta, -halfHeight, radius * sinTheta),
            normal,
            Vec2((float)i / segments, 0)
        });
        
        mesh.vertices.push_back({
            Vec3(radius * cosTheta, halfHeight, radius * sinTheta),
            normal,
            Vec2((float)i / segments, 1)
        });
    }
    
    int bottomCenter = mesh.vertices.size();
    mesh.vertices.push_back({Vec3(0, -halfHeight, 0), Vec3(0, -1, 0), Vec2(0.5f, 0.5f)});
    
    int topCenter = mesh.vertices.size();
    mesh.vertices.push_back({Vec3(0, halfHeight, 0), Vec3(0, 1, 0), Vec2(0.5f, 0.5f)});
    
    // Side faces - counter-clockwise winding
    for (int i = 0; i < segments; i++) {
        int bottomLeft = i * 2;
        int topLeft = i * 2 + 1;
        int bottomRight = (i + 1) * 2;
        int topRight = (i + 1) * 2 + 1;
        
        mesh.indices.push_back(bottomLeft);
        mesh.indices.push_back(topLeft);
        mesh.indices.push_back(bottomRight);
        
        mesh.indices.push_back(bottomRight);
        mesh.indices.push_back(topLeft);
        mesh.indices.push_back(topRight);
    }
    
    // Bottom cap - counter-clockwise from outside (looking up)
    for (int i = 0; i < segments; i++) {
        int current = i * 2;
        int next = (i + 1) * 2;
        mesh.indices.push_back(bottomCenter);
        mesh.indices.push_back(next);
        mesh.indices.push_back(current);
    }
    
    // Top cap - counter-clockwise from outside (looking down)
    for (int i = 0; i < segments; i++) {
        int current = i * 2 + 1;
        int next = (i + 1) * 2 + 1;
        mesh.indices.push_back(topCenter);
        mesh.indices.push_back(current);
        mesh.indices.push_back(next);
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Primitive;
    return mesh;
}

Mesh Primitive::createCone(float radius, float height, int segments) {
    if (segments < 3) segments = 3;
    
    Mesh mesh;
    float halfHeight = height * 0.5f;
    
    int apexIndex = 0;
    mesh.vertices.push_back({Vec3(0, halfHeight, 0), Vec3(0, 1, 0), Vec2(0.5f, 1)});
    
    for (int i = 0; i <= segments; i++) {
        float theta = i * 2.0f * Math::PI / segments;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);
        
        // Normal for side faces points outward and slightly up
        float angle = std::atan2(radius, height);
        Vec3 normal(
            cosTheta * std::cos(angle),
            std::sin(angle),
            sinTheta * std::cos(angle)
        );
        
        mesh.vertices.push_back({
            Vec3(radius * cosTheta, -halfHeight, radius * sinTheta),
            normal,
            Vec2((float)i / segments, 0)
        });
    }
    
    int centerIndex = mesh.vertices.size();
    mesh.vertices.push_back({Vec3(0, -halfHeight, 0), Vec3(0, -1, 0), Vec2(0.5f, 0)});
    
    // Side faces - counter-clockwise winding
    for (int i = 0; i < segments; i++) {
        int current = i + 1;
        int next = (i + 1) % segments + 1;
        
        mesh.indices.push_back(apexIndex);
        mesh.indices.push_back(current);
        mesh.indices.push_back(next);
    }
    
    // Bottom cap - counter-clockwise winding
    for (int i = 0; i < segments; i++) {
        int current = i + 1;
        int next = (i + 1) % segments + 1;
        mesh.indices.push_back(centerIndex);
        mesh.indices.push_back(next);
        mesh.indices.push_back(current);
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Primitive;
    return mesh;
}

Mesh Primitive::createTorus(float radius, float tubeRadius, int radialSegments, int tubularSegments) {
    if (radialSegments < 3) radialSegments = 3;
    if (tubularSegments < 3) tubularSegments = 3;
    
    Mesh mesh;
    
    for (int i = 0; i <= radialSegments; i++) {
        float u = i * 2.0f * Math::PI / radialSegments;
        float cosU = std::cos(u);
        float sinU = std::sin(u);
        
        for (int j = 0; j <= tubularSegments; j++) {
            float v = j * 2.0f * Math::PI / tubularSegments;
            float cosV = std::cos(v);
            float sinV = std::sin(v);
            
            Vec3 position(
                (radius + tubeRadius * cosV) * cosU,
                tubeRadius * sinV,
                (radius + tubeRadius * cosV) * sinU
            );
            
            Vec3 normal(
                cosV * cosU,
                sinV,
                cosV * sinU
            );
            
            Vec2 uv(
                (float)j / tubularSegments,
                (float)i / radialSegments
            );
            
            mesh.vertices.push_back({position, normal, uv});
        }
    }
    
    for (int i = 0; i < radialSegments; i++) {
        for (int j = 0; j < tubularSegments; j++) {
            int current = i * (tubularSegments + 1) + j;
            int next = current + tubularSegments + 1;
            
            // Counter-clockwise winding
            mesh.indices.push_back(current);
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next);
            
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next + 1);
            mesh.indices.push_back(next);
        }
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Organic;
    return mesh;
}

Mesh Primitive::createPlane(float width, float depth, int segmentsX, int segmentsZ) {
    if (segmentsX < 1) segmentsX = 1;
    if (segmentsZ < 1) segmentsZ = 1;
    
    Mesh mesh;
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    
    for (int z = 0; z <= segmentsZ; z++) {
        float zPos = -halfD + z * (depth / segmentsZ);
        for (int x = 0; x <= segmentsX; x++) {
            float xPos = -halfW + x * (width / segmentsX);
            
            mesh.vertices.push_back({
                Vec3(xPos, 0, zPos),
                Vec3(0, 1, 0),
                Vec2((float)x / segmentsX, (float)z / segmentsZ)
            });
        }
    }
    
    for (int z = 0; z < segmentsZ; z++) {
        for (int x = 0; x < segmentsX; x++) {
            int current = z * (segmentsX + 1) + x;
            int next = current + segmentsX + 1;
            
            // Counter-clockwise winding when viewed from above
            mesh.indices.push_back(current);
            mesh.indices.push_back(next);
            mesh.indices.push_back(current + 1);
            
            mesh.indices.push_back(current + 1);
            mesh.indices.push_back(next);
            mesh.indices.push_back(next + 1);
        }
    }
    
    mesh.calculateBounds();
    mesh.type = Mesh::MeshType::Primitive;
    return mesh;
}

Mesh Primitive::createGrid(int size, float spacing) {
    Mesh mesh;
    // Grid is typically rendered as lines, not triangles
    // This is a placeholder - you'd use line rendering for this
    return mesh;
}

Mesh Primitive::createUVSphere(float radius, int rings, int sectors) {
    return createSphere(radius, std::max(rings, sectors));
}

Mesh Primitive::createIcosphere(float radius, int subdivisions) {
    int segments = 4 * (1 << subdivisions);
    return createSphere(radius, segments);
}

void Primitive::addVertex(std::vector<Vertex>& vertices, const Vec3& pos, const Vec3& normal, const Vec2& uv) {
    vertices.push_back({pos, normal, uv});
}

void Primitive::addIndex(std::vector<uint32_t>& indices, uint32_t i1, uint32_t i2, uint32_t i3) {
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

} // namespace Xangine