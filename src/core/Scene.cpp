/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Primitives.h"
#include "Ecs.h"
#include "BasicShaderSystem.h"
#include "BasicUniformUpdaterSystem.h"
#include "UvShaderSystem.h"
#include "RotatorSystem.h"
#include "TextureBinderSystem.h"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
#include "BlinnPhongShaderSystem.h"
#include "MaterialComponents.h"
#include "ModelLoader.h"

Scene::Scene()
    : mCube(primitives::cube<UvVertex>()),
      mMainCamera(std::make_shared<MainCamera>()),
      mUvRrComponent(ecs::create<RenderInformation>()),
      mPhongRenderComponent(ecs::create<RenderInformation>()),
      mMainFbo(glm::ivec2(1920, 1080)),
      mInversionFbo(glm::ivec2(1920, 1080))
{
    ecs::Component basicCore = ecs::create<RenderInformation>();
    
    createUvCubeEntity();
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                ecs::Entity parent = createPhongModel(glm::vec3(i * 3.f, j * 3.f, k * 3.f), path::resources() + "models/Bole.obj");
                ecs::add(parent, Rotator { 1.f, 1.f });
            }
        }
    }
    
    createPhongModel(glm::vec3(-5.f, 0.f, 0.f), path::resources() + "models/pbr-spheres/StoneCladding.obj");
    
    
    // Creation order of system still matters.
    ecs::createSystem<TextureBinderSystem>       ();
    ecs::createSystem<BasicUniformUpdaterSystem> ();
    ecs::createSystem<RotatorSystem>             ();
    ecs::createSystem<BasicShaderSystem>         ({ basicCore },         mMainCamera);
    ecs::createSystem<UvShaderSystem>            ({ mUvRrComponent },    mMainCamera);
    ecs::createSystem<BlinnPhongShaderSystem>    ({ mPhongRenderComponent }, mMainCamera, mDirectionalLight);
    
    ecs::start();
}

ecs::Entity Scene::createUvCubeEntity() const
{
    ecs::Entity eUvCube = ecs::create();
    Mesh<UvVertex, NoMaterial> cube = mCube[0];
    cube.renderInformation.fbo = mMainFbo.getId();
    ecs::add(eUvCube, mUvRrComponent, cube.renderInformation);
    
    ecs::add(eUvCube, std::make_shared<BasicUniforms>());
    ecs::add(eUvCube, Transform());
    ecs::add(eUvCube, UvUniforms{ glm::vec3(1.f, 0.f, 1.f) });
    ecs::add(eUvCube, Texture());
    ecs::add(eUvCube, TexturePath(path::textures() + "HaSquare.png"));
    return eUvCube;
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
        
        mesh.renderInformation.fbo = mMainFbo.getId();
        
        ecs::add(modelSlot, mPhongRenderComponent, mesh.renderInformation);
        ecs::add(modelSlot, mesh.material);
        ecs::add(modelSlot, transformUniforms);
    }
    
    return parent;
}

void Scene::onUpdate()
{
    // This needs to be in its own class.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const glm::vec4 bgColour { 0.f, 0.f, 0.2f, 1.f };
    glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(bgColour));
    const float depth { 1.f };
    glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
    
    mMainFbo.clear();
    mInversionFbo.clear();
    
    mMainFbo.update();
    
    mMainCamera->setProjectionMatrix(mMainFbo.getSize());
    mMainCamera->update();
    ecs::update();
    
    mInversionShader.draw(mInversionFbo.getId(), mMainFbo.getTexture(), mInversionFbo.getSize());
}

void Scene::onImguiUpdate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    mDirectionalLight->onImguiUpdate();
    mInversionFbo.imguiUpdate();
    mMainFbo.imguiUpdate();
    mMainCamera->imguiUpdate();
    ImGui::ShowDemoWindow();
}
