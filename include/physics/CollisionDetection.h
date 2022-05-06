/**
 * @file CollisionSystem.h
 * @author Ryan Purse
 * @date 20/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "physics/components/BoundingVolumes.h"
#include "UniformComponents.h"
#include "PhysicsHelpers.h"
#include "physics/components/Physics.h"
#include "physics/octree/Tree.h"

class Renderer;

/**
 * @author Ryan Purse
 * @date 20/04/2022
 */
class CollisionDetection
    : public ecs::BaseSystem<std::shared_ptr<BoundingVolume>, std::shared_ptr<BasicUniforms>, Velocity>
{
    struct BoundedCollisionEntity
    {
        std::shared_ptr<BoundingVolume> boundingVolume;
        std::shared_ptr<BasicUniforms>  basicUniforms;
        Velocity                        velocity;
        octree::AABB                    bounds;
    };
    
public:
    CollisionDetection(Renderer &renderer, std::shared_ptr<octree::Tree<CollisionEntity>> tree);

    void onUpdate() override;
    
    void treeCollision();
    void collisionLhs();
    void collisionRhs(const BoundingSphere &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity);
    void collisionRhs(const BoundingBox &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity);
    
    /** Sphere Vs. Sphere */
    HitRecord collisionCheck(
        const BoundingSphere &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
        const BoundingSphere &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity);
    
    /** Box Vs. Sphere */
    HitRecord collisionCheck(
        const BoundingBox &lhs,    const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
        const BoundingSphere &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity);
    
    /** Box Vs. Box */
    std::vector<HitRecord> collisionCheck(
        const BoundingBox &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
        const BoundingBox &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity);
    
protected:
    std::vector<BoundedCollisionEntity> mCollisionEntities;
    std::shared_ptr<octree::Tree<CollisionEntity>> mTree;
    Renderer &mRenderer;
    
    void traverseTree(
        std::shared_ptr<BoundingSphere> lhsEntity, std::shared_ptr<BasicUniforms> &uniforms,
        const Velocity &velocity, octree::AABB bounds);
    
    void traverseTree(
        std::shared_ptr<BoundingBox> lhsEntity, std::shared_ptr<BasicUniforms> &uniforms,
        const Velocity &velocity, octree::AABB bounds);
};


