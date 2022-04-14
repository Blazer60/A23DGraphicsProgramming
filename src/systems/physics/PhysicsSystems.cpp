/**
 * @file Gravity.cpp
 * @author Ryan Purse
 * @date 13/04/2022
 */


#include "PhysicsSystems.h"
#include "Physics.h"
#include "Components.h"
#include "Timers.h"

Gravity::Gravity()
{
    mEntities.forEach([this](DynamicObject &dynamicObject) {
        auto &[force, mass] = dynamicObject;
        force.y -= mass * gravitationalConstant;
    });
}

EulerIntegration::EulerIntegration()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass] = dynamicObject;
        const float deltaTime = timers::deltaTime<float>();
        
        const glm::vec3 acceleration = force / mass;
        
        velocity.value      += acceleration   * deltaTime;
        transform.position  += velocity.value * deltaTime;
    
        force = glm::vec3(0.f);
    });
}

RungeKutta2::RungeKutta2()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass] = dynamicObject;
        const float deltaTime = timers::deltaTime<float>();
        
        const glm::vec3 acceleration0 = force / mass;
        const glm::vec3 k0 = deltaTime * acceleration0;
        
        force *= 0.5f * k0;
        const glm::vec3 acceleration1 = force / mass;
        const glm::vec3 k1 = deltaTime * deltaTime * acceleration1;
        
        velocity.value += 0.5f * (k0 + k1);
        transform.position += velocity.value * deltaTime;
        
        force = glm::vec3(0.f);
    });
}

RungeKutta4::RungeKutta4()
{
    mEntities.forEach([](DynamicObject &dynamicObject, Velocity &velocity, Transform &transform) {
        auto &[force, mass] = dynamicObject;
        const float deltaTime = timers::deltaTime<float>();
        
        glm::vec3 acceleration = force / mass;
        const glm::vec3 k0 = deltaTime * acceleration;
        
        force += k0 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k1 = deltaTime * acceleration;
        
        force += k1 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k2 = deltaTime * acceleration;
        
        force += k2 * 0.5f;
        acceleration = force / mass;
        const glm::vec3 k3 = deltaTime * acceleration;
        
        velocity.value += (k0 + 2.f * k1 + 2.f * k2 + k3) / 6.f;
        transform.position += velocity.value * deltaTime;
        
        force = glm::vec3(0.f);
    });
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
        const float deltaTime = timers::deltaTime<float>();
        auto &[force, mass]   = dynamicObject;
        
        for (const auto &binomial : mBinomials)
        {
            const glm::vec3 kBi = binomial * deltaTime * force / mass;
            
            force          += kBi;
            velocity.value += kBi;
        }
        
        transform.position += velocity.value * deltaTime;
        force = glm::vec3(0.f);
    });
}
