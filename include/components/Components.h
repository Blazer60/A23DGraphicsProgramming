/**
 * @file Components.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

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

struct Vertex {};

struct BasicVertex
        : Vertex
{
    glm::vec3 position  { 0.f };
    glm::vec3 colour    { 0.f };
};

struct Mesh {};

struct BasicMesh
        : Mesh
{
    std::vector<BasicVertex>    vertices;
    std::vector<unsigned int>   indices;
};
