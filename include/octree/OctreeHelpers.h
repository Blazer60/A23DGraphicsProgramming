/**
 * @file OctreeHelpers.h
 * @author Ryan Purse
 * @date 27/04/2022
 */


#pragma once

#include "Pch.h"
#include <array>

namespace octree
{
    struct AABB
    {
        glm::vec3 position { 0.f };
        glm::vec3 halfSize { 1.f };
    };
    
    bool contains(const AABB &outer, const AABB &inner);
    bool intersects(const AABB &lhs, const AABB &rhs);
    
    static constexpr uint32_t sVertexCount { 8 };
    std::array<glm::vec3, sVertexCount> localiseToPoint(const glm::vec3 &point, const AABB &aabb);
    
    template<typename T>
    struct Package
    {
        AABB bounds { };
        T    data   { };
    };
    
    enum region : char {
        TopNorthWest,       TopSouthWest,       TopSouthEast,       TopNorthEast,
        BottomNorthWest,    BottomSouthWest,    BottomSouthEast,    BottomNorthEast };
}
