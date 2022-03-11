/**
 * @file MeshComponents.h
 * @author Ryan Purse
 * @date 21/02/2022
 */


#pragma once

#include "glm.hpp"

#include <vector>
#include <memory>

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
        
    glm::vec3 position  { 0.f };
    glm::vec2 uv        { 0.f };
    glm::vec3 normal    { 0.f };
};

/**
 * @brief An interface for the mesh class so that systems can get useful
 * information without knowing the type of Vertex it's working with.
 */
class IMesh
{
public:
    IMesh() = default;
    explicit IMesh(std::vector<unsigned int> i)
        : indices(std::move(i)) {}
        
    /**
     * @returns The number of indices assigned to this mesh.
     */
    [[nodiscard]] int64_t indicesCount() const { return static_cast<int64_t>(indices.size()); };
    
    /**
     * @returns A pointer to the start of indices. Used by OpenGL to submit data.
     */
    [[nodiscard]] const void *indicesData() const { return static_cast<const void *>(&indices[0]); }
    
    /**
     * @returns The number of vertices assigned to this mesh.
     */
    [[nodiscard]] virtual int64_t verticesCount() const = 0;
    
    /**
     * @returns A pointer to the start of vertices. Used by OpenGL to submit data.
     */
    [[nodiscard]] virtual const void *verticesData() const = 0;
    
    // public since it's still a component. Helper functions are just most likely used.
    std::vector<unsigned int> indices;
};

template<typename Vertex>
class Mesh
        : public IMesh
{
public:
    Mesh() = default;
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i)
            : IMesh(std::move(i)), vertices(std::move(v)) {}
    
    /**
     * @returns The number of vertices assigned to this mesh.
     */
    [[nodiscard]] int64_t verticesCount() const final { return static_cast<int64_t>(vertices.size()); }
    
    /**
     * @returns A pointer to the start of vertices. Used by OpenGL to submit data.
     */
    [[nodiscard]] const void *verticesData() const final { return static_cast<const void *>(&vertices[0]); };
    
    std::vector<Vertex> vertices;
};

typedef std::shared_ptr<IMesh> SharedMesh;
typedef std::unique_ptr<IMesh> UniqueMesh;
