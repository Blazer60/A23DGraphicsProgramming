/**
 * @file BlinnPhongGeometryShader.h
 * @author Ryan Purse
 * @date 21/03/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BaseSystem.h"
#include "RenderComponents.h"
#include "UniformComponents.h"
#include "MaterialComponents.h"
#include "MainCamera.h"
#include "DirectionalLight.h"
#include "Shader.h"
#include "FilePaths.h"
#include "FramebufferObject.h"

/**
 * A Blinn-Phong Shader the writes to the Geometry buffer rather than the main buffer.
 * @author Ryan Purse
 * @date 21/03/2022
 */
class BlinnPhongGeometryShader
    : public ecs::BaseSystem<RenderInformation, std::shared_ptr<BasicUniforms>, BlinnPhongMaterial>
{
public:
    BlinnPhongGeometryShader(std::shared_ptr<MainCamera> camera, std::shared_ptr<FramebufferObject> framebuffer);
    
    void onUpdate() override;

protected:
    Shader mShader { path::shaders() + "/blinn-phong/BlinnPhong.vert", path::shaders() + "/blinn-phong/BlinnPhongGeometry.frag" };
    std::shared_ptr<MainCamera> mCamera;
    std::shared_ptr<FramebufferObject> mFrameBufferObject;
};


