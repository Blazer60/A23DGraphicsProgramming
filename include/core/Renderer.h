/**
 * @file Renderer.h
 * @author Ryan Purse
 * @date 30/03/2022
 */


#pragma once


#include "MainCamera.h"
#include "RenderPipeline.h"
#include "DeferredLightShader.h"
#include "Ecs.h"
#include "RenderComponents.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"
#include "PreFilterShader.h"
#include "MipViewerShader.h"
#include "Bloom.h"

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
    
    unsigned int geometryFboName { 0 };
    
protected:
    // NOTE: Order of declaration matters here.
    ecs::Core &mEcs;
    
public:
    // Tags let us know which object should be rendered in which way.
    const Component basicTag    { mEcs.create<RenderInformation>() };
    const Component uvTag       { mEcs.create<RenderInformation>() };
    const Component phongTag    { mEcs.create<RenderInformation>() };
    const Component geometryTag { mEcs.create<RenderInformation>() };
    
    std::shared_ptr<MainCamera> mCamera;  // Must be declared first. Other object rely on this being set.
    RenderPipeline              mRenderPipeline;
    DeferredLightShader         mDeferredLightingShader;
    PreFilterShader             mPreFilterShader;
    MipViewerShader             mDownSamplingMipViewerShader;
    MipViewerShader             mUpSamplingMipViewerShader;
    Bloom                 mBloomShader;
    
    // Debug Helpers
    Mesh<UvVertex, NoMaterial> mBox { load::model<UvVertex, NoMaterial>(path::resources() + "models/physics/Box.obj")[0] };
    Shader mBoxShader { path::shaders() + "physics/BoxVolume.vert",       path::shaders() + "physics/BoxVolume.frag" };
};


