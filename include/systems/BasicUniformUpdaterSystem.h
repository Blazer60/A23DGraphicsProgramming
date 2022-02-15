/**
 * @file BasicUniformUpdaterSystem.h
 * @author Ryan Purse
 * @date 14/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"

/**
 * Updates the Basic uniforms by the transform of an object.
 * @author Ryan Purse
 * @date 14/02/2022
 */
class BasicUniformUpdaterSystem
        : public ecs::BaseSystem<std::shared_ptr<BasicUniforms>, Transform>
{
public:
    BasicUniformUpdaterSystem();
};


