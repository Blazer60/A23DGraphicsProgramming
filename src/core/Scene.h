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

#include "Renderer.h"
#include "Primitives.h"

#include "ScenePhysics.h"

#include "Ecs.h"

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
    ecs::Core mEcs { ecs::initFlag::AutoInitialise };
    
    Entity createPhongModel(const glm::vec3 position, Model<PhongVertex, BlinnPhongMaterial> &meshes);
    
    glm::ivec2 mSreenSize { 1920, 1080 };  // Must be at the top.
    
    Model<PhongVertex, BlinnPhongMaterial> mBanana {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/Bole.obj")
    };
    
    Model<PhongVertex, BlinnPhongMaterial> mStoneCladding {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/pbr-spheres/StoneCladding.obj")
    };
    Model<PhongVertex, BlinnPhongMaterial> mFloor {
        load::model<PhongVertex, BlinnPhongMaterial>(
            path::resources() + "models/floor/Floor.obj")
    };
    
    std::shared_ptr<MainCamera> mMainCamera { std::make_shared<MainCamera>(glm::vec3(0.f, 10.f, 15.f)) };
    
    Renderer mRenderer { mMainCamera, mEcs };
    ScenePhysics mScenePhysics { mEcs };
    
    Entity mAlpha;
    Entity mBeta;
};


