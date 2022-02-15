/**
 * @file BasicMeshBinderSystem.cpp
 * @author Ryan Purse
 * @date 12/02/2022
 */


#include "BasicBinderSystem.h"

#include <glew.h>


void BasicBinderSystem::onStart()
{
    std::cout << "Generating Vao\n";
    mEntities.forEach([](RenderCoreElements &coreElements, Vbo &vbo, Ebo &ebo, const std::shared_ptr<BasicMesh> &mesh) {
        glCreateBuffers(1, &vbo.id);
        glCreateBuffers(1, &ebo.id);
        glNamedBufferData(vbo.id, mesh->vertices.size() * sizeof(BasicVertex), &mesh->vertices[0], GL_STATIC_DRAW);
        glNamedBufferData(ebo.id, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
        coreElements.eboCount = mesh->indices.size();
        
        glCreateVertexArrays(1, &coreElements.vao);
        
        const unsigned int bindingIndex = 0;
        const unsigned int offSet = 0;
        const unsigned int stride = sizeof(BasicVertex);
        
        glEnableVertexArrayAttrib(coreElements.vao, 0);
        // vao, attrib index, count, type, normalized, offset
        glVertexArrayAttribFormat(coreElements.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, position));
        glVertexArrayAttribBinding(coreElements.vao, 0, bindingIndex);
        
        glEnableVertexArrayAttrib(coreElements.vao, 1);
        glVertexArrayAttribFormat(coreElements.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(BasicVertex, colour));
        glVertexArrayAttribBinding(coreElements.vao, 1, bindingIndex);
        
        glVertexArrayVertexBuffer(coreElements.vao, bindingIndex, vbo.id, offSet, stride);
        glVertexArrayElementBuffer(coreElements.vao, ebo.id);
    });
}

void BasicBinderSystem::onUpdate()
{
    mEntities.forEach([](RenderCoreElements &_, Vbo &vbo, Ebo &ebo, const std::shared_ptr<BasicMesh> &mesh) {
    
    });
}
