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

/**
 * Sets up the rendering pipeline for a single scene. Used to setup deferred rendering and deferred lighting.
 * @author Ryan Purse
 * @date 30/03/2022
 */
class Renderer
{
public:
    explicit Renderer(std::weak_ptr<MainCamera> camera);
    
    void clear();
    void update();
    void imguiUpdate();
    
    // Tags let us know which object should be rendered in which way.
    const ecs::Component basicTag    { ecs::create<RenderInformation>() };
    const ecs::Component uvTag       { ecs::create<RenderInformation>() };
    const ecs::Component phongTag    { ecs::create<RenderInformation>() };
    const ecs::Component geometryTag { ecs::create<RenderInformation>() };
    
    unsigned int geometryFboName   { 0 };
    
protected:
    // NOTE: Order of declaration matters here.
    std::weak_ptr<MainCamera>               mCamera;  // Must be declared first. Other object rely on this being set.
    glm::ivec2                              mScreenSize { 1920, 1080 };
    RenderPipeline                          mRenderPipeline;
    std::shared_ptr<TextureBufferObject>    mMainTexture;
    DeferredLightShader                     mDeferredLightingShader;
    
    std::shared_ptr<DirectionalLight>       mDirectionalLight;
};


