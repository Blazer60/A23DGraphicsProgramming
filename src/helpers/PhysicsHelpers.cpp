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
        return glm::length(toBox3(point, halfSize));
    }
    
    glm::vec3 toBox3(const glm::vec3 &point, const glm::vec3 &halfSize)
    {
        return glm::max(glm::abs(point) - halfSize, glm::vec3(0.f));
    }
    
    float sphereToSphere(const glm::vec3 &pointA, const float radiusA, const glm::vec3 &pointB, const float radiusB)
    {
        return sdf::toSphere(pointB - pointA, radiusA) - radiusB;
    }
    
    float sphereToBox(const glm::vec3 &point, const float radius, const glm::vec3 &halfSize)
    {
        return sdf::toBox(point, halfSize) - radius;
    }
}

namespace sat
{
    bool inRange(const glm::vec3 &value, const glm::vec3 &lower, const glm::vec3 &upper)
    {
        return glm::all(glm::lessThan(lower, value) && glm::lessThan(value, upper));
    }
}

