/**
 * @file Gravity.h
 * @author Ryan Purse
 * @date 13/04/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Timers.h"

class DynamicObject;
class Velocity;
class Transform;
class Kinematic;
class AngularVelocity;
class Torque;
class AngularObject;

/**
 * Applies gravity to all dynamic objects within the scene.
 * @author Ryan Purse
 * @date 13/04/2022
 */
class Gravity
    : public ecs::BaseSystem<DynamicObject>
{
public:
    Gravity();
protected:
    float gravitationalConstant { 9.81f };
};

/**
 * Applies Euler's method to everything within the scene.
 * @author Ryan Purse
 * @date 13/04/2022
 */
class EulerIntegration
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    EulerIntegration();
};

/**
 * @brief Applies rk2's method to everything within the scene.
 * @author Ryan Purse
 * @date 13/04/2022
 */
class RungeKutta2
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    RungeKutta2();
};

class RungeKutta4
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    RungeKutta4();
};

class RungeKutta
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    explicit RungeKutta(uint32_t degree);
protected:
    std::vector<float>  mBinomials;
    uint32_t            mDegree { 4 };
};

class KinematicSystem
    : public ecs::BaseSystem<Kinematic, Velocity, Transform>
{
public:
    KinematicSystem();
};

class AngularEulerIntegration
    : public ecs::BaseSystem<Torque, AngularObject, AngularVelocity, Transform>
{
public:
    AngularEulerIntegration();
};

class MomentumRk4
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    MomentumRk4();
    
protected:
    glm::vec3 calculateMomentum(const float time, const glm::vec3 &force);
};
