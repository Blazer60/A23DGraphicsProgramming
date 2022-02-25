/**
 * @file ObjLoader.cpp
 * @author Ryan Purse
 * @date 23/02/2022
 */


#include "ObjLoader.h"
#include "CommonLoader.h"
#include "RenderComponents.h"
#include "MeshComponents.h"

std::vector<uint32_t>
loadObj(std::string_view path, const GenVertexSignature &genVertexDelegate, const GetSizeSignature &getSizeDelegate)
{
    // All the information that is stored in the .obj file.
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    // Generated on the fly.
    std::vector<uint32_t>                       indices;
    std::unordered_map<std::string, uint32_t>   vertexLocations;
    
    const auto generateObjVertex = [&](const std::string_view &arg) {
        const std::vector<std::string> nums = split(arg, '/');
        uint32_t positionIndex  { 0 };
        uint32_t uvIndex        { 0 };
        uint32_t normalIndex    { 0 };
    
        switch (nums.size())
        {
            case 3:
                normalIndex = std::stoi(nums[2]);
            case 2:
                if (!nums[1].empty())
                    uvIndex = std::stoi(nums[1]);
            case 1:
            default:
                positionIndex = std::stoi(nums[0]);
        }
        
        // .obj files start counting at zero, so we can use it as a sanity check.
        return ObjVertex {
            positionIndex   == 0 ? glm::vec3(0.f) : positions[positionIndex - 1],
            uvIndex         == 0 ? glm::vec2(0.f) : uvs[uvIndex - 1],
            normalIndex     == 0 ? glm::vec3(0.f) : normals[normalIndex - 1],
        };
    };
    
    const auto generateOrGetIndices = [&](const std::vector<std::string> &vertexIdentifiers) {
        std::vector<uint32_t> uniqueIndices;
        for (const std::string &identifier : vertexIdentifiers)
        {
            if (vertexLocations.count(identifier) > 1)
            {
                uniqueIndices.emplace_back(vertexLocations.at(identifier));
                continue;
            }
        
            genVertexDelegate(generateObjVertex(identifier));
            const uint32_t lastIndex = getSizeDelegate() - 1;
            vertexLocations.emplace(identifier, lastIndex);
            uniqueIndices.emplace_back(lastIndex);
        }
        return uniqueIndices;
    };
    
    const auto generateIndices = [&](const std::vector<uint32_t> &uniqueIndices) {
        const uint32_t baseVertexIndex = uniqueIndices[0];
        // Indexes into the uniqueIndices.
        uint32_t firstIndex  = 1;
        uint32_t secondIndex = 2;
        // Create a fan out of the indices.
        for (int i = 0; i < uniqueIndices.size() - 2; ++i)
        {
            indices.emplace_back(baseVertexIndex);
            indices.emplace_back(uniqueIndices[firstIndex++]);
            indices.emplace_back(uniqueIndices[secondIndex++]);
        }
    };
    
    const auto createFace = [&](std::string_view args) {
        std::vector<uint32_t> uniqueIndices = generateOrGetIndices(split(args));
        generateIndices(uniqueIndices);
    };
    
    const ArgumentList argumentList {
            {"#",      doNothing },
            {"o",      doNothing },
            {"s",      doNothing },
            {"g",      doNothing },
            {"mtlib",  doNothing },
            {"usemtl", doNothing },
            {"v",      [&positions](std::string_view arg)  { positions.emplace_back(createVec<3>(arg)); } },
            {"vt",     [&uvs](std::string_view arg)        { uvs.emplace_back(createVec<2>(arg)); } },
            {"vn",     [&normals](std::string_view arg)    { normals.emplace_back(createVec<3>(arg)); } },
            {"f",      createFace },
    };
    
    parseFile(path, argumentList);
    return std::move(indices);
}

Mesh<UvVertex> loadObjUvVertex(std::string_view path)
{
    std::vector<UvVertex> vertices;
    
    const auto genVertex = [&vertices](const ObjVertex &objVertex) {
        vertices.emplace_back(UvVertex { objVertex.position, objVertex.uv });
    };
    
    const auto getSize = [&vertices]() {
        return static_cast<uint32_t>(vertices.size());
    };
    
    return { std::move(vertices), std::move(loadObj(path, genVertex, getSize)) };
}

Mesh<BasicVertex> loadObjBasicVertex(std::string_view path)
{
    std::vector<BasicVertex> vertices;
    
    const auto genVertex = [&vertices](const ObjVertex &objVertex) {
        vertices.emplace_back(BasicVertex { objVertex.position, glm::vec3(1.f, 0.f, 1.f) });
    };
    
    const auto getSize = [&vertices]() {
        return static_cast<uint32_t>(vertices.size());
    };
    
    return { std::move(vertices), std::move(loadObj(path, genVertex, getSize)) };
}
