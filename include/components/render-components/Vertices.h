/**
 * @file MeshComponents.h
 * @author Ryan Purse
 * @date 21/02/2022
 */


#pragma once

#include "Pch.h"
#include "glm.hpp"

#include <vector>
#include <memory>

/**
 * @brief An instruction that defines how a single element should be setup with the vao.
 */
struct Instruction
{
    GLenum type;
    int count;
};

typedef std::vector<Instruction> Instructions;

/** A Base Vertex to inherit your own vertex from. */
struct Vertex {};

/**
 * Contains a position, uv coords and normal. Loaded in from .obj files.
 * Define a conversion constructor to transition from this type.
 */
struct ObjVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

/**
 * @brief Contains position and colour.
 */
struct BasicVertex
        : public Vertex
{
    BasicVertex() = default;
    
    constexpr explicit BasicVertex(const glm::vec3 &_position, const glm::vec3 &_colour=glm::vec3(1.f))
        : position(_position), colour(_colour) {}
        
    constexpr explicit BasicVertex(const ObjVertex &objVertex)
        : position(objVertex.position), colour(glm::vec3(1.f, 0.f, 1.f)) {}
    
    static Instructions instructions()
    {
        return {
            { GL_FLOAT, 3 },
            { GL_FLOAT, 3 },
        };
    }
    
    glm::vec3 position  { 0.f };
    glm::vec3 colour    { 0.f };
};

/**
 * @brief Contains position and uvs.
 */
struct UvVertex
        : public Vertex
{
    UvVertex() = default;
    
    constexpr UvVertex(const glm::vec3 &_position, const glm::vec2 &_uv)
        : position(_position), uv(_uv) {}
        
    constexpr explicit UvVertex(const ObjVertex &objVertex)
        : position(objVertex.position), uv(objVertex.uv) {}
    
    static Instructions instructions()
    {
        return {
            { GL_FLOAT, 3 },
            { GL_FLOAT, 2 },
        };
    }
    
    glm::vec3 position  { 0.f };
    glm::vec2 uv        { 0.f };
};

/** Contains positions, uvs and normals. */
struct PhongVertex
        : public Vertex
{
    PhongVertex() = default;
    
    constexpr PhongVertex(const glm::vec3 &_position, const glm::vec2 &_uv, const glm::vec3 &_normal)
        : position(_position), uv(_uv), normal(_normal) {}
    
    constexpr explicit PhongVertex(const ObjVertex &objVertex)
        : position(objVertex.position), uv(objVertex.uv), normal(objVertex.normal) {}
    
    static Instructions instructions()
    {
        return {
            { GL_FLOAT, 3 },
            { GL_FLOAT, 2 },
            { GL_FLOAT, 3 },
        };
    }
        
    glm::vec3 position  { 0.f };
    glm::vec2 uv        { 0.f };
    glm::vec3 normal    { 0.f };
};
