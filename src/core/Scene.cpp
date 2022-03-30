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
#include "DirectionalLightShaderSystem.h"

Scene::Scene()
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                ecs::Entity parent = createPhongModel(glm::vec3(i * 3.f, j * 3.f + 1.f, k * 3.f), path::resources() + "models/Bole.obj");
                ecs::add(parent, Rotator { 1.f, 1.f });
            }
        }
    }
    
    createPhongModel(glm::vec3(-5.f, 1.0f, 0.f), path::resources() + "models/pbr-spheres/StoneCladding.obj");
    createPhongModel(glm::vec3(0.f, 0.f, 0.f), path::resources() + "models/floor/Floor.obj");
    
    ecs::Entity light = ecs::create();
    ecs::add(light, light::DirectionalLight());
    
    // Creation order of system still matters.
    ecs::createSystem<TextureBinderSystem>       ();
    ecs::createSystem<BasicUniformUpdaterSystem> ();
    ecs::createSystem<RotatorSystem>             ();
    ecs::start();
}

ecs::Entity Scene::createPhongModel(glm::vec3 position, std::string_view path)
{
    // Our transform uniforms that will be distributed to every child in the hierarchy.
    auto transformUniforms = std::make_shared<BasicUniforms>();
    
    ecs::Entity parent    = ecs::create();
    ecs::Entity model     = ecs::create();  // Just contains model slots for hierarchical purity.
    
    // The parent only needs a transform, uniforms for the transform and a model child entity.
    ecs::add(parent, model);
    ecs::add(parent, Transform { position } );
    ecs::add(parent, transformUniforms);
    
    auto meshes = load::model<PhongVertex, BlinnPhongMaterial>(path);
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
