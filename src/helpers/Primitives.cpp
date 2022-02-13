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
    
    const auto red = glm::vec3(1.f, 0.f, 0.f);
    
    std::vector<BasicVertex> vertices {
            BasicVertex{ ltb, red }, BasicVertex{ rtb, red }, BasicVertex{ rtf, red }, BasicVertex{ ltf, red },  // Top Face
            BasicVertex{ rbb, red }, BasicVertex{ rtb, red }, BasicVertex{ ltb, red }, BasicVertex{ lbb, red },  // Back Face
            BasicVertex{ rtf, red }, BasicVertex{ rtb, red }, BasicVertex{ rbb, red }, BasicVertex{ rbf, red },  // Right Face
            BasicVertex{ ltf, red }, BasicVertex{ lbf, red }, BasicVertex{ lbb, red }, BasicVertex{ ltb, red },  // Left Face
            BasicVertex{ rbf, red }, BasicVertex{ lbf, red }, BasicVertex{ ltf, red }, BasicVertex{ rtf, red },  // Front Face
            BasicVertex{ lbb, red }, BasicVertex{ lbf, red }, BasicVertex{ rbf, red }, BasicVertex{ rbb, red },  // Bottom Face
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
