/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Ecs.h"
#include "ModelMatrixUpdater.h"
#include "RotatorSystem.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"
#include "CollisionDetection.h"
#include "imgui.h"

Scene::Scene()
{
    // Creation order of systems still matters.
    mEcs.createSystem<ModelMatrixUpdater>();
    mEcs.createSystem<RotatorSystem>();
}

Entity Scene::createModel(const glm::vec3 position, const Model<PhongVertex, BlinnPhongMaterial> &meshes)
{
    // Our transform uniforms that will be distributed to every child in the hierarchy.
    auto transformUniforms = std::make_shared<ModelMatrix>();
    
    Entity parent    = mEcs.create();
    Entity model     = mEcs.create();  // Just contains model slots for hierarchical purity.
    
    // The parent only needs a transform, uniforms for the transform and a model child entity.
    mEcs.add(parent, model);
    mEcs.add(parent, Transform { position } );
    mEcs.add(parent, transformUniforms);
    
    for (const auto &mesh : meshes)
    {
        Entity modelSlot = mEcs.create();
        mEcs.add(model, modelSlot);
        
        mEcs.add(modelSlot, mRenderer.geometryTag, mesh.renderInformation);
        mEcs.add(modelSlot, mesh.material);
        mEcs.add(modelSlot, transformUniforms);
    }
    
    return parent;
}

Entity Scene::createModel(const glm::vec3 &position, const Model<PhongVertex, EmissivePbrMaterial> &meshes)
{
    // Our transform uniforms that will be distributed to every child in the hierarchy.
    auto transformUniforms = std::make_shared<ModelMatrix>();
    
    Entity parent    = mEcs.create();
    Entity model     = mEcs.create();  // Just contains model slots for hierarchical purity.
    
    // The parent only needs a transform, uniforms for the transform and a model child entity.
    mEcs.add(parent, model);
    mEcs.add(parent, Transform { position } );
    mEcs.add(parent, transformUniforms);
    
    for (const auto &mesh : meshes)
    {
        Entity modelSlot = mEcs.create();
        mEcs.add(model, modelSlot);
        
        mEcs.add(modelSlot, mRenderer.emissiveTag, mesh.renderInformation);
        mEcs.add(modelSlot, mesh.material);
        mEcs.add(modelSlot, transformUniforms);
    }
    
    return parent;
}

void Scene::onFixedUpdate()
{
    mEcs.fixedUpdate();
}

void Scene::onUpdate()
{
    mMainCamera->update();
    mEcs.update();
}

void Scene::onRender()
{
    mRenderer.clear();
    mEcs.render();
    mRenderer.update();
}

void Scene::onImguiUpdate()
{
    mEcs.imGui();
    mRenderer.imguiUpdate();
    mMainCamera->imguiUpdate();
}

void Scene::onImguiMenuUpdate()
{
    mRenderer.imguiMenuUpdate();
}
