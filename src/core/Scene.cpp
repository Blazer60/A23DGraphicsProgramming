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

Scene::Scene()
    : mCube(primitives::basicCube()),
      mTri(primitives::basicTriangle()),
      mUvCube(primitives::uvCube()),
      mMainCamera(std::make_shared<MainCamera>()),
      mUvRrComponent(ecs::create<RenderCoreElements>()),
      mMainFbo(glm::ivec2(1920, 1080)),
      mInversionFbo(glm::ivec2(1920, 1080))
{
    ecs::Component basicCore = ecs::create<RenderCoreElements>();
    
    ecs::create<Vbo>(ecs::TypeDefault);
    ecs::create<Ebo>(ecs::TypeDefault);
    ecs::create<BasicSharedMesh>(ecs::TypeDefault);
    ecs::create<UvSharedMesh>(ecs::TypeDefault);
    ecs::create<Transform>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicUniforms>>(ecs::TypeDefault);
    ecs::create<Rotator>(ecs::TypeDefault);
    ecs::create<UvUniforms>(ecs::TypeDefault);
    ecs::create<Texture>(ecs::TypeDefault);
    ecs::create<TexturePath>(ecs::TypeDefault);
    ecs::create<SharedMesh>(ecs::TypeDefault);
    
    ecs::create<ecs::Entity>(ecs::TypeDefault);
    
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
    
    ecs::createSystem<TextureBinderSystem>();
    
    ecs::createSystem<BasicUniformUpdaterSystem>();
    ecs::createSystem<RotatorSystem>();
    
    ecs::UType basicBinderSystemType { basicCore, ecs::get<Vbo>(), ecs::get<Ebo>(), ecs::get<SharedMesh>() };
    ecs::createSystem<BinderSystem<BasicVertex>>(basicBinderSystemType);
    
    ecs::UType uvBinderSystemType { mUvRrComponent, ecs::get<Vbo>(), ecs::get<Ebo>(), ecs::get<SharedMesh>() };
    ecs::createSystem<BinderSystem<UvVertex>>(uvBinderSystemType);
    
    ecs::UType basicShaderSystemType { basicCore, ecs::get<std::shared_ptr<BasicUniforms>>() };
    ecs::createSystem<BasicShaderSystem>(basicShaderSystemType, mMainCamera);
    
    ecs::UType uVShaderSystemType { mUvRrComponent, ecs::get<std::shared_ptr<BasicUniforms>>(), ecs::get<UvUniforms>(), ecs::get<Texture>() };
    ecs::createSystem<UvShaderSystem>(uVShaderSystemType, mMainCamera);
    
    ecs::start();
}

ecs::Entity Scene::createUvCubeEntity() const
{
    ecs::Entity eUvCube = ecs::create();
    RenderCoreElements coreElements;
    coreElements.fbo = mMainFbo.getId();
    ecs::add(eUvCube, mUvRrComponent, coreElements);
    ecs::add(eUvCube, SharedMesh(mUvCube));
    ecs::add(eUvCube, Vbo());
    ecs::add(eUvCube, Ebo());
    ecs::add(eUvCube, std::make_shared<BasicUniforms>());
    ecs::add(eUvCube, Transform());
    ecs::add(eUvCube, UvUniforms{ glm::vec3(1.f, 0.f, 1.f) });
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
    
    ecs::add(childA, SharedMesh(mCube));
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
    mMainFbo.imguiUpdate();
    mInversionFbo.imguiUpdate();
    ImGui::ShowDemoWindow();
}
