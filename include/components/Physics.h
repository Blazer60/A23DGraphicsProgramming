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
