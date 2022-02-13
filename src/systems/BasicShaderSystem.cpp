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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const glm::vec4 bgColour = { 0.f, 0.f, 0.f, 0.f };
    glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(bgColour));
    
    mEntities.forEach([this](const Vao &vao, const Fbo &fbo, const EboCount &eboCount) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
        mShader.bind();
        glBindVertexArray(vao.id);
        glDrawElements(GL_TRIANGLES, eboCount.count, GL_UNSIGNED_INT, nullptr);
    });
}
