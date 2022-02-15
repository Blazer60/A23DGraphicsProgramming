/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Primitives.h"
#include "Ecs.h"
#include "BasicBinderSystems.h"
#include "BasicShaderSystem.h"
#include "BasicUniformUpdaterSystem.h"
#include "RotatorSystem.h"
#include "gtc/type_ptr.hpp"

Scene::Scene()
    : mCube(primitives::basicCube()),
      mTri(primitives::basicTriangle()),
      mMainCamera(std::make_shared<MainCamera>())
{
    ecs::Component basicCore = ecs::create<RenderCoreElements>();
    
    ecs::create<Vbo>(ecs::TypeDefault);
    ecs::create<Ebo>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicMesh>>(ecs::TypeDefault);
    ecs::create<Transform>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicUniforms>>(ecs::TypeDefault);
    ecs::create<Rotator>(ecs::TypeDefault);
    
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
    
    ecs::createSystem<BasicUniformUpdaterSystem>();
    ecs::createSystem<RotatorSystem>();
    
    ecs::UType basicBinderSystemType { basicCore, ecs::get<Vbo>(), ecs::get<Ebo>(), ecs::get<std::shared_ptr<BasicMesh>>() };
    ecs::createSystem<BasicBinderSystem>(basicBinderSystemType);
    
    ecs::UType basicShaderSystemType { basicCore, ecs::get<std::shared_ptr<BasicUniforms>>() };
    ecs::createSystem<BasicShaderSystem>(basicShaderSystemType, mMainCamera);
    
    ecs::start();
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
    
    ecs::add(childA, mCube);
    ecs::add(childA, basicCore, RenderCoreElements());
    ecs::add(childA, Vbo());
    ecs::add(childA, Ebo());
    ecs::add(childA, uniforms);
    
    ecs::add(childB, mTri);
    ecs::add(childB, basicCore, RenderCoreElements());
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
    
    mMainCamera->update();
    ecs::update();
}

void Scene::onImguiUpdate()
{

}
