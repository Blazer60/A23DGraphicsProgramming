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

TestingScene::TestingScene()
{
    mAlpha = createPhongModel(glm::vec3(0.f, 9.f, 0.f), mStoneCladding);
    mCollisionResponse.makePhysicsObject(mAlpha, glm::vec3(0.f, 5.f, 0.f), 100.f, 0.1f);
    mCollisionResponse.makeBoundingBox(mAlpha, true);
    
    mBeta  = createPhongModel(glm::vec3(0.f, 6.f, 0.f), mStoneCladding);
    mCollisionResponse.makePhysicsObject(mBeta, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.1f);
    mCollisionResponse.makeBoundingBox(mBeta, true);
    
    for (int i = 0; i < 30; ++i)
    {
        Entity entity = createPhongModel(glm::vec3(-40.f + i * 2.1f, 5.f, 3.f), mStoneCladding);
        mCollisionResponse.makeBoundingBox(entity, false);
    }
    
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
        mEcs.createSystem<CollisionDetection>(mRenderer);
        mEcs.createSystem<RungeKutta2>();
        mEcs.createSystem<KinematicSystem>();
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
}
