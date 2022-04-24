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
    // const int count = 3;
    // for (int i = 0; i < count; ++i)
    // {
    //     for (int j = 0; j < count; ++j)
    //     {
    //         for (int k = 0; k < count; ++k)
    //         {
    //             Entity parent = createPhongModel(glm::vec3(i, j + 10.f, k), mBanana);
    //             mEcs.add(parent, DynamicObject { glm::vec3(0.f, 1.f, 0.f), 1.f });
    //             mEcs.add(parent, Velocity());
    //             std::shared_ptr<BoundingVolume> boundingBox = std::make_shared<BoundingSphere>(parent, 0.2f);
    //             boundingBox->callbacks.subscribe([]() {
    //                 std::cout << "Callback\n";
    //             });
    //             mEcs.add(parent, std::move(boundingBox));
    //         }
    //     }
    // }
    
    const auto addImpulse = [this](Entity entity, Entity other, const glm::vec3 &position, const glm::vec3 &normal) {
        auto &dynamicObject = mEcs.getComponent<DynamicObject>(entity);
        auto &velocity      = mEcs.getComponent<Velocity>(entity);
        auto &transform     = mEcs.getComponent<Transform>(entity);
        
        const float impulse = -(1.f + 0.8f) * glm::dot(velocity.value, normal) / (1.f / dynamicObject.mass);
        velocity.value += normal * impulse / dynamicObject.mass;
        
        const glm::vec3 contactForce = dynamicObject.force * glm::dot(glm::normalize(dynamicObject.force), normal);
        
        dynamicObject.force += contactForce;
    };
    
    mAlpha = createPhongModel(glm::vec3(1.1f, 4.f, 0.f), mStoneCladding);
    std::shared_ptr<BoundingVolume> boundingSphereAlpha = std::make_shared<BoundingSphere>(mAlpha);
    boundingSphereAlpha->callbacks.subscribe(addImpulse);
    mEcs.add(mAlpha, DynamicObject { glm::vec3(0.f, -1.f, 0.f), 10.f });
    mEcs.add(mAlpha, Velocity { glm::vec3(0.f, -2.f, 0.f) });
    mEcs.add(mAlpha, boundingSphereAlpha);
    
    mBeta  = createPhongModel(glm::vec3(-1.f, 4.f, 5.f), mStoneCladding);
    std::shared_ptr<BoundingVolume> boundingSphereBeta = std::make_shared<BoundingBox>(mBeta);
    boundingSphereBeta->callbacks.subscribe([](Entity alpha, Entity other, const glm::vec3 &position, const glm::vec3 &normal) {
        std::cout << position.x << ", " << position.y << ", " << position.z << " | Beta Hit\n";
    });
    mEcs.add(mBeta, Velocity());
    mEcs.add(mBeta, boundingSphereBeta);
    
    // auto triangleFan = primitives::triangleFanCircle<PhongVertex, BlinnPhongMaterial>(20);
    // createPhongModel(glm::vec3(0.f, 1.f, 0.f), triangleFan);
    
    // createPhongModel(glm::vec3(-5.f, 1.0f, 0.f), mStoneCladding);
    
    Entity floor = createPhongModel(glm::vec3(0.f, -1.f, 0.f), mFloor);
    std::shared_ptr<BoundingVolume> floorHitBox = std::make_shared<BoundingBox>(floor, glm::vec3(2.f, 1.f, 2.f));  //
    mEcs.add(floor, Velocity { glm::vec3(0.f, 0.0f, 0.f) });
    mEcs.add(floor, floorHitBox);
    mEcs.add(floor, Kinematic());
    
    
    Entity light = mEcs.create();
    mEcs.add(light, light::DirectionalLight());
    
    Entity point = mEcs.create();
    mEcs.add(point, light::PointLight( {
        glm::vec4(5.f, 3.f, 0.f, 1.f)
    } ));
    
    // Creation order of system still matters.
    mEcs.createSystem<TextureBinderSystem>();
    mEcs.createSystem<BasicUniformUpdaterSystem>();
    mEcs.createSystem<RotatorSystem>();
    
    // mEcs.createSystem<Gravity>();  // Toggle with P
    // mEcs.createSystem<RungeKutta>(4);
    // mEcs.createSystem<CollisionSystem>();
    // mEcs.createSystem<EulerIntegration>();
    // mEcs.createSystem<RungeKutta2>();
    // mEcs.createSystem<RungeKutta4>();
    
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
        mEcs.createSystem<CollisionSystem>();
        // mEcs.createSystem<RungeKutta>(4);
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
        
        auto &dynamicObject = mEcs.getComponent<DynamicObject>(mAlpha);
        ImGui::DragFloat3("Force", glm::value_ptr(dynamicObject.force), 0.1f);
        
        ImGui::End();
    }
}
