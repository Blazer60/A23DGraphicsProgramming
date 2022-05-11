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
    if (mEcs.hasComponent<AngularObject>(entity))
    {
        staticRotationalCollision(entity, other, position, normal);
    }
    else if (mEcs.hasComponent<DynamicObject>(other))
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
    
    const float vRel        = glm::dot(normal, velocity.value);
    const float numerator   = -(1.f + physicsMaterial.bounciness) * vRel;
    const float term1       = 1.f / dynamicObject.mass;
    const float j           = numerator / term1;
    const glm::vec3 force   = j * normal;
    
    dynamicObject.momentum += force;
    velocity.value          = dynamicObject.momentum / dynamicObject.mass;
}

void CollisionResponse::typedStaticCollision(Component dynamicType, Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal)
{
    auto &dynamicObject           = mEcs.getComponent<DynamicObject>(entity, dynamicType);
    auto &velocity                = mEcs.getComponent<Velocity>(entity);
    const auto &transform         = mEcs.getComponent<Transform>(entity);
    const auto &physicsMaterial   = mEcs.getComponent<PhysicsMaterial>(entity);
    
    const float vRel        = glm::dot(normal, velocity.value);
    const float numerator   = -(1.f + physicsMaterial.bounciness) * vRel;
    const float term1       = 1.f / dynamicObject.mass;
    const float j           = numerator / term1;
    const glm::vec3 force   = j * normal;
    
    dynamicObject.momentum += force;
    velocity.value          = dynamicObject.momentum / dynamicObject.mass;
}

void CollisionResponse::dynamicCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal)
{
    auto &lhsDynamicObject          = mEcs.getComponent<DynamicObject>(entity);
    auto &lhsVelocity               = mEcs.getComponent<Velocity>(entity);
    const auto &lhsPhysicsMaterial  = mEcs.getComponent<PhysicsMaterial>(entity);
    
    auto &rhsDynamicObject          = mEcs.getComponent<DynamicObject>(other);
    auto &rhsVelocity               = mEcs.getComponent<Velocity>(other);
    const auto &rhsPhysicsMaterial  = mEcs.getComponent<PhysicsMaterial>(other);
    
    const float vRel        = glm::dot(normal, lhsVelocity.value - rhsVelocity.value);
    
    if (vRel < 0.f)
        return;  // Both objects are separating from one another.
    
    const float e           = (lhsPhysicsMaterial.bounciness + rhsPhysicsMaterial.bounciness) / 2.f;
    const float numerator   = -(1.f + e) * vRel;
    const float term1       = 1.f / lhsDynamicObject.mass;
    const float term2       = 1.f / rhsDynamicObject.mass;
    const float j           = numerator / (term1 + term2);
    const glm::vec3 force   = j * normal;
    
    lhsDynamicObject.momentum += force;
    rhsDynamicObject.momentum -= force;
    
    lhsVelocity.value = lhsDynamicObject.momentum / lhsDynamicObject.mass;
    rhsVelocity.value = rhsDynamicObject.momentum / rhsDynamicObject.mass;
}

void CollisionResponse::staticRotationalCollision(
    Entity entity, Entity other,
    const glm::vec3 &position, const glm::vec3 &normal)
{
    auto &dynamicObject           = mEcs.getComponent<DynamicObject>(entity);
    auto &velocity                = mEcs.getComponent<Velocity>(entity);
    const auto &transform         = mEcs.getComponent<Transform>(entity);
    const auto &physicsMaterial   = mEcs.getComponent<PhysicsMaterial>(entity);
    auto &angularObject           = mEcs.getComponent<AngularObject>(entity);
    auto &angularVelocity         = mEcs.getComponent<AngularVelocity>(entity);
    auto &torque                  = mEcs.getComponent<Torque>(entity);
    
    debug::log("Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z));
    
    const glm::vec3 &omega = angularVelocity.omega;
    const glm::vec3 paDot = velocity.value + glm::cross(omega, position - transform.position);
    const glm::vec3 ra    = position - transform.position;
    const float     vRel  = glm::dot(normal, paDot);
    
    const float numerator = -(1.f + physicsMaterial.bounciness) * vRel;
    const float term1     = 1.f / dynamicObject.mass;
    const float term3     = glm::dot(normal, glm::cross(angularObject.inverseInertia * glm::cross(ra, normal), ra));
    const float         j = numerator / (term1 + term3);
    const glm::vec3 force = j * normal;
    
    // Applying Impulse
    dynamicObject.momentum += force;
    angularObject.angularMomentum += glm::cross(ra, force);
    
    // Recomputing auxiliaries in response
    velocity.value = dynamicObject.momentum / dynamicObject.mass;
    angularVelocity.omega = angularObject.inverseInertia * angularObject.angularMomentum;
}
