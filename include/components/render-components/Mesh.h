/**
 * @file Mesh.h
 * @author Ryan Purse
 * @date 14/03/2022
 */


#pragma once


#include "glew.h"
#include "RawMesh.h"

#include <cstdint>
#include <vector>

void setVaoLayout(unsigned int vao, const Instructions &instructions);

/**
 * @brief The returned information for rendering an object to the screen with OpenGL.
 * @tparam TMaterial - The type of material that you want to store.
 * @author Ryan Purse
 * @date 14/03/2022
 */
template<typename TVertex, typename TMaterial>
struct Mesh
{
    Mesh(const std::vector<TVertex> &vertices, const std::vector<uint32_t> &indices, TMaterial mat);
    Mesh(const load::RawMesh<TVertex> &mesh, TMaterial mat);
    
    RenderInformation   renderInformation;
    TMaterial           material;
    unsigned int        vbo { 0 };
    unsigned int        ebo { 0 };
};

template<typename TVertex, typename TMaterial>
using Model = std::vector<Mesh<TVertex, TMaterial>>;


template<typename TVertex, typename TMaterial>
Mesh<TVertex, TMaterial>::Mesh(const std::vector<TVertex> &vertices, const std::vector<uint32_t> &indices,
                               TMaterial mat)
    : material(std::move(mat))
{
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);
    glNamedBufferData(vbo, vertices.size() * sizeof(TVertex), static_cast<const void *>(&vertices[0]), GL_STATIC_DRAW);
    glNamedBufferData(ebo, indices.size() * sizeof(uint32_t), static_cast<const void *>(&indices[0]), GL_STATIC_DRAW);
    renderInformation.eboCount = indices.size();
    
    glCreateVertexArrays(1, &renderInformation.vao);
    
    const unsigned int bindingIndex = 0;
    const unsigned int offSet = 0;
    const unsigned int stride = sizeof(TVertex);
    
    setVaoLayout(renderInformation.vao, TVertex::instructions());
    
    glVertexArrayVertexBuffer(renderInformation.vao, bindingIndex, vbo, offSet, stride);
    glVertexArrayElementBuffer(renderInformation.vao, ebo);
}

template<typename TVertex, typename TMaterial>
Mesh<TVertex, TMaterial>::Mesh(const load::RawMesh<TVertex> &mesh, TMaterial mat)
    : material(std::move(mat))
{
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);
    glNamedBufferData(vbo, mesh.verticesSize(), mesh.verticesData(), GL_STATIC_DRAW);
    glNamedBufferData(ebo, mesh.indicesSize(), mesh.indicesData(), GL_STATIC_DRAW);
    renderInformation.eboCount = mesh.indicesCount();
    
    glCreateVertexArrays(1, &renderInformation.vao);
    
    const unsigned int bindingIndex = 0;
    const unsigned int offSet = 0;
    const unsigned int stride = sizeof(TVertex);
    
    setVaoLayout(renderInformation.vao, TVertex::instructions());
    
    glVertexArrayVertexBuffer(renderInformation.vao, bindingIndex, vbo, offSet, stride);
    glVertexArrayElementBuffer(renderInformation.vao, ebo);
}


