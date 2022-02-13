/**
 * @file Primitives.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "Primitives.h"

std::shared_ptr<BasicMesh> primitives::basicCube()
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
    
    return std::make_shared<BasicMesh>(vertices, indices);
}

std::shared_ptr<BasicMesh> primitives::basicTriangle()
{
    const auto red = glm::vec3(1.f, 0.f, 0.f);
    std::vector<BasicVertex> vertices {
            BasicVertex { glm::vec3( 0.0f,   0.5f,  0.0f), red },
            BasicVertex { glm::vec3(-0.5f,  -0.5f,  0.0f), red },
            BasicVertex { glm::vec3( 0.5f,  -0.5f,  0.0f), red },
    };
    
    std::vector<unsigned int> indices { 0, 2, 1, 0, 1, 2 };
    return std::make_shared<BasicMesh>(vertices, indices);
}
