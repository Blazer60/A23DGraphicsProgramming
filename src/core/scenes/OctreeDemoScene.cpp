/**
 * @file OctreeDemoScene.cpp
 * @author Ryan Purse
 * @date 09/05/2022
 */


#include "OctreeDemoScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "TreeBuilder.h"
#include "CollisionDetection.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"

#include <random>

OctreeDemoScene::OctreeDemoScene()
{
    for (int i = 0; i < 40; ++i)
    {
        glm::vec3 position = glm::vec3(randomValue(), randomValue(), randomValue());
        Entity crate = createModel(position, mCrate);
        mCollisionResponse.makeBoundingBox(crate, false);
        mEcs.add(crate, Rotator { randomValue(), randomValue(), position });
    }
    
    Entity floor = createModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
    
    mEcs.createSystem<TreeBuilder>(mTree);
    mEcs.createSystem<CollisionDetection>(mRenderer, mTree);
}

float OctreeDemoScene::randomValue()
{
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<float> random(-30.f, 30.f);
    return random(rng);
}

void OctreeDemoScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}

void OctreeDemoScene::onImguiUpdate()
{
    Scene::onImguiUpdate();
    if (ImGui::CollapsingHeader("Octree Debugging"))
    {
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
    ImGui::TextWrapped("The Octree will update based on the bounding boxes of each item. Open Octree Debugging to "
                       "enable boundary drawing. You can also reset the scene in the navbar under scenes.");
    ImGui::TextWrapped("Note that collision detection is still happening between items but no responses have been"
                       " registered. Debug drawing is not instanced so there is a decrease in performance "
                       "when toggled.");
}
