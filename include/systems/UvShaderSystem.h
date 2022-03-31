/**
 * @file UvShaderSystem.h
 * @author Ryan Purse
 * @date 15/02/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "Components.h"
#include "RenderComponents.h"
#include "UniformComponents.h"
#include "TextureComponents.h"
#include "Shader.h"
#include "MainCamera.h"
#include "FilePaths.h"

/**
 * Renders Uv Meshes to the specified Vbo
 * @author Ryan Purse
 * @date 15/02/2022
 */
class UvShaderSystem
        : public ecs::BaseSystem<RenderInformation, std::shared_ptr<BasicUniforms>, UvUniforms, Texture>
{
public:
    explicit UvShaderSystem(std::shared_ptr<MainCamera> camera);
    
    void onStart() override;
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "/uv/Uv.vert", path::shaders() + "/uv/Uv.frag" };
    std::shared_ptr<MainCamera> mCamera;
};


