/**
 * @file EmissivePbrGeometryShader.h
 * @author Ryan Purse
 * @date 18/05/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BaseSystem.h"
#include "RenderComponents.h"
#include "UniformComponents.h"
#include "MaterialComponents.h"
#include "MainCamera.h"
#include "Shader.h"
#include "FilePaths.h"
#include "FramebufferObject.h"
#include "Components.h"

/**
 * Writes emissive materials to the geometry buffer.
 * @author Ryan Purse
 * @date 18/05/2022
 */
class EmissivePbrGeometryShader
    : public ecs::BaseSystem<RenderInformation, std::shared_ptr<ModelMatrix>, EmissivePbrMaterial>
{
public:
    EmissivePbrGeometryShader(std::shared_ptr<MainCamera> camera, std::shared_ptr<FramebufferObject> output);
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "/blinn-phong/BlinnPhong.vert", path::shaders() + "/geometry/EmissiveGeometry.frag" };
    std::shared_ptr<MainCamera> mCamera;
    std::shared_ptr<FramebufferObject> mOutput;
};
