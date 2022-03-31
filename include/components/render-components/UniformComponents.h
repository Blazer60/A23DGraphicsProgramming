/**
 * @file UniformComponents.h
 * @author Ryan Purse
 * @date 21/02/2022
 */


#pragma once

#include "Pch.h"
#include "glm.hpp"

/**
 * @brief Contains a single model matrix that can be updated every frame.
 */
struct BasicUniforms
{
    glm::mat4 modelMat { 1.f };
};

/**
 * @brief Contains a colour to colour an entire mesh.
 */
struct UvUniforms
{
    glm::vec3 colour { 1.f };
};
