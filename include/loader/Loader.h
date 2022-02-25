/**
 * @file Loader.h
 * @author Ryan Purse
 * @date 23/02/2022
 */


#pragma once

#include "ObjLoader.h"
#include "MeshComponents.h"

enum class fileExtension : uint32_t { None, Obj };

fileExtension getExtension(std::string_view path);

/**
 * @brief Loads the specified model
 * @param path - The path to the model that you want to load.
 */
template<typename VertexSpecification>
SharedMesh loadModel(std::string_view path)
{
    // Check that it's an .obj file.
    // Check the vertex type. Match up and create implementation.
    
    const fileExtension type = getExtension(path);
    
    // Check if the end of the string has .obj file format.
    if (type == fileExtension::Obj)
    {
        if (typeid(VertexSpecification) == typeid(UvVertex))
        {
            Mesh<UvVertex> mesh = loadObjUvVertex(path);
            return std::make_shared<Mesh<UvVertex>>(mesh);
        }
        if (typeid(VertexSpecification) == typeid(BasicVertex))
        {
            Mesh<BasicVertex> mesh = loadObjBasicVertex(path);
            return std::make_shared<Mesh<BasicVertex>>(mesh);
        }
        else
            debug::log("Unknown vertex type: " + std::string(typeid(VertexSpecification).name()), debug::severity::Warning);
    }
    else
        debug::log("Unknown file type: " + std::string(path), debug::severity::Warning);
    
    return SharedMesh();
}
