/**
 * @file CollisionSystem.cpp
 * @author Ryan Purse
 * @date 20/04/2022
 */


#include "CollisionDetection.h"
#include "PhysicsHelpers.h"
#include "Renderer.h"
#include "Timers.h"

#include <numeric>
#include "gtx/component_wise.hpp"
#include <unordered_set>

CollisionDetection::CollisionDetection(Renderer &renderer, std::shared_ptr<octree::Tree<CollisionEntity>> tree) :
    mRenderer(renderer), mTree(std::move(tree))
{
    mEntities.forEach([this](
        std::shared_ptr<BoundingVolume> &boundingVolume,
        std::shared_ptr<BasicUniforms> &basicUniforms,
        const Velocity &velocity)
    {
        const glm::vec3 center = basicUniforms->modelMat * glm::vec4(velocity.value * timers::fixedTime<float>(), 1.f);
        if (auto sphere = std::dynamic_pointer_cast<BoundingSphere>(boundingVolume))
        {
            octree::AABB bounds { center, glm::vec3(sphere->radius) };
            
            traverseTree(sphere, basicUniforms, velocity, bounds);
        }
        if (auto box = std::dynamic_pointer_cast<BoundingBox>(boundingVolume))
        {
            auto points = physics::boxToVertex(basicUniforms->modelMat, box->halfSize);
            glm::vec3 max = glm::vec3(0.f);
            
            for (auto &point : points)
                max = glm::max(max, glm::abs(point));
            
            max = (max - glm::abs(center));
            octree::AABB bounds { center, max };
            
            traverseTree(box, basicUniforms, velocity, bounds);
        }
    });
    scheduleFor(ecs::FixedUpdate);
}

HitRecord CollisionDetection::collisionCheck(
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

HitRecord CollisionDetection::collisionCheck(
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

std::vector<HitRecord> CollisionDetection::collisionCheck(
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

void CollisionDetection::traverseTree(
    std::shared_ptr<BoundingSphere> lhsEntity,
    std::shared_ptr<BasicUniforms> &uniforms,
    const Velocity &velocity,
    octree::AABB bounds)
{
    const glm::mat4 &lhsModelMatrix = uniforms->modelMat;
    const glm::vec3 &lhsVelocity = velocity.value;
    
    std::vector<CollisionEntity> intersectingEntities = mTree->getIntersecting(bounds);
    for (const auto &rhsEntity : intersectingEntities)
    {
        if (rhsEntity.boundingVolume->entity == lhsEntity->entity)
            continue;
    
        const glm::mat4 &rhsModelMatrix = rhsEntity.basicUniforms->modelMat;
        const glm::vec3 &rhsVelocity = rhsEntity.velocity.value;
    
        if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhsEntity.boundingVolume))
        {
            HitRecord record = collisionCheck(*lhsEntity, lhsModelMatrix, lhsVelocity, *rhsSphere, rhsModelMatrix, rhsVelocity);
            if (record.hit)
                lhsEntity->callbacks.broadcast(lhsEntity->entity, rhsSphere->entity, record.position, record.normal);
        }
        if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhsEntity.boundingVolume))
        {
            HitRecord record = collisionCheck(*rhsBox, rhsModelMatrix, rhsVelocity, *lhsEntity, lhsModelMatrix, lhsVelocity);
            if (record.hit)
                lhsEntity->callbacks.broadcast(lhsEntity->entity, rhsBox->entity, record.position, record.normal);
        }
    }
}

void CollisionDetection::traverseTree(
    std::shared_ptr<BoundingBox> lhsEntity,
    std::shared_ptr<BasicUniforms> &uniforms,
    const Velocity &velocity,
    octree::AABB bounds)
{
    const glm::mat4 &lhsModelMatrix = uniforms->modelMat;
    const glm::vec3 &lhsVelocity = velocity.value;
    
    std::vector<CollisionEntity> intersectingEntities = mTree->getIntersecting(bounds);
    for (const auto &rhsEntity : intersectingEntities)
    {
        if (rhsEntity.boundingVolume->entity == lhsEntity->entity)
            continue;
    
        const glm::mat4 &rhsModelMatrix = rhsEntity.basicUniforms->modelMat;
        const glm::vec3 &rhsVelocity = rhsEntity.velocity.value;
    
        if (auto rhsSphere = std::dynamic_pointer_cast<BoundingSphere>(rhsEntity.boundingVolume))
        {
            HitRecord record = collisionCheck(*lhsEntity, lhsModelMatrix, lhsVelocity, *rhsSphere, rhsModelMatrix, rhsVelocity);
            record.normal *= -1.f;
            if (record.hit)
                lhsEntity->callbacks.broadcast(lhsEntity->entity, rhsSphere->entity, record.position, record.normal);
        }
        if (auto rhsBox = std::dynamic_pointer_cast<BoundingBox>(rhsEntity.boundingVolume))
        {
            // Test in both directions since we are using vertex collision tests.
            std::vector<HitRecord> lhsHits = collisionCheck(*lhsEntity, lhsModelMatrix, lhsVelocity, *rhsBox, rhsModelMatrix, rhsVelocity);
            std::vector<HitRecord> rhsHits = collisionCheck(*rhsBox, rhsModelMatrix, rhsVelocity, *lhsEntity, lhsModelMatrix, lhsVelocity);
        
            if (lhsHits.empty() && rhsHits.empty())
                continue;
        
            // Average out all the hits.
            HitRecord hit { true, glm::vec3(0.f), glm::vec3(0.f) };
            for (const HitRecord &record : lhsHits)
            {
                hit.position += record.position;
                hit.normal   -= record.normal;  // Opposite direction
            }
            for (const HitRecord &record : rhsHits)
            {
                hit.position += record.position;
                hit.normal   += record.normal;
            }
        
            const float count = static_cast<float>((lhsHits.size() + rhsHits.size()));
            hit.position /= count;
            hit.normal   /= count;
        
            lhsEntity->callbacks.broadcast(lhsEntity->entity, rhsBox->entity, hit.position, hit.normal);
        }
    }
}
