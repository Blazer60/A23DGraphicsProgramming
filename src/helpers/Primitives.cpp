/**
 * @file Primitives.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "Primitives.h"
#include "Components.h"

SharedMesh primitives::basicCube()
{
    const auto rtb = glm::vec3( 0.5f,  0.5f,  0.5f);
    const auto ltb = glm::vec3(-0.5f,  0.5f,  0.5f);
    const auto ltf = glm::vec3(-0.5f,  0.5f, -0.5f);
    const auto rtf = glm::vec3( 0.5f,  0.5f, -0.5f);
    const auto rbb = glm::vec3( 0.5f, -0.5f,  0.5f);
    const auto lbb = glm::vec3(-0.5f, -0.5f,  0.5f);
    const auto lbf = glm::vec3(-0.5f, -0.5f, -0.5f);
    const auto rbf = glm::vec3( 0.5f, -0.5f, -0.5f);
    
    const auto white  = glm::vec3(1.f, 1.f, 1.f);
    const auto green  = glm::vec3(0.f, 0.59f, 0.27f);
    const auto red    = glm::vec3(0.72f, 0.04f, 0.19f);
    const auto blue   = glm::vec3(0.f, 0.27f, 0.68f);
    const auto orange = glm::vec3(1.f, 0.34f, 0.f);
    const auto yellow = glm::vec3(1.f, 0.84f, 0.f);
    
    std::vector<BasicVertex> vertices {
            BasicVertex{ ltb, white }, BasicVertex{ rtb, white }, BasicVertex{ rtf, white }, BasicVertex{ ltf, white },  // Top Face
            BasicVertex{ rbb, green }, BasicVertex{ rtb, green }, BasicVertex{ ltb, green }, BasicVertex{ lbb, green },  // Back Face
            BasicVertex{ rtf, red }, BasicVertex{ rtb, red }, BasicVertex{ rbb, red }, BasicVertex{ rbf, red },  // Right Face
            BasicVertex{ ltf, blue }, BasicVertex{ lbf, blue }, BasicVertex{ lbb, blue }, BasicVertex{ ltb, blue },  // Left Face
            BasicVertex{ rbf, orange }, BasicVertex{ lbf, orange }, BasicVertex{ ltf, orange }, BasicVertex{ rtf, orange },  // Front Face
            BasicVertex{ lbb, yellow }, BasicVertex{ lbf, yellow }, BasicVertex{ rbf, yellow }, BasicVertex{ rbb, yellow },  // Bottom Face
    };
    
    std::vector<unsigned int> indices {
            0,  1,  2,  0,  2,  3,  // Top Face
            4,  5,  6,  4,  6,  7,  // Back Face
            8,  9, 10,  8, 10, 11,  // Right Face
            12, 13, 14, 12, 14, 15,  // Left Face
            16, 17, 18, 16, 18, 19,  // Front Face
            20, 21, 22, 20, 22, 23   // Bottom Face
    };
    
    return std::make_shared<Mesh<BasicVertex>>(vertices, indices);
}

SharedMesh primitives::basicTriangle()
{
    const auto red = glm::vec3(1.f, 0.f, 0.f);
    std::vector<BasicVertex> vertices {
            BasicVertex { 2.f * glm::vec3( 0.0f,   0.5f,  0.0f), red },
            BasicVertex { 2.f * glm::vec3(-0.5f,  -0.5f,  0.0f), red },
            BasicVertex { 2.f * glm::vec3( 0.5f,  -0.5f,  0.0f), red },
    };
    
    std::vector<unsigned int> indices { 0, 2, 1, 0, 1, 2 };
    return std::make_shared<Mesh<BasicVertex>>(std::move(vertices), std::move(indices));
}

SharedMesh primitives::uvCube()
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
    
    
    std::vector<UvVertex> vertices {
            UvVertex{ ltb, bottomRight }, UvVertex{ rtb, bottomLeft }, UvVertex{ rtf, topLeft }, UvVertex{ ltf, topRight },  // Top Face
            UvVertex{ rbb, bottomRight }, UvVertex{ rtb, bottomLeft }, UvVertex{ ltb, topLeft }, UvVertex{ lbb, topRight },  // Back Face
            UvVertex{ rtf, bottomRight }, UvVertex{ rtb, bottomLeft }, UvVertex{ rbb, topLeft }, UvVertex{ rbf, topRight },  // Right Face
            UvVertex{ ltf, bottomRight }, UvVertex{ lbf, bottomLeft }, UvVertex{ lbb, topLeft }, UvVertex{ ltb, topRight },  // Left Face
            UvVertex{ rbf, bottomRight }, UvVertex{ lbf, bottomLeft }, UvVertex{ ltf, topLeft }, UvVertex{ rtf, topRight },  // Front Face
            UvVertex{ lbb, bottomRight }, UvVertex{ lbf, bottomLeft }, UvVertex{ rbf, topLeft }, UvVertex{ rbb, topRight },  // Bottom Face
    };
    
    std::vector<unsigned int> indices {
            0,  1,  2,  0,  2,  3,  // Top Face
            4,  5,  6,  4,  6,  7,  // Back Face
            8,  9, 10,  8, 10, 11,  // Right Face
            12, 13, 14, 12, 14, 15,  // Left Face
            16, 17, 18, 16, 18, 19,  // Front Face
            20, 21, 22, 20, 22, 23   // Bottom Face
    };
    return std::make_shared<Mesh<UvVertex>>(std::move(vertices), std::move(indices));
}

SharedMesh primitives::uvPlane()
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
    
    std::vector<UvVertex> vertices {
        UvVertex{ tl, topLeft }, UvVertex{ tr, topRight },
        UvVertex{ bl, bottomLeft }, UvVertex{ br, bottomRight }
    };
    
    std::vector<unsigned int> indices {
        0, 2, 1, 2, 3, 1
    };
    
    return std::make_shared<Mesh<UvVertex>>(vertices, indices);
}
