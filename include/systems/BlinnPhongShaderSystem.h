/**
 * @file BlinnPhongShaderSystem.h
 * @author Ryan Purse
 * @date 07/03/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BaseSystem.h"
#include "RenderComponents.h"
#include "UniformComponents.h"
#include "TextureComponents.h"
#include "Shader.h"
#include "MainCamera.h"
#include "DirectionalLight.h"
#include "FilePaths.h"
#include "MaterialComponents.h"

/**
 * Does forward Blinn-phong shading of models.
 * @author Ryan Purse
 * @date 07/03/2022
 */
class BlinnPhongShaderSystem
        : public ecs::BaseSystem<RenderInformation, std::shared_ptr<BasicUniforms>, BlinnPhongMaterial>
{
public:
    BlinnPhongShaderSystem(std::shared_ptr<MainCamera> camera, std::shared_ptr<DirectionalLight> directionalLight);
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "/blinn-phong/BlinnPhong.vert", path::shaders() + "/blinn-phong/BlinnPhong.frag" };
    std::shared_ptr<MainCamera> mCamera;
    std::shared_ptr<DirectionalLight> mDirectionalLight;
};


