/**
 * @file ModelLoader.h
 * @author Ryan Purse
 * @date 12/03/2022
 */


#pragma once

#include "MaterialComponents.h"
#include "RenderComponents.h"
#include "MeshComponents.h"
#include "CommonLoader.h"
#include "MaterialLoader.h"
#include "RawMesh.h"

#include <utility>

namespace load
{
    /**
     * @brief The returned information for rendering an object to the screen with OpenGL.
     * @tparam TMaterial - The type of material that you want to store.
     */
    template<typename TVertex, typename TMaterial>
    struct Mesh  // todo: move this to global namespace.
    {
        Mesh(const std::vector<TVertex> &vertices, const std::vector<uint32_t> &indices, TMaterial mat)
            : material(mat)
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
        
        Mesh(const RawMesh<TVertex> &mesh, TMaterial mat)
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
        
        RenderInformation   renderInformation;
        TMaterial           material;
        unsigned int        vbo { 0 };
        unsigned int        ebo { 0 };
    };
    
    template<typename TVertex, typename TMaterial>
    using Model = std::vector<Mesh<TVertex, TMaterial>>;
    
    /**
     * @brief Stores the raw data loaded by the object file.
     */
    class ObjectFile;
    
    template<typename TVertex, typename TMaterial>
    std::pair<RawMeshes<TVertex>, Materials<TMaterial>> parseObject(std::string_view path);

    /**
     * @brief Loads a model with the correct Vertex type and material Type specified.
     * @tparam TVertex - The type of vertex that you want to use.
     * @tparam TMaterial - The type of material that you want to use.
     * @param path - The path to the model.
     * @returns Information that can be used to render the model.
     */
    template<typename TVertex, typename TMaterial=NoMaterial>
    Model<TVertex, TMaterial> model(std::string_view path)
    {
        const auto [meshes, materials] = parseObject<TVertex, TMaterial>(path);
        
        Model<TVertex, TMaterial> model;
        
        for (const auto &[name, rawMesh] : meshes)
        {
            TMaterial material = materials.count(name) > 0 ? materials.at(name) : TMaterial();
            model.emplace_back(rawMesh, material);
        }
        
        return std::move(model);
    }
    
    template<typename TVertex, typename TMaterial>
    std::pair<RawMeshes<TVertex>, Materials<TMaterial>> parseObject(std::string_view path)
    {
        ObjectFile objectFile;
        RawMeshes<TVertex> meshes;
        std::string materialGroupName { "Null" };
    
        Materials<TMaterial> materials;
    
        const auto generateFace = [&](std::string_view arg) {
            meshes[materialGroupName].createVertices(arg, [&](std::string_view arg) {
                return objectFile.createVertex(arg);
            });
        };
    
        const auto loadMaterial = [&](std::string_view arg) {
            if (typeid(TMaterial) == typeid(NoMaterial))  // todo: can this be made constexpr?
                return;  // We don't want to load a material if no material is set.
            materials = load::material<TMaterial>(convertRelativePath(path, arg));
        };
    
        const ArgumentList argumentList {
            {"#",      doNothing },
            {"o",      doNothing },
            {"s",      doNothing },
            {"g",      doNothing },
            {"mtllib", loadMaterial },
            {"usemtl", [&materialGroupName](std::string_view arg) { materialGroupName = std::string(arg); } },
            {"v",      [&objectFile](std::string_view arg) { objectFile.positions.emplace_back(createVec<3>(arg));  } },
            {"vt",     [&objectFile](std::string_view arg) { objectFile.uvs.emplace_back(createVec<2>(arg));        } },
            {"vn",     [&objectFile](std::string_view arg) { objectFile.normals.emplace_back(createVec<3>(arg));    } },
            {"f",      generateFace },
        };
    
        parseFile(path, argumentList);
        return { std::move(meshes), std::move(materials) };
    }
    
    class ObjectFile
    {
    public:
        ObjVertex createVertex(std::string_view arg);
        
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
    };
}



