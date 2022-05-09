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

Gravity::Gravity()
{
    mEntities.forEach([this](DynamicObject &dynamicObject) {
        auto &[force, mass, _] = dynamicObject;
        force.y -= mass * gravitationalConstant;
    });
    scheduleFor(ecs::FixedUpdate);
}

EulerIntegration::EulerIntegration()
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

RungeKutta2::RungeKutta2()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, _] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
        
        const glm::vec3 acceleration0 = force / mass;
        const glm::vec3 k0 = fixedTime * acceleration0;
        
        force *= 0.5f * k0;
        const glm::vec3 acceleration1 = force / mass;
        const glm::vec3 k1 = fixedTime * fixedTime * acceleration1;
        
        velocity.value += 0.5f * (k0 + k1);
        transform.position += velocity.value * fixedTime;
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

RungeKutta4::RungeKutta4()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass, _] = dynamicObject;
        const float fixedTime = timers::fixedTime<float>();
        
        glm::vec3 acceleration = force / mass;
        const glm::vec3 k0 = fixedTime * acceleration;
        
        force += k0 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k1 = fixedTime * acceleration;
        
        force += k1 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k2 = fixedTime * acceleration;
        
        force += k2 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k3 = fixedTime * acceleration;
        
        velocity.value += (k0 + 2.f * k1 + 2.f * k2 + k3) / 6.f;
        transform.position += velocity.value * fixedTime;
        
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

RungeKutta::RungeKutta(uint32_t degree)
    : mDegree(degree)
{
    float a = static_cast<float>(mDegree) - 1.f;
    float b = 1.f;
    float value = 1.f;
    for (int i = 0; i < mDegree; ++i)
    {
        mBinomials.emplace_back(1.f / value);
        value *= a--;
        value /= b++;
    }
    
    mEntities.forEach([this](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        const float deltaTime = timers::fixedTime<float>();
        auto &[force, mass, _]   = dynamicObject;
        
        for (const auto &binomial : mBinomials)
        {
            const glm::vec3 kBi = binomial * deltaTime * force / mass;
            
            force          += kBi;
            velocity.value += kBi;
        }
        
        transform.position += velocity.value * deltaTime;
        force = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}

KinematicSystem::KinematicSystem()
{
    mEntities.forEach([](const Kinematic &kinematic, const Velocity &velocity, Transform &transform) {
        transform.position += velocity.value * timers::fixedTime<float>();
    });
    scheduleFor(ecs::FixedUpdate);
}

AngularEulerIntegration::AngularEulerIntegration()
{
    mEntities.forEach([](Torque &torque, AngularObject &angularObject, AngularVelocity &angularVelocity, Transform &transform) {
        angularObject.angularMomentum += torque.tau * timers::fixedTime<float>();
        
        glm::mat3 rotation = glm::mat3_cast(transform.rotation);
        angularObject.inverseInertia = rotation * angularObject.inverseBodyInertia * glm::transpose(rotation);
        
        const glm::vec3 &omega = angularVelocity.omega;
        const glm::mat3 omegaStar = glm::mat3(
            0.f,      -omega.z, omega.y,
            omega.z,  0.f,      -omega.x,
            -omega.y, omega.x,  0.f);

        rotation = rotation + omegaStar * rotation * timers::fixedTime<float>();

        transform.rotation = glm::normalize(glm::quat(rotation));
        
        angularVelocity.omega = angularObject.inverseInertia * angularObject.angularMomentum;
        
        torque.tau = glm::vec3(0.f);
    });
    scheduleFor(ecs::FixedUpdate);
}
