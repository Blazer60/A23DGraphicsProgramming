/**
 * @file TreeBuilder.h
 * @author Ryan Purse
 * @date 06/05/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Physics.h"
#include "BoundingVolumes.h"
#include "UniformComponents.h"
#include "Tree.h"

class CollisionEntity;

/**
 * @author Ryan Purse
 * @date 06/05/2022
 */
class TreeBuilder
    : public ecs::BaseSystem<std::shared_ptr<BoundingVolume>, std::shared_ptr<ModelMatrix>, Velocity>
{
public:
    explicit TreeBuilder(std::shared_ptr<octree::Tree<CollisionEntity>> tree);
    
    void onUpdate() override;

protected:
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree;
};
