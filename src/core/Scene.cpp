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
#include "gtc/type_ptr.hpp"

Scene::Scene()
    : mCube(primitives::basicCube()),
      mTri(primitives::basicTriangle()),
      mMainCamera(std::make_shared<MainCamera>())
{
    ecs::Component basicVao = ecs::create<Vao>();
    ecs::create<Vbo>(ecs::TypeDefault);
    ecs::create<Ebo>(ecs::TypeDefault);
    ecs::create<EboCount>(ecs::TypeDefault);
    ecs::create<Fbo>(ecs::TypeDefault);
    ecs::create<std::shared_ptr<BasicMesh>>(ecs::TypeDefault);
    ecs::create<Transform>(ecs::TypeDefault);
    ecs::create<BasicUniforms>(ecs::TypeDefault);
    
    const int count = 3;
    for (int x = 0; x < count; ++x)
    {
        for (int y = 0; y < count; ++y)
        {
            for (int z = 0; z < count; ++z)
            {
                ecs::Entity entity = ecs::create();
                ecs::add(entity, mCube);
                ecs::add(entity, basicVao, Vao());
                ecs::add(entity, Vbo());
                ecs::add(entity, Ebo());
                ecs::add(entity, Fbo());
                ecs::add(entity, EboCount());
                ecs::add(entity, Transform { { x * 4.f, y * 4.f, z * 4.f } });
                ecs::add(entity, BasicUniforms());
            }
        }
    }
    
    ecs::createSystem<BasicMeshBinderSystem>();
    ecs::createSystem<BasicUniformUpdaterSystem>();
    
    ecs::UType basicVaoBinderSystemType { basicVao, ecs::getComponentIdOf<Vbo>(), ecs::getComponentIdOf<Ebo>() };
    ecs::createSystem<BasicVaoBinderSystem>(basicVaoBinderSystemType);
    
    ecs::UType basicShaderSystemType {
        basicVao, ecs::getComponentIdOf<Fbo>(),
        ecs::getComponentIdOf<EboCount>(), ecs::getComponentIdOf<BasicUniforms>()
    };
    ecs::createSystem<BasicShaderSystem>(basicShaderSystemType, mMainCamera);
    
    ecs::start();
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
