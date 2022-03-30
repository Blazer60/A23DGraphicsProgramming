/**
 * @file Primitives.h
 * @author Ryan Purse
 * @date 12/02/2022
 */


#pragma once

#include "Vertices.h"
#include "ModelLoader.h"

namespace primitives
{
    typedef std::pair<std::vector<ObjVertex>, std::vector<uint32_t>> PrimitiveType;
    
    /**
     * @brief Creates a cube model.
     * @tparam TVertex - The type of Vertex you want.
     * @tparam TMaterial - The type of Material you want (default is nothing).
     */
    template<typename TVertex, typename TMaterial=NoMaterial>
    Model<TVertex, TMaterial> cube();
    
    /**
     * @brief Creates a plane.
     * @tparam TVertex - The type of Vertex you want.
     * @tparam TMaterial - The type of Material you want (default is nothing).
     */
    template<typename TVertex, typename TMaterial=NoMaterial>
    Model<TVertex, TMaterial> plane();
    
    /**
     * @brief
     * @tparam TVertex
     * @tparam TMaterial
     * @param points
     * @return
     */
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> triangleFanCircle(unsigned int points);
    
    /**
     * @brief Creates what ever you define provided it returns object vertices and indices for it.
     * @tparam TVertex - The type of Vertex you want.
     * @tparam TMaterial - The type of Material you want (default is nothing).
     * @param makeCallback - How to make the model.
     */
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> make(const std::function<PrimitiveType()> &makeCallback);
    
    [[nodiscard]] PrimitiveType objectCube();
    [[nodiscard]] PrimitiveType objectPlane();
    [[nodiscard]] PrimitiveType objectTriangleFanCircle(unsigned int points);
    
    
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> make(const std::function<PrimitiveType()> &makeCallback)
    {
        const auto [objectVertices, indices] = makeCallback();
        
        std::vector<TVertex> vertices;
        vertices.reserve(objectVertices.size());
        for (const auto &item : objectVertices)
            vertices.emplace_back(item);
        
        Mesh<TVertex, TMaterial> mesh(vertices, indices, TMaterial());
        return { std::move(mesh) };
    }
    
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> cube()
    {
        return make<TVertex, TMaterial>(objectCube);
    }
    
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> plane()
    {
        return make<TVertex, TMaterial>(objectPlane);
    }
    
    template<typename TVertex, typename TMaterial>
    Model<TVertex, TMaterial> triangleFanCircle(const unsigned int points)
    {
        return make<TVertex, TMaterial>([&points]() {
            return objectTriangleFanCircle(points);
        });
    }
}