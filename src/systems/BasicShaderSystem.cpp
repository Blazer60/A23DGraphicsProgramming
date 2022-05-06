/**
 * @file BasicShaderSystem.cpp
 * @author Ryan Purse
 * @date 13/02/2022
 */


#include "BasicShaderSystem.h"

#include <utility>
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"

BasicShaderSystem::BasicShaderSystem(std::shared_ptr<MainCamera> camera)
    : mCamera(std::move(camera))
{
    // Never changes, so it can be defined in the constructor.
    mEntities.forEach([this](const RenderInformation &renderCoreElements, const std::shared_ptr<BasicUniforms> &uniforms) {
        glBindFramebuffer(GL_FRAMEBUFFER, renderCoreElements.fbo);
        glBindVertexArray(renderCoreElements.vao);
        
        mShader.set("u_mvp", mCamera->getVpMatrix() * uniforms->modelMat);
        
        glDrawElements(GL_TRIANGLES, renderCoreElements.eboCount, GL_UNSIGNED_INT, 0);
    });
}

void BasicShaderSystem::onUpdate()
{
    mShader.bind();
    
    // glm::mat4 projectionMat = glm::perspective(glm::radians(45.f), 1920.f / 1080.f, 0.0001f, 100.f);
    // glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f));
    
    // mSphereShader.set("u_mvp", mCamera->getVpMatrix());
}
