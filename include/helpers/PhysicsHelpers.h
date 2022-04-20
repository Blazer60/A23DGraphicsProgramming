/**
 * @file Physics.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"

/**
 * @brief Signed Distance Field. General Rule: d \< 0: hit, no hit otherwise.
 */
namespace sdf
{
    /**
     * @param point - A point in space
     * @param radius - The radius of the sphere
     */
    float toSphere(const glm::vec3 &point, const float radius);
    
    /**
     * @param point - A point in space
     * @param halfSize - The half size of the box.
     */
    float toBox(const glm::vec3 &point, const glm::vec3 &halfSize);
}

namespace physics
{
    bool SphereToSphere(const glm::vec3 &pointA, const float radiusA, const glm::vec3 &pointB, const float radiusB);
    
    [[maybe_unused]] bool SphereToBox(const glm::vec3 &point, const float radius, const glm::vec3 &halfSize);
}
