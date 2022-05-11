/**
 * @file Gravity.cpp
 * @author Ryan Purse
 * @date 13/04/2022
 */


#include "PhysicsSystems.h"
#include "physics/components/Physics.h"
#include "Components.h"
#include "Timers.h"
#include "gtc/quaternion.hpp"
#include "PhysicsHelpers.h"

Gravity::Gravity()
{
    mEntities.forEach([this](DynamicObject &dynamicObject) {
        auto &[force, mass, _] = dynamicObject;
        force.y -= mass * gravitationalConstant;
    });
    scheduleFor(ecs::FixedUpdate);
}

LinearEulerMethod::LinearEulerMethod()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, momentum] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
    
        // Update the position first. Collision responses can update auxiliary values.
        transform.position += velocity.value * fixedTime;
        
        // Calculate the forces for the next frame.
        momentum        += force * fixedTime;
        velocity.value  = momentum / mass;
        force           = glm::vec3(0.f);  // Reset the forces for the next frame.
    });
    scheduleFor(ecs::FixedUpdate);
}

LinearKinematicSystem::LinearKinematicSystem()
{
    mEntities.forEach([](const Kinematic &kinematic, const Velocity &velocity, Transform &transform) {
        transform.position += velocity.value * timers::fixedTime<float>();
    });
    scheduleFor(ecs::FixedUpdate);
}

AngularEulerMethod::AngularEulerMethod()
{
    mEntities.forEach([](Torque &torque, AngularObject &angularObject, AngularVelocity &angularVelocity, Transform &transform) {
        angularObject.angularMomentum += torque.tau * timers::fixedTime<float>();
        
        glm::mat3 rotation = glm::mat3_cast(transform.rotation);
        angularObject.inverseInertia = rotation * angularObject.inverseBodyInertia * glm::transpose(rotation);
        
        angularVelocity.omega = angularObject.inverseInertia * angularObject.angularMomentum;
        
        const glm::vec3 &omega = angularVelocity.omega;
        const glm::mat3 omegaStar = glm::mat3(
            0.f,      -omega.z, omega.y,
            omega.z,  0.f,      -omega.x,
            -omega.y, omega.x,  0.f);

        rotation = rotation + omegaStar * rotation * timers::fixedTime<float>();

        transform.rotation = glm::normalize(glm::quat_cast(rotation));
        
        torque.tau = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

LinearRk4::LinearRk4()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, momentum] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
        
        // This is added on before as collision reactions can impact the velocity.
        // This makes everything slightly more stable as a tiny amount of energy is added rather than removed.
        // Should ideally be moved to after rk4 has happened.
        transform.position += velocity.value * fixedTime;
        
        const glm::vec3 k1 = physics::calculateMomentum(fixedTime,           force);
        const glm::vec3 k2 = physics::calculateMomentum(0.5f * fixedTime,    force + 0.5f * fixedTime * k1);
        const glm::vec3 k3 = physics::calculateMomentum(0.5f * fixedTime,    force + 0.5f * fixedTime * k2);
        const glm::vec3 k4 = physics::calculateMomentum(fixedTime,           force + fixedTime * k3);
        
        // Weighted average of each K value to determine momentum.
        momentum += (k1 + 2.f * k2 + 2.f * k3 + k4) / 6.f;
        velocity.value = momentum / mass;
    
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

LinearRk2::LinearRk2()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, momentum] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
    
        // This is added on before as collision reactions can impact the velocity.
        // This makes everything slightly more stable as a tiny amount of energy is added rather than removed.
        // Should ideally be moved to after rk2 has happened.
        transform.position += velocity.value * fixedTime;
        
        const glm::vec3 k1 = physics::calculateMomentum(fixedTime, force);
        const glm::vec3 k2 = physics::calculateMomentum(0.5f * fixedTime, force + 0.5f * fixedTime * k1);
        
        momentum += (k1 + k2) / 2.f;
        velocity.value = momentum / mass;
        
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

LinearRk::LinearRk(const uint32_t degree)
    : mDegree(degree)
{
    float a = static_cast<float>(mDegree) - 1.f;
    float b = 1.f;
    float value = 1.f;
    for (int i = 0; i < mDegree; ++i)
    {
        mBinomials.emplace_back(1.f / value);
        mSum += value;
        value *= a--;
        value /= b++;
    }
    
    mEntities.forEach([this](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, momentum] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
    
        // This is added on before as collision reactions can impact the velocity.
        // This makes everything slightly more stable as a tiny amount of energy is added rather than removed.
        // Should ideally be moved to after rk(n) has happened.
        transform.position += velocity.value * fixedTime;
        
        glm::vec3 momentumDelta = glm::vec3(0.f);
        glm::vec3 previousK = glm::vec3(0.f);
        for (const float binomial : mBinomials)
        {
            const glm::vec3 k = physics::calculateMomentum(binomial * fixedTime, force + binomial * fixedTime * previousK);
            momentumDelta += (1.f / binomial) * k;
            previousK = k;
        }
        
        momentum += momentumDelta / mSum;
        velocity.value = momentum / mass;
        
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}
