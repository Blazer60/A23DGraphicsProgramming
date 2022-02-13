/**
 * @file BasicShaderSystem.h
 * @author Ryan Purse
 * @date 13/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"
#include "Shader.h"

/**
 * Renders basic items using the specified shader.
 * @author Ryan Purse
 * @date 13/02/2022
 */
class BasicShaderSystem
        : public ecs::BaseSystem<Vao, Fbo, EboCount>
{
public:
    BasicShaderSystem();
    
    void onUpdate() override;

protected:
    Shader mShader { "../res/shaders/basic/Basic.vert", "../res/shaders/basic/Basic.frag" };
};


