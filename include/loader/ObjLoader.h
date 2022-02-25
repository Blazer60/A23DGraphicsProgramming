/**
 * @file ObjLoader.h
 * @author Ryan Purse
 * @date 23/02/2022
 */


#pragma once

#include <functional>
#include "MeshComponents.h"

struct ObjVertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct ObjMesh
{
    std::vector<ObjVertex> vertices;
    std::vector<uint32_t>  indices;
};

typedef std::function<void(const ObjVertex&)> GenVertexSignature;
typedef std::function<uint32_t()> GetSizeSignature;

/**
 * @brief Loads an .obj file.
 * @param path - The path to the file that you want to load.
 */
[[nodiscard]] std::vector<uint32_t>
loadObj(std::string_view path, const GenVertexSignature &genVertexDelegate, const GetSizeSignature &getSizeDelegate);

Mesh<UvVertex> loadObjUvVertex(std::string_view path);
Mesh<BasicVertex> loadObjBasicVertex(std::string_view path);
