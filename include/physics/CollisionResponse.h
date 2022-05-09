/**
 * @file ScenePhysics.h
 * @author Ryan Purse
 * @date 24/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"

/**
 * A collection of collision based responses that are tied to a specific scene.
 * @author Ryan Purse
 * @date 24/04/2022
 */
class CollisionResponse
{
public:
    explicit CollisionResponse(ecs::Core &ecs);
    
    void makePhysicsObject(
        const Entity entity, const glm::vec3 &velocity=glm::vec3{0.f},
        const float mass=100.f, const float bounciness=0.2f);
    
    void makeBoundingBox(const Entity entity, const bool isDynamic = true, const glm::vec3 &halfSize = glm::vec3(1.f));
    void makeBoundingSphere(const Entity entity, const bool isDynamic = true, const float radius = 1.f);
    
    void response(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal);
    
    void staticCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal);
    
    void dynamicCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal);
protected:
    ecs::Core &mEcs;
    
    void staticRotationalCollision(Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal);
};
