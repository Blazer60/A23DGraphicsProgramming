/**
 * @file BoundingVolumes.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Callback.h"

struct BoundingVolume
{
    explicit BoundingVolume(const Entity entity) :
        entity(entity)
    {}
    
    virtual ~BoundingVolume() = default;
    
    Entity entity { 0 };
    Callback<> callbacks;
};

struct BoundingSphere
    : BoundingVolume
{
    explicit BoundingSphere(const Entity entity, const float radius=1.f) :
        BoundingVolume(entity), radius(radius)
    {}
    
    float radius { 1.f };
};

struct BoundingBox
    : BoundingVolume
{
    explicit BoundingBox(const Entity entity, const glm::vec3 &halfSize=glm::vec3(1.f)) :
        BoundingVolume(entity), halfSize(halfSize)
    {}
    
    glm::vec3 halfSize { 1.f };
};
