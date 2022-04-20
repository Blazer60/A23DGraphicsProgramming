/**
 * @file CollisionSystem.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<BasicUniforms> &basicUniforms)
    {
        mBoundingVolumes.push_back(boundingVolume);
        mBasicUniforms.push_back(basicUniforms);
    });
}

void CollisionSystem::onUpdate()
{
    for (auto &item : mBoundingVolumes)
        item->callbacks.broadcast();
    
    // Reset for next round.
    mBoundingVolumes.clear();
    mBasicUniforms.clear();
}
