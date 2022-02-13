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
#include "gtc/type_ptr.hpp"

// GLfloat vertices[] =
//         {
//                 -0.5f, -0.5f , 0.0f,  1.f, 0.f, 0.f,
//                 -0.5f,  0.5f , 0.0f,  1.f, 0.f, 0.f,
//                 0.5f,  0.5f , 0.0f,   1.f, 0.f, 0.f,
//                 0.5f, -0.5f , 0.0f, 1.f, 0.f, 0.f,
//         };

std::vector<BasicVertex> vertices = {
    BasicVertex{ glm::vec3(-0.5f, -0.5f , 0.0f), glm::vec3(1.f, 0.f, 1.f) },
    BasicVertex{ glm::vec3(-0.5f,  0.5f , 0.0f), glm::vec3(1.f, 0.f, 1.f) },
    BasicVertex{ glm::vec3(0.5f,  0.5f , 0.0f), glm::vec3(1.f, 0.f, 1.f) },
    BasicVertex{ glm::vec3(0.5f, -0.5f , 0.0f), glm::vec3(1.f, 0.f, 1.f) },
};

std::vector<unsigned int> indices =
        {
                0, 2, 1,
                0, 3, 2
        };

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
    ecs::update();
}

void Scene::onImguiUpdate()
{

}
