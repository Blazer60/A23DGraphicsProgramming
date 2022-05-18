/**
 * @file RotationDemoScene.cpp
 * @author Ryan Purse
 * @date 11/05/2022
 */


#include "RotationDemoScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "CollisionDetection.h"
#include "gtc/type_ptr.hpp"
#include "ModelDestroyer.h"
#include "imgui.h"
#include "physics/TreeBuilder.h"

RotationDemoScene::RotationDemoScene()
{
    mAlpha  = createModel(glm::vec3(0.f, 5.f, 0.f), mStoneCladding);
    const float alphaMass = 10.f;
    mCollisionResponse.makePhysicsObject(mAlpha, glm::vec3(0.f, 0.f, 0.f), alphaMass, 0.9f);
    mCollisionResponse.makeBoundingSphere(mAlpha, true, 1.f);
    auto &betaDynamicObject = mEcs.getComponent<DynamicObject>(mAlpha);
    betaDynamicObject.momentum = glm::vec3(0.f, 0.f, 10.f);
    mEcs.add(mAlpha, Torque { glm::vec3(0.f, 0.f, 0.f) });
    mEcs.add(mAlpha, AngularObject { glm::mat3(2.f / 5.f * alphaMass) });
    mEcs.add(mAlpha, AngularVelocity { });
    
    Entity floor = createModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
}

RotationDemoScene::~RotationDemoScene()
{
    destroy::model(mStoneCladding);
    destroy::model(mFloor);
}

void RotationDemoScene::onUpdate()
{
    Scene::onUpdate();
}

void RotationDemoScene::onRender()
{
    Scene::onRender();
}

void RotationDemoScene::onImguiUpdate()
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
    
    if (ImGui::CollapsingHeader("Alpha Settings"))
    {
        auto &transform = mEcs.getComponent<Transform>(mAlpha);
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
        ImGui::DragFloat4("Rotation", glm::value_ptr(transform.rotation), 0.1f);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);
        auto &torque = mEcs.getComponent<Torque>(mAlpha);
        ImGui::DragFloat3("Torque", glm::value_ptr(torque.tau), 0.1f);
        auto &angularObject = mEcs.getComponent<AngularObject>(mAlpha);
        ImGui::DragFloat3("Angular Momentum", glm::value_ptr(angularObject.angularMomentum), 0.1f);
    }
    
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
    
    ImGui::TextWrapped("I have had a go at trying to implement rotational physics. Euluer's Method works fine but I was unable to make the impulse work correctly.");
}
