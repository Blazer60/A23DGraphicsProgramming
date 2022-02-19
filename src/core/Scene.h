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
#include "BinderSystem.h"

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
    BasicSharedMesh mCube;
    BasicSharedMesh mTri;
    UvSharedMesh mUvCube;
    
    FrameBufferObject mMainFbo;
    FrameBufferObject mInversionFbo;
    
    std::shared_ptr<MainCamera> mMainCamera;
    
    ecs::Component mUvRrComponent;
    
    PostProcessingShader mInversionShader { path::shaders() + "post-processing/inversion/Inversion.vert", path::shaders() + "post-processing/inversion/Inversion.frag" };
    
    ecs::Entity createUvCubeEntity() const;
    
    void createChildThingAt(ecs::Component basicCore, glm::vec3 position);
};


