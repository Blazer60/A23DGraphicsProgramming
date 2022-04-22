/**
 * @file Physics.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "PhysicsHelpers.h"
#include <gtx/component_wise.hpp>


namespace physics
{
    glm::vec3 sign3(const glm::vec3 &value)
    {
        return { sign(value.x), sign(value.y), sign(value.z) };
    }
}

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
    
    glm::vec3 boxNormal(const glm::vec3 &point, const glm::vec3 &halfSize)
    {
        // The point must be in positive space for min and max to work.
        const glm::vec3     beta    = glm::abs(point);
        const glm::vec3     sign    = physics::sign3(point);
        
        const glm::vec3     alpha   = beta / halfSize;          // Since we are dealing with more than 1x1x1 cubes.
        const float         max     = glm::compMax(alpha);      // Only interested in the largest value.
        const glm::vec3     gamma   = glm::floor(alpha / max);  // Normalised for us here.
        const glm::vec3     normal  = sign * gamma;             // Add back in the signs.
        
        return normal;
    }
}

namespace sat
{
    bool inRange(const glm::vec3 &value, const glm::vec3 &lower, const glm::vec3 &upper)
    {
        return glm::all(glm::lessThan(lower, value) && glm::lessThan(value, upper));
    }
}

