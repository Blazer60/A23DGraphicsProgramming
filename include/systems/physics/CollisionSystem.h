/**
 * @file CollisionSystem.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Physics.h"
#include "UniformComponents.h"

/**
 * @author Ryan Purse
 * @date 20/04/2022
 */
class CollisionSystem
    : public ecs::BaseSystem<std::shared_ptr<BoundingVolume>, std::shared_ptr<BasicUniforms>>
{
public:
    CollisionSystem();

    void onUpdate() override;
protected:
    std::vector<std::shared_ptr<BoundingVolume>>    mBoundingVolumes;
    std::vector<std::shared_ptr<BasicUniforms>>     mBasicUniforms;
};


