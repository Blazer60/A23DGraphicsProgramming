/**
 * @file ScenePhysics.cpp
 * @author Ryan Purse
 * @date 24/04/2022
 */


#include "CollisionResponse.h"
#include "physics/components/Physics.h"
#include "PhysicsHelpers.h"
#include "Components.h"
#include "physics/components/BoundingVolumes.h"
#include "Timers.h"

CollisionResponse::CollisionResponse(ecs::Core &ecs) :
    mEcs(ecs)
{}

void CollisionResponse::makePhysicsObject(
    const Entity entity, const glm::vec3 &velocity,
    const float mass, const float bounciness)
{
    if (!mEcs.hasComponent<DynamicObject>(entity))
        mEcs.add(entity, DynamicObject { glm::vec3(0.f), mass } );
    
    if (!mEcs.hasComponent<Velocity>(entity))
        mEcs.add(entity, Velocity { velocity } );
    
    if (!mEcs.hasComponent<Transform>(entity))
        mEcs.add(entity, Transform {  } );
    
    if (!mEcs.hasComponent<PhysicsMaterial>(entity))
        mEcs.add(entity, PhysicsMaterial { bounciness } );
}

void CollisionResponse::makeBoundingBox(const Entity entity, const bool isDynamic, const glm::vec3 &halfSize)
{
    if (!mEcs.hasComponent<std::shared_ptr<BoundingVolume>>(entity))
    {
        std::shared_ptr<BoundingVolume> boundingVolume = std::make_shared<BoundingBox>(entity, halfSize);
        if (isDynamic)
            boundingVolume->callbacks.subscribe([this](
                Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal) {
                response(entity, other, position, normal);
            });
        
        mEcs.add(entity, boundingVolume);
    }
    if (!mEcs.hasComponent<Velocity>(entity))
        mEcs.add(entity, Velocity {  } );
}

void CollisionResponse::makeBoundingSphere(const Entity entity, const bool isDynamic, const float radius)
{
    if (!mEcs.hasComponent<std::shared_ptr<BoundingVolume>>(entity))
    {
        std::shared_ptr<BoundingVolume> boundingVolume = std::make_shared<BoundingSphere>(entity, radius);
        if (isDynamic)
            boundingVolume->callbacks.subscribe([this](
                Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal) {
                response(entity, other, position, normal);
            });
        
        mEcs.add(entity, boundingVolume);
    }
}

void CollisionResponse::response(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal)
{
    if (mEcs.hasComponent<DynamicObject>(other))
        dynamicCollision(entity, other, position, normal);
    else
        staticCollision(entity, other, position, normal);
}

void CollisionResponse::staticCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal)
{
    auto &dynamicObject           = mEcs.getComponent<DynamicObject>(entity);
    auto &velocity                = mEcs.getComponent<Velocity>(entity);
    const auto &transform         = mEcs.getComponent<Transform>(entity);
    const auto &physicsMaterial   = mEcs.getComponent<PhysicsMaterial>(entity);
    
    const float impulse = -(1.f + physicsMaterial.bounciness) * glm::dot(velocity.value, normal) / (1.f / dynamicObject.mass);
    velocity.value += normal * impulse / dynamicObject.mass;
    
 
    const glm::vec3 contactForce = dynamicObject.force * glm::dot(physics::normalise(dynamicObject.force), normal);

    dynamicObject.force += contactForce;

}

void CollisionResponse::dynamicCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal)
{
    return;
    const auto &lhsDynamicObject     = mEcs.getComponent<DynamicObject>(entity);
    const auto &lhsVelocity          = mEcs.getComponent<Velocity>(entity);
    const auto &lhsPhysicsMaterial   = mEcs.getComponent<PhysicsMaterial>(entity);
    // auto       &lhsAccumulator       = mEcs.getComponent<Accumulator>(entity);
    
    const auto &rhsDynamicObject     = mEcs.getComponent<DynamicObject>(other);
    const auto &rhsVelocity          = mEcs.getComponent<Velocity>(other);
    const auto &rhsPhysicsMaterial   = mEcs.getComponent<PhysicsMaterial>(other);
    
    // const glm::vec3 relativeVelocity    = rhsVelocity.value - lhsVelocity.value;
    const glm::vec3 relativeVelocity    = lhsVelocity.value - rhsVelocity.value;
    const float     inverseMass         = (1.f / lhsDynamicObject.mass) + (1.f / rhsDynamicObject.mass);
    const float     dot                 = glm::dot(relativeVelocity, normal);
    const float     bounciness          = (lhsPhysicsMaterial.bounciness + rhsPhysicsMaterial.bounciness) / 2.f;
    const float     impulse             = -(1.f + bounciness) * dot / inverseMass;
    
    // lhsAccumulator.velocity += normal * impulse / lhsDynamicObject.mass;
    
    
    const glm::vec3 lhsContactForce = lhsDynamicObject.force * glm::dot(physics::normalise(lhsDynamicObject.force), normal) * timers::fixedTime<float>();
    const glm::vec3 rhsContactForce = rhsDynamicObject.force * glm::dot(physics::normalise(rhsDynamicObject.force), normal) * timers::fixedTime<float>();

    // lhsAccumulator.force += lhsContactForce + rhsContactForce;
}
