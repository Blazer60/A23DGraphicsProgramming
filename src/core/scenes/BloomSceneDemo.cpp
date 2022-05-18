/**
 * @file TestingScene.cpp
 * @author Ryan Purse
 * @date 05/05/2022
 */


#include "BloomSceneDemo.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "CollisionDetection.h"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
#include "physics/TreeBuilder.h"

BloomSceneDemo::BloomSceneDemo()
{
    mBanana = createModel(glm::vec3(-0.8f, 0.4f, -0.8f), mBananaModel);
    auto &bananaTransform = mEcs.getComponent<Transform>(mBanana);
    bananaTransform.scale = glm::vec3(0.5f);
    bananaTransform.rotation = glm::quat(0.356f, -0.486f, 0.798f, -0.005f);
    
    createModel(glm::vec3(0.f, 0.f, 0.f), mJapanModel);
    
    mPointLight = mEcs.create();
    mEcs.add(mPointLight, light::PointLight());
    mEcs.add(mPointLight, Position { glm::vec3(0.f, 4.f, 0.f) });
}

void BloomSceneDemo::onImguiUpdate()
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
    if (ImGui::CollapsingHeader("Banana"))
    {
        auto &transform = mEcs.getComponent<Transform>(mBanana);
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
        ImGui::DragFloat4("Rotation", glm::value_ptr(transform.rotation), 0.1f);
        transform.rotation = glm::normalize(transform.rotation);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);
    }
}
