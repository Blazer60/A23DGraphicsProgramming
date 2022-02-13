/**
 * @file BasicShaderSystem.cpp
 * @author Ryan Purse
 * @date 13/02/2022
 */


#include "BasicShaderSystem.h"
#include "gtc/type_ptr.hpp"

void BasicShaderSystem::onUpdate()
{
    // todo: Make foreach run immediately after onUpdate has been called.
    // frame buffer needs clearing properly.
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // const glm::vec4 bgColour { 0.f, 0.f, 0.2f, 1.f };
    // glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(bgColour));
    mShader.bind();
    
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.f), 1920.f / 1080.f, 0.0001f, 100.f);
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f));
    
    // mShader.set("u_mvp", projectionMat * view);
    
    mEntities.forEach([this](const Vao &vao, const Fbo &fbo, const EboCount &eboCount) {
        // glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
        mShader.bind();
        glBindVertexArray(vao.id);
        glDrawElements(GL_TRIANGLES, eboCount.count, GL_UNSIGNED_INT, 0);
    });
}
