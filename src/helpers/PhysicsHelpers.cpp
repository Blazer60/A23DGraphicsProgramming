/**
 * @file Physics.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "PhysicsHelpers.h"

namespace sdf
{
    float toSphere(const glm::vec3 &point, const float radius)
    {
        return glm::length(point) - radius;
    }
    
    float toBox(const glm::vec3 &point, const glm::vec3 &halfSize)
    {
        return glm::length(glm::max(glm::abs(point)-halfSize, glm::vec3(0.f)));
    }
}

namespace physics
{
    bool SphereToSphere(const glm::vec3 &pointA, const float radiusA, const glm::vec3 &pointB, const float radiusB)
    {
        const float distance = sdf::toSphere(pointB - pointA, radiusA) - radiusB;
        return distance <= 0;
    }
    
    bool SphereToBox(const glm::vec3 &point, const float radius, const glm::vec3 &halfSize)
    {
        const float distance = sdf::toBox(point, halfSize) - radius;
        return distance <= 0;
    }
}
