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
#include "CollisionDetection.h"
#include "imgui.h"

Scene::Scene()
{
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
    mRenderer.update();
}

void Scene::onImguiUpdate()
{
    mRenderer.imguiUpdate();
    mMainCamera->imguiUpdate();
    ImGui::ShowDemoWindow();
}

void Scene::onImguiMenuUpdate()
{
    mRenderer.imguiMenuUpdate();
}
