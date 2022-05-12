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
    
    float Jlinear = 0.0f;	// Linear impulse
    float Jangular = 0.0f;	// Angular impulse
    float e = physicsMaterial.bounciness;	// Friction
    glm::vec3 r1 = glm::vec3(0.0f, 1.0f, 0.0f); // Lever between the COM and the point of contact
    
    float oneOverMass1 = 1.0f / dynamicObject.mass;	// 1/m of object 1
    float oneOverMass2 = 0.0f;			// 1/m of object 2
    glm::vec3 vA = velocity.value;			// velocity of object 1
    glm::vec3 vB = glm::vec3(0.0f, 0.0f, 0.0f); // velocity of object 2
    glm::vec3 relativeVelocity = vA - vB;	// The relative velocity of both objects
    glm::vec3 contactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    
    Jlinear = (glm::dot(-(1.0f + e) * (vA), contactNormal)) / oneOverMass1 + oneOverMass2;
    Jangular = (glm::dot(-(1.0f + e) * (relativeVelocity), contactNormal)) / (oneOverMass1 + oneOverMass2 + glm::dot(angularObject.inverseInertia * (r1 * contactNormal), contactNormal));
    
    glm::vec3 impulseForce = (Jangular + Jlinear) * contactNormal; // Fi = (Jang + Jlin) * cN
    glm::vec3 contactForce = (9.81f * dynamicObject.mass)* contactNormal;
    
    dynamicObject.force += impulseForce + contactForce;
    dynamicObject.momentum += impulseForce; // Adding the impulse onto the velocity
    velocity.value = dynamicObject.momentum / dynamicObject.mass;
    
    glm::vec3 forwardRelativeVelocity = relativeVelocity - glm::dot(relativeVelocity, contactNormal) * contactNormal; // Finding the relative velocity perpendicular to the contact normal
    
    glm::vec3 forwardRelativeDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    if (forwardRelativeVelocity != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        forwardRelativeDirection = glm::normalize(forwardRelativeVelocity); // gets a normalized vector of the direction travelled perpendicular to the contact normal
    }
    
    float mu = 0.3f;
    glm::vec3 frictionDirection = forwardRelativeDirection * -1.0f; // friction direction acts in opposite direction of direction travelled
    glm::vec3 frictionForce = frictionDirection * glm::length(contactForce) * mu; // friction = mu * normal force
    
    if (glm::length(forwardRelativeVelocity) - ((glm::length(frictionForce) / dynamicObject.mass) * timers::fixedTime<float>()) > 0.0f) // Checks to see if the friction force would reverse the direction of travel
    {
        dynamicObject.force += frictionForce;
    }
    else
    {
        frictionForce = forwardRelativeVelocity *-1.0f; // Adds enough friction to stop the object
        dynamicObject.force += frictionForce;
    }
    
    glm::vec3 tempTorque = (glm::cross(r1, contactForce)) + (glm::cross(r1, frictionForce));
    tempTorque -= angularObject.angularMomentum * 5.f;
    
    torque.tau += tempTorque;
}
