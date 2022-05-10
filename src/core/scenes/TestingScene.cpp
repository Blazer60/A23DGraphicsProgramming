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
    mAlpha = createPhongModel(glm::vec3(1.f, 9.f, 0.f), mStoneCladding);
    mCollisionResponse.makePhysicsObject(mAlpha, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.2f);
    mCollisionResponse.makeBoundingBox(mAlpha, true);
    auto &alphaDynamicObject = mEcs.getComponent<DynamicObject>(mAlpha);
    alphaDynamicObject.momentum = glm::vec3(0.f);
    
    
    mBeta  = createPhongModel(glm::vec3(4.f, 14.f, 0.f), mStoneCladding);
    const float betaMass = 10.f;
    mCollisionResponse.makePhysicsObject(mBeta, glm::vec3(0.f, 0.f, 0.f), betaMass, 1.f);
    mCollisionResponse.makeBoundingSphere(mBeta, true, 1.f);
    auto &betaDynamicObject = mEcs.getComponent<DynamicObject>(mBeta);
    betaDynamicObject.momentum = glm::vec3(0.f, 0.f, 0.f);
    // mEcs.add(mBeta, Torque { glm::vec3(1.f, 0.f, 0.f) });
    // mEcs.add(mBeta, AngularObject { glm::mat3(2.f / 5.f * betaMass) });
    // mEcs.add(mBeta, AngularVelocity { });
    
    Entity floor = createPhongModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
    
    Entity point = mEcs.create();
    mEcs.add(point, light::PointLight({
        glm::vec4(5.f, 3.f, 0.f, 1.f) }));
}

void TestingScene::onUpdate()
{
    Scene::onUpdate();
    
    static bool setup = false;
    if (!setup && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P))
    {
        mEcs.createSystem<Gravity>();
        mEcs.createSystem<TreeBuilder>(mTree);
        mEcs.createSystem<CollisionDetection>(mRenderer, mTree);
        mEcs.createSystem<MomentumRk4>();
        mEcs.createSystem<KinematicSystem>();
        mEcs.createSystem<AngularEulerIntegration>();
        setup = true;
        std::cout << "Starting Physics\n";
    }
}

void TestingScene::onImguiUpdate()
{
    Scene::onImguiUpdate();
    if (ImGui::CollapsingHeader("Alpha Settings"))
    {
        if (ImGui::TreeNode("Transform"))
        {
            auto &transform = mEcs.getComponent<Transform>(mAlpha);
            ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
            ImGui::DragFloat4("Rotation", glm::value_ptr(transform.rotation), 0.1f);
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Misc."))
        {
            auto &velocity = mEcs.getComponent<Velocity>(mAlpha);
            ImGui::DragFloat3("Velocity", glm::value_ptr(velocity.value), 0.1f);

            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader("Beta Settings"))
    {
        auto &transform = mEcs.getComponent<Transform>(mBeta);
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
        ImGui::DragFloat4("Rotation", glm::value_ptr(transform.rotation), 0.1f);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);
        auto &torque = mEcs.getComponent<Torque>(mBeta);
        ImGui::DragFloat3("Torque", glm::value_ptr(torque.tau), 0.1f);
        auto &angularObject = mEcs.getComponent<AngularObject>(mBeta);
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
}

void TestingScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}
