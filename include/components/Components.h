/**
 * @file Components.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include <utility>
#include <vector>
#include "glm.hpp"

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
    unsigned int count;
};

struct Fbo
{
    unsigned int id;
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