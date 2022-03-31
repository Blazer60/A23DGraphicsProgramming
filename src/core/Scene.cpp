/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Ecs.h"
#include "BasicShaderSystem.h"
#include "BasicUniformUpdaterSystem.h"
#include "RotatorSystem.h"
#include "TextureBinderSystem.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"
#include "systems/lighting/DirectionalLightShaderSystem.h"

Scene::Scene()
{
    const int count = 5;
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < count; ++j)
        {
            for (int k = 0; k < count; ++k)
            {
                ecs::Entity parent = createPhongModel(glm::vec3(i * 3.f, j * 3.f + 3.f, k * 3.f), mBanana);
                ecs::add(parent, Rotator { 1.f, 1.f });
            }
        }
    }
    
    auto triangleFan = primitives::triangleFanCircle<PhongVertex, BlinnPhongMaterial>(20);
    createPhongModel(glm::vec3(0.f, 1.f, 0.f), triangleFan);
    
    createPhongModel(glm::vec3(-5.f, 1.0f, 0.f), mStoneCladding);
    createPhongModel(glm::vec3(0.f, 0.f, 0.f), mFloor);
    
    ecs::Entity light = ecs::create();
    ecs::add(light, light::DirectionalLight());
    
    ecs::Entity point = ecs::create();
    ecs::add(point, light::PointLight( {
        glm::vec4(5.f, 3.f, 0.f, 1.f)
    } ));
    
    // Creation order of system still matters.
    ecs::createSystem<TextureBinderSystem>       ();
    ecs::createSystem<BasicUniformUpdaterSystem> ();
    ecs::createSystem<RotatorSystem>             ();
    ecs::start();
}

ecs::Entity Scene::createPhongModel(const glm::vec3 position, Model<PhongVertex, BlinnPhongMaterial> &meshes)
{
    // Our transform uniforms that will be distributed to every child in the hierarchy.
    auto transformUniforms = std::make_shared<BasicUniforms>();
    
    ecs::Entity parent    = ecs::create();
    ecs::Entity model     = ecs::create();  // Just contains model slots for hierarchical purity.
    
    // The parent only needs a transform, uniforms for the transform and a model child entity.
    ecs::add(parent, model);
    ecs::add(parent, Transform { position } );
    ecs::add(parent, transformUniforms);
    
    for (auto &mesh : meshes)
    {
        ecs::Entity modelSlot = ecs::create();
        ecs::add(model, modelSlot);
        
        mesh.renderInformation.fbo = mRenderer.geometryFboName;
        
        ecs::add(modelSlot, mRenderer.geometryTag, mesh.renderInformation);
        ecs::add(modelSlot, mesh.material);
        ecs::add(modelSlot, transformUniforms);
    }
    
    return parent;
}

void Scene::onUpdate()
{
    mRenderer.clear();
    
    mMainCamera->update();
    ecs::update();
    
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
}
