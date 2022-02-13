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
#include "gtc/type_ptr.hpp"

Scene::Scene()
    : mCube(primitives::basicCube()), mTri(primitives::basicTriangle())
{
    ecs::Component basicVao = ecs::create<Vao>();
    ecs::create<Vbo>(ecs::TypeDefault);
    ecs::create<Ebo>(ecs::TypeDefault);
    ecs::create<EboCount>(ecs::TypeDefault);
    ecs::create<Fbo>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicMesh>>(ecs::TypeDefault);
    
    ecs::Entity entity = ecs::create();
    ecs::add(entity, mCube);
    ecs::add(entity, basicVao, Vao());
    ecs::add(entity, Vbo());
    ecs::add(entity, Ebo());
    ecs::add(entity, Fbo());
    ecs::add(entity, EboCount());
    
    ecs::createSystem<BasicMeshBinderSystem>();
    
    ecs::UType basicVaoBinderSystemType { basicVao, ecs::getComponentIdOf<Vbo>(), ecs::getComponentIdOf<Ebo>() };
    ecs::createSystem<BasicVaoBinderSystem>(basicVaoBinderSystemType);
    
    ecs::UType basicShaderSystemType { basicVao, ecs::getComponentIdOf<Fbo>(), ecs::getComponentIdOf<EboCount>() };
    ecs::createSystem<BasicShaderSystem>(basicShaderSystemType);
    
    ecs::start();
}

void Scene::onUpdate()
{
    // This needs to be in its own class.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const glm::vec4 bgColour { 0.f, 0.f, 0.2f, 1.f };
    glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(bgColour));
    ecs::update();
}

void Scene::onImguiUpdate()
{

}
