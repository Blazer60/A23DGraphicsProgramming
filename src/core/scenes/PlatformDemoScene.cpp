/**
 * @file PlatformSceneDemo.cpp
 * @author Ryan Purse
 * @date 10/05/2022
 */


#include "PlatformDemoScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "TreeBuilder.h"
#include "CollisionDetection.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"

PlatformDemoScene::PlatformDemoScene()
{
    mRedBall = createPhongModel(glm::vec3(-13.1f, 14.1f, -0.3f), mRedSphere);
    mCollisionResponse.makePhysicsObject(mRedBall, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.8f);
    mCollisionResponse.makeBoundingSphere(mRedBall, true, 1.f);
    
    mRamp = createPhongModel(glm::vec3(-10.2f, 7.8f, 0.f), mFloor);
    mCollisionResponse.makeBoundingBox(mRamp, false, glm::vec3(10.f, 0.1f, 10.f));
    mEcs.getComponent<Transform>(mRamp).rotation = glm::quat(0.85f, 0.242f, 0.455f, -0.106f);
    
    mGround = createPhongModel(glm::vec3(-0.4f, 0.2f, 15.1f), mFloor);
    mCollisionResponse.makeBoundingBox(mGround, false, glm::vec3(10.f, 0.1f, 10.f));
    mEcs.getComponent<Transform>(mGround).rotation = glm::quat(0.936f, 0.f, 0.353f, 0.f);
    
    mCrate = createPhongModel(glm::vec3(8.6f, 2.5f, 18.4f), mCrateModel);
    mCollisionResponse.makeBoundingBox(mCrate, false, glm::vec3(1.f, 1.f, 1.f));
    auto &crateTransform = mEcs.getComponent<Transform>(mCrate);
    crateTransform.rotation = glm::quat(0.406f, 0.f, 0.914f, 0.f);
    crateTransform.scale = glm::vec3(1.f, 2.f, 10.f);
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
}

void PlatformDemoScene::onUpdate()
{
    Scene::onUpdate();
}

void PlatformDemoScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}

void PlatformDemoScene::onImguiUpdate()
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
    {
        auto &transform = mEcs.getComponent<Transform>(mRedBall);
        auto &dynamicObject = mEcs.getComponent<DynamicObject>(mRedBall);
        auto &physicsMaterial = mEcs.getComponent<PhysicsMaterial>(mRedBall);
    
        std::string hash = std::to_string(mRedBall);
        
        ImGui::DragFloat3(std::string("Position##" + hash).c_str(),     glm::value_ptr(transform.position), 0.1f);
        ImGui::DragFloat4(std::string("Rotation##" + hash).c_str(),     glm::value_ptr(transform.rotation), 0.1f);
        ImGui::DragFloat3(std::string("Scale##" + hash).c_str(),        glm::value_ptr(transform.scale), 0.1f);
        ImGui::DragFloat3(std::string("Force##" + hash).c_str(),        glm::value_ptr(dynamicObject.force), 0.1f);
        ImGui::DragFloat( std::string("Mass##" + hash).c_str(),         &dynamicObject.mass);
        ImGui::DragFloat3(std::string("Momentum##" + hash).c_str(),     glm::value_ptr(dynamicObject.momentum), 0.1f);
        ImGui::DragFloat( std::string("Bounciness##" + hash).c_str(),   &physicsMaterial.bounciness);
    }
    if (ImGui::CollapsingHeader("Ramp Settings"))
        showTransformSettings(mRamp);
    
    if (ImGui::CollapsingHeader("Floor Settings"))
        showTransformSettings(mGround);
    
    if (ImGui::CollapsingHeader("Crate Settings"))
        showTransformSettings(mCrate);
    
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
    
    ImGui::TextWrapped("Collision detection is setup to handle any transform thrown at it. Try changing each of the entities settings to see what happens.");
}

void PlatformDemoScene::showTransformSettings(Entity entity)
{
    auto &transform = mEcs.getComponent<Transform>(entity);
    
    std::string hash = std::to_string(entity);
    
    ImGui::DragFloat3(std::string("Position##" + hash).c_str(),   glm::value_ptr(transform.position), 0.1f);
    ImGui::DragFloat4(std::string("Rotation##" + hash).c_str(),   glm::value_ptr(transform.rotation), 0.1f);
    ImGui::DragFloat3(std::string("Scale##" + hash).c_str(),      glm::value_ptr(transform.scale), 0.1f);
    
    transform.rotation = glm::normalize(transform.rotation);
}
