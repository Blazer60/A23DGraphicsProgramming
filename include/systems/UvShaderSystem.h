/**
 * @file UvShaderSystem.h
 * @author Ryan Purse
 * @date 15/02/2022
 */


#pragma once

#include "Ecs.h"
#include "Components.h"
#include "Shader.h"
#include "MainCamera.h"

/**
 * Renders Uv Meshes to the specified Vbo
 * @author Ryan Purse
 * @date 15/02/2022
 */
class UvShaderSystem
        : public ecs::BaseSystem<RenderCoreElements, std::shared_ptr<BasicUniforms>, UvUniforms>
{
public:
    explicit UvShaderSystem(std::shared_ptr<MainCamera> camera);
    
    void onStart() override;
    
    void onUpdate() override;

protected:
    Shader mShader { "../res/shaders/uv/Uv.vert", "../res/shaders/uv/Uv.frag" };
    std::shared_ptr<MainCamera> mCamera;
};


