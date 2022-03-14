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
    Model<UvVertex, NoMaterial> mCube;
    
    FrameBufferObject mMainFbo;
    FrameBufferObject mInversionFbo;
    
    std::shared_ptr<MainCamera> mMainCamera;
    
    ecs::Component mUvRrComponent;
    ecs::Component mPhongRenderComponent;
    
    PostProcessingShader mInversionShader { path::shaders() + "post-processing/inversion/Inversion.vert", path::shaders() + "post-processing/inversion/Inversion.frag" };
    
    std::shared_ptr<DirectionalLight> mDirectionalLight { std::make_shared<DirectionalLight>() };
    
    ecs::Entity createUvCubeEntity() const;
    
    ecs::Entity createPhongModel(glm::vec3 position, std::string_view path);
};


