/**
 * @file CollisionSystem.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "CollisionSystem.h"
#include "PhysicsHelpers.h"
#include <numeric>

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
    collisionLhs(); // O(n^2)
    
    // Reset for next round.
    mBoundingVolumes.clear();
    mBasicUniforms.clear();
}

void CollisionSystem::collisionLhs()
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &lhs = mBoundingVolumes[i];
        const glm::mat4 modelMat = mBasicUniforms[i]->modelMat;
        
        if (auto lhsSphere = std::dynamic_pointer_cast<BoundingSphere>(lhs))
            collisionRhs(*lhsSphere, modelMat);
        if (auto lhsBox = std::dynamic_pointer_cast<BoundingBox>(lhs))
            collisionRhs(*lhsBox, modelMat);
    }
}

void CollisionSystem::collisionRhs(const BoundingSphere &lhs, const glm::mat4 &lhsModelMat)
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &rhs = mBoundingVolumes[i];
        const glm::mat4 &rhsModelMat = mBasicUniforms[i]->modelMat;
    
        bool hit = false;
        if (lhs.entity == rhs->entity)
            continue;
        
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
            hit = collisionCheck(lhs, lhsModelMat, *rhsSphere, rhsModelMat);
        
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
            hit = collisionCheck(*rhsBox, glm::inverse(rhsModelMat), lhs, lhsModelMat);
        
        
        if (hit)
            lhs.callbacks.broadcast();
    }
}

void CollisionSystem::collisionRhs(const BoundingBox &lhs, const glm::mat4 &lhsModelMat)
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &rhs = mBoundingVolumes[i];
        const glm::mat4 &rhsModelMat = mBasicUniforms[i]->modelMat;
        
        bool hit = false;
        if (lhs.entity == rhs->entity)
            continue;
        
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
            hit = collisionCheck(lhs, glm::inverse(lhsModelMat), *rhsSphere, rhsModelMat);
        
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
            hit = collisionCheck(lhs, glm::inverse(lhsModelMat), *rhsBox, rhsModelMat);
        
        
        if (hit)
            lhs.callbacks.broadcast();
    }
}

bool CollisionSystem::collisionCheck(
    const BoundingSphere &lhs, const glm::mat4 &lhsModelMat,
    const BoundingSphere &rhs, const glm::mat4 &rhsModelMat)
{
    const glm::vec3 pointA = lhsModelMat * glm::vec4(0.f);
    const float radiusA = lhs.radius;
    
    const glm::vec3 pointB = rhsModelMat * glm::vec4(0.f);
    const float radiusB = rhs.radius;
    
    return physics::SphereToSphere(pointA, radiusA, pointB, radiusB);
}

bool CollisionSystem::collisionCheck(
    const BoundingBox &lhs,     const glm::mat4 &lhsInverseModelMat,
    const BoundingSphere &rhs,  const glm::mat4 &rhsModelMat)
{
    const glm::vec3 point = lhsInverseModelMat * rhsModelMat * glm::vec4(0.f);
    return physics::SphereToBox(point, rhs.radius, lhs.halfSize);
}

bool CollisionSystem::collisionCheck(
    const BoundingBox &lhs, const glm::mat4 &lhsInverseModelMat,
    const BoundingBox &rhs, const glm::mat4 &rhsModelMat)
{
    const glm::vec3 &halfSize = rhs.halfSize;
    const glm::vec4 coords[] = {
        glm::vec4(+halfSize.x, +halfSize.y, +halfSize.z, 1.f),  // East, Up, North
        glm::vec4(+halfSize.x, +halfSize.y, -halfSize.z, 1.f),  // East, Up, South
        glm::vec4(-halfSize.x, +halfSize.y, -halfSize.z, 1.f),  // West, Up, South
        glm::vec4(-halfSize.x, +halfSize.y, +halfSize.z, 1.f),  // West, Up, North
        
        glm::vec4(+halfSize.x, -halfSize.y, +halfSize.z, 1.f),  // East, Down, North
        glm::vec4(+halfSize.x, -halfSize.y, -halfSize.z, 1.f),  // East, Down, South
        glm::vec4(-halfSize.x, -halfSize.y, -halfSize.z, 1.f),  // West, Down, South
        glm::vec4(-halfSize.x, -halfSize.y, +halfSize.z, 1.f),  // West, Down, North
    };
    
    float min = std::numeric_limits<float>::infinity();
    for (const auto &coord : coords)
    {
        const glm::vec3 point = lhsInverseModelMat * rhsModelMat * coord;
        glm::min(min, sdf::toBox(point, lhs.halfSize));
    }
    
    return min < 0.f;
}
