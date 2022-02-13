/**
 * @file Scene.cpp
 * @author Ryan Purse
 * @date 11/02/2022
 */


#include "Scene.h"
#include "Primitives.h"
#include "Ecs.h"
#include "BasicBinderSystems.h"
#include "Shader.h"

Scene::Scene()
    : mCube(primitives::basicCube())
{
    ecs::Component basicVao = ecs::create<Vao>();
    ecs::create<Vbo>(ecs::TypeDefault);
    ecs::create<Ebo>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicMesh>>(ecs::TypeDefault);
    
    ecs::Entity entity = ecs::create();
    ecs::add(entity, mCube);
    ecs::add(entity, basicVao, Vao());
    ecs::add(entity, Vbo());
    ecs::add(entity, Ebo());
    
    ecs::createSystem<BasicMeshBinderSystem>();
    
    ecs::UType basicVaoBinderSystemType { basicVao, ecs::getComponentIdOf<Vbo>(), ecs::getComponentIdOf<Ebo>() };
    ecs::createSystem<BasicVaoBinderSystem>(basicVaoBinderSystemType);
    
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
