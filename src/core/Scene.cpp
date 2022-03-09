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
#include "Loader.h"
#include "BlinnPhongShaderSystem.h"

Scene::Scene()
    : mCube(primitives::basicCube()),
      mTri(primitives::basicTriangle()),
      mUvCube(primitives::uvCube()),
      mMainCamera(std::make_shared<MainCamera>()),
      mUvRrComponent(ecs::create<RenderCoreElements>()),
      mPhongRenderComponent(ecs::create<RenderCoreElements>()),
      mMainFbo(glm::ivec2(1920, 1080)),
      mInversionFbo(glm::ivec2(1920, 1080)),
      mTeapot(loadModel<BasicVertex>(path::resources() + "models/UtahTeapot.obj")),
      mPhongTeapot(loadModel<PhongVertex>(path::resources() + "models/UtahTeapot.obj"))
{
    ecs::Component basicCore = ecs::create<RenderCoreElements>();
    
    const int count = 3;
    for (int x = 0; x < count; ++x)
    {
        for (int y = 0; y < count; ++y)
        {
            for (int z = 0; z < count; ++z)
            {
                createChildThingAt(basicCore, glm::vec3(x, y, z) * 4.f);
            }
        }
    }
    
    createUvCubeEntity();
    createTeapotEntity();
    
    // Creation order of system still matters.
    ecs::createSystem<TextureBinderSystem>       ();
    ecs::createSystem<BasicUniformUpdaterSystem> ();
    ecs::createSystem<RotatorSystem>             ();
    ecs::createSystem<BinderSystem<BasicVertex>> ({ basicCore });  // Default types are automatically appended.
    ecs::createSystem<BinderSystem<UvVertex>>    ({ mUvRrComponent });
    ecs::createSystem<BinderSystem<PhongVertex>> ({ mPhongRenderComponent });
    ecs::createSystem<BasicShaderSystem>         ({ basicCore },         mMainCamera);
    ecs::createSystem<UvShaderSystem>            ({ mUvRrComponent },    mMainCamera);
    ecs::createSystem<BlinnPhongShaderSystem>    ({ mPhongRenderComponent }, mMainCamera, mDirectionalLight);
    
    ecs::start();
}

ecs::Entity Scene::createUvCubeEntity() const
{
    ecs::Entity eUvCube = ecs::create();
    RenderCoreElements coreElements;
    coreElements.fbo = mMainFbo.getId();
    ecs::add(eUvCube, mUvRrComponent, coreElements);
    ecs::add(eUvCube, SharedMesh(mUvCube));
    // ecs::add(eUvCube, mTeapot);
    ecs::add(eUvCube, Vbo());
    ecs::add(eUvCube, Ebo());
    ecs::add(eUvCube, std::make_shared<BasicUniforms>());
    ecs::add(eUvCube, Transform());
    ecs::add(eUvCube, UvUniforms{ glm::vec3(1.f, 0.f, 1.f) });
    ecs::add(eUvCube, Texture());
    ecs::add(eUvCube, TexturePath(path::textures() + "HaSquare.png"));
    return eUvCube;
}

ecs::Entity Scene::createTeapotEntity() const
{
    ecs::Entity eUvCube = ecs::create();
    RenderCoreElements coreElements;
    coreElements.fbo = mMainFbo.getId();
    ecs::add(eUvCube, mPhongRenderComponent, coreElements);
    ecs::add(eUvCube, mPhongTeapot);
    // ecs::add(eUvCube, mTeapot);
    ecs::add(eUvCube, Vbo());
    ecs::add(eUvCube, Ebo());
    ecs::add(eUvCube, std::make_shared<BasicUniforms>());
    ecs::add(eUvCube, Transform { glm::vec3(-15.f, 0.f, 0.f) });
    ecs::add(eUvCube, UvUniforms{ glm::vec3(1.f, 1.f, 1.f) });
    ecs::add(eUvCube, Texture());
    ecs::add(eUvCube, TexturePath(path::textures() + "HaSquare.png"));
    return eUvCube;
}

void Scene::createChildThingAt(ecs::Component basicCore, glm::vec3 position)
{
    ecs::Entity parent = ecs::create();
    ecs::Entity childA = ecs::create();
    ecs::Entity childB = ecs::create();
    
    ecs::add(parent, childA);
    auto uniforms = std::make_shared<BasicUniforms>();
    ecs::add(parent, uniforms);
    ecs::add(parent, Transform { position });
    ecs::add(parent, Rotator { 0.f, 1.f });
    
    ecs::add(childA, mTeapot);
    RenderCoreElements coreElements;
    coreElements.fbo = mMainFbo.getId();
    ecs::add(childA, basicCore, coreElements);
    ecs::add(childA, Vbo());
    ecs::add(childA, Ebo());
    ecs::add(childA, uniforms);
    
    ecs::add(childB, SharedMesh(mTri));
    ecs::add(childB, basicCore, coreElements);
    ecs::add(childB, Vbo());
    ecs::add(childB, Ebo());
    ecs::add(childB, uniforms);
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
