/**
 * @file CollisionSystem.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BoundingVolumes.h"
#include "UniformComponents.h"
#include "PhysicsHelpers.h"

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
    
    void collisionLhs();
    void collisionRhs(const BoundingSphere &lhs, const glm::mat4 &lhsModelMat);
    void collisionRhs(const BoundingBox &lhs, const glm::mat4 &lhsModelMat);
    
    HitRecord collisionCheck(
        const BoundingSphere &lhs, const glm::mat4 &lhsModelMat,
        const BoundingSphere &rhs, const glm::mat4 &rhsModelMat);
    
    HitRecord collisionCheck(
        const BoundingBox &lhs,     const glm::mat4 &lhsModelMat,
        const BoundingSphere &rhs,  const glm::mat4 &rhsModelMat);
    
    std::vector<HitRecord> collisionCheck(
        const BoundingBox &lhs, const glm::mat4 &lhsModelMat,
        const BoundingBox &rhs, const glm::mat4 &rhsModelMat);
    
protected:
    std::vector<std::shared_ptr<BoundingVolume>>    mBoundingVolumes;
    std::vector<std::shared_ptr<BasicUniforms>>     mBasicUniforms;
};


