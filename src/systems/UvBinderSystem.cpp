/**
 * @file UvBinderSystem.cpp
 * @author Ryan Purse
 * @date 15/02/2022
 */


#include "UvBinderSystem.h"

#include <glew.h>

void UvBinderSystem::onStart()
{
    std::cout << "Generating Uv Core Elements for UV Vertex\n";
    mEntities.forEach([](RenderCoreElements &coreElements, Vbo &vbo, Ebo &ebo, const UvSharedMesh &mesh) {
        using VertexSpecification = UvVertex;
        
        glCreateBuffers(1, &vbo.id);
        glCreateBuffers(1, &ebo.id);
        glNamedBufferData(vbo.id, mesh->vertices.size() * sizeof(VertexSpecification), &mesh->vertices[0], GL_STATIC_DRAW);
        glNamedBufferData(ebo.id, mesh->indices.size() * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
        coreElements.eboCount = mesh->indices.size();
        
        glCreateVertexArrays(1, &coreElements.vao);
    
        const unsigned int bindingIndex = 0;
        const unsigned int offSet = 0;
        const unsigned int stride = sizeof(VertexSpecification);
    
        glEnableVertexArrayAttrib(coreElements.vao, 0);
        // vao, attrib index, count, type, normalized, offset
        glVertexArrayAttribFormat(coreElements.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexSpecification, position));
        glVertexArrayAttribBinding(coreElements.vao, 0, bindingIndex);
    
        glEnableVertexArrayAttrib(coreElements.vao, 1);
        glVertexArrayAttribFormat(coreElements.vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexSpecification, uv));
        glVertexArrayAttribBinding(coreElements.vao, 1, bindingIndex);
    
        glVertexArrayVertexBuffer(coreElements.vao, bindingIndex, vbo.id, offSet, stride);
        glVertexArrayElementBuffer(coreElements.vao, ebo.id);
    });
}

void UvBinderSystem::onUpdate()
{
    mEntities.forEach([](RenderCoreElements &coreElements, Vbo &vbo, Ebo &ebo, const UvSharedMesh &mesh) {
    
    });
}
