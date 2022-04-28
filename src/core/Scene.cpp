/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Ecs.h"
#include "BasicUniformUpdaterSystem.h"
#include "RotatorSystem.h"
#include "TextureBinderSystem.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"
#include "systems/lighting/DirectionalLightShaderSystem.h"

#include "Physics.h"
#include "PhysicsSystems.h"
#include "CollisionSystem.h"
#include "BoundingVolumeVisual.h"
#include "gtc/type_ptr.hpp"

Scene::Scene()
{
    mAlpha = createPhongModel(glm::vec3(2.f, 9.f, 0.f), mStoneCladding);
    // mScenePhysics.makePhysicsObject(mAlpha, glm::vec3(0.f, 1.f, 0.f), 100.f, 0.1f);
    mScenePhysics.makeBoundingBox(mAlpha, false);
    
    // mBeta  = createPhongModel(glm::vec3(0.f, 6.f, 0.f), mStoneCladding);
    // mScenePhysics.makePhysicsObject(mBeta, glm::vec3(0.f, 0.f, 0.f), 100.f, 0.1f);
    // mScenePhysics.makeBoundingBox(mBeta, false);
    
    Entity floor = createPhongModel(glm::vec3(0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(50.f, 0.1f, 50.f));
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    
    Entity light = mEcs.create();
    mEcs.add(light, light::DirectionalLight());
    
    Entity point = mEcs.create();
    mEcs.add(point, light::PointLight( {
        glm::vec4(5.f, 3.f, 0.f, 1.f)
    } ));
    
    // Creation order of systems still matters.
    mEcs.createSystem<TextureBinderSystem>();
    mEcs.createSystem<BasicUniformUpdaterSystem>();
    mEcs.createSystem<RotatorSystem>();
    
    mEcs.start();
}

Entity Scene::createPhongModel(const glm::vec3 position, Model<PhongVertex, BlinnPhongMaterial> &meshes)
{
    // Our transform uniforms that will be distributed to every child in the hierarchy.
    auto transformUniforms = std::make_shared<BasicUniforms>();
    
    Entity parent    = mEcs.create();
    Entity model     = mEcs.create();  // Just contains model slots for hierarchical purity.
    
    // The parent only needs a transform, uniforms for the transform and a model child entity.
    mEcs.add(parent, model);
    mEcs.add(parent, Transform { position } );
    mEcs.add(parent, transformUniforms);
    
    for (auto &mesh : meshes)
    {
        Entity modelSlot = mEcs.create();
        mEcs.add(model, modelSlot);
        
        mesh.renderInformation.fbo = mRenderer.geometryFboName;
        
        mEcs.add(modelSlot, mRenderer.geometryTag, mesh.renderInformation);
        mEcs.add(modelSlot, mesh.material);
        mEcs.add(modelSlot, transformUniforms);
    }
    
    return parent;
}

void Scene::onUpdate()
{
    mRenderer.clear();
    
    mMainCamera->update();
    mEcs.update();
    
    static bool setup = false;
    if (!setup && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_P))
    {
        mEcs.createSystem<Gravity>();
        mEcs.createSystem<CollisionSystem>(mRenderer);
        mEcs.createSystem<RungeKutta>(4);
        mEcs.createSystem<RungeKutta2>();
        mEcs.createSystem<KinematicSystem>();
        setup = true;
        std::cout << "Starting Physics\n";
    }
    
    mRenderer.update();
}

void Scene::onImguiUpdate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    mRenderer.imguiUpdate();
    mMainCamera->imguiUpdate();
    ImGui::ShowDemoWindow();
    
    if(ImGui::Begin("Alpha Settings"))
    {
        auto &transform = mEcs.getComponent<Transform>(mAlpha);
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.1f);
        ImGui::DragFloat4("Rotation", glm::value_ptr(transform.rotation), 0.1f);
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);
        
        auto &velocity = mEcs.getComponent<Velocity>(mAlpha);
        ImGui::DragFloat3("Velocity", glm::value_ptr(velocity.value), 0.1f);
        
        // auto &dynamicObject = mEcs.getComponent<DynamicObject>(mAlpha);
        // ImGui::DragFloat3("Force", glm::value_ptr(dynamicObject.force), 0.1f);
        
        ImGui::End();
    }
}
