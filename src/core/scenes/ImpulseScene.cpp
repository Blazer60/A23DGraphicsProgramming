/**
 * @file ImpulseScene.cpp
 * @author Ryan Purse
 * @date 09/05/2022
 */


#include "ImpulseScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "TreeBuilder.h"
#include "CollisionDetection.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"

ImpulseScene::ImpulseScene()
{
    mRedBall = createPhongModel(glm::vec3(-6.f, 5.f, 0.f), mRedSphere);
    mCollisionResponse.makePhysicsObject(mRedBall, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.2f);
    mCollisionResponse.makeBoundingSphere(mRedBall, true, 1.f);
    
    mGreenBall = createPhongModel(glm::vec3(-2.f, 5.f, 0.f), mGreenSphere);
    mCollisionResponse.makePhysicsObject(mGreenBall, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.4f);
    mCollisionResponse.makeBoundingSphere(mGreenBall, true, 1.f);
    
    mBlueBall = createPhongModel(glm::vec3(2.f, 5.f, 0.f), mBlueSphere);
    mCollisionResponse.makePhysicsObject(mBlueBall, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.6f);
    mCollisionResponse.makeBoundingSphere(mBlueBall, true, 1.f);
    
    mYellowBall = createPhongModel(glm::vec3(6.f, 5.f, 0.f), mYellowSphere);
    mCollisionResponse.makePhysicsObject(mYellowBall, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.8f);
    mCollisionResponse.makeBoundingSphere(mYellowBall, true, 1.f);
    
    Entity floor = createPhongModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
}

void ImpulseScene::onUpdate()
{
    Scene::onUpdate();
}

void ImpulseScene::onImguiUpdate()
{
    if (ImGui::Button("Start Physics") && !mSetup)
    {
        mEcs.createSystem<Gravity>();
        mEcs.createSystem<TreeBuilder>(mTree);
        mEcs.createSystem<CollisionDetection>(mRenderer, mTree);
        mEcs.createSystem<LinearEulerMethod>();
        mEcs.createSystem<LinearKinematicSystem>();
        mEcs.createSystem<AngularEulerMethod>();
        mSetup = true;
    }
    
    Scene::onImguiUpdate();
    
    if (ImGui::CollapsingHeader("Red Ball Settings"))
        showSettings(mRedBall);
    
    if (ImGui::CollapsingHeader("Green Ball Settings"))
        showSettings(mGreenBall);
    
    if (ImGui::CollapsingHeader("Blue Ball Settings"))
        showSettings(mBlueBall);
    
    if (ImGui::CollapsingHeader("Yellow Ball Settings"))
        showSettings(mYellowBall);

    if (ImGui::CollapsingHeader("Octree Debugging"))
    {
        ImGui::TextWrapped("The tree will only be updated once the physics simulation has started.");
    
        ImGui::Checkbox("Tree Bounds", &mShowBounds);
        ImGui::Checkbox("Tree Element Bounds", &mShowElementBounds);
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("Tree elements can only be shown if Toggle Tree Bounds is set true.");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
}

void ImpulseScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}

void ImpulseScene::showSettings(Entity entity)
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
