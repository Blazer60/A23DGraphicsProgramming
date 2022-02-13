/**
 * @file BasicShaderSystem.cpp
 * @author Ryan Purse
 * @date 13/02/2022
 */


#include "BasicShaderSystem.h"
#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"

BasicShaderSystem::BasicShaderSystem()
{
    // Never changes, so it can be defined in the constructor.
    mEntities.forEach([](const Vao &vao, const Fbo &fbo, const EboCount &eboCount) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
        glBindVertexArray(vao.id);
        glDrawElements(GL_TRIANGLES, eboCount.count, GL_UNSIGNED_INT, 0);
    });
}

void BasicShaderSystem::onUpdate()
{
    mShader.bind();
    
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.f), 1920.f / 1080.f, 0.0001f, 100.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f));
    
    mShader.set("u_mvp", projectionMat * view);
}
