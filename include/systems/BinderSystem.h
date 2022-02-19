/**
 * @file BinderSystem.h
 * @author Ryan Purse
 * @date 18/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

#include <typeinfo>

namespace vaoImpl
{
    struct Instruction
    {
        GLenum type;
        int count;
    };
    
    typedef std::vector<Instruction> Instructions;
    
    void layout(unsigned int vao, const Instructions &instructions);
}

/**
 * Specifies how a Vao should be set up based on the Vertex given to it.
 * @author Ryan Purse
 * @date 18/02/2022
 */
template<typename VertexSpecification>
class BinderSystem
        : public ecs::BaseSystem<RenderCoreElements, Vbo, Ebo, std::shared_ptr<IMesh>>
{
public:
    BinderSystem();
    void onStart() override;
    
    void onUpdate() override;
    
    void setVertexArrayLayout(unsigned int vao);
};


template<typename VertexSpecification>
BinderSystem<VertexSpecification>::BinderSystem()
{
    mEntities.forEach([this](RenderCoreElements &coreElements, Vbo &vbo, Ebo &ebo, std::shared_ptr<IMesh> &mesh) {
        glCreateBuffers(1, &vbo.id);
        glCreateBuffers(1, &ebo.id);
        glNamedBufferData(vbo.id, mesh->verticesCount() * sizeof(VertexSpecification), mesh->verticesData(), GL_STATIC_DRAW);
        glNamedBufferData(ebo.id, mesh->indices.size() * sizeof(unsigned int), mesh->indicesData(), GL_STATIC_DRAW);
        coreElements.eboCount = mesh->indices.size();
        
        glCreateVertexArrays(1, &coreElements.vao);
        
        const unsigned int bindingIndex = 0;
        const unsigned int offSet = 0;
        const unsigned int stride = sizeof(VertexSpecification);
        
        setVertexArrayLayout(coreElements.vao);
        
        glVertexArrayVertexBuffer(coreElements.vao, bindingIndex, vbo.id, offSet, stride);
        glVertexArrayElementBuffer(coreElements.vao, ebo.id);
    });
}

template<typename VertexSpecification>
void BinderSystem<VertexSpecification>::onStart()
{

}

template<typename VertexSpecification>
void BinderSystem<VertexSpecification>::onUpdate()
{

}

template<typename VertexSpecification>
void BinderSystem<VertexSpecification>::setVertexArrayLayout(unsigned int vao)
{
    if (typeid(VertexSpecification) == typeid(BasicVertex))
        vaoImpl::layout(vao, {
            { GL_FLOAT, 3 },
            { GL_FLOAT, 3 },
        });
    else if (typeid(VertexSpecification) == typeid(UvVertex))
        vaoImpl::layout(vao, {
            { GL_FLOAT, 3 },
            { GL_FLOAT, 2 },
        });
    else
        debug::log("Vertex type does not have an instruction set.", debug::severity::Fatal);
}
