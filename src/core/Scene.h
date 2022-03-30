/**
 * @file Scene.h
 * @author Ryan Purse
 * @date 11/02/2022
 */


#pragma once

#include "Components.h"
#include "Shader.h"
#include "MainCamera.h"
#include "Common.h"
#include "FrameBufferObject.h"
#include "PostProcessingShader.h"
#include "FilePaths.h"
#include "DirectionalLight.h"
#include "ModelLoader.h"
#include "TextureBufferObject.h"
#include "Core.h"
#include "RenderPipeline.h"
#include "DeferredLightShader.h"

#include <memory>

/**
 * An abstraction to differentiate from core functionality and ECS Systems
 * @author Ryan Purse
 * @date 11/02/2022
 */
class Scene
{
public:
    Scene();
    ~Scene() = default;
    
    void onUpdate();
    
    void onImguiUpdate();

protected:
    glm::ivec2 mSreenSize { 1920, 1080 };  // Must be at the top.
    
    Model<UvVertex, NoMaterial> mCube;
    
    RenderPipeline mRenderPipeline;
    
    
    FrameBufferObject mMainFbo;
    std::shared_ptr<TextureBufferObject> mMainTexture;
    
    std::shared_ptr<MainCamera> mMainCamera;
    
    ecs::Component mUvRrComponent;
    ecs::Component mPhongRenderComponent;
    ecs::Component mGeometryRenderComponent;
    
    DeferredLightShader mDeferredLightShader;
    
    PostProcessingShader mInversionShader { path::shaders() + "post-processing/inversion/Inversion.vert", path::shaders() + "post-processing/inversion/Inversion.frag" };
    
    std::shared_ptr<DirectionalLight> mDirectionalLight { std::make_shared<DirectionalLight>() };
    
    ecs::Entity createUvCubeEntity() const;
    
    ecs::Entity createPhongModel(glm::vec3 position, std::string_view path);
};


