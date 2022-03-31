/**
 * @file Components.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "Pch.h"
#include "glm.hpp"
#include "detail/type_quat.hpp"

/** Holds the position, orientation and scale of an object. Note: this should be split to improve cache miss optimisation. */
struct Transform
{
    glm::vec3 position { 0.f };
    glm::quat rotation { glm::vec3(0.f) };
    glm::vec3 scale    { 1.f };
};

/** Holds information about how to rotate an object. */
struct Rotator
{
    float       time            { 0.f };
    float       height          { 0.f };
    glm::vec3   originalHeight  { 0.f };
};
