/**
 * @file Loader.h
 * @author Ryan Purse
 * @date 23/02/2022
 */


#pragma once

#include "ObjLoader.h"
#include "MeshComponents.h"
#include "MaterialComponents.h"

enum class fileExtension : uint32_t { None, Obj };

fileExtension getExtension(std::string_view path);

/**
 * @brief Loads the specified model
 * @param path - The path to the model that you want to load.
 */
template<typename VertexSpecification>
SharedMesh loadModel(std::string_view path)
{
    const fileExtension type = getExtension(path);
    
    // Check if the end of the string has .obj file format.
    if (type == fileExtension::Obj)
        return loadObj<VertexSpecification, BlinnPhongMaterial>(path);
    else
        debug::log("Unknown file type: " + std::string(path), debug::severity::Warning);
    
    return {  };
}
