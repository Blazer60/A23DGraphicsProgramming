/**
 * @file CollisionSystem.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "CollisionSystem.h"
#include "PhysicsHelpers.h"
#include "Renderer.h"
#include "Timers.h"

#include <numeric>
#include <gtx/component_wise.hpp>
#include <unordered_set>

CollisionSystem::CollisionSystem(Renderer &renderer) :
    mRenderer(renderer)
{
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<BasicUniforms> &basicUniforms,
        const Velocity &velocity)
    {
        CollisionEntity entity = { boundingVolume, basicUniforms, velocity };
        mCollisionEntities.push_back(entity);
        
        const glm::vec3 center = basicUniforms->modelMat * glm::vec4(velocity.value, 1.f);
        if (auto sphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
        {
            const glm::vec3 axisAlignedHalfSize = velocity.value + glm::vec3(sphere->radius);
            
            mTree.insert(boundingVolume, octree::AABB { center, axisAlignedHalfSize } );
        }
        if (auto box = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
        {
            auto points = physics::boxToVertex(basicUniforms->modelMat, box->halfSize);
            glm::vec3 max = glm::vec3(0.f);
            
            for (auto &point : points)
            {
                point += velocity.value;
                max = glm::max(max, glm::abs(point));
            }
            mTree.insert(boundingVolume, octree::AABB { center, max });
        }
    });
}

void CollisionSystem::onUpdate()
{
    collisionLhs(); // O(n^2)
    
    mTree.debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, true);
    
    // Reset for next round.
    mCollisionEntities.clear();
}

void CollisionSystem::collisionLhs()
{
    for (const auto &[boundingVolume, basicUniforms, velocity] : mCollisionEntities)
    {
        const auto &modelMat = basicUniforms->modelMat;
        
        if (auto lhsSphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
            collisionRhs(*lhsSphere, modelMat, velocity.value);
        if (auto lhsBox = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
            collisionRhs(*lhsBox, modelMat, velocity.value);
    }
}

void CollisionSystem::collisionRhs(
    const BoundingSphere &lhs,
    const glm::mat4 &lhsModelMat,
    const glm::vec3 &lhsVelocity)
{
    for (const auto &[boundingVolume, basicUniforms, velocity] : mCollisionEntities)
    {
        const auto &rhsModelMat = basicUniforms->modelMat;
        const auto &rhsVelocity = velocity.value;
    
        if (lhs.entity == boundingVolume->entity)
            continue;
    
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
        {
            HitRecord hit = collisionCheck(lhs, lhsModelMat, lhsVelocity, *rhsSphere, rhsModelMat, rhsVelocity);
            if (hit.hit)
                lhs.callbacks.broadcast(lhs.entity, boundingVolume->entity, hit.position, hit.normal);
        }
    
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
        {
            HitRecord hit = collisionCheck(*rhsBox, rhsModelMat, rhsVelocity, lhs, lhsModelMat, lhsVelocity);
            if (hit.hit)
                lhs.callbacks.broadcast(lhs.entity, boundingVolume->entity, hit.position, hit.normal);
        }
    }
}

void CollisionSystem::collisionRhs(const BoundingBox &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity)
{
    for (const auto &[boundingVolume, basicUniforms, velocity] : mCollisionEntities)
    {
        const auto &modelMat = basicUniforms->modelMat;
        const auto &rhsVelocity = velocity.value;
        
        if (lhs.entity == boundingVolume->entity)
            continue;
    
        else if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
        {
            auto hit = collisionCheck(lhs, lhsModelMat, lhsVelocity, *rhsSphere, modelMat, rhsVelocity);
            hit.normal *= -1.f;
            if (hit.hit)
                lhs.callbacks.broadcast(lhs.entity, rhsSphere->entity, hit.position, hit.normal);
        }
    
        else if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
        {
            // Test in both directions since we are using vertex collision tests.
            auto lhsHits    = collisionCheck(lhs, lhsModelMat, lhsVelocity, *rhsBox, modelMat, rhsVelocity);
            auto rhsHits    = collisionCheck(*rhsBox, modelMat, rhsVelocity, lhs, lhsModelMat, lhsVelocity);
        
            if (lhsHits.empty() && rhsHits.empty())
                continue;
        
            // Average out all the hits.
            HitRecord hit { true, glm::vec3(0.f), glm::vec3(0.f) };
            for (const auto &lhsHit : lhsHits)
            {
                hit.position += lhsHit.position;
                hit.normal   -= lhsHit.normal;  // Opposite direction
            }
        
            for (const auto &rhsHit : rhsHits)
            {
                hit.position += rhsHit.position;
                hit.normal   += rhsHit.normal;
            }
    
            hit.position /= lhsHits.size() + rhsHits.size();
            hit.normal   /= lhsHits.size() + rhsHits.size();
        
            lhs.callbacks.broadcast(lhs.entity, rhsBox->entity, hit.position, hit.normal);
        }
    }
}

HitRecord CollisionSystem::collisionCheck(
    const BoundingSphere &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
    const BoundingSphere &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity)
{
    const glm::vec3 pointA   = lhsModelMat * glm::vec4(lhsVelocity * timers::fixedTime<float>(), 1.f);
    const float radiusA      = lhs.radius;
    
    const glm::vec3 pointB   = rhsModelMat * glm::vec4(rhsVelocity * timers::fixedTime<float>(), 1.f);
    const float radiusB      = rhs.radius;
    
    const glm::vec3 offset   = pointB - pointA;
    const float distance     = sdf::toSphere(offset, radiusA) - radiusB;
    const glm::vec3 normal   = glm::normalize(offset);
    const glm::vec3 position = pointA + radiusA * normal;
    
    return { distance <= 0, position, normal };
}

HitRecord CollisionSystem::collisionCheck(
    const BoundingBox &lhs,    const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
    const BoundingSphere &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity)
{
    const glm::mat4 lhsInverseModelMat  = glm::inverse(lhsModelMat);
    
    // Determines how far in front it should detect the collision.
    const glm::vec4 relativeVelocity    = glm::vec4((rhsVelocity - lhsVelocity) * timers::fixedTime<float>(), 1.f);
    const glm::vec3 point               = lhsInverseModelMat * rhsModelMat * relativeVelocity;
    const float     distance            = sdf::sphereToBox(point, rhs.radius, lhs.halfSize);
    
    const glm::vec3 signs  = physics::sign3(point);
    const glm::vec3 normal = glm::normalize(lhsModelMat * glm::vec4(signs * sdf::toBox3(point, lhs.halfSize), 0.f));
    
    const glm::vec3 position = rhsModelMat * relativeVelocity + glm::vec4(rhs.radius * -normal, 1.f);
    return { distance <= 0, position, normal };
}

std::vector<HitRecord> CollisionSystem::collisionCheck(
    const BoundingBox &lhs, const glm::mat4 &lhsModelMat, const glm::vec3 &lhsVelocity,
    const BoundingBox &rhs, const glm::mat4 &rhsModelMat, const glm::vec3 &rhsVelocity)
{
    const glm::mat4 lhsInverseModelMat  = glm::inverse(lhsModelMat);
    const glm::vec3 &halfSize           = rhs.halfSize;
    const glm::vec4 relativeVelocity    = glm::vec4((rhsVelocity - lhsVelocity) * timers::fixedTime<float>(), 1.f);
    const glm::vec3 boxCenter           = lhsInverseModelMat * rhsModelMat * relativeVelocity;
    const glm::vec3 normal              = lhsModelMat * glm::vec4(sdf::boxNormal(boxCenter, lhs.halfSize), 0.f);
    
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
        const glm::vec3 distance = sdf::toBox3(point + glm::vec3(relativeVelocity), lhs.halfSize);
        if (glm::length(distance) <= 0)
            hitRecords.emplace_back(
                true,
                lhsModelMat * glm::vec4(physics::sign3(point + glm::vec3(relativeVelocity)) * distance, 1.f),
                normal);
    }
    
    return hitRecords;
}
