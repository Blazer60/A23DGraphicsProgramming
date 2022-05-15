/**
 * @file DirectionalLightShaderSystem.h
 * @author Ryan Purse
 * @date 23/03/2022
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
 * Handles the rendering of directional lights to a framebuffer.
 * @author Ryan Purse
 * @date 23/03/2022
 */
class DirectionalLightShaderSystem
    : public ecs::BaseSystem<light::DirectionalLight>
{
public:
    DirectionalLightShaderSystem(
        std::shared_ptr<MainCamera> camera,
        std::shared_ptr<FramebufferObject> lightAccumulationBuffer,
        std::shared_ptr<TextureBufferObject> positions,
        std::shared_ptr<TextureBufferObject> normals,
        std::shared_ptr<TextureBufferObject> albedo);
    
    ~DirectionalLightShaderSystem();
    
    void onUpdate() override;

protected:
    void bind();
    
    Shader mShader {
        path::shaders() + "/ScreenQuad.vert",
        path::shaders() + "/lighting/DirectionalLighting.frag"
    };
    
    
    unsigned int    mVao        { 0 };
    unsigned int    mEbo        { 0 };
    unsigned int    mVbo        { 0 };
    int             mEboCount   { 0 };
    
    std::shared_ptr<MainCamera>          mCamera;
    std::shared_ptr<FramebufferObject>   mLightAccumulationBuffer;
    
    // Todo: this needs to be converted into a struct of Texture buffers called the geometry buffer.
    std::shared_ptr<TextureBufferObject> mPositions;
    std::shared_ptr<TextureBufferObject> mNormals;
    std::shared_ptr<TextureBufferObject> mAlbedo;
};


