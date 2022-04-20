/**
 * @file Physics.h
 * @author Ryan Purse
 * @date 13/04/2022
 */


#pragma once

#include "Pch.h"
#include "Callback.h"

struct DynamicObject
{
    glm::vec3   force   { 0.f };
    float       mass    { 10.f };
};

struct Velocity
{
    glm::vec3 value { 0.f };
};

struct BoundingVolume
{
    explicit BoundingVolume(const Entity entity) :
        entity(entity)
    {}
    
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
