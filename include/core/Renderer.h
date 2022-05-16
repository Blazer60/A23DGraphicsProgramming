/**
 * @file Renderer.h
 * @author Ryan Purse
 * @date 30/03/2022
 */


#pragma once


#include "MainCamera.h"
#include "DeferredLightShader.h"
#include "Ecs.h"
#include "RenderComponents.h"
#include "Mesh.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"
#include "MipViewerShader.h"
#include "Bloom.h"
#include "WindowHelpers.h"
#include "MipmapTexture.h"

struct ViewSettings
{
    bool positionTexture    { false };
    bool normalTexture      { false };
    bool albedoTexture      { false };
    bool diffuseTexture     { false };
    bool specularTexture    { false };
    bool lightTargetTexture { true  };
    bool downSampleTextures { true  };
    bool upSampleTextures   { false };
    bool postProcessTexture { true };
};

/**
 * Sets up the rendering pipeline for a single scene. Used to setup deferred rendering and deferred lighting.
 * @author Ryan Purse
 * @date 30/03/2022
 */
class Renderer
{
public:
    Renderer(std::shared_ptr<MainCamera> camera, ecs::Core &EntityComponentSystem);
    
    void clear();
    void update();
    void imguiUpdate();
    void imguiMenuUpdate();
    
    void drawBox(const glm::mat4 &modelMatrix, const glm::vec3 &halfSize);
    
    
protected:
    // NOTE: Order of declaration matters here.
    ecs::Core &mEcs;
    
public:
    // Tags let us know which object should be rendered in which way.
    const Component geometryTag { mEcs.create<RenderInformation>() };
    
protected:
    glm::ivec2 mSize { window::bufferSize() };
    
    std::shared_ptr<MainCamera> mCamera;  // Must be declared first. Other object rely on this being set.
    
    const int mMipmapLevels { 8 };  // Must be before frame and texture buffer creation.
    
    std::shared_ptr<FramebufferObject>                  mGeometry;
    std::shared_ptr<TextureBufferObject>                mPosition;
    std::shared_ptr<TextureBufferObject>                mNormal;
    std::shared_ptr<TextureBufferObject>                mAlbedo;
    std::shared_ptr<FramebufferObject>                  mLightAccumulator;
    std::shared_ptr<TextureBufferObject>                mDiffuse;
    std::shared_ptr<TextureBufferObject>                mSpecular;
    std::shared_ptr<FramebufferObject>                  mOutput;
    std::shared_ptr<TextureBufferObject>                mLightTarget;
    std::vector<std::shared_ptr<FramebufferObject>>     mDownSampleBuffers;
    std::shared_ptr<MipmapTexture>                      mDownSampleTexture;
    std::vector<std::shared_ptr<FramebufferObject>>     mUpSampleBuffers;
    std::shared_ptr<MipmapTexture>                      mUpSampleTexture;
    std::shared_ptr<FramebufferObject>                  mComposite;
    std::shared_ptr<TextureBufferObject>                mPostProcess;
    
    ViewSettings mShow;
    
    DeferredLightShader         mDeferredLightingShader;
    MipViewerShader             mDownSamplingMipViewerShader;
    MipViewerShader             mUpSamplingMipViewerShader;
    Bloom                       mBloomShader;
    
    // Debug Helpers
    Mesh<UvVertex, NoMaterial> mBox { load::model<UvVertex, NoMaterial>(path::resources() + "models/physics/Box.obj")[0] };
    Shader mBoxShader { path::shaders() + "physics/BoxVolume.vert",       path::shaders() + "physics/BoxVolume.frag" };
};


