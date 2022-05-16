/**
 * @file TreeBuilder.cpp
 * @author Ryan Purse
 * @date 06/05/2022
 */


#include "TreeBuilder.h"
#include "PhysicsHelpers.h"
#include "Timers.h"

TreeBuilder::TreeBuilder(std::shared_ptr<octree::Tree<CollisionEntity>> tree)
    : mTree(std::move(tree))
{
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<ModelMatrix> &basicUniforms,
        const Velocity &velocity)
    {
        const glm::vec3 center = basicUniforms->value * glm::vec4(velocity.value * timers::fixedTime<float>(), 1.f);
        if (auto sphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
        {
            const glm::vec3 axisAlignedHalfSize = glm::vec3(sphere->radius);
        
            octree::AABB bounds { center, axisAlignedHalfSize };
            mTree->insert({ boundingVolume, basicUniforms, velocity }, bounds);
        }
        if (auto box = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
        {
            auto points = physics::boxToVertex(basicUniforms->value, box->halfSize);
            glm::vec3 max = glm::vec3(0.f);
        
            for (auto &point : points)
            {
                max = glm::max(max, glm::abs(point));
            }
            max = (max - glm::abs(center));
            octree::AABB bounds { center, max };
            mTree->insert({ boundingVolume, basicUniforms, velocity }, bounds);
        }
    });
    scheduleFor(ecs::PreFixedUpdate);
}

void TreeBuilder::onUpdate()
{
    mTree->reset();
}
