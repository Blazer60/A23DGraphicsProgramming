/**
 * @file Components.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include <utility>
#include <vector>
#include "glm.hpp"
#include "detail/type_quat.hpp"
#include <filesystem>

/** Core elements used for rendering an item to the screen by OpenGL. ONLY items that never change each frame. E.g.: No uniforms */
struct RenderCoreElements;

/** Vertex Buffer Object. Stores vertex data on the GPU. */
struct Vbo;

/** Element Buffer Object. Tells the GPU what order each vertex should be rendered. */
struct Ebo;

/** Holds the position, orientation and scale of an object. Note: this should be split to improve cache miss optimisation. */
struct Transform;

/** Holds information about how to rotate an object. */
struct Rotator;

/** Uniforms used when dealing with a basic mesh */
struct BasicUniforms;

/** A basic vertex */
struct BasicVertex;

/** A vertex that stores UV information */
struct UvVertex;

/** A mesh that holds a specific type of vertex */
template<typename Vertex>
struct Mesh;

struct RenderCoreElements
{
    unsigned int    vao      { 0 };
    unsigned int    fbo      { 0 };
    int             eboCount { 0 };
    // Matrix Uniforms are not included since they need to be updated every frame.
};

struct Vbo
{
    unsigned int id;
};

struct Ebo
{
    unsigned int id;
};

struct Transform
{
    glm::vec3 position { 0.f };
    glm::quat rotation { glm::vec3(0.f) };
    glm::vec3 scale    { 1.f };
};

struct Rotator
{
    float       time            { 0.f };
    float       height          { 0.f };
    glm::vec3   originalHeight  { 0.f };
};

struct BasicUniforms
{
    glm::mat4 modelMat { 1.f };
};

struct UvUniforms
{
    glm::vec3 colour { 1.f };
};

struct Texture
{
    unsigned int id;
};

struct TexturePath
{
    explicit TexturePath(std::filesystem::path filePath)
        : path(std::move(filePath)) { }
        
    std::filesystem::path path;
};


struct BasicVertex
{
    glm::vec3 position  { 0.f };
    glm::vec3 colour    { 0.f };
};

struct UvVertex
{
    glm::vec3 position  { 0.f };
    glm::vec2 uv        { 0.f };
};

struct IMesh
{
    IMesh() = default;
    explicit IMesh(std::vector<unsigned int> i)
        : indices(std::move(i)) {}
    
    std::vector<unsigned int>   indices;
    
    [[nodiscard]] int64_t indicesCount() const { return static_cast<int64_t>(indices.size()); };
    [[nodiscard]] virtual int64_t verticesCount() const = 0;
    [[nodiscard]] const void *indicesData() const { return static_cast<const void *>(&indices[0]); }
    [[nodiscard]] virtual const void *verticesData() const = 0;
};

template<typename Vertex>
struct Mesh
        : public IMesh
{
    Mesh() = default;
    Mesh(std::vector<Vertex> v, std::vector<unsigned int> i)
        : IMesh(std::move(i)),
          vertices(std::move(v)) {}
    
    std::vector<Vertex> vertices;
    
    [[nodiscard]] int64_t verticesCount() const override { return static_cast<int64_t>(vertices.size()); }
    [[nodiscard]] const void *verticesData() const override { return static_cast<const void *>(&vertices[0]); };
};

typedef std::shared_ptr<IMesh>              SharedMesh;
