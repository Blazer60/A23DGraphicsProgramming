/**
 * @file RawMesh.h
 * @author Ryan Purse
 * @date 13/03/2022
 */


#pragma once

#include "CommonLoader.h"
#include "Vertices.h"

#include <functional>

namespace load
{
    template<typename TVertex>
    class RawMesh;
    
    /** A typedef short-hand for named vertex groups generated by an Object File. */
    template<typename TVertex>
    using RawMeshes = std::unordered_map<std::string, RawMesh<TVertex>>;
    
    typedef std::function<ObjVertex(std::string_view arg)> createObjectSignature;
    
    /**
     * @brief Contains vertex data that is generated from the Object file. Grouped by the material names.
     * @tparam TVertex - The type of vertex that you want to store.
     * @author Ryan Purse
     * @date 13/03/2022
     */
    template<typename TVertex>
    class RawMesh
    {
    public:
        void createVertices(std::string_view args, const createObjectSignature &createObjectVertex);
        void createVertex(const std::string &id, const createObjectSignature &createObjectVertex);
        void createIndices(const std::vector<uint32_t> &uniqueIndices);
        
        [[nodiscard]] uint32_t verticesSize() const { return mVertices.size() * sizeof(TVertex); }
        [[nodiscard]] uint32_t indicesSize() const { return mIndices.size() * sizeof(uint32_t); }
        [[nodiscard]] const void *verticesData() const { return static_cast<const void *>(&(mVertices.at(0))); }
        [[nodiscard]] const void *indicesData() const { return static_cast<const void *>(&(mIndices.at(0))); }
        [[nodiscard]] int indicesCount() const { return static_cast<int>(mIndices.size()); }
    
    protected:
        std::vector<uint32_t>                       mIndices;
        std::vector<TVertex>                        mVertices;
        std::unordered_map<std::string, uint32_t>   mVertexLocations;
    };
    
    template<typename TVertex>
    void RawMesh<TVertex>::createVertices(std::string_view args, const createObjectSignature &createObjectVertex)
    {
        std::vector<uint32_t> uniqueIndices;
        const std::vector<std::string> vertexIds = split(args);
        
        for (const std::string &id : vertexIds)
        {
            if (mVertexLocations.count(id) < 1)
                createVertex(id, createObjectVertex);
            
            uniqueIndices.emplace_back(mVertexLocations.at(id));
        }
        
        createIndices(uniqueIndices);
    }
    
    template<typename TVertex>
    void RawMesh<TVertex>::createVertex(const std::string &id, const createObjectSignature &createObjectVertex)
    {
        mVertices.emplace_back(createObjectVertex(id));
        mVertexLocations.emplace(id, static_cast<uint32_t>(mVertices.size()) - 1);
    }
    
    template<typename TVertex>
    void RawMesh<TVertex>::createIndices(const std::vector<uint32_t> &uniqueIndices)
    {
        const uint32_t baseVertexIndex = uniqueIndices[0];
        // Indexes into the uniqueIndices.
        uint32_t firstIndex  = 1;
        uint32_t secondIndex = 2;
        // Create a fan out of the indices.
        for (int i = 0; i < uniqueIndices.size() - 2; ++i)
        {
            mIndices.emplace_back(baseVertexIndex);
            mIndices.emplace_back(uniqueIndices[firstIndex++]);
            mIndices.emplace_back(uniqueIndices[secondIndex++]);
        }
    }
}




