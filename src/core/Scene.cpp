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
#include "Shader.h"

Scene::Scene()
    : mCube(primitives::basicCube())
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
    
    Shader shader("../res/shaders/basic/Basic.vert", "../res/shaders/basic/Basic.frag");
}

void Scene::onUpdate()
{
    ecs::update();
}

void Scene::onImguiUpdate()
{

}
