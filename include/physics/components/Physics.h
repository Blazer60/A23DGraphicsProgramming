/**
 * @file Physics.h
 * @author Ryan Purse
 * @date 13/04/2022
 */


#pragma once

#include "Pch.h"
#include "Callback.h"

struct DynamicObject
{
    glm::vec3   force   { 0.f };
    float       mass    { 10.f };
};

struct Velocity
{
    glm::vec3 value { 0.f };
};

struct Torque
{
    glm::vec3 tau { 0.f };
};

struct AngularObject
{
    glm::mat3 inverseBodyInertia { 1.f };
    glm::vec3 angularMomentum { 0.f };
};

struct Kinematic
{
    // Only used for testing currently. Has no data assigned to it.
};

struct PhysicsMaterial
{
    float bounciness { 0.2f };
};
