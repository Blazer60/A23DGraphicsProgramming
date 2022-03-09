/**
 * @file BlinnPhongShaderSystem.h
 * @author Ryan Purse
 * @date 07/03/2022
 */


#pragma once

#include "Ecs.h"
#include "BaseSystem.h"
#include "RenderComponents.h"
#include "UniformComponents.h"
#include "TextureComponents.h"
#include "Shader.h"
#include "MainCamera.h"
#include "DirectionalLight.h"
#include "FilePaths.h"

/**
 * Does forward Blinn-phong shading of models.
 * @author Ryan Purse
 * @date 07/03/2022
 */
class BlinnPhongShaderSystem
        : public ecs::BaseSystem<RenderCoreElements, std::shared_ptr<BasicUniforms>, UvUniforms, Texture>
{
public:
    explicit BlinnPhongShaderSystem(std::shared_ptr<MainCamera> camera, std::shared_ptr<DirectionalLight> directionalLight);
    
    void onStart() override;
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "/blinn-phong/BlinnPhong.vert", path::shaders() + "/blinn-phong/BlinnPhong.frag" };
    std::shared_ptr<MainCamera> mCamera;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
};


