/**
 * @file TestingScene.cpp
 * @author Ryan Purse
 * @date 05/05/2022
 */


#include "TestingScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "CollisionDetection.h"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
#include "physics/TreeBuilder.h"

TestingScene::TestingScene()
{
    mBanana = createPhongModel(glm::vec3(0.f, 2.f, 0.f), mBananaModel);
    
    createPhongModel(glm::vec3(0.f), mFloor);
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
    
    // Entity sun2 = mEcs.create();
    // mEcs.add(sun2, light::DirectionalLight {
    //     glm::normalize(glm::vec3(1.f, 1.f, 1.f)),
    //     glm::vec3(0.f, 1.f, 1.f)
    // });
    
    Entity point = mEcs.create();
    mEcs.add(point, light::PointLight({
        glm::vec4(5.f, 3.f, 0.f, 1.f) }));
}

void TestingScene::onImguiUpdate()
{
    Scene::onImguiUpdate();
}
