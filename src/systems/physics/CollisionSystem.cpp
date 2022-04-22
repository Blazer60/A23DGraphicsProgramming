/**
 * @file CollisionSystem.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "CollisionSystem.h"
#include "PhysicsHelpers.h"

#include <numeric>
#include <gtx/component_wise.hpp>
#include <unordered_set>

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
    
        HitRecord hit;
        if (lhs.entity == rhs->entity)
            continue;
        
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
            hit = collisionCheck(lhs, lhsModelMat, *rhsSphere, rhsModelMat);
        
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
        {
            hit = collisionCheck(*rhsBox, rhsModelMat, lhs, lhsModelMat);
            hit.normal *= -1.f;
        }
        
        
        if (hit.hit)
            lhs.callbacks.broadcast(lhs.entity, rhs->entity, hit.position, hit.normal);
    }
}

void CollisionSystem::collisionRhs(const BoundingBox &lhs, const glm::mat4 &lhsModelMat)
{
    for (int i = 0; i < mBoundingVolumes.size(); ++i)
    {
        const std::shared_ptr<BoundingVolume> &rhs = mBoundingVolumes[i];
        const glm::mat4 &rhsModelMat = mBasicUniforms[i]->modelMat;
        
        if (lhs.entity == rhs->entity)
            continue;
        
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhs))
        {
            auto hit = collisionCheck(lhs, lhsModelMat, *rhsSphere, rhsModelMat);
            if (hit.hit)
                lhs.callbacks.broadcast(lhs.entity, rhs->entity, hit.position, hit.normal);
        }
        
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhs))
        {
            auto hits = collisionCheck(lhs, lhsModelMat, *rhsBox, rhsModelMat);
            for (auto &hit : hits)
            {
                lhs.callbacks.broadcast(lhs.entity, rhs->entity, hit.position, hit.normal);
            }
            hits = collisionCheck(*rhsBox, rhsModelMat, lhs, lhsModelMat);
            for (auto &hit : hits)
            {
                hit.normal *= -1.f;
                lhs.callbacks.broadcast(lhs.entity, rhs->entity, hit.position, hit.normal);
            }
        }
    }
}

HitRecord CollisionSystem::collisionCheck(
    const BoundingSphere &lhs, const glm::mat4 &lhsModelMat,
    const BoundingSphere &rhs, const glm::mat4 &rhsModelMat)
{
    const glm::vec3 pointA   = lhsModelMat * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const float radiusA      = lhs.radius;
    
    const glm::vec3 pointB   = rhsModelMat * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const float radiusB      = rhs.radius;
    
    const glm::vec3 offset   = pointB - pointA;
    const float distance     = sdf::toSphere(offset, radiusA) - radiusB;
    const glm::vec3 normal   = glm::normalize(offset);
    const glm::vec3 position = pointA + radiusA * normal;
    
    return { distance <= 0, position, normal };
}

HitRecord CollisionSystem::collisionCheck(
    const BoundingBox &lhs,     const glm::mat4 &lhsModelMat,
    const BoundingSphere &rhs,  const glm::mat4 &rhsModelMat)
{
    const glm::mat4 lhsInverseModelMat = glm::inverse(lhsModelMat);
    const glm::vec3 point = lhsInverseModelMat * rhsModelMat * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const float distance = sdf::sphereToBox(point, rhs.radius, lhs.halfSize);
    
    const glm::vec3 normal = glm::normalize(lhsModelMat * glm::vec4(point, 0.f));  // We don't want to translate it.
    const glm::vec3 position = rhsModelMat * glm::vec4(0.f, 0.f, 0.f, 1.f) + glm::vec4(rhs.radius * -normal, 1.f);
    return { distance <= 0, position, normal };
}

std::vector<HitRecord> CollisionSystem::collisionCheck(
    const BoundingBox &lhs, const glm::mat4 &lhsModelMat,
    const BoundingBox &rhs, const glm::mat4 &rhsModelMat)
{
    const glm::mat4 lhsInverseModelMat = glm::inverse(lhsModelMat);
    const glm::vec3 &halfSize = rhs.halfSize;
    
    const glm::vec3 boxCenter = lhsInverseModelMat * rhsModelMat * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const glm::vec3 normal = lhsModelMat * glm::vec4(sdf::boxNormal(boxCenter, lhs.halfSize), 0.f);
    
    const glm::vec3 coords[] = {
        lhsInverseModelMat * rhsModelMat * glm::vec4(+halfSize.x, +halfSize.y, +halfSize.z, 1.f),  // East, Up, North
        lhsInverseModelMat * rhsModelMat * glm::vec4(+halfSize.x, +halfSize.y, -halfSize.z, 1.f),  // East, Up, South
        lhsInverseModelMat * rhsModelMat * glm::vec4(-halfSize.x, +halfSize.y, -halfSize.z, 1.f),  // West, Up, South
        lhsInverseModelMat * rhsModelMat * glm::vec4(-halfSize.x, +halfSize.y, +halfSize.z, 1.f),  // West, Up, North
        
        lhsInverseModelMat * rhsModelMat * glm::vec4(+halfSize.x, -halfSize.y, +halfSize.z, 1.f),  // East, Down, North
        lhsInverseModelMat * rhsModelMat * glm::vec4(+halfSize.x, -halfSize.y, -halfSize.z, 1.f),  // East, Down, South
        lhsInverseModelMat * rhsModelMat * glm::vec4(-halfSize.x, -halfSize.y, -halfSize.z, 1.f),  // West, Down, South
        lhsInverseModelMat * rhsModelMat * glm::vec4(-halfSize.x, -halfSize.y, +halfSize.z, 1.f),  // West, Down, North
    };
    
    std::vector<HitRecord> hitRecords;
    for (const auto &point : coords)
    {
        const glm::vec3 distance = sdf::toBox3(point, lhs.halfSize);
        if (glm::length(distance) <= 0)
            hitRecords.emplace_back(
                true,
                lhsModelMat * glm::vec4(physics::sign3(point) * distance, 1.f),
                normal);
    }
    
    return hitRecords;
}
