/**
 * @file Primitives.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "Primitives.h"
#include "Components.h"

primitives::PrimitiveType primitives::objectCube()
{
    // Positions
    const auto rtb = glm::vec3( 0.5f,  0.5f,  0.5f);
    const auto ltb = glm::vec3(-0.5f,  0.5f,  0.5f);
    const auto ltf = glm::vec3(-0.5f,  0.5f, -0.5f);
    const auto rtf = glm::vec3( 0.5f,  0.5f, -0.5f);
    const auto rbb = glm::vec3( 0.5f, -0.5f,  0.5f);
    const auto lbb = glm::vec3(-0.5f, -0.5f,  0.5f);
    const auto lbf = glm::vec3(-0.5f, -0.5f, -0.5f);
    const auto rbf = glm::vec3( 0.5f, -0.5f, -0.5f);
    
    // UV Coords.
    const auto bottomLeft   = glm::vec2(0.f, 0.f);
    const auto bottomRight  = glm::vec2(1.f, 0.f);
    const auto topRight     = glm::vec2(1.f, 1.f);
    const auto topLeft      = glm::vec2(0.f, 1.f);
    
    typedef ObjVertex VertexType;
    const std::vector<VertexType> objectVertices {
        VertexType{ ltb, bottomRight }, VertexType{ rtb, bottomLeft }, VertexType{ rtf, topLeft }, VertexType{ ltf, topRight },  // Top Face
        VertexType{ rbb, bottomRight }, VertexType{ rtb, bottomLeft }, VertexType{ ltb, topLeft }, VertexType{ lbb, topRight },  // Back Face
        VertexType{ rtf, bottomRight }, VertexType{ rtb, bottomLeft }, VertexType{ rbb, topLeft }, VertexType{ rbf, topRight },  // Right Face
        VertexType{ ltf, bottomRight }, VertexType{ lbf, bottomLeft }, VertexType{ lbb, topLeft }, VertexType{ ltb, topRight },  // Left Face
        VertexType{ rbf, bottomRight }, VertexType{ lbf, bottomLeft }, VertexType{ ltf, topLeft }, VertexType{ rtf, topRight },  // Front Face
        VertexType{ lbb, bottomRight }, VertexType{ lbf, bottomLeft }, VertexType{ rbf, topLeft }, VertexType{ rbb, topRight },  // Bottom Face
    };
    
    const std::vector<unsigned int> indices {
        0,  1,  2,  0,  2,  3,  // Top Face
        4,  5,  6,  4,  6,  7,  // Back Face
        8,  9, 10,  8, 10, 11,  // Right Face
        12, 13, 14, 12, 14, 15,  // Left Face
        16, 17, 18, 16, 18, 19,  // Front Face
        20, 21, 22, 20, 22, 23   // Bottom Face
    };
    return { std::move(objectVertices), std::move(indices) };
}

primitives::PrimitiveType primitives::objectPlane()
{
    const auto tl = glm::vec3(-1.f,  1.f,  0.f);
    const auto tr = glm::vec3( 1.f,  1.f,  0.f);
    const auto bl = glm::vec3(-1.f, -1.f,  0.f);
    const auto br = glm::vec3( 1.f, -1.f,  0.f);
    
    // UV Coords.
    const auto bottomLeft   = glm::vec2(0.f, 0.f);
    const auto bottomRight  = glm::vec2(1.f, 0.f);
    const auto topRight     = glm::vec2(1.f, 1.f);
    const auto topLeft      = glm::vec2(0.f, 1.f);
    
    const std::vector<ObjVertex> vertices {
        ObjVertex { tl, topLeft    },   ObjVertex { tr, topRight    },
        ObjVertex { bl, bottomLeft },   ObjVertex { br, bottomRight }
    };
    
    const std::vector<unsigned int> indices { 0, 2, 1, 2, 3, 1 };
    
    return { std::move(vertices), std::move(indices) };
}

primitives::PrimitiveType primitives::objectTriangleFanCircle(unsigned int points)
{
    if (points < 4)
    {
        debug::log("You must have more than 4 points to create a triangle fan circle.", debug::severity::Minor);
        return {};
    }
    
    std::vector<ObjVertex> vertices;
    vertices.reserve(points);
    
    const float angleDelta = glm::two_pi<float>() / static_cast<float>(points);
    float angle = 0.f;
    
    while (angle < glm::two_pi<float>())
    {
        // Assuming that NDC are [-1, 1] and UVs are [0, 1]
        const glm::vec2 position { glm::cos(angle), glm::sin(angle) };
        const glm::vec2 uvs { (position + 1.f) / 2.f };
        vertices.emplace_back(ObjVertex { glm::vec3(position, 0.f), uvs });
        
        angle += angleDelta;
    }
    
    std::vector<unsigned int> indices;
    indices.reserve(3 * points);
    
    for (int i = 0; i < static_cast<int>(points) - 2; ++i)
    {
        indices.emplace_back(0);  // Our anchor.
        indices.emplace_back(i + 1);
        indices.emplace_back(i + 2);
    }
    return { std::move(vertices), std::move(indices) };
}
