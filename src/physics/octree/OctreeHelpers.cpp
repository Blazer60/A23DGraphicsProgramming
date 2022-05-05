/**
 * @file OctreeHelpers.cpp
 * @author Ryan Purse
 * @date 27/04/2022
 */


#include "physics/octree/OctreeHelpers.h"
#include "PhysicsHelpers.h"

namespace octree
{
    std::array<glm::vec3, sVertexCount> localiseToPoint(const glm::vec3 &point, const AABB &aabb)
    {
        const glm::vec3 halfSize = aabb.halfSize;
        return {
            (aabb.position + glm::vec3(+halfSize.x, +halfSize.y, +halfSize.z)) - point,
            (aabb.position + glm::vec3(+halfSize.x, +halfSize.y, -halfSize.z)) - point,
            (aabb.position + glm::vec3(-halfSize.x, +halfSize.y, -halfSize.z)) - point,
            (aabb.position + glm::vec3(-halfSize.x, +halfSize.y, +halfSize.z)) - point,
            (aabb.position + glm::vec3(+halfSize.x, -halfSize.y, +halfSize.z)) - point,
            (aabb.position + glm::vec3(+halfSize.x, -halfSize.y, -halfSize.z)) - point,
            (aabb.position + glm::vec3(-halfSize.x, -halfSize.y, -halfSize.z)) - point,
            (aabb.position + glm::vec3(-halfSize.x, -halfSize.y, +halfSize.z)) - point
        };
    }
    
    bool contains(const octree::AABB &outer, const octree::AABB &inner)
    {
        const auto points = localiseToPoint(outer.position, inner);
        for (const auto &point : points)
        {
            if (glm::length(sdf::toBox(point, outer.halfSize)) > 0)
                return false;
        }
        return true;
    }
    
    bool intersects(const AABB &lhs, const AABB &rhs)
    {
        const auto points = localiseToPoint(lhs.position, rhs);
        for (const auto &point : points)
        {
            if (glm::length(sdf::toBox(point, lhs.halfSize)) <= 0)
                return true;
        }
        return false;
    }
}


