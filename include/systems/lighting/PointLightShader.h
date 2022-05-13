/**
 * @file PointLightShader.h
 * @author Ryan Purse
 * @date 31/03/2022
 */


#pragma once

#include "Pch.h"
#include "Ecs.h"
#include "BaseSystem.h"
#include "LightingComponents.h"
#include "Shader.h"
#include "FramebufferObject.h"
#include "FilePaths.h"
#include "MainCamera.h"

/**
 * Renders all of the point lights within the scene.
 * @author Ryan Purse
 * @date 31/03/2022
 */
class PointLightShader
    : public ecs::BaseSystem<light::PointLight>
{
public:
    PointLightShader(
        std::shared_ptr<MainCamera> camera,
        std::shared_ptr<FramebufferObject> lightAccumulationBuffer,
        std::shared_ptr<TextureBufferObject> positions,
        std::shared_ptr<TextureBufferObject> normals,
        std::shared_ptr<TextureBufferObject> albedo);
    
    ~PointLightShader();
    
    void onUpdate() override;
protected:
    void bind();
    
    Shader mShader {
        path::resources() + "shaders/lighting/Lighting.vert",
        path::resources() + "shaders/lighting/PointLighting.frag"
    };
    
    unsigned int    mVao        { 0 };
    unsigned int    mEbo        { 0 };
    unsigned int    mVbo        { 0 };
    int             mEboCount   { 0 };
    
    std::shared_ptr<MainCamera>          mCamera;
    std::shared_ptr<FramebufferObject>   mLightAccumulationBuffer;
    
    std::shared_ptr<TextureBufferObject> mPositions;
    std::shared_ptr<TextureBufferObject> mNormals;
    std::shared_ptr<TextureBufferObject> mAlbedo;
};


