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
    for (auto &lhs : mBoundingVolumes)
    {
    
    }
    
    // Reset for next round.
    mBoundingVolumes.clear();
    mBasicUniforms.clear();
}

void CollisionSystem::collisionLhs()
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &lhs = mBoundingVolumes[i];
        const glm::mat4 inverseModelMat = glm::inverse(mBasicUniforms[i]->modelMat);
        
        if (auto lhsSphere = std::dynamic_pointer_cast<BoundingSphere>(lhs))
            collisionRhs(*lhsSphere, inverseModelMat);
        if (auto lhsBox = std::dynamic_pointer_cast<BoundingBox>(lhs))
            collisionRhs(*lhsBox, inverseModelMat);
    }
}

void CollisionSystem::collisionRhs(const BoundingSphere &lhs, const glm::mat4 &inverseModelMat)
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &rhs = mBoundingVolumes[i];
        const glm::mat4 &modelMat = mBasicUniforms[i]->modelMat;
        
        if (lhs.entity == rhs->entity)
            continue;
        if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
        {
        
        }
        if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
        {
        
        }
    }
    for (const auto &rhs : mBoundingVolumes)
    {
    
    }
}

void CollisionSystem::collisionRhs(const BoundingBox &lhs, const glm::mat4 &inverseModelMat)
{
    for (const auto &rhs : mBoundingVolumes)
    {
        if (lhs.entity == rhs->entity)
            continue;
        if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
        {
        
        }
        if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
        {
        
        }
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
