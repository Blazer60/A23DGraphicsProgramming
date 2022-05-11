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


class Gravity
    : public ecs::BaseSystem<DynamicObject>
{
public:
    Gravity();
protected:
    const float gravitationalConstant { 9.81f };
};


class LinearEulerMethod
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    LinearEulerMethod();
};

class LinearKinematicSystem
    : public ecs::BaseSystem<Kinematic, Velocity, Transform>
{
public:
    LinearKinematicSystem();
};

class AngularEulerMethod
    : public ecs::BaseSystem<Torque, AngularObject, AngularVelocity, Transform>
{
public:
    AngularEulerMethod();
};

class LinearRk4
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    LinearRk4();
};

class LinearRk2
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    LinearRk2();
};

class LinearRk
    : public ecs::BaseSystem<DynamicObject, Velocity, Transform>
{
public:
    explicit LinearRk(const uint32_t degree);
protected:
    std::vector<float> mBinomials;
    float mSum { 0.f };
    const uint32_t mDegree;
};
