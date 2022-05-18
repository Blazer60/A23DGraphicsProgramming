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
    mBanana = createPhongModel(glm::vec3(0.f, 50.f, 0.f), mBananaModel);
    
    createPhongModel(glm::vec3(0.f, 0.f, 0.f), mJapanModel);
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
    
    mPointLight = mEcs.create();
    mEcs.add(mPointLight, light::PointLight());
    mEcs.add(mPointLight, Position { glm::vec3(0.f, 4.f, 0.f) });
}

void TestingScene::onImguiUpdate()
{
    Scene::onImguiUpdate();
    if (ImGui::CollapsingHeader("Point Light"))
    {
        auto &pointLight = mEcs.getComponent<light::PointLight>(mPointLight);
        auto &position = mEcs.getComponent<Position>(mPointLight);
        ImGui::DragFloat3("Position", glm::value_ptr(position.value), 0.1f);
        ImGui::DragFloat3("Color", glm::value_ptr(pointLight.intensity), 0.1f);
        ImGui::DragFloat("Distance", &pointLight.distance, 0.1f);
        ImGui::DragFloat("Power", &pointLight.power, 0.1f);
    }
}
