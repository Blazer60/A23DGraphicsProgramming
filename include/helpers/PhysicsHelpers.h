/**
 * @file Physics.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"

struct HitRecord
{
    HitRecord() = default;
    HitRecord(bool hit) :
        hit(hit)
    {}
    HitRecord(bool hit, const glm::vec3 &position, const glm::vec3 &normal) :
        hit(hit), position(position), normal(normal)
    {}
    
    bool        hit       { false };
    glm::vec3   position  { 0.f };
    glm::vec3   normal    { 1.f, 0.f, 0.f };
};

namespace physics
{
    template<typename T>
    T sign(const T &value)
    {
        return (T(0) < value) - (value < T(0));
    }
    
    glm::vec3 sign3(const glm::vec3 &value);
}

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
    
    glm::vec3 toBox3(const glm::vec3 &point, const glm::vec3 &halfSize);
    
    float sphereToSphere(const glm::vec3 &pointA, const float radiusA, const glm::vec3 &pointB, const float radiusB);
    
    float sphereToBox(const glm::vec3 &point, const float radius, const glm::vec3 &halfSize);
    
    /**
     * @brief Gets a normal for a point an a box by judging which edge it's closest to.
     * @param point - The point relative to the box's origin.
     * @param halfSize - The box's extent.
     * @returns A normal pointing away from the box's origin.
     */
    glm::vec3 boxNormal(const glm::vec3 &point, const glm::vec3 &halfSize);
}

/**
 * @brief Separate Axis Theorem. Tests if two Object Oriented Bounding Boxes are intersecting.
 */
namespace sat
{
    bool inRange(const glm::vec3 &value, const glm::vec3 &lower, const glm::vec3 &upper);
    
    
}
