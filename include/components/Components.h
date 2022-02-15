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

struct Vao
{
    unsigned int id;
};

struct Vbo
{
    unsigned int id;
};

struct Ebo
{
    unsigned int id;
};

struct EboCount
{
    int count;
};

struct Fbo
{
    unsigned int id { 0 };
};

struct Transform
{
    glm::vec3 position { 0.f };
    glm::quat rotation { glm::vec3(0.f) };
    glm::vec3 scale    { 1.f };
};

struct Rotator
{
    float time { 0.f };
    float height { 0.f };
    glm::vec3 originalHeight { 0.f };
};

struct BasicUniforms
{
    glm::mat4 modelMat { 1.f };
};

// struct Vertex {};

struct BasicVertex
        // : Vertex
{
    glm::vec3 position  { 0.f };
    glm::vec3 colour    { 0.f };
};

struct BasicMesh
{
    BasicMesh() = default;
    BasicMesh(std::vector<BasicVertex> v, std::vector<unsigned int> i)
        : vertices(std::move(v)), indices(std::move(i)) {}
    
    std::vector<BasicVertex>    vertices;
    std::vector<unsigned int>   indices;
};
