/**
 * @file OdeDemoScene.cpp
 * @author Ryan Purse
 * @date 10/05/2022
 */


#include "OdeDemoScene.h"
#include "BoundingVolumes.h"
#include "Physics.h"
#include "LightingComponents.h"
#include "PhysicsSystems.h"
#include "TreeBuilder.h"
#include "CollisionDetection.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"

OdeDemoScene::OdeDemoScene()
{
    setupBall(mRedBall,     mEulerTag,  glm::vec3(-6.f, 5.f, 0.f), mRedSphere);
    setupBall(mGreenBall,   mRk2Tag,    glm::vec3(-2.f, 5.f, 0.f), mGreenSphere);
    setupBall(mBlueBall,    mRk4Tag,    glm::vec3( 2.f, 5.f, 0.f), mBlueSphere);
    setupBall(mYellowBall,  mRkNTag,    glm::vec3( 6.f, 5.f, 0.f), mYellowSphere);
    
    
    Entity floor = createPhongModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    Entity sun = mEcs.create();
    mEcs.add(sun, light::DirectionalLight());
}

void OdeDemoScene::onUpdate()
{
    Scene::onUpdate();
}

void OdeDemoScene::onRender()
{
    // Scene::onRender() has been completely overridden to let the tree render.
    mRenderer.clear();
    if (mShowBounds)
        mTree->debugDrawTree([this](const glm::mat4 &m, const glm::vec3 &h) { mRenderer.drawBox(m, h); }, mShowElementBounds);
    mEcs.render();
    mRenderer.update();
}

void OdeDemoScene::onImguiUpdate()
{
    if (ImGui::Button("Start Physics") && !mSetup)
    {
        mEcs.createSystem<TreeBuilder>(mTree);
        mEcs.createSystem<CollisionDetection>(mRenderer, mTree);
        
        mEcs.createSystem<Gravity>({ mEulerTag });
        mEcs.createSystem<LinearEulerMethod>({ mEulerTag });
    
        mEcs.createSystem<Gravity>({ mRk2Tag });
        mEcs.createSystem<LinearRk2>({ mRk2Tag });
    
        mEcs.createSystem<Gravity>({ mRk4Tag });
        mEcs.createSystem<LinearRk4>({ mRk4Tag });
    
        mEcs.createSystem<Gravity>({ mRkNTag });
        mEcs.createSystem<LinearRk>({ mRkNTag }, mRkNValue);
        
        mSetup = true;
    }
    
    Scene::onImguiUpdate();
    
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
    
    ImGui::TextWrapped("Red Sphere    - Euler's Method");
    ImGui::TextWrapped("Green Sphere  - Rk2 Method");
    ImGui::TextWrapped("Blue Sphere   - Rk4 Method");
    ImGui::TextWrapped("Yellow Sphere - RkN Method");
    
    const uint32_t min = 1;
    ImGui::DragScalar("RkN Order", ImGuiDataType_U32, &mRkNValue, 1.f, &min);
    ImGui::SameLine();
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted("The Order must be greater than 0!");
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void OdeDemoScene::setupBall(
    Entity ball, Component type, const glm::vec3 position,
    Model<PhongVertex, BlinnPhongMaterial> &model)
{
    // We can't use collision response factories as they set up components using the default channels.
    ball = createPhongModel(position, model);
    mEcs.add(ball, type, DynamicObject { glm::vec3(0.f), 100.f });
    mEcs.add(ball, Velocity());
    mEcs.add(ball, PhysicsMaterial { 0.9f });
    std::shared_ptr<BoundingVolume> boundingVolume = std::make_shared<BoundingSphere>(ball, 1.f);
    boundingVolume->callbacks.subscribe([this, type](Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal) {
        mCollisionResponse.typedStaticCollision(type, entity, other, position, normal);
    });
    mEcs.add(ball, boundingVolume);
}
