/**
 * @file BasicMeshBinderSystem.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "BasicBinderSystems.h"

#include <glew.h>

void BasicMeshBinderSystem::onStart()
{
    std::cout << "Generating Vbo and Ebo\n";
    mEntities.forEach([](const std::shared_ptr<BasicMesh> &mesh, Vbo &vbo, Ebo &ebo, EboCount &eboCount) {
        glCreateBuffers(1, &vbo.id);
        glCreateBuffers(1, &ebo.id);
        glNamedBufferData(vbo.id, mesh->vertices.size() * sizeof(BasicVertex), &mesh->vertices[0], GL_STATIC_DRAW);
        glNamedBufferData(ebo.id, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
        eboCount.count = mesh->indices.size();
    });
}

void BasicMeshBinderSystem::onUpdate()
{
    mEntities.forEach([](const std::shared_ptr<BasicMesh> &mesh, Vbo &vbo, Ebo &ebo, EboCount &eboCount) {
    
    });
}


void BasicVaoBinderSystem::onStart()
{
    std::cout << "Generating Vao\n";
    mEntities.forEach([](Vao &vao, Vbo &vbo, Ebo &ebo) {
        glCreateVertexArrays(1, &vao.id);
        
        const unsigned int bindingIndex = 0;
        const unsigned int offSet = 0;
        const unsigned int stride = sizeof(BasicVertex);
        
        glEnableVertexArrayAttrib(vao.id, 0);
        // vao, attrib index, count, type, normalized, offset
        glVertexArrayAttribFormat(vao.id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, position));
        glVertexArrayAttribBinding(vao.id, 0, bindingIndex);
        
        glEnableVertexArrayAttrib(vao.id, 1);
        glVertexArrayAttribFormat(vao.id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, colour));
        glVertexArrayAttribBinding(vao.id, 1, bindingIndex);
        
        glVertexArrayVertexBuffer(vao.id, bindingIndex, vbo.id, offSet, stride);
        glVertexArrayElementBuffer(vao.id, ebo.id);
    });
}

void BasicVaoBinderSystem::onUpdate()
{
    mEntities.forEach([](Vao &vao, Vbo &vbo, Ebo &ebo) {
    
    });
}
