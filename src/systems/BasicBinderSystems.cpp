/**
 * @file BasicMeshBinderSystem.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "BasicBinderSystems.h"

#include <glew.h>

void BasicMeshBinderSystem::onUpdate()
{
    // todo: Make an on start method and get the difference between onEach in onStart and onUpdate.
    // todo: Make onUpdate set forEach to nothing.
    // todo: You could have another function that only runs once before update.
    std::cout << "Generating Vbo and Ebo\n";
    mEntities.forEach([](const std::shared_ptr<BasicMesh> &mesh, Vbo &vbo, Ebo &ebo) {
        glCreateBuffers(1, &vbo.id);
        glCreateBuffers(1, &ebo.id);
        glNamedBufferData(vbo.id, mesh->vertices.size() * sizeof(BasicVertex), &mesh->vertices[0], GL_STATIC_DRAW);
        glNamedBufferData(ebo.id, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
    });
}


void BasicVaoBinderSystem::onUpdate()
{
    std::cout << "Generating Vao\n";
    mEntities.forEach([](Vao &vao, Vbo &vbo, Ebo &ebo){
        glCreateVertexArrays(1, &vao.id);
        
        const int bindingIndex = 0;
        const int offSet = 0;
        const int stride = sizeof(BasicVertex);
        
        glEnableVertexArrayAttrib(vao.id, 0);
        glVertexArrayAttribBinding(vao.id, 0, bindingIndex);
        // vao, attrib index, count, type, normalized, offset
        glVertexArrayAttribFormat(vao.id, 0, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, position));
        
        glEnableVertexArrayAttrib(vao.id, 1);
        glVertexArrayAttribBinding(vao.id, 1, bindingIndex);
        glVertexArrayAttribFormat(vao.id, 1, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, colour));
        
        glVertexArrayVertexBuffer(vao.id, bindingIndex, vbo.id, offSet, stride);
        glVertexArrayElementBuffer(vao.id, ebo.id);
    });
}
