/**
 * @file DynamicImpulseDemoScene.cpp
 * @author Ryan Purse
 * @date 10/05/2022
 */


#include "DynamicImpulseDemoScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "TreeBuilder.h"
#include "CollisionDetection.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"

DynamicImpulseDemoScene::DynamicImpulseDemoScene()
{
    mRedBall = createPhongModel(glm::vec3(-6.f, 5.f, 0.f), mRedSphere);
    mCollisionResponse.makePhysicsObject(mRedBall, glm::vec3(0.f), 100.f, 0.1f);
    mCollisionResponse.makeBoundingSphere(mRedBall, true, 1.f);
    mEcs.getComponent<DynamicObject>(mRedBall).momentum = glm::vec3(100.f, 0.f, 0.f);
    
    mGreenBall = createPhongModel(glm::vec3(6.f, 5.f, 0.f), mGreenSphere);
    mCollisionResponse.makePhysicsObject(mGreenBall, glm::vec3(0.f, 0.f, 0.f), 10.f, 0.1f);
    mCollisionResponse.makeBoundingSphere(mGreenBall, true, 1.f);
    mEcs.getComponent<DynamicObject>(mGreenBall).momentum = glm::vec3(-100.f, 0.f, 0.f);
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
}

void DynamicImpulseDemoScene::onUpdate()
{
    Scene::onUpdate();
}

void DynamicImpulseDemoScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}

void DynamicImpulseDemoScene::onImguiUpdate()
{
    if (ImGui::Button("Start Physics") && !mSetup)
    {
        mEcs.createSystem<TreeBuilder>(mTree);
        mEcs.createSystem<CollisionDetection>(mRenderer, mTree);
        mEcs.createSystem<EulerIntegration>();
        mEcs.createSystem<KinematicSystem>();
        mEcs.createSystem<AngularEulerIntegration>();
        mSetup = true;
    }
    
    Scene::onImguiUpdate();
    
    if (ImGui::CollapsingHeader("Red Ball Settings"))
        showBallSettings(mRedBall);
    
    if (ImGui::CollapsingHeader("Green Ball Settings"))
        showBallSettings(mGreenBall);
    
    ImGui::TextWrapped("Impulse also works between two dynamic objects. You can change their settings above to see what happens.");
}

void DynamicImpulseDemoScene::showBallSettings(Entity entity)
{
    auto &transform = mEcs.getComponent<Transform>(entity);
    auto &dynamicObject = mEcs.getComponent<DynamicObject>(entity);
    auto &physicsMaterial = mEcs.getComponent<PhysicsMaterial>(entity);
    
    std::string hash = std::to_string(entity);
    
    ImGui::DragFloat3(std::string("Position##" + hash).c_str(),     glm::value_ptr(transform.position), 0.1f);
    ImGui::DragFloat4(std::string("Rotation##" + hash).c_str(),     glm::value_ptr(transform.rotation), 0.1f);
    ImGui::DragFloat3(std::string("Scale##" + hash).c_str(),        glm::value_ptr(transform.scale), 0.1f);
    ImGui::DragFloat3(std::string("Force##" + hash).c_str(),        glm::value_ptr(dynamicObject.force), 0.1f);
    ImGui::DragFloat( std::string("Mass##" + hash).c_str(),         &dynamicObject.mass);
    ImGui::DragFloat3(std::string("Momentum##" + hash).c_str(),     glm::value_ptr(dynamicObject.momentum), 0.1f);
    ImGui::DragFloat( std::string("Bounciness##" + hash).c_str(),   &physicsMaterial.bounciness);
}
