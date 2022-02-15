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
    std::shared_ptr<BasicMesh> mCube;
    std::shared_ptr<BasicMesh> mTri;
    
    std::shared_ptr<MainCamera> mMainCamera;
    
    void createChildThingAt(ecs::Component vao, glm::vec3 position);
};


