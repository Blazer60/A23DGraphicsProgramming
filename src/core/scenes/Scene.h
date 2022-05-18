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
#include "FramebufferObject.h"
#include "FilePaths.h"
#include "ModelLoader.h"
#include "TextureBufferObject.h"
#include "DeferredLightShader.h"

#include "Renderer.h"
#include "Primitives.h"

#include "CollisionResponse.h"

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
    virtual ~Scene() = default;
    
    virtual void onFixedUpdate();
    
    virtual void onUpdate();
    
    virtual void onRender();
    
    virtual void onImguiUpdate();
    
    virtual void onImguiMenuUpdate();

protected:
    Entity createModel(const glm::vec3 position, const Model<PhongVertex, BlinnPhongMaterial> &meshes);
    Entity createModel(const glm::vec3 &position, const Model<PhongVertex, EmissivePbrMaterial> &meshes);
    
    ecs::Core                   mEcs                { ecs::initFlag::AutoInitialise };
    std::shared_ptr<MainCamera> mMainCamera         { std::make_shared<MainCamera>(glm::vec3(0.f, 10.f, 15.f)) };
    Renderer                    mRenderer           { mMainCamera, mEcs };
    CollisionResponse           mCollisionResponse  { mEcs };
};


