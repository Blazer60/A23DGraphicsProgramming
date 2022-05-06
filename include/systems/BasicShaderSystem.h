/**
 * @file BasicShaderSystem.h
 * @author Ryan Purse
 * @date 13/02/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "UniformComponents.h"
#include "RenderComponents.h"
#include "Shader.h"
#include "MainCamera.h"
#include "FilePaths.h"

/**
 * Renders basic items using the specified shader.
 * @author Ryan Purse
 * @date 13/02/2022
 */
class BasicShaderSystem
        : public ecs::BaseSystem<RenderInformation, std::shared_ptr<BasicUniforms>>
{
public:
    explicit BasicShaderSystem(std::shared_ptr<MainCamera> camera);
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "basic/Basic.vert", path::shaders() + "basic/Basic.frag" };
    std::shared_ptr<MainCamera> mCamera;
};


